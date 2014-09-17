import com.cycling74.max.*;
import com.cycling74.jitter.*;
import java.net.*;
import java.awt.image.BufferedImage;
//import java.awt.RenderingHints;
//import java.awt.Graphics2D;
import HOG.*;
import shared.*;
import skin.*;


public class MaxNewFaceDetector extends MaxObject {

    private final int SCAN_MODE = 0;
    private final int TRACK_MODE = 1;

    private final int[] box_color = {255, 255, 255, 0}; // Yellow in ARGB 

    private final static int[] sizes = new int[] 
    {40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 144, 160};
    
    public double threshold = 8d; // default detection threshold
   
    private JitterMatrix fullres_jm, lowres_jm, face_jm, skin_jm;
    private int fullres_width, fullres_height, lowres_width, lowres_height;

    private BufferedImage current_frame;

    private double[] skinScore;

    private final int MAX_NUM_FACES = 2; // maximum no. of faces to detect
    private Candidate[] faces = new Candidate[MAX_NUM_FACES];
    private int num_faces = 0;

    private Face_Scorer scorer;
    private Skin_Detector sd;

    private int track_x, track_y, track_size;
    private int current_mode;

    private boolean detecting_face, detecting_skin, enable;

    private long start;

    private void turnOffAGC() {
	Thread t = new SendCommandThread("ExpMode=manual");
	t.start();
	post("AGC = off");
    }

    private void turnOnAGC() {
	Thread t = new SendCommandThread("ExpMode=full");
	t.start();
	post("AGC = on");
    }


    public MaxNewFaceDetector(String xml, String xml_color) {

	Authenticator.setDefault (new MyAuthenticator ("cameraman","cameraman"));

	try {
	    post("FD: Loading face-detection features " + xml);
	    scorer = new Face_Scorer(xml);
	
	    post("FD: Loading skin-color features " + xml_color);
	    sd = new Skin_Detector(xml_color);
	}
	catch (Exception e) {
	    e.printStackTrace();
	}
	fullres_jm = new JitterMatrix();
	lowres_jm  = new JitterMatrix();

	detecting_face = false;
	detecting_skin = false;
	enable = true;

	turnOnAGC();
	current_mode = SCAN_MODE;

	declareIO(2,3);
	declareAttribute("threshold");
    }

    public void start() {
	enable = true;
    }

    public void stop() {
	enable = false;
    }

    public void mode(int[] l) {
	if (l.length > 0) {
	    if (l[0] == SCAN_MODE && current_mode != SCAN_MODE) {
		post("FD: Face detector is now in scan mode");
		//turnOnAGC();
		current_mode = l[0];
	    }
	    else if (l[0] == TRACK_MODE){
		
		if (current_mode != TRACK_MODE) {
		    post("FD: Face detector is now in tracking mode");
		    //turnOffAGC();
		    current_mode = l[0];
		}
		
		track_x    = l[1]*lowres_width/fullres_width;
		track_y    = l[2]*lowres_height/fullres_height;
		track_size = l[3]*lowres_width/fullres_width;		

		post("FD: tracking ... (" + l[1] + "," + l[2] + "," + l[3] + ")" );
	    }
	}
	else 
	    post("FD: Invalid mode");
    }
    

