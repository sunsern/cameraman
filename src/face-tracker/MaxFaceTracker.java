import com.cycling74.max.*;
import com.cycling74.jitter.*;
import java.awt.image.BufferedImage;
import facealign.*;
import facetracker.*;
import particlefilter.*;
import facerecognition.*;
import shared.*;
import java.util.Arrays;

public class MaxFaceTracker extends MaxObject {

    private final int[] sizes = new int[]                                
    {24, 32, 40, 48, 56, 64, 72, 80, 88, 96, 104, 112, 120, 128, 136, 144, 152, 160 };  

    private final double varianceThreshold = 500;

    private final double threshold = 12d; // default detection threshold

    private final int DETECTOR_SLEEP_TIME = 250;

    private final int[] RED    = new int[] {255,255,0,0}; 
    private final int[] GREEN  = new int[] {255,0,255,0}; 
    private final int[] YELLOW = new int[] {255,255,255,0}; 
    private final int[] WHITE  = new int[] {255,255,255,255}; 

    private final int RESIZE_FACTOR = 2;

    private JitterMatrix fullres_jm, lowres_jm, skin_jm;
    private int fullresWidth, fullresHeight, lowresWidth, lowresHeight;

    private int[] offset = new int[2];
    private int[] row;

    private FaceRecognizer faceRecognizer;
	        
    private FaceState lastFaceState;
    private double lastScore, lastVariance;

    private FaceScorer scorer, scorerFullSize;
    private FaceScorerMask mask;
    private FaceTracker faceTracker;
  
    private boolean recognizerRunning, hasRecognized;
    private boolean enable, hasFace, trackerRunning, detectorRunning;
    private int noFaceTime;
    
    private final int MAX_NUM_FACES = 2; // maximum no. of faces to detect      
    private Candidate[] faces = new Candidate[MAX_NUM_FACES];
    private int num_faces = 0;   

    private Thread trackerThread, detectorThread, recognizerThread; 
    
    private boolean initializeCmd = false;
        
    private boolean moveCmd = false;
    private int moveX, moveY;
    private long moveTime;

    private boolean concentrateCmd = false;
    private int concentrateX, concentrateY, concentrateSize;
    private long concentrateTime;

    public MaxFaceTracker(String xml, String xml_color) {

	post("FD: Loading face-detection features " + xml);
	post("FD: Loading skin-color features " + xml_color);

	scorer = new FaceScorer(xml, xml_color);
	scorerFullSize = new FaceScorer(xml, xml_color);
	mask = new FaceScorerMask(
	    "/Users/cameraman/cameraman/face-tracker/mean_img_small.png");

	lowresWidth = 320;
	lowresHeight = 240;
	
	BufferedImage currentFrame = new BufferedImage(lowresWidth,lowresHeight,
						       BufferedImage.TYPE_INT_ARGB);

	scorer.loadImage(currentFrame, RESIZE_FACTOR);
	
	skin_jm = new JitterMatrix(1,"float32",lowresWidth/RESIZE_FACTOR,
				   lowresHeight/RESIZE_FACTOR);

	faceTracker = new FaceTracker(scorer, lowresWidth, lowresHeight,
				      new TrackerConfig());
	faceTracker.setInitialFace(lowresWidth/2,lowresHeight/2,lowresWidth/8);

	faceRecognizer = new FaceRecognizer(
	    "/Users/cameraman/cameraman/face-tracker/FaceRecognition/mean.csv",
	    "/Users/cameraman/cameraman/face-tracker/FaceRecognition/eigenfaces.csv");

	fullresWidth = 640;
	fullresHeight = 480;
	row = new int[fullresWidth * 4];

	lastFaceState = null;
	lastScore = -100;

	fullres_jm = new JitterMatrix();
	lowres_jm  = new JitterMatrix();

	trackerThread = new TrackerThread();
	detectorThread = new DetectorThread();
	//recognizerThread = new RecognizerThread();

	trackerThread.start();
	detectorThread.start();
	//recognizerThread.start();

	hasRecognized = false;
	hasFace = false;
	enable = false;

	trackerRunning = false;
	detectorRunning = false;
	recognizerRunning = false;

	declareIO(2,4);
    }

    public void verify() {
// 	post("verify recieved!");
	face_start();
    }


    public void user_lost() {
// 	post("user_lost recieved!");
	face_stop();
    }

