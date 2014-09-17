package core.nh.videotracker;

import core.nh.particlefilter.State;

public class TargetState implements State {

  // (x,y,size,vx,vy,vsize,theta)
  private final int NUMSTATES = 7;
  private double[] state;

  public TargetState() {
    state = new double[NUMSTATES];
  }

  public TargetState(double x, double y, double size, double theta) {
    state = new double[NUMSTATES];
    state[0] = x;
    state[1] = y;
    state[2] = size;
    state[3] = 0.0f;
    state[4] = 0.0f;
    state[5] = 0.0f;
    state[6] = theta;
  }

  public TargetState(double[] v) {
    state = new double[NUMSTATES];
    System.arraycopy(v, 0, state, 0, v.length);
  }

  public double[] getStateVector() {
    return state;
  }

  public void setStateVector(double[] state) {
    if (state != null && state.length == NUMSTATES)
      this.state = state;
  }

  public void update() {
    state[0] = state[0] + state[3];
    state[1] = state[1] + state[4];
    state[2] = state[2] + state[5];
  }

  public boolean isSimilar(TargetState t2) {
	  if (Math.abs(this.state[0]-t2.state[0]) > 5) return false;
	  if (Math.abs(this.state[1]-t2.state[1]) > 5) return false;
	  if (Math.abs(this.state[2]-t2.state[2]) > 16) return false;
	  return true;
  }

}