    private void drawBox(int line_width) {

	int[] offset = new int[2];
	int[] row = new int[fullres_width * 4];

	for (int j=0;j<num_faces;j++) {
	    int x = faces[j].x * fullres_width / lowres_width;
	    int y = faces[j].y * fullres_height / lowres_height;
	    int size = faces[j].size * fullres_width / lowres_width;

	    // out of bound, dont draw
	    if (x < 0 || x+size >= fullres_width ||
		y < 0 || y+size >= fullres_height) {
		continue;
	    }

	    int xx,yy;

	    for (int k=0;k<line_width;k++) {
		
		// top
		offset[0] = 0;
		offset[1] = y+k;
		
		face_jm.copyVectorToArray(0,offset,row,fullres_width*4,0);
		for (int i=0;i<size;i++) {
		    xx = x+i;
		    row[4*xx]   = box_color[0];
		    row[4*xx+1] = box_color[1];
		    row[4*xx+2] = box_color[2];
		    row[4*xx+3] = box_color[3];
		}
		face_jm.copyArrayToVector(0,offset,row,fullres_width*4,0);

		// bottom
		offset[0] = 0;
		offset[1] = y+size-1-k;
		
		face_jm.copyVectorToArray(0,offset,row,fullres_width*4,0);
		for (int i=0;i<size;i++) {
		    xx = x+i;
		    row[4*xx]   = box_color[0];
		    row[4*xx+1] = box_color[1];
		    row[4*xx+2] = box_color[2];
		    row[4*xx+3] = box_color[3];
		}
		face_jm.copyArrayToVector(0,offset,row,fullres_width*4,0);

	    }

	    // left & right
	    for (int i=0;i<size;i++) {
		offset[0] = 0;
		offset[1] = y+i;
		face_jm.copyVectorToArray(0,offset,row,fullres_width*4,0);
		
		for (int k=0;k<line_width;k++) {
		    for (int p=0;p<4;p++) {
			row[(x+k)*4 + p] = box_color[p];
			row[(x+size-1-k)*4 + p] = box_color[p];
		    }
		}
		
		face_jm.copyArrayToVector(0,offset,row,fullres_width*4,0);
	    }    
	}
    }
    
    public void jit_matrix(String s) {

	int inlet_no = getInlet();

	// full res: 640 x 480
	if (inlet_no == 0) {
	    
	    fullres_jm.frommatrix(s);
	    
	    int[] dim = fullres_jm.getDim();
	    if (dim[0] != fullres_width || dim[1] != fullres_height) {
		fullres_width = dim[0];
		fullres_height = dim[1];
		face_jm = new JitterMatrix(4,"char",fullres_width,fullres_height);
	    }

	    face_jm.frommatrix(s);

	    // draw boxes around faces
	    if (num_faces > 0) {
		drawBox(4);
	    }
	    
	    outlet(0,"jit_matrix",face_jm.getName());	    

	}

	// low res: 320 x 240
	else if (inlet_no == 1) {


	    lowres_jm.frommatrix(s);

	    int[] dim = lowres_jm.getDim();
	    if (dim[0] != lowres_width || dim[1] != lowres_height) {
		lowres_width = dim[0];
		lowres_height = dim[1];
		skin_jm = new JitterMatrix(1,"float32",lowres_width,lowres_height);
		skinScore = new double[dim[0] * dim[1]];
	    }

	    current_frame = lowres_jm.createBufferedImage();
	    
	    /*
	    if (!processing_input) {
		processing_input = true; 
		Thread t = new InputThread();
		t.start();
	    }
	    */

	    if (!detecting_skin && enable) {
		detecting_skin = true; 
		Thread t = new SkinThread();
		t.start();
	    }

	    if (!detecting_face && enable) {
		detecting_face = true;
		Thread t = new FaceThread();
		t.start();		
	    }

	    // output face event
	    if (num_faces > 0) {
		Atom[] face_array = new Atom[num_faces*4];
		Long current_time = new Long(System.currentTimeMillis());
		for (int i=0;i < num_faces;i++) {
		    Candidate c = faces[i];
		    face_array[i*4] = Atom.newAtom(
			new Integer(c.x * fullres_width / lowres_width));
		    face_array[i*4+1] = Atom.newAtom(
			new Integer(c.y * fullres_height /  lowres_height));
		    face_array[i*4+2] = Atom.newAtom(
			new Integer(c.size * fullres_height /  lowres_height));
		    face_array[i*4+3] = Atom.newAtom(current_time.floatValue());
		}
		outlet(2,"face",face_array);
	    }

	    
	}
   
    }

    /*
    private BufferedImage rescale(BufferedImage img, int w, int h) {
	
	Object hint = RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR;
	
	BufferedImage tmp = new BufferedImage(w,h,BufferedImage.TYPE_INT_RGB);
	Graphics2D g2 = tmp.createGraphics();
	g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION, hint);
	g2.drawImage(tmp,0,0,w,h,null);
	g2.dispose();
	return tmp;

    }
    */

