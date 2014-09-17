import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class MaxFaceDetector extends MaxObject {
   
    private static int MAX_NUM_FACES = 5;
    private static int MAX_NUM_CANDIDATES = 200000;

    private JitterMatrix jm = new JitterMatrix();
    private int[] dim;
    private int[] offset = new int[] {0,0};
    private int[] raw,buf;

    private Candidate[] candidates = new Candidate[MAX_NUM_CANDIDATES];
    private int num_candidates = 0;

    private Candidate[] faces = new Candidate[MAX_NUM_FACES];
    private int num_faces = 0;

    private Cascade cc;
    
    private static int[] sizes = new int[] {29, 35, 41, 50, 60, 72, 86, 103, 124, 149, 178};
    private int img_width, img_height, output_width, output_height;
    
    private boolean enable;
    
    private void generateCandidates(Candidate[] candidates, 
				    int image_w, int image_h) {
	for (int i=sizes.length-1; i >= 0; i--) {
	    for (int y=0; y+sizes[i] < image_h;y+= 2+(sizes[i]/20) ) {
		for (int x=0; x+sizes[i] < image_w;x+= 2+(sizes[i]/20) ) {
		    Candidate c = new Candidate(x,y,sizes[i]);
		    candidates[num_candidates++] = c;
		    if (num_candidates >= MAX_NUM_CANDIDATES) {
			post("Error: MAX_NUM_CANDIDATES is too low");
			return;
		    }
		}
	    }
	}
    }
    
    
    public MaxFaceDetector(int ow, int oh, int w, int h) {
	
	img_width = w;
	img_height = h;
	output_width = ow;
	output_height = oh;
	
	cc = new Cascade("haarcascade_frontalface_alt.xml");
	generateCandidates(candidates,w,h);
	
	raw = new int[w*h];	
	buf = new int[w*4];
	
	enable = false;
    }
    
    public void start() {
	enable = true;
	post("FaceDetector enabled");
    }
    
    public void stop() {
	enable = false;
	post("FaceDetector disabled");
    }
    
    
    public void jit_matrix(String s) {
	
	jm.frommatrix(s);
	num_faces = 0;
	
	if (enable) {
	    dim = jm.getDim();
	    for (int i = 0; i < dim[1]; i++) {
		offset[1] = i;
		jm.copyVectorToArray(0, offset, buf, dim[0]*4, 0);
		
		/* convert rgb to grayscale */
		for (int j = 0; j < dim[0]; j++) {
		    raw[i*img_width+j] = (int)(0.3*buf[j*4+1]+0.59*buf[j*4+2]+0.11*buf[j*4+3]);
		}
	    }
	    
	    YUVImage image = new YUVImage(img_width,img_height,raw);
	    IntegralImage ii = new IntegralImage(image);
	    
	    for (int i=0;i<num_candidates;i++) {
		
		if (num_faces >= MAX_NUM_FACES) break;
		Candidate c = candidates[i];				

		if (cc.processCandidate(c, ii)) {
		    int updated = 0;          		

		    for (int j=0;j<num_faces;j++) {
			Candidate f = faces[j];
			if (f.equals(c)) {
			    if (c.size > f.size) { 
				f.x = (f.x+c.x)/2;
				f.y = (f.y+c.y)/2;
				f.size = (f.size+c.size)/2;
				f.weight = f.weight + 1;
			    }
			    else {
				f.x = (f.weight*f.x+c.x)/(f.weight+1);
				f.y = (f.weight*f.y+c.y)/(f.weight+1);
				f.size = (f.weight*f.size+c.size)/(f.weight+1);
				f.weight = f.weight + 1;	
			    }
			    updated = 1;
			    break;
			}
		    }

		    if (updated == 0) faces[num_faces++] = c;		   
		}    	
	    }
	    
	}
	
	Atom[] face_array = new Atom[num_faces*4+1];
	int k = 0;
	face_array[k++] = Atom.newAtom("face");
	
	for (int i=0;i < num_faces;i++) {
	    Candidate c = faces[i];
	    face_array[k] = Atom.newAtom(new Integer(c.x * output_width / img_width));
	    face_array[k+1] = Atom.newAtom(new Integer(c.y * output_height / img_height));
	    face_array[k+2] = Atom.newAtom(new Integer(c.size * output_height / img_height));
	    Long time = new Long(System.currentTimeMillis());
	    face_array[k+3] = Atom.newAtom(time.floatValue());
	    k += 4;
	}
	
	outlet(0,face_array);

    }
    
}
