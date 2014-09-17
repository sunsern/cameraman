package particlefilter;

public interface State {
	
	public double[] getStateVector();
	public void setStateVector(double[] state);
	public void update();

}
