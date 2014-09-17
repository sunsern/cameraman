package features;

import shared.*;

/**
 * Used to store skin color information of a candidate window.
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class HsvSkinCache {
	
		Candidate candidate;
		HsvSkinIntegralImage ii;
		
	  public HsvSkinCache(Candidate c, HsvSkinIntegralImage ii) {
	  	this.candidate = c;
	  	this.ii = ii;
	  }
		
		public double getFeatureValue(int index) {
			return HsvSkinFeature.getSkinFeatureAtIndex(candidate, ii, index);
		}	
}
