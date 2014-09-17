import com.cycling74.max.*;
import com.cycling74.jitter.*;
import java.net.*;

public class MaxFaceDetector extends MaxObject {

    /* constants */
    private static int SCAN_MODE = 0;
    private static int TRACK_MODE = 1;

    private static int STEP = 25;  // (1 + size[i]/STEP)

    private static int MAX_NUM_FACES = 5;
    private static int MAX_NUM_CANDIDATES_SCAN  = 150000;
    private static int MAX_NUM_CANDIDATES_TRACK = 30000;

    private static double MARGIN = 0.5;

    /* variables */
    private JitterMatrix jm;
    private byte[] raw;

    private double[] hsv;

    private Candidate[] candidates_scan = new Candidate[MAX_NUM_CANDIDATES_SCAN];
    private int num_candidates_scan;

    private Candidate[] candidates_track = new Candidate[MAX_NUM_CANDIDATES_TRACK];
    private int num_candidates_track;

    private Candidate[] faces = new Candidate[MAX_NUM_FACES];
    private Candidate[] faces_out = new Candidate[MAX_NUM_FACES];
    private int num_faces = 0, num_faces_out = 0;

    private static int[] sizes = new int[] 
    {25, 30, 33, 36, 40, 44, 48, 53, 58, 64, 70, 80, 93, 107, 123, 141, 162};

    private Cascade cc;
    private ColorScorer cs;
        
    private int output_width, output_height, image_width, image_height;
    private boolean enable, detecting;
    private int current_mode;

    private static String CamCtrlEncoding = "Basic " + 
	new sun.misc.BASE64Encoder().encode("cameraman:cameraman".getBytes());


    private static void turnOffAGC() {
	//Thread t = new Thread() {
	//    public void run() {
		try {
		    URL inq = new URL("http://192.168.1.11/command/camera.cgi?ExpMode=manual");
		    URLConnection inqc = inq.openConnection();
		    inqc.setRequestProperty("Authorization", CamCtrlEncoding);
		    post("FaceDetector: Automatic Gain Control is disabled");
		}
		catch (Exception e) {
		    e.printStackTrace();
		}
		//    }
		//};

		//t.start();
    }

    private static void turnOnAGC() {
	//Thread t = new Thread() {
	//public void run() {
		try {
		    URL inq = new URL("http://192.168.1.11/command/camera.cgi?ExpMode=full");
		    URLConnection inqc = inq.openConnection();
		    inqc.setRequestProperty("Authorization", CamCtrlEncoding);
		    post("FaceDetector: Automatic Gain Control is enabled");
		}
		catch (Exception e) {
		    e.printStackTrace();
		}
		//}
		//};

		//t.start();
    }


    public MaxFaceDetector(int ow, int oh, int w, int h, 
			   String xml, String xml_color) {
	
	image_width = w;
	image_height = h;

	output_width = ow;
	output_height = oh;
	
	post("Loading Haar's features " + xml);
	cc = new Cascade(xml);
	
	post("Loading skin-color features " + xml_color);
	cs = new ColorScorer(xml_color);
	
	post("Initializing detection windows");
	generateCandidatesScanMode(w,h);
	
	raw = new byte[w*h];
	hsv = new double[w*h*4];

	jm = new JitterMatrix();
	
	enable = false;
	current_mode = SCAN_MODE;
	detecting = false;

	turnOnAGC();

	declareIO(1,1);
	
    }

    
    private void generateCandidatesTrackMode(int image_w, int image_h, 
					     int x, int y, int size) {
	int size_range = 20, size_step = 5;
	int y_range = size, y_step = 1+size/STEP;
	int x_range = size, x_step = 1+size/STEP;

	num_candidates_track = 0;

	for (int current_size = size+size_range; 
	     current_size >= size-size_range; 
	     current_size -= size_step) {
	    if (current_size > image_width) continue;
	    if (current_size < 30) break;
	    for (int yy = y-y_range; yy <= y+y_range; yy += y_step) {
		if (yy < 0) continue;
		if (yy + current_size >= image_h) break;
		for (int xx = x-x_range; xx <= x+x_range; xx += x_step) {
		    if (xx < 0) continue;
		    if (xx + current_size >= image_w) break;
		    Candidate c = new Candidate(xx,yy,current_size);
		    candidates_track[num_candidates_track++] = c;
		    if (num_candidates_track >= MAX_NUM_CANDIDATES_TRACK) {
			post("Error: MAX_NUM_CANDIDATES_TRACK is too low");
			return;
		    }
		}
	    }
	}
	post("Number of candidates = "+num_candidates_track);
    }


    private void generateCandidatesScanMode(int image_w, int image_h) {
	
	num_candidates_scan = 0;

	for (int i=sizes.length-1; i >= 0; i--) {
	    
	    //post("window size: " + sizes[i]);
	    
	    for (int y=0; y+sizes[i] < image_h;y+= 1+(sizes[i]/STEP) ) {
		for (int x=0; x+sizes[i] < image_w;x+= 1+(sizes[i]/STEP) ) {
		    Candidate c = new Candidate(x,y,sizes[i]);
		    candidates_scan[num_candidates_scan++] = c;
		    if (num_candidates_scan >= MAX_NUM_CANDIDATES_SCAN) {
			post("Error: MAX_NUM_CANDIDATES_SCAN is too low");
			return;
		    }
		}
	    }
	}
	//post(""+num_candidates_scan);
    }
    
    public void mode(int[] l) {
	if (l.length > 0) {
	    if (l[0] == SCAN_MODE && current_mode != SCAN_MODE) {
		post("FaceDetector is now in scan mode");
		current_mode = l[0];

		turnOnAGC();
	    }
	    else if (l[0] == TRACK_MODE){
		post("FaceDetector is now in tracking mode");

		if (current_mode != TRACK_MODE)
		    turnOffAGC();

		current_mode = l[0];
		int x = l[1]*image_width/output_width;
		int y = l[2]*image_height/output_height;
		int s = l[3]*image_width/output_width;
		
		//generateCandidatesTrackMode(image_width,image_height,x,y,s);

	    }
	}
	else 
	    post("FaceDetector: Invalid mode");
    }
    
    public void start() {
	enable = true;
	post("FaceDetector enabled");
    }
    
    public void stop() {
	enable = false;
	post("FaceDetector disabled");
    }
    
    private byte[] detectSkin(double[] hsv, int w, int h) {
    	byte[] positive = new byte[w*h];
    	double[] temp = new double[4];
	int idx;
    	for (int i = 0;i < h; i++) {
	    for (int j = 0;j < w; j++) {		
		idx = i*w+j;
		temp[0] = hsv[idx*4 + 1];
		temp[1] = hsv[idx*4 + 2];
		temp[2] = hsv[idx*4 + 3];
		double s=cs.getScore(temp);
		if (s > MARGIN) {
		    positive[idx] = 1;
		}
		else {
		    positive[idx] = 0;
		}
	    }
    	}
    	return positive;
    }

    
    public void jit_matrix(String s) {

	jm.frommatrix(s);
	
	num_faces = 0;

	if (enable && !detecting) {

	    jm.setType("float32");
	    jm.copyMatrixToArray(hsv);

	    Thread t = new Thread() {
		public void run() {
	    
		    detecting = true; 
		    
		    /* Extract V  */
		    for (int i=0; i < raw.length; i++) {
			raw[i] = (byte)(hsv[i*4+3] * 255);
		    }
	    
		    /* Detect Skin */
		    byte[] skin = detectSkin(hsv, image_width, image_height);
		    
		    IntegralImage skin_ii = 
			new IntegralImage(skin, image_width, image_height, false);
		    IntegralImage  ii = 
			new IntegralImage(raw, image_width, image_height, true);
		    
		    if (current_mode == SCAN_MODE) {
			
			for (int i=0;i<num_candidates_scan;i++) {
			    
			    if (num_faces >= MAX_NUM_FACES) break;
			    Candidate c = candidates_scan[i];				
			    
			    // speed hack
			    boolean abort = false;
			    for (int j=0;j<num_faces;j++) {
				Candidate f = faces[j];
				if (f.isContaining(c) || (c.size < f.size && f.isOverlap(c))) {
				    abort = true;
				    break;
				}
			    }
			    if (abort) continue;
			    
			    if (cc.processCandidate(c, ii, skin_ii)) {
				
				int updated = 0;          		
				
				for (int j=0;j<num_faces;j++) {
				    Candidate f = faces[j];
				    if (f.isOverlap(c)) {
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
				
				if (updated == 0) faces[num_faces++] = c;		   
			    }   
			}
			
		    }
		    else if (current_mode == TRACK_MODE) {
			
                        // use candidates_scan instead for now
			for (int i=0;i<num_candidates_scan;i++) {
			    
			    if (num_faces >= MAX_NUM_FACES) break;
			    Candidate c = candidates_scan[i];				
			    
			    // speed hack
			    boolean abort = false;
			    for (int j=0;j<num_faces;j++) {
				Candidate f = faces[j];
				if (f.isContaining(c) || (c.size < f.size && f.isOverlap(c))) {
				    abort = true;
				    break;
				}
			    }
			    if (abort) continue;
			    
			    
			    if (cc.processCandidate(c, ii, skin_ii)) {
				
				int updated = 0;          		
				
				for (int j=0;j<num_faces;j++) {
				    Candidate f = faces[j];
				    if (f.isOverlap(c)) {
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
				
				if (updated == 0) faces[num_faces++] = c;		   
			    }   
			}	    
		    }	
		    		    
		    for (int i=0; i< num_faces;i++) {
			faces_out[i] = faces[i];
		    }

		    num_faces_out = num_faces;

		    Atom[] face_array = new Atom[num_faces_out*4];
		    Long current_time = new Long(System.currentTimeMillis());
		    
		    for (int i=0;i < num_faces_out;i++) {
			Candidate c = faces[i];
			face_array[i*4] = Atom.newAtom(new Integer(c.x * output_width / image_width));
			face_array[i*4+1] = Atom.newAtom(new Integer(c.y * output_height / image_height));
			face_array[i*4+2] = Atom.newAtom(new Integer(c.size * output_height / image_height));
			face_array[i*4+3] = Atom.newAtom(current_time.floatValue());
		    }
		    
		    outlet(0,"face",face_array);

		    detecting = false;
		}
	    };

	    t.start();

	}
	else {
	    Atom[] face_array = new Atom[num_faces_out*4];
	    Long current_time = new Long(System.currentTimeMillis());

	    for (int i=0;i < num_faces_out;i++) {
		Candidate c = faces[i];
		face_array[i*4] = Atom.newAtom(new Integer(c.x * output_width / image_width));
		face_array[i*4+1] = Atom.newAtom(new Integer(c.y * output_height / image_height));
		face_array[i*4+2] = Atom.newAtom(new Integer(c.size * output_height / image_height));
		face_array[i*4+3] = Atom.newAtom(current_time.floatValue());
	    }
		    
	    outlet(0,"face",face_array);

	}

    }

}
