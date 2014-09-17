
public class HSVFeature extends Feature {

	private int HSVindex;
	
	public double getFeatureValue(double[] hsv) {
		double v = hsv[HSVindex];
		return v;
	}

	public boolean testCandidate(double[] hsv) {
		double v = hsv[HSVindex];
		return v < this.getThreshold();
	}

	public HSVFeature() {
		/* nothing to do here */
	}

	public int getHSVindex() {
		return HSVindex;
	}

	public void setHSVindex(int vindex) {
		HSVindex = vindex;
	}

}