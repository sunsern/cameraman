package features;


public class HSVFeature extends AbstractFeature {

	public static final int TYPEID = 2;
	public static final String NAME = "HSV";
	private int HSVindex;
	
	public double getFeatureValue(double[] hsv) {
		double v = hsv[HSVindex];
		return v;
	}

	public boolean testCandidate(double[] hsv) {
		double v = hsv[HSVindex];
		return v < this.getThreshold();
	}

	public int getHSVindex() {
		return HSVindex;
	}

	public void setHSVindex(int vindex) {
		HSVindex = vindex;
	}

}