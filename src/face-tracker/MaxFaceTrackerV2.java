import com.cycling74.max.*;
import com.cycling74.jitter.*;
import java.awt.image.BufferedImage;

import core.nh.particlefilter.Particle;
import core.nh.videotracker.TargetDetector;
import core.nh.videotracker.TargetParticle;
import core.nh.videotracker.TargetState;
import core.nh.videotracker.TargetTracker;
import app.facetracker.FaceScorer;
import app.facetracker.FaceTrackerConfig;

public class MaxFaceTrackerV2 extends MaxObject {

    private final double ctThreshold = 100; // NormalHedge Ct threshold
    private final double threshold = 12d; // default detection threshold

    private final int DETECTOR_SLEEP_TIME = 250;

    private final int[] RED    = new int[] {255,255,0,0}; 
    private final int[] GREEN  = new int[] {255,0,255,0}; 
    private final int[] YELLOW = new int[] {255,255,255,0}; 
    private final int[] WHITE  = new int[] {255,255,255,255}; 

    private final int RESIZE_FACTOR = 4;

    private JitterMatrix fullres_jm, skin_jm;
    private int width, height;

    private int[] offset = new int[2];
    private int[] row;

    private TargetState lastFaceState;
    private double lastScore;
    private double lastCt;
    private double lastVariance;


    private FaceScorer scorer;
    private TargetTracker faceTracker;
    private TargetDetector faceDetector;
    private FaceTrackerConfig faceConfig;

    private boolean enable, hasFace, trackerRunning, detectorRunning;
    private int noFaceTime;
    
    private Thread trackerThread, detectorThread;
    
    private boolean initializeCmd = false;
        
    private boolean moveCmd = false;
    private int moveX, moveY;
    private long moveTime;

    private boolean concentrateCmd = false;
    private int concentrateX, concentrateY, concentrateSize;
    private double concentrateAmount;
    private long concentrateTime;

    public MaxFaceTrackerV2(String face_xml, String skin_xml) {

	post("FD: Loading face-detection features " + face_xml);
	post("FD: Loading skin-color features " + skin_xml);

	// setup scorer
	scorer = new FaceScorer(face_xml, skin_xml);

	width = 640;
	height = 480;
	
	BufferedImage currentFrame = new BufferedImage(width,height,
						       BufferedImage.TYPE_INT_ARGB);
	
	scorer.loadImage(currentFrame, RESIZE_FACTOR);
	
	// setup config
	faceConfig = new FaceTrackerConfig();

	// setup tracker
	faceTracker = new TargetTracker(scorer,width,height,faceConfig);
	
	// setup detector
	faceDetector = new TargetDetector(scorer);


	skin_jm = new JitterMatrix(1,"float32",width/RESIZE_FACTOR,
				   height/RESIZE_FACTOR);

	
	// initialize particles
	faceTracker.initialParticlesAt(width/2,height/2,width/8);
	
	row = new int[width * 4];

	lastFaceState = null;
	lastScore = scorer.getMinScore();
	lastCt = 10000;

	fullres_jm = new JitterMatrix();
	
	trackerThread = new TrackerThread();
	detectorThread = new DetectorThread();

	trackerThread.start();
	detectorThread.start();

	hasFace = false;
	enable = false;

	trackerRunning = false;
	detectorRunning = false;

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
	hasFace = false;
	initializeCmd = true;
    }

    public void anything(String msg, Atom[] args) {
	post("Unhandled message: " + msg);	
    }

    private void moveParticles(int x, int y) {
	moveCmd = true;
	moveX = -x;
	moveY = -y;
	moveTime = System.currentTimeMillis();
    }

    private void concentrateParticles(int x, int y, int size, double amount) {
	concentrateCmd = true;
	concentrateX = x;
	concentrateY = y;
	concentrateSize = size;
	concentrateAmount = amount;
	concentrateTime = System.currentTimeMillis();
    }

    public void camera_move(long[] p) {
// 	post("camera_move received");
	if (p != null && p.length == 2) {
	    moveParticles((int)p[0],(int)p[1]);
	}
    }

