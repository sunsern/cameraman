package HSV;

import shared.Feature;

public class HSV_Feature extends Feature {

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