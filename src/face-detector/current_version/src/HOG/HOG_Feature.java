package HOG;

import shared.*;

public class HOG_Feature extends Feature {

	public static final String NAME = "HOG";
	public int hogindex;
	
	public double getFeatureValue(HOG_Cache cache) {
		return cache.getFeatureValue(hogindex);
	}
	
	public boolean testCandidate(HOG_Cache cache) {
		double v = cache.getFeatureValue(hogindex);
		return v < this.getThreshold();
	}

}