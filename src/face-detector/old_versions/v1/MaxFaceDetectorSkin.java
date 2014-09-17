import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class MaxFaceDetectorSkin extends MaxObject {
    
    private static int MAX_NUM_FACES = 5;
    private static int MAX_NUM_CANDIDATES = 300000;
    
    private JitterMatrix jm = new JitterMatrix();
    private int[] dim;
    private int[] offset = new int[] {0,0};
    private int[] buf;
    private byte[] raw;
    private double[] hsv;
    
    private Candidate[] candidates = new Candidate[MAX_NUM_CANDIDATES];
    private int num_candidates = 0;
    
    private Candidate[] faces = new Candidate[MAX_NUM_FACES];
    private int num_faces = 0;
    
    private CascadeWithSkinColor cc;
    
    private static int[] sizes = new int[] {20, 23, 26, 30, 34, 40, 46, 53,
					    61, 70, 80, 93, 107, 123, 141, 162, 187, 215};
    private int img_width, img_height, output_width, output_height;
    
    private boolean enable;
    
    
    private static void rgb2hsv(int _r, int _g, int _b, double hsv[]) {
	double var_R = (_r / 255f);	double var_G = (_g / 255f);	double var_B = (_b / 255f);
	double var_Min; double var_Max; double del_Max;
	if (var_R > var_G) { var_Min = var_G;var_Max = var_R;}
	else {var_Min = var_R; var_Max = var_G;	}
	if (var_B > var_Max) var_Max = var_B;
	if (var_B < var_Min) var_Min = var_B;
	del_Max = var_Max - var_Min;
	double H = 0, S, V;
	V = (var_Max + var_Min) / 2f;
	if (del_Max == 0) {	H = 0;	S = 0; }
	else {
	    if (V < 0.5) S = del_Max / (var_Max + var_Min);
	    else S = del_Max / (2 - var_Max - var_Min);
	    double del_R = (((var_Max - var_R) / 6f) + (del_Max / 2f))/ del_Max;
	    double del_G = (((var_Max - var_G) / 6f) + (del_Max / 2f))/ del_Max;
	    double del_B = (((var_Max - var_B) / 6f) + (del_Max / 2f))/ del_Max;
	    if (var_R == var_Max) H = del_B - del_G;
	    else if (var_G == var_Max) H = (1 / 3f) + del_R - del_B;
	    else H = (2 / 3f) + del_G - del_R;
	    if (H < 0) H += 1;
	    if (H > 1) H -= 1;
	}
	hsv[0] = H; hsv[1] = S; hsv[2] = V;
    }
    
    private void generateCandidates(Candidate[] candidates, 
				    int image_w, int image_h) {
	
	/* bigger candidates first */
	for (int i=sizes.length-1; i >= 0; i--) {
	    for (int y=0; y+sizes[i] < image_h;y+= 1+(sizes[i]/20) ) {
		for (int x=0; x+sizes[i] < image_w;x+= 1+(sizes[i]/20) ) {
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
    
    
    public MaxFaceDetectorSkin(int ow, int oh, int w, int h) {
	
	img_width = w;
	img_height = h;
	output_width = ow;
	output_height = oh;
	
	String cascade_fn = "smaller_haarcascade.xml";
	cc = new CascadeWithSkinColor(cascade_fn);
	post("Loading "+cascade_fn);
	
	generateCandidates(candidates,w,h);
	post("Initializing detection windows");
	
	post("Using margin = 0.0");

	raw = new byte[w*h];	
	buf = new int[w*4];
	hsv = new double[w*h*3];
	
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
    
    
    private static byte[] detectSkin(double[] hsv, int w, int h) {
	byte[] positive = new byte[w * h];
	double[] temp = new double[3];
	
	for (int i = 0;i < h; i++) {
	    for (int j = 0;j < w; j++) {			
		temp[0] = hsv[(i*w+j)*3];
		temp[1] = hsv[(i*w+j)*3 + 1];
		temp[2] = hsv[(i*w+j)*3 + 2];
		double s=SkinScorer.getScore(temp);
		double margin=0.0;
		// definitely correct
		if (s < -margin) {
		    positive[i*w+j] = 1;
		}
		// definitely incorrect
		else if (s > margin) {
		    positive[i*w+j] = 0;
		}
		// not sure
		else {
		    if (i == 0 || j == 0) {
			positive[i*w+j] = 0;
		    }
		    else {
			int vote = 0;
			if (positive[(i-1)*w+j] == 1) vote++;
			if (positive[(i-1)*w+(j-1)] == 1) vote++;
			if (positive[i*w+(j-1)] == 1) vote++;
			if (vote >= 2) {
			    positive[i*w+j] = 1;
			}
			else {
			    positive[i*w+j] = 0;
			}
		    }
		}
	    }
	}
	
	return positive;
    }
    
    
    public void jit_matrix(String s) {
	
	jm.frommatrix(s);
	num_faces = 0;
	
	if (enable) {
	    dim = jm.getDim();
	    for (int i = 0; i < dim[1]; i++) {
		offset[1] = i;
		jm.copyVectorToArray(0, offset, buf, dim[0]*4, 0);
		
		/* compute hsv and grayscale */
		double[] temphsv = new double[3];
		for (int j = 0; j < dim[0]; j++) {
		    int r = buf[j*4+1];
		    int g = buf[j*4+2];
		    int b = buf[j*4+3];
		    
		    raw[i*img_width+j] = (byte)(0.3*r+0.59*g+0.11*b);
		    
		    rgb2hsv(r,g,b,temphsv);
		    
		    hsv[(i*img_width+j)*3] = temphsv[0];
		    hsv[(i*img_width+j)*3+1] = temphsv[1];
		    hsv[(i*img_width+j)*3+2] = temphsv[2];
		    
		    
		}
	    }
	    
	    /* Detect Skin */
	    byte[] skin = detectSkin(hsv, img_width, img_height);
	    
	    IntegralImage skin_ii = new IntegralImage(skin, img_width, img_height, false);
	    IntegralImage ii = new IntegralImage(raw, img_width, img_height, true);
	    
	    for (int i=0;i<num_candidates;i++) {
		
		if (num_faces >= MAX_NUM_FACES) break;
		Candidate c = candidates[i];				
		
		if (cc.processCandidate(c, ii, skin_ii)) {
		    int updated = 0;          		
		    
		    for (int j=0;j<num_faces;j++) {
			Candidate f = faces[j];
			if (f.isContaining(c)) {
			    /* new detection is completely contained in old ones */
			    updated = -1;
			}
			else {  
			    if (f.isOverlap(c)) {
				if (f.isSizeSimilar(c)) {
				    if (c.size > f.size) {
					f.x = (f.x + c.x) / 2;
					f.y = (f.y + c.y) / 2;
					f.size = (f.size + c.size) / 2;
					f.weight = f.weight + 1;
					updated = 1;
					break;
				    } else {
					f.x = (f.weight * f.x + c.x)
					    / (f.weight + 1);
					f.y = (f.weight * f.y + c.y)
					    / (f.weight + 1);
					f.size = (f.weight * f.size + c.size)
					    / (f.weight + 1);
					f.weight = f.weight + 1;
					updated = 1;
					break;
				    }
				}
				else {
				    /* overlap but the new candidate is way too small */
				    updated = -1;
				}
			    }
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
