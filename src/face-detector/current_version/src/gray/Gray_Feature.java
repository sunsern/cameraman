package gray;

import shared.*;

/**
 * Used to store skin color feature 
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Gray_Feature extends Feature {

	public static final String NAME = "gray";
	public int grayindex;
	
	public double getFeatureValue(Gray_Cache cache) {
		return cache.getFeatureValue(grayindex);
	}

	public boolean testCandidate(Gray_Cache cache) {
		double v = cache.getFeatureValue(grayindex);
		return v < this.getThreshold();
	}

}