    private class FaceThread extends Thread {
    public void run() {
	
	//while (true) {

	long start = System.currentTimeMillis();

	Candidate[] temp_faces = new Candidate[MAX_NUM_FACES];
	int num_temp_faces = 0;

	int fw = lowres_width;
	int fh = lowres_height;

        // create integral images
	Skin_IntegralImage skin_ii = new Skin_IntegralImage(skinScore, fw, fh);
	HOG_IntegralImage hog_ii = HOG_Core.getHOGIntegralImage(current_frame);

	if (current_mode == SCAN_MODE) {

	detect:
	    for (int i=sizes.length-1; i >= 0; i--) {
		int SS = sizes[i]/sizes[0];

		// too many faces found
		if (num_temp_faces >= MAX_NUM_FACES){
		    break detect;
		}


		for (int y=0; y+sizes[i] < fh; y+= 3+SS ) {
		    for (int x=0; x+sizes[i] < fw; x+= 3+SS ) {
						
			Candidate c = new Candidate(x,y,sizes[i]);
			
			// skip windows with low skin color 
			double average_skin_score = 
			    skin_ii.getSum(1,c.x,c.y,c.size,c.size) / (c.size*c.size);

			if (average_skin_score < 0.1d) {
			    continue;
			}

                        // speed hack
			boolean skip = false;
			for (int j=0;j<num_temp_faces;j++) {
			    Candidate f = temp_faces[j];
			    if (f.isContaining(c)) {
				skip = true;
				break;
			    }
			}
			if (skip) continue;
			

			HOG_Cache hogcache = new HOG_Cache(c,hog_ii);
			Skin_Cache skincache = new Skin_Cache(c,skin_ii);
					      
			double score = scorer.getCandidateScore(c,skincache,hogcache,null);
			
			if (score > threshold) {

			    // weight by score and its size
			    c.weight = (int)score;
			    if (c.weight < 1) c.weight = 1;

			    c.weight = c.weight * c.size;

			    // combine detections
			    boolean updated = false;          		
			    for (int j=0;j<num_temp_faces;j++) {
				Candidate f = temp_faces[j];
				if (f.isSameDetection(c)) {

				    // average
				    /*
				    f.x = (f.weight * f.x + c.weight * c.x)
					/ (f.weight + c.weight);
				    f.y = (f.weight * f.y + c.weight * c.y)
					/ (f.weight + c.weight);
				    f.size = (f.weight * f.size + c.weight * c.size)
					/ (f.weight + c.weight);
				    f.weight = f.weight + c.weight;
				    */
				    
				    // Union
				    f.x = Math.min(f.x,c.x);
				    f.y = Math.min(f.y,c.y);

				    int temp_size_1 = Math.max(f.x+f.size,c.x+c.size) -
					Math.min(f.x,c.x);
				    int temp_size_2 = Math.max(f.y+f.size,c.y+c.size) -
					Math.min(f.y,c.y);

				    f.size = Math.min(temp_size_1,temp_size_2);

				    updated = true;
				    break;
				}
			    }

			    // a new face found
			    if (!updated && num_temp_faces < MAX_NUM_FACES) 
				temp_faces[num_temp_faces++] = c;		   
			}
		    }
		}
	    }
	}
	else if (current_mode == TRACK_MODE) {
	    
	    int size_step = 8, size_range = track_size / 2;
	    int y_range = track_size;
	    int x_range = track_size;

	    Candidate[] track_faces = new Candidate[MAX_NUM_FACES * 2];
	    int num_track_faces = 0;
   
	    for (int current_size = track_size+size_range; 
		 current_size >= track_size-size_range; 
		 current_size -= size_step) {
		
		if (current_size > fh) continue;
		if (current_size < sizes[0]) break;
		
		for (int yy = track_y-y_range; 
		     yy <= track_y+y_range; 
		     yy += 1+current_size/sizes[0]) {

		    if (yy < 0) continue;
		    if (yy + current_size >= fh) break;
		    
		    for (int xx = track_x-x_range; 
			 xx <= track_x+x_range; 
			 xx += 1+current_size/sizes[0]) {

			if (xx < 0) continue;
			if (xx + current_size >= fw) break;
			
			Candidate c = new Candidate(xx,yy,current_size);
						
			HOG_Cache hogcache = new HOG_Cache(c,hog_ii);
			Skin_Cache skincache = new Skin_Cache(c,skin_ii);
			
			// skip windows with low skin color 
			double average_skin_score = 
			    skin_ii.getSum(1,c.x,c.y,c.size,c.size) / (c.size*c.size);

			if (average_skin_score < 0.1d) {
			    continue;
			}
			
			double score = scorer.getCandidateScore(c,skincache,hogcache,null);

			if (score > 5d) {

			    c.weight = (int)score;
			    if (c.weight < 1) c.weight = 1;
			    
			    // vector is not yet full. just add.
			    if (num_track_faces < 2*MAX_NUM_FACES) {
				track_faces[num_track_faces] = c;
				num_track_faces++;
			    }
			    else {
				//replace face with min score with new face
				int min = track_faces[0].weight;
				int idx = 0;
				for (int j=0;j<num_track_faces;j++) {
				    // find index to replace
				    if (track_faces[j].weight < min) {
					idx = j;
					min = track_faces[j].weight;
				    }
				}
				track_faces[idx] = c;
			    }

			}
		
		    }
		
		}
	
	    }
	    
	    // merge faces using their weights
	    for (int i=0;i<num_track_faces;i++) {
		Candidate c = track_faces[i];
		boolean updated = false;          		
		for (int j=0;j<num_temp_faces;j++) {
		    Candidate f = temp_faces[j];
		    if (f.isOverlap(c) || c.isOverlap(f)) {
			f.x = (f.weight * f.x + c.weight * c.x)
			    / (f.weight + c.weight);
			f.y = (f.weight * f.y + c.weight * c.y)
			    / (f.weight + c.weight);
			f.size = (f.weight * f.size + c.weight * c.size)
			    / (f.weight + c.weight);
			f.weight = f.weight + c.weight;
			updated = true;
			break;
		    }
		}
		// if still have room, add new face
		if (!updated && num_temp_faces < MAX_NUM_FACES) 
		    temp_faces[num_temp_faces++] = c;		   
	    }
	
	    // this mode, we only output 1 face. output face with max score
	    if (num_temp_faces > 0) {
		int max_score = 0;
		int idx = 0;
		for (int i=0;i<num_temp_faces;i++) {
		    if (temp_faces[i].weight > max_score) {
			idx = i;
			max_score = temp_faces[i].weight;
		    }
		}
		if (idx > 0) temp_faces[0] = temp_faces[idx];
		num_temp_faces = 1;
	    }

	}	
	    
	// update faces_out
	for (int i=0; i < num_temp_faces;i++) {
	    faces[i] = temp_faces[i];
	}
	num_faces = num_temp_faces;

	// free memory
	temp_faces = null;
	hog_ii = null;
	skin_ii = null;

	//post("FD: detection time = " + (System.currentTimeMillis()-start));

	detecting_face = false;
	//}
    }
    }

    private class SkinThread extends Thread {
    public void run() {
	
	//while (true) {

	int fw = lowres_width;
	int fh = lowres_height;
	
	int[] rgbs = new int[fw * fh];
	current_frame.getRGB(0, 0,fw, fh, rgbs, 0, fw);

	// downsample: S x S
	int S = 4;
	int smaller_fw = fw / S;
	int smaller_fh = fh / S;

	int[] smaller_rgbs = new int[3 * smaller_fw * smaller_fh];
	for (int j=0;j<smaller_fh;j++) {
	    for (int i=0;i<smaller_fw;i++) {
		int r=0,g=0,b=0;
		for (int p=0;p<S;p++) {
		    for (int q=0;q<S;q++) {
			r += (rgbs[(j*S+p)*fw+(i*S+q)]>>16) & 0xff;
			g += (rgbs[(j*S+p)*fw+(i*S+q)]>>8) & 0xff;
			b += (rgbs[(j*S+p)*fw+(i*S+q)]) & 0xff;
		    }
		}
		r /= S*S;
		g /= S*S;
		b /= S*S;
		smaller_rgbs[3*(j*smaller_fw + i)]   = r;
		smaller_rgbs[3*(j*smaller_fw + i)+1] = g;
		smaller_rgbs[3*(j*smaller_fw + i)+2] = b;
	    }
	}  


        /*
	BufferedImage smaller = rescale(current_frame,smaller_fw,smaller_fh);
	int[] smaller_rgbs = new int[smaller_fw * smaller_fh];
	smaller.getRGB(0, 0,smaller_fw, smaller_fh, smaller_rgbs, 0, smaller_fw);
	*/
	
	
	// change color space
	int l = smaller_fw * smaller_fh;
	double[] hsv = new double[l*3];
	double[] temp_hsv = new double[3];

	for (int j=0; j<l; j++) {
	    int r = smaller_rgbs[3*j];
	    int g = smaller_rgbs[3*j+1];
	    int b = smaller_rgbs[3*j+2];
	    Util.rgb2hsv(r, g, b, temp_hsv);
	    hsv[j*3]   = temp_hsv[0];
	    hsv[j*3+1] = temp_hsv[1];
	    hsv[j*3+2] = temp_hsv[2];
	}

	// Detect Skin
	double[] tempScore = sd.getScoreVector(hsv, smaller_fw, smaller_fh);

        // upsample: S x S
	double[] largerTemp = new double[fh*fw];
	for (int j=0;j<fh;j++) {
	    for (int i=0;i<fw;i++) {
		largerTemp[j*fw+i] = tempScore[(j/S)*smaller_fw + (i/S)];
	    }
	}  
	
	double[] smooth = new double[fh*fw];
	
	// smoothing
	for (int i=0;i<fh;i++) {
	    smooth[i*fw] = largerTemp[i*fw];
	    smooth[i*fw+(fw-1)] = largerTemp[i*fw+(fw-1)];
	}
	for (int j=0;j<fw;j++) {
	    smooth[j] = largerTemp[j];
	    smooth[(fh-1)*fw+j] = largerTemp[(fh-1)*fw+j];
	}
	
	for (int i = 1; i < fh-1; i++) {
	  for (int j = 1; j < fw-1;j++) {
		
		//
		//  weight mask
		//
		//    1.0 1.0 1.0 
		//    1.0 2.0 1.0
		//    1.0 1.0 1.0
		//
		
		double temp = largerTemp[(i-1)*fw+(j-1)] +
		    largerTemp[(i-1)*fw+j] +
		    largerTemp[(i-1)*fw+j+1] +
		    largerTemp[i*fw+j-1] +
		    2 * largerTemp[i*fw+j] +
		    largerTemp[i*fw+j+1] +
		    largerTemp[(i+1)*fw+j-1] +
		    largerTemp[(i+1)*fw+j] +
		    largerTemp[(i+1)*fw+j+1];
		
		smooth[i*fw+j] = temp / 10d;
	  }
	} 
	
	skinScore = smooth;
	
	// output skin
	skin_jm.copyArrayToMatrix(skinScore);
	outlet(1,"jit_matrix",skin_jm.getName());

	// free memory
	rgbs = null;
	smaller_rgbs = null;
	hsv = null;
	tempScore = null;
	largerTemp = null;

	detecting_skin = false;
	//}
    }
    }
   
    private class SendCommandThread extends Thread {
        String command;
        public SendCommandThread(String cmd) {
            command = cmd;
        }

        public void run() {
            try {
                URL inq = new URL("http://192.168.1.11/command/camera.cgi?" + command);
                URLConnection inqc = inq.openConnection();
                inq.openStream();
            }
            catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    private class MyAuthenticator extends Authenticator {
        private String username, password;

        public MyAuthenticator(String user, String pass) {
            username = user;
            password = pass;
        }

        protected PasswordAuthentication getPasswordAuthentication() {
            //post("Requesting Host  : " + getRequestingHost());
            //post("Requesting Port  : " + getRequestingPort());
            //post("Requesting Prompt : " + getRequestingPrompt());
            //post("Requesting Protocol: " + getRequestingProtocol());
            //post("Requesting Scheme : " + getRequestingScheme());
            //post("Requesting Site  : " + getRequestingSite());
            return new PasswordAuthentication(username, password.toCharArray());
        }
    }

}
