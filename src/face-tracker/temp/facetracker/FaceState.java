package facetracker;

import particlefilter.State;

public class FaceState implements State {

	// (x,y,size,vx,vy)
	private final int STATES = 5;
	private double[] facestate;
	
	public FaceState() {
		facestate = new double[STATES];
	}
	
	public FaceState(double x, double y, double size) {
		facestate = new double[STATES];
		facestate[0] = x;
 		facestate[1] = y;
 		facestate[2] = size;
   	}
	
	public double[] getStateVector() {
		return facestate;
	}

	public void setStateVector(double[] state) {
		if (state != null && state.length == STATES)
			facestate = state;
	}

	public void update() {
		// x = x+vx
		facestate[0] = facestate[0] + facestate[3];
		// y = y+vy
		facestate[1] = facestate[1] + facestate[4];
	}
	
}
