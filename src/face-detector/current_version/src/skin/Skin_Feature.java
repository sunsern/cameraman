package skin;

import shared.*;

/**
 * Used to store skin color feature 
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Skin_Feature extends Feature {

	public static final String NAME = "skincolor";
	public int skinindex;
	
	public double getFeatureValue(Skin_Cache cache) {
		return cache.getFeatureValue(skinindex);
	}

	public boolean testCandidate(Skin_Cache cache) {
		double v = cache.getFeatureValue(skinindex);
		return v < this.getThreshold();
	}

}
