package features;

import shared.*;

/**
 * Used to store skin color information of a candidate window.
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class SkinCache {
	
		Candidate candidate;
		SkinIntegralImage ii;
		
	  public SkinCache(Candidate c, SkinIntegralImage ii) {
	  	this.candidate = c;
	  	this.ii = ii;
	  }
		
		public double getFeatureValue(int index) {
			return SkinFeature.getSkinFeatureAtIndex(candidate, ii, index);
		}	
}
