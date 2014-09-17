import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class MaxColorDetector extends MaxObject {
    
    private static float CLIP_SCORE_THRESHOLD = -5.0f;
    
    private float[] smooth_score;
    private float[] raw_score;
    private float[] input;
    private int[] output;
    private boolean[] positive;
    private int _p, _w, _h;
    private JitterMatrix jm,score_jm;
    private ColorScorer cs;
    private double margin;
    private int voting, smoothing;
    private boolean detecting;
    
    public MaxColorDetector(int w, int h, String xml_filename) {
	_w = w;
	_h = h;
	_p = 4;
		
	input = new float[_p*_w*_h];
	output = new int[_p*_w*_h];
	positive = new boolean[_w*_h];
	raw_score = new float[_w*_h];
	smooth_score = new float[_w*_h];

	jm = new JitterMatrix();
	score_jm = new JitterMatrix(1,"float32",w,h);
	cs = new ColorScorer(xml_filename);
		
	margin = 1.0;
	voting = 1;
	smoothing = 1;

	detecting = false;

	declareIO(1,2);
	declareAttribute("voting");
	declareAttribute("margin");
	declareAttribute("smoothing");
    }
	

    private class MyThread extends Thread {
    public void run() {
	
	detecting = true;

	double[] hsv  = new double[3];
	int idx;
	
	for (int i = 0; i < _h; i++) {
	    for (int j = 0; j < _w;j++) {
		idx = (i*_w+j)*_p;
		
 		hsv[0] = input[idx+1];
 		hsv[1] = input[idx+2];
 		hsv[2] = input[idx+3];
				
		double s = cs.getScore(hsv);

		// clip the score so that it wont go below some threshold
		raw_score[i*_w+j] = Math.max(CLIP_SCORE_THRESHOLD, (float)s);

		/* case 1: definitely correct */
		if (s > margin) {
		    // set pixel to green
		    output[idx]   = 255;
		    output[idx+1] = 0;
		    output[idx+2] = 255;
		    output[idx+3] = 0;
		    positive[i*_w+j] = true;
		}
		/* case 2: definitely incorrect */
		else if (s < -margin) {
		    // set to V
		    output[idx]   = 255;
		    output[idx+1] = (int)(input[idx+3]*255);
		    output[idx+2] = (int)(input[idx+3]*255);
		    output[idx+3] = (int)(input[idx+3]*255);
		    positive[i*_w+j] = false;
		}
		/* case 3: not sure because the score is with the margins */
		else {
		    if (voting == 1) {
			if (i==0 || j ==0) {
			    // if not sure near border, say no
			    output[idx]   = 255;
			    output[idx+1] = (int)(input[idx+3]*255);
			    output[idx+2] = (int)(input[idx+3]*255);
			    output[idx+3] = (int)(input[idx+3]*255);
			    positive[i*_w+j] = false;
			}
			else  {
			    int vote = 0;
			    if (positive[(i-1)*_w+j]) vote++;
			    if (positive[(i-1)*_w+(j-1)]) vote++;
			    if (positive[i*_w+(j-1)]) vote++;
			    if (vote >= 2 && s > 0) {
				// set pixel to green
				output[idx]   = 255;
				output[idx+1] = 0;
				output[idx+2] = 255;
				output[idx+3] = 0;
				positive[i*_w+j] = true;
			    }
			    else {
				// set pixel to black
				output[idx]   = 255;
				output[idx+1] = (int)(input[idx+3]*255);
				output[idx+2] = (int)(input[idx+3]*255);
				output[idx+3] = (int)(input[idx+3]*255);
				positive[i*_w+j] = false;
			    }
			}
		    }
		    else {
			// set pixel to red
			output[idx]   = 255;
			output[idx+1] = 255;
			output[idx+2] = 0;
			output[idx+3] = 0;
		    }
		}
	    }
	}

	if (smoothing == 1) {
	    // assume that border pixels are not skin
	    for (int i=0;i<_h;i++) {
		smooth_score[i*_w] = CLIP_SCORE_THRESHOLD;
		smooth_score[i*_w+(_w-1)] = CLIP_SCORE_THRESHOLD;
	    }
	    for (int j=0;j<_w;j++) {
		smooth_score[j] = CLIP_SCORE_THRESHOLD;
		smooth_score[(_h-1)*_w+j] = CLIP_SCORE_THRESHOLD;
	    }
	    
	    for (int i = 1; i < _h-1; i++) {
		for (int j = 1; j < _w-1;j++) {
	
		    /**
		     *  weight mask
		     *
		     *    1.0 1.0 1.0 
		     *    1.0 2.0 1.0
		     *    1.0 1.0 1.0
		     */

		    float temp = raw_score[(i-1)*_w+(j-1)] +
			raw_score[(i-1)*_w+j] +
			raw_score[(i-1)*_w+j+1] +
			raw_score[i*_w+j-1] +
			2 * raw_score[i*_w+j] +
			raw_score[i*_w+j+1] +
			raw_score[(i+1)*_w+j-1] +
			raw_score[(i+1)*_w+j] +
			raw_score[(i+1)*_w+j+1];

		    smooth_score[i*_w+j] = temp / 10f;
		}
	    }
	}		
	detecting = false;
    }
    }

    public void jit_matrix(String s) {

	jm.frommatrix(s);
	jm.setType("float32");
	jm.copyMatrixToArray(input);

	if (!detecting) {
	    MyThread t = new MyThread();
	    t.start();
	}
	
	jm.setType("char");
	jm.copyArrayToMatrix(output);
	outlet(0,"jit_matrix",jm.getName());
	
	if (smoothing == 1) 
	    score_jm.copyArrayToMatrix(smooth_score);
	else
	    score_jm.copyArrayToMatrix(raw_score);

	outlet(1,"jit_matrix",score_jm.getName());
	
    }
}