    private void drawBox(int line_width) {

	TargetState fs = lastFaceState;

	if (fs != null) {

	    // draw particles
	    Particle[] particleVector = faceTracker.pv.getParticles();   
	    for (int j=0;j<particleVector.length;j++) {
		TargetParticle fp = (TargetParticle)particleVector[j];
		TargetState s = (TargetState)fp.getCurrentState();
		double[] temp = s.getStateVector();
		int x = (int)temp[0];
		int y = (int)temp[1];
		fullres_jm.setcell(new int[] {x,y}, RED);
		fullres_jm.setcell(new int[] {x+1,y}, RED);
		fullres_jm.setcell(new int[] {x,y+1}, RED);
		fullres_jm.setcell(new int[] {x+1,y+1}, RED);		
	    }
	    
	    // read last location
	    double[] vec = fs.getStateVector();
	    int x = (int)(vec[0] - vec[2]/2);
	    int y = (int)(vec[1] - vec[2]/2);
	    int size = (int)(vec[2]);
	
	    // set box color
	    int[] box_color;
	    if (lastScore > 20d) box_color = RED;
	    else if (lastScore > 12d) box_color = GREEN;
	    else if (lastScore > 8d) box_color = YELLOW;
	    else box_color = WHITE;
	    
	    int xx,yy;
	    for (int k=0;k<line_width;k++) {
		
		if (y+k >= 0 && y+k < height) {
   
		    // top
		    offset[0] = 0;
		    offset[1] = y+k;
	    
		    fullres_jm.copyVectorToArray(0,offset,row,width*4,0);
		    for (int i=0;i<size;i++) {
			xx = x+i;
			if (xx >= 0 && xx < width) {
			    row[4*xx]   = box_color[0];
			    row[4*xx+1] = box_color[1];
			    row[4*xx+2] = box_color[2];
			    row[4*xx+3] = box_color[3];
			}
		    }
		    fullres_jm.copyArrayToVector(0,offset,row,width*4,0);
	    
		}

		if (y+size-1-k >= 0 && y+size-1-k < height) {

		    // bottom
		    offset[0] = 0;
		    offset[1] = y+size-1-k;
		    
		    fullres_jm.copyVectorToArray(0,offset,row,width*4,0);
		    for (int i=0;i<size;i++) {
			xx = x+i;
			if (xx >= 0 && xx < width) {
			    row[4*xx]   = box_color[0];
			    row[4*xx+1] = box_color[1];
			    row[4*xx+2] = box_color[2];
			    row[4*xx+3] = box_color[3];
			}
		    }
		    fullres_jm.copyArrayToVector(0,offset,row,width*4,0);
		    
		}
	    }

	    // left & right
	    for (int i=0;i<size;i++) {

		if (y+i >= 0 && y+i < height) {

		    offset[0] = 0;
		    offset[1] = y+i;
		    fullres_jm.copyVectorToArray(0,offset,row,width*4,0);
		    
		    for (int k=0;k<line_width;k++) {
			for (int p=0;p<4;p++) {
			    if (x+k >= 0 && x+k < width) 
				row[(x+k)*4 + p] = box_color[p];
			    if (x+size-1-k >= 0 && x+size-1-k < width)
				row[(x+size-1-k)*4 + p] = box_color[p];
			}
		    }
		    
		    fullres_jm.copyArrayToVector(0,offset,row,width*4,0);

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
	    if (dim[0] != width || dim[1] != height) {
		width = dim[0];
		height = dim[1];
		row = new int[width * 4];
	    }
	    
	    if (enable) {
		
		// draw
		drawBox(2);
		
		// output face event
		TargetState fs = lastFaceState;
		if (fs != null) {
		    double[] vec = fs.getStateVector();
		    int x = (int)(vec[0]-vec[2]/2);
		    int y = (int)(vec[1]-vec[2]/2);
		    int size = (int)(vec[2]);
		    if (lastCt < ctThreshold && lastScore > 0) {
			Atom[] face_array = new Atom[5];
			long ctime = System.currentTimeMillis();
			int top_time = (int)((ctime >> 32) & 0xffffffff);
			int bottom_time = (int)((ctime) & 0xffffffff);
			face_array[0] = Atom.newAtom((int)x);
			face_array[1] = Atom.newAtom((int)y);
			face_array[2] = Atom.newAtom((int)size);
			face_array[3] = Atom.newAtom(top_time);
			face_array[4] = Atom.newAtom(bottom_time);
			outlet(2,"face",face_array);	
		    }
		}
		
	    }
	    
	    outlet(0,"jit_matrix",fullres_jm.getName());

	    if (!hasFace) {
		if (lastFaceState != null &&
		    lastCt < ctThreshold &&
		    lastScore > threshold) {
		    hasFace = true;
		    noFaceTime = 0;
		    post("face_acquired");
		    outlet(2,"face_acquired");
		}
	    }
	    else {
		if (lastScore < threshold-8) {
		    noFaceTime++;
		    hasFace = false;
		    post("face_lost");
		    outlet(2,"face_lost");
		}
		else noFaceTime = 0;
		if (noFaceTime > 10) {
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
		synchronized(trackerThread) {
		    trackerThread.notify();
		}
	    }
	}
    }
    
    

    
    private class DetectorThread extends Thread {
    public void run() {

	while (true) {
	    
	    try {
		synchronized(this) {
		    this.wait();
		}
	    }
	    catch (InterruptedException e) {
	    }

	    detectorRunning = true;

	    long start = System.currentTimeMillis();

	    TargetState t = faceDetector.localizeTarget(fullres_jm.createBufferedImage(),threshold);

	    if (t != null) {

		post("t is not null");
		double[] face = t.getStateVector();
    
		if (initializeCmd) {
		    int x = (int)(face[0] + face[2]/2);
		    int y = (int)(face[1] + face[2]/2);
		    concentrateParticles(x,y,(int)face[2],1.0);
		    initializeCmd = false;
		    moveCmd = false;
		}
		else {
		    //
		    // control logic
		    //
		    if (lastScore < threshold-8) {
			//post("BAD: Tracker lost the face");
			int x = (int)(face[0] + face[2]/2);
			int y = (int)(face[1] + face[2]/2);
			concentrateParticles(x,y,(int)face[2],0.85);
		    }
		    else {
			int x = (int)(face[0] + face[2]/2);
			int y = (int)(face[1] + face[2]/2);
			concentrateParticles(x,y,(int)face[2],0.15);
		    }
		}
	    }
		
	    post("DetectorThread time: " + (System.currentTimeMillis() - start));
	    
	    try {
		sleep(DETECTOR_SLEEP_TIME);
	    }
	    catch (Exception e) {}
	    
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
	    catch (InterruptedException e) {
	    }
	    	    
	    trackerRunning = true;

	    long startskin = System.currentTimeMillis();
	        
	    scorer.loadImage(fullres_jm.createBufferedImage(),RESIZE_FACTOR);
    
	    //skin_jm.copyArrayToMatrix(scorer.skinScore);
	    //outlet(1,"jit_matrix",skin_jm.getName());

	    post("skindetect time: " + (System.currentTimeMillis() - startskin));

	    // update tracker
	    //if (enable && !initializeCmd) {
	    if (enable) {

		long start = System.currentTimeMillis();

		if (concentrateCmd && moveCmd) {
		    if (concentrateTime > moveTime) {
			if (concentrateAmount > 0.99) {
			    faceTracker.concentrateParticles(concentrateX,concentrateY,concentrateSize);
			}
			else {
			    faceTracker.spawnSomeAtNewLocation(concentrateX,concentrateY,concentrateSize,concentrateAmount);
			}
		    }
		    else {
			faceTracker.translateParticles(moveX,moveY);
		    }
		    concentrateCmd = false;
		    moveCmd = false;
		}
		else if (concentrateCmd) {
		    if (concentrateAmount > 0.99) {
			faceTracker.concentrateParticles(concentrateX,concentrateY,concentrateSize);
		    }
		    else {
			faceTracker.spawnSomeAtNewLocation(concentrateX,concentrateY,concentrateSize,concentrateAmount);
		    }
		    concentrateCmd = false;
		}
		else if (moveCmd) {
		    faceTracker.translateParticles(moveX,moveY);
		    moveCmd = false;
		}
		
		lastCt = faceTracker.resampleAndUpdateWeights();
		lastFaceState = faceTracker.getWeightedAverageState();
		lastVariance = faceTracker.getVariance();
		lastScore = scorer.getScore(lastFaceState);
		faceTracker.updateStates();
		
		post("Tracker time: " + (System.currentTimeMillis() - start));
		post("Variance = " + lastVariance);
	    }
	    else {
		lastFaceState = null;
	    }
		
	    trackerRunning = false;
	    
	}
    }
    }


    public void notifyDeleted() {
	trackerThread = null;
	detectorThread = null;
    }
   

}
