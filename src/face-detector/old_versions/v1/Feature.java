
public abstract class Feature {

	protected double threshold, rValue, lValue;
		
	public double getFeatureValue(Candidate c, IntegralImage ii, int window_size) {
		return 0.0;
	}
	
	public double testCandidate(Candidate c, IntegralImage ii, int window_size) {
		return 0.0;
	}
	
	public double getThreshold() {
		return threshold;
	}
	public void setThreshold(double t) {
		threshold = t;
	}
	
	public double getLeftValue() {
		return lValue;
	}
	public void setLeftValue(double v) {
		lValue = v;
	}
	
	public double getRightValue() {
		return rValue;
	}
	public void setRightValue(double v) {
		rValue = v;
	}
}
