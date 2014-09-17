import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class StopBox extends MaxObject {
    
    private final int[] red = {255, 255, 0, 0};
    private final int[] blue = {255, 0, 0, 255};
    private final int[] black = {255, 0, 0, 0};

    public int box_x, box_y, box_size, default_x, default_y;
    private boolean showBox = false;
    public int[] color = red;

    // added by Sunsern
    private final int NUM_FRAME = 60;
    private float frame_score = 0f; // score of the current frame
    private float[] frame_score_hist = new float[NUM_FRAME]; // score of the last NUM_FRAME frames
    private int frame_score_hist_idx = 0;

    private JitterMatrix input_jm = new JitterMatrix();
    private JitterMatrix score_jm = new JitterMatrix();
    private int input_w, input_h, line_width;
    private int[] offset = new int[2];

    private float getFrameScore(int tx, int ty, int tsize) {
    	int[] dim = score_jm.getDim();
    	int planecount = score_jm.getPlanecount();
	float[] row = new float[dim[0] * planecount];
	float score;

    	int x = tx*dim[0]/input_w;
    	int y = ty*dim[1]/input_h;
    	int size = tsize*dim[1]/input_h;

	score = 0f;
	for(int j = 0; j<size; j++) {
	    offset[0] = 0;
	    offset[1] = y+j;
	    score_jm.copyVectorToArray(0, offset, row, dim[0]*planecount, 0);
	    for (int i=x; i<x+size; i++) {
		score += row[i*planecount]; 
	    }
	}
	return score / (size*size);
    }


    private boolean handInBox(int tx, int ty, int tsize) {
	float mean = 0f;
	for (int i=0;i<NUM_FRAME;i++) {
	    mean += frame_score_hist[i];
	}
	mean = mean / (float)NUM_FRAME;
	return (mean > 0f);
    }

    private void drawVertical(int x, int y, int size) {
	int[] dim = input_jm.getDim();
	int planecount = input_jm.getPlanecount();
    	int[] row = new int[dim[0] * planecount];

	for (int i=0;i<size;i++) {
            offset[0] = 0;
            offset[1] = y+i;
            input_jm.copyVectorToArray(0, offset, row, dim[0]*planecount, 0);
	    
            for (int k=0;k<line_width;k++) {
                for (int p=0;p<planecount;p++) {
                    row[(x+k)*planecount + p] = color[p];
                    row[(x+size-1-k)*planecount + p] = color[p];
                }
            }
	    
            input_jm.copyArrayToVector(0, offset, row, dim[0]*planecount, 0);
        }
    }

    private void drawTop(int x, int y, int len) {
    	int[] dim = input_jm.getDim();
    	int planecount = input_jm.getPlanecount();
    	int[] row = new int[dim[0] * planecount];
	
	for (int j=0;j<line_width;j++) {
	    offset[0] = 0;
	    offset[1] = y + j;

	    input_jm.copyVectorToArray(0, offset, row, dim[0]*planecount, 0);
	    for (int i=x; i<x+len; i++) {
		for (int p=0; p<planecount; p++) {
		    row[p+i*planecount] = color[p];
		}
	    }
	    
	    input_jm.copyArrayToVector(0, offset, row, dim[0]*planecount, 0);
	    
	}
    }

    private void drawBottom(int x, int y, int len) {
    	int[] dim = input_jm.getDim();
    	int planecount = input_jm.getPlanecount();
    	int[] row = new int[dim[0] * planecount];
	
	for (int j=0;j<line_width;j++) {
	    offset[0] = 0;
	    offset[1] = y - j;

	    input_jm.copyVectorToArray(0, offset, row, dim[0]*planecount, 0);
	    for (int i=x; i<x+len; i++) {
		for (int p=0; p<planecount; p++) {
		    row[p+i*planecount] = color[p];
		}
	    }
	    
	    input_jm.copyArrayToVector(0, offset, row, dim[0]*planecount, 0);
	    
	}
    }

    private void fillSquare(int x, int y, int size) {
    	int[] dim = input_jm.getDim();
    	int planecount = input_jm.getPlanecount();
    	int[] row = new int[dim[0] * planecount];
	
	for (int j=0;j<size;j++) {
	    offset[0] = 0;
	    offset[1] = y + j;

	    input_jm.copyVectorToArray(0, offset, row, dim[0]*planecount, 0);
	    for (int i=x; i<x+size; i++) {
		for (int p=0; p<planecount; p++) {
		    row[p+i*planecount] = color[p];
		}
	    }
	    
	    input_jm.copyArrayToVector(0, offset, row, dim[0]*planecount, 0);
	    
	}
    }

	    
    
    private void drawBox(int tx,int ty,int tsize) {
    	int[] dim = input_jm.getDim();
    	int planecount = input_jm.getPlanecount();
    	int[] row = new int[dim[0] * planecount];

    	int x = tx*dim[0]/input_w;
    	int y = ty*dim[1]/input_h;
    	int size = tsize*dim[1]/input_h;

	drawTop(x, y, size);
	drawBottom(x, y+size, size);
	drawVertical(x,y,size);

	int s = size / 2;
	fillSquare(x+s/2,y+s/2,s);

    }

    public void face(int[] l) {
	/*
	if (l.length >= 3) {
	    box_x = l[0] + l[2] +  100;
	    if (box_x > input_w) box_x = default_x;

	    box_y = l[1] + 30;
	    if (box_y+box_size > input_h) box_y = default_y;
	}
	*/
    }

    
    public StopBox(int d_x, int d_y, int b_x, int b_y, int b_size) {

	post("SB: Initializing StopBox");

    	input_w = d_x;
    	input_h = d_y;

	line_width = 4;

	box_x = b_x;
	default_x = b_x;
	box_y = b_y;
	default_y = b_y;
	box_size = b_size;

	showBox = false;
	
    	declareIO(2,3);

    }

    public void jit_matrix(String s) {

	int inlet_no = getInlet();
	
	// input video with face
  	if (inlet_no == 0) {
	    
	    input_jm.frommatrix(s);
	    
	    if (showBox) {
		drawBox(box_x, box_y, box_size);
		if (handInBox(box_x, box_y, box_size)) {
		    outlet(1, "stop_recording");
		}
	    }

	    outlet(0,"jit_matrix",input_jm.getName());
	}
	// skin score 
	else if (inlet_no == 1) {
	    score_jm.frommatrix(s);
	    frame_score = getFrameScore(box_x, box_y, box_size);
	    frame_score_hist[frame_score_hist_idx] = frame_score;
	    frame_score_hist_idx = (frame_score_hist_idx + 1) % NUM_FRAME; 
	    outlet(2,frame_score);
	}
    }

    public void start_recording() {	
	showBox = true;
    }

    public void stop_recording() {
	showBox = false;
    }

	    
    
}