    public void face_start() {
	outlet(3, "set", new Atom[]{Atom.newAtom("")});
	reset();
	enable = true;
    }

    public void face_stop() {
	outlet(3, "set", new Atom[]{Atom.newAtom("")});
	enable = false;
    }

    public void reset() {
	//faceRecognizer.reset();
	//hasRecognized = false;
	hasFace = false;
	initializeCmd = true;
    }

    public void anything(String msg, Atom[] args) {
	post("Unhandled message: " + msg);	
    }

    private void moveParticles(int x, int y) {
	moveCmd = true;
	moveX = -(x*lowresWidth / fullresWidth);
	moveY = -(y*lowresHeight / fullresHeight);
	moveTime = System.currentTimeMillis();
    }

    public void camera_move(long[] p) {
// 	post("camera_move received");
	if (p != null && p.length == 2) {
	    moveParticles((int)p[0],(int)p[1]);
	}
    }

    private void drawBox(int line_width) {

	FaceState fs = lastFaceState;

	if (fs != null) {

	    Particle[] particleVector = faceTracker.pv.getParticles();   
	    
	    for (int j=0;j<particleVector.length;j++) {
		FaceParticle fp = (FaceParticle)particleVector[j];
		if (!fp.isFixedParticle()) {
		    FaceState s = (FaceState)fp.getCurrentState();
		    double[] temp = s.getStateVector();
		    int x = (int)temp[0] * fullresWidth / lowresWidth;
		    int y = (int)temp[1] * fullresHeight / lowresHeight;
		    fullres_jm.setcell(new int[] {x,y}, RED);
		    fullres_jm.setcell(new int[] {x+1,y}, RED);
		    fullres_jm.setcell(new int[] {x,y+1}, RED);
		    fullres_jm.setcell(new int[] {x+1,y+1}, RED);
		}
	    }
	    
	    // read last location
	    double[] vec = fs.getStateVector();
	    int x = (int)(vec[0] - vec[2]/2);
	    int y = (int)(vec[1] - vec[2]/2);
	    int size = (int)(vec[2]);
	
	    // scale
	    x = x * fullresWidth / lowresWidth;
	    y = y * fullresHeight / lowresHeight;
	    size = size * fullresWidth / lowresWidth;

	    int xx,yy;
	    
	    int[] box_color;
	    if (lastScore > 20d) box_color = RED;
	    else if (lastScore > 12d) box_color = GREEN;
	    else if (lastScore > 8d) box_color = YELLOW;
	    else box_color = WHITE;
	    

	    for (int k=0;k<line_width;k++) {
		
		if (y+k >= 0 && y+k < fullresHeight) {
   
		    // top
		    offset[0] = 0;
		    offset[1] = y+k;
	    
		    fullres_jm.copyVectorToArray(0,offset,row,fullresWidth*4,0);
		    for (int i=0;i<size;i++) {
			xx = x+i;
			if (xx >= 0 && xx < fullresWidth) {
			    row[4*xx]   = box_color[0];
			    row[4*xx+1] = box_color[1];
			    row[4*xx+2] = box_color[2];
			    row[4*xx+3] = box_color[3];
			}
		    }
		    fullres_jm.copyArrayToVector(0,offset,row,fullresWidth*4,0);
	    
		}

		if (y+size-1-k >= 0 && y+size-1-k < fullresHeight) {

		    // bottom
		    offset[0] = 0;
		    offset[1] = y+size-1-k;
		    
		    fullres_jm.copyVectorToArray(0,offset,row,fullresWidth*4,0);
		    for (int i=0;i<size;i++) {
			xx = x+i;
			if (xx >= 0 && xx < fullresWidth) {
			    row[4*xx]   = box_color[0];
			    row[4*xx+1] = box_color[1];
			    row[4*xx+2] = box_color[2];
			    row[4*xx+3] = box_color[3];
			}
		    }
		    fullres_jm.copyArrayToVector(0,offset,row,fullresWidth*4,0);
		    
		}
	    }

	    // left & right
	    for (int i=0;i<size;i++) {

		if (y+i >= 0 && y+i < fullresHeight) {

		    offset[0] = 0;
		    offset[1] = y+i;
		    fullres_jm.copyVectorToArray(0,offset,row,fullresWidth*4,0);
		    
		    for (int k=0;k<line_width;k++) {
			for (int p=0;p<4;p++) {
			    if (x+k >= 0 && x+k < fullresWidth) 
				row[(x+k)*4 + p] = box_color[p];
			    if (x+size-1-k >= 0 && x+size-1-k < fullresWidth)
				row[(x+size-1-k)*4 + p] = box_color[p];
			}
		    }
		    
		    fullres_jm.copyArrayToVector(0,offset,row,fullresWidth*4,0);

		}
	    }    
	}
    }
    

    public void jit_matrix(String s) {

	int inlet_no = getInlet();

	// full res: 640 x 480
	if (inlet_no == 0) {
	    
	    fullres_jm.frommatrix(s);
	 
	    int[] dim = fullres_jm.getDim();
	    if (dim[0] != fullresWidth || dim[1] != fullresHeight) {
		fullresWidth = dim[0];
		fullresHeight = dim[1];
		row = new int[fullresWidth * 4];
	    }
	    	    
	    if (enable) {
		drawBox(2);
	    }
	    
	    outlet(0,"jit_matrix",fullres_jm.getName());

	}

	// low res: 320 x 240
	else if (inlet_no == 1) {

	    lowres_jm.frommatrix(s);
	    
	    int[] dim = lowres_jm.getDim();
	    if (dim[0] != lowresWidth || dim[1] != lowresHeight) {
		lowresWidth = dim[0];
		lowresHeight = dim[1];
		skin_jm = new JitterMatrix(1,"float32",
					   lowresWidth/RESIZE_FACTOR,
					   lowresHeight/RESIZE_FACTOR);
		faceTracker = new FaceTracker(scorer, 
					      lowresWidth, 
					      lowresHeight,new TrackerConfig());
		faceTracker.setInitialFace(lowresWidth/2,
					   lowresHeight/2,
					   lowresWidth/8);
	    }
	    		
	    if (enable) {

		// output face event
		FaceState fs = lastFaceState;
		if (fs != null) {
		    double[] vec = fs.getStateVector();
		    int x = (int)(vec[0]-vec[2]/2);
		    int y = (int)(vec[1]-vec[2]/2);
		    int size = (int)(vec[2]);
		    if (lastVariance < varianceThreshold &&
			lastScore > 0 && 
			vec[0] >=0 && vec[0] < lowresWidth && 
			vec[1] >=0 && vec[1] < lowresHeight && 
			size > 0) {
			Atom[] face_array = new Atom[5];
			long ctime = System.currentTimeMillis();
			int top_time = (int)((ctime >> 32) & 0xffffffff);
			int bottom_time = (int)((ctime) & 0xffffffff);
			face_array[0] = Atom.newAtom((int)(x*fullresWidth/lowresWidth));
			face_array[1] = Atom.newAtom((int)(y*fullresHeight/lowresHeight));
			face_array[2] = Atom.newAtom((int)(size*fullresHeight/lowresHeight));
			face_array[3] = Atom.newAtom(top_time);
			face_array[4] = Atom.newAtom(bottom_time);
			outlet(2,"face",face_array);	

			// also trigger recognizer
                        /*
			if (!hasRecognized && !recognizerRunning) {
			    synchronized(recognizerThread) {
				recognizerThread.notify();
			    }
			}
			*/

		    }
		}
	    }

	    
	    if (!hasFace) {
		if (lastFaceState != null &&
		    lastVariance < varianceThreshold &&
		    lastScore > threshold) {
		    hasFace = true;
		    noFaceTime = 0;
		    post("face_acquired");
		    outlet(2,"face_acquired");
		}
	    }
	    else {
		if (lastVariance > varianceThreshold) {
		    noFaceTime++;
		    hasFace = false;
		    post("face_lost");
		    outlet(2,"face_lost");
		}
		if (noFaceTime > 15) {
		    hasFace = false;
		    post("face_lost");
		    outlet(2,"face_lost");
		}
	    }

	    if (enable && !detectorRunning) {
		synchronized(detectorThread) {
		    detectorThread.notify();
		}
	    }
	    

	    if (!trackerRunning) {
		outlet(1,"jit_matrix",skin_jm.getName());
		//post("setting skin matrix");
		synchronized(trackerThread) {
		    trackerThread.notify();
		}
	    }
	    
	}   
    }


    private double distanceFromCurrentFace(double x, double y, double size) {
	FaceState fs = lastFaceState;
	if (fs == null) {
	    return 0;
	}
	else {
	    double[] current = fs.getStateVector();
	    return (Math.pow(current[0]-x,2) + 
		    Math.pow(current[1]-y,2) +
		    Math.pow(current[2]-size,2));
	}
    }


    private void concentrateParticles(int x, int y, int size) {
	concentrateCmd = true;
	concentrateX = x;
	concentrateY = y;
	concentrateSize = size;
	concentrateTime = System.currentTimeMillis();
    }

    
    private class DetectorThread extends Thread {
    public void run() {

	while (true) {
	    
	    try {
		synchronized(this) {
		    this.wait();
		}
	    }
	    catch (Exception e) {
		post(Arrays.toString(e.getStackTrace()));
	    }


	    detectorRunning = true;

	    long start = System.currentTimeMillis();

	    Candidate[] temp_faces = new Candidate[MAX_NUM_FACES];
	    int num_temp_faces = 0;

	    BufferedImage currentFrame = lowres_jm.createBufferedImage();
	 
	    scorerFullSize.loadImage(currentFrame,2);
	
            detect:
	    for (int i=sizes.length-1; i >= 0; i--) {                           
		int SS = sizes[i]/sizes[0];                                     
		
		// too many faces found                                         
		if (num_temp_faces >= MAX_NUM_FACES){                           
		    break detect;                                               
		}                                                               
            
            
		for (int y=0; y+sizes[i] < lowresHeight; y+= 3+SS ) {                     
		    for (int x=0; x+sizes[i] < lowresWidth; x+= 3+SS ) {                 
			Candidate c = new Candidate(x,y,sizes[i]);
			
			double score = scorerFullSize.getScore(c);    
			
			if (score > threshold) {                  
			    // weight by score and its size                     
			    //c.weight = (int)score;                              
			    //if (c.weight < 1) c.weight = 1;                     
			    
			    //c.weight = c.weight * c.size;                       
			    
			    // combine detections                               
			    boolean updated = false;                            
			    for (int j=0;j<num_temp_faces;j++) {                
				Candidate f = temp_faces[j];                    
				if (f.isSameDetection(c)) {                     
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
	    
	    num_faces = num_temp_faces;
	    faces = temp_faces;
	    
	    //post("DetectorThread time: " + (System.currentTimeMillis() - start));
	    //post("Faces = " + num_faces);

	    if (initializeCmd) {
		if (num_faces > 0) {
		    int x = faces[0].x + faces[0].size/2;
		    int y = faces[0].y + faces[0].size/2;
		    concentrateParticles(x,y,faces[0].size);
		    initializeCmd = false;
		    moveCmd = false;
		}
	    }
	    else {
		// control logic
		if (lastVariance > varianceThreshold) {
		    //post("BAD: Tracker lost the face");
		    if (num_faces > 0) {
			//post(">set Tracker");
			//while (trackerRunning) {}
			int x = faces[0].x + faces[0].size/2;
			int y = faces[0].y + faces[0].size/2;
			concentrateParticles(x,y,faces[0].size);
			//faceTracker.concentrateParticles(x,y,faces[0].size);
		    }
		}
		else {
		    if (num_faces > 0) {
			double s = scorerFullSize.getScore(faces[0]);
			int x = faces[0].x + faces[0].size/2;
			int y = faces[0].y + faces[0].size/2;
			double d = distanceFromCurrentFace(x,y,faces[0].size);
			if (s > lastScore && d < 750) {
			    //post("ADJUST: Detector has a better face");
			    //post(">set Tracker");
			    //while (trackerRunning) {}
			    concentrateParticles(x,y,faces[0].size);
			    //faceTracker.concentrateParticles(x,y,faces[0].size);
			}
		    }
		    
		    if (lastScore < threshold) {
			//post("BAD: Tracker has low score face");
		    }
		    else {
			//post("GOOD: Tracker has a good face");
		    }
		}
		
	    }

	    try {
		sleep(DETECTOR_SLEEP_TIME);
	    }
	    catch (Exception e) {
		post(Arrays.toString(e.getStackTrace()));
	    }

	    detectorRunning = false;	    
	}

    }
    }


    private class TrackerThread extends Thread {
    public void run() {

	while (true) {
	  
	    try {
		synchronized(this) {
		    this.wait();
		}
	    }
	    catch (Exception e) {
		post(Arrays.toString(e.getStackTrace()));
	    }
	    	    
	    trackerRunning = true;

	    long startskin = System.currentTimeMillis();
	        
	    int[] boundary = faceTracker.getBoundary();

	    BufferedImage currentFrame = lowres_jm.createBufferedImage();
	    
	    // perform skin detect
	    scorer.loadImage(currentFrame,RESIZE_FACTOR,
			     boundary[0],
			     boundary[1],
			     boundary[2],
			     boundary[3]);

	    skin_jm.copyArrayToMatrix(scorer.skinScore);
	    //outlet(1,"jit_matrix",skin_jm.getName());

	    //post("skindetect time: " + (System.currentTimeMillis() - startskin));


	    // update tracker
	    if (enable && !initializeCmd) {

		long start = System.currentTimeMillis();

		if (concentrateCmd && moveCmd) {
		    if (concentrateTime > moveTime) {
			faceTracker.concentrateParticles(concentrateX,concentrateY,concentrateSize);
		    }
		    else {
			faceTracker.translateParticles(moveX,moveY);
		    }
		    concentrateCmd = false;
		    moveCmd = false;
		}
		else if (concentrateCmd) {
		    faceTracker.concentrateParticles(concentrateX,concentrateY,concentrateSize);
		    concentrateCmd = false;
		}
		else if (moveCmd) {
		    faceTracker.translateParticles(moveX,moveY);
		    moveCmd = false;
		}
		
		faceTracker.resampleAndUpdateWeights();
		lastFaceState = faceTracker.getWeightedAverageFaceState();
		lastVariance = faceTracker.getVariance();
		lastScore = scorer.getScore(lastFaceState);
		faceTracker.updateStates();
		
		//post("Tracker time: " + (System.currentTimeMillis() - start));
		//post("Variance = " + lastVariance);
	    }
	    else {
		lastFaceState = null;
	    }
		

	    trackerRunning = false;
	    
	}
    }
    }

    private class RecognizerThread extends Thread {
    public void run() {

	while (true) {
	  
	    try {
		synchronized(this) {
		    this.wait();
		}
	    }
	    catch (InterruptedException e) {
		post(Arrays.toString(e.getStackTrace()));
	    }
	    	    
	    recognizerRunning = true;
	    
	    FaceState fs = lastFaceState;
	    
	    if (fs != null && lastScore > threshold) {

		long start = System.currentTimeMillis();

		BufferedImage small = lowres_jm.createBufferedImage();
		mask.loadImage(small);
		
		double[] vec = fs.getStateVector();
		int cx = (int)(vec[0]);
		int cy = (int)(vec[1]);
		int csize = (int)(vec[2]);

		double max = -1000;
		int ns = csize;
		int nx = cx-ns/2;
		int ny = cy-ns/2;

		post("input");
		post(">x=" + nx);
		post(">y=" + ny);
		post(">s=" + ns);

		for (int s=10;s>=-10;s-=2) {
		    for (int i=-10;i<=10;i+=2) {
			for (int j=-10;j<=10;j+=2) {
			    int size = csize+s;
			    int x = (cx+i)-size/2;
			    int y = (cy+j)-size/2;
			    if (size > 0 && x >= 0 && y >= 0 &&
				x+size < 320 && y+size < 240) {
				double score = mask.getScore(x,y,size);
				if (score > max) {
				    nx = x;
				    ny = y;
				    ns = size;
				    max = score;
				}
			    }
			}
		    }	
		}
		
		post("output");
		post(">x=" + nx);
		post(">y=" + ny);
		post(">s=" + ns);
		post(">corr=" + max);

		
		if (max > 0.2) {
		    JitterMatrix temp = new JitterMatrix("temp");
		    BufferedImage large = temp.createBufferedImage();
		    
		    String p = faceRecognizer.recognize(large,nx*2,ny*2,ns*2);
		
		    if (p != null) {
			hasRecognized = true;
			outlet(3, "set", new Atom[]{Atom.newAtom(p)});
		    }
		}

		post("recognizer time: " + (System.currentTimeMillis() - start));

	    }
	    
	    recognizerRunning = false;
	    
	}
    }
    }


    public void notifyDeleted() {
	trackerThread = null;
	detectorThread = null;
	recognizerThread = null;
    }
   

}
