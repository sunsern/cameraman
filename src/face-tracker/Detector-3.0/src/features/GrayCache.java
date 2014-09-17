package features;

import shared.*;

/**
 * Used to store skin color information of a candidate window.
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class GrayCache {
	
		Candidate candidate;
		public GrayIntegralImage ii;
		double[] greyFeature; 
		boolean cached;
		
	  public GrayCache(Candidate c, GrayIntegralImage ii) {
	  	this.candidate = c;
	  	this.ii = ii;
	  	cached = false;
		}
		
		public double getFeatureValue(int index) {
			if (cached) {
				return greyFeature[index]; 
			}
			else {
				greyFeature = GrayFeature.getGreyFeatureVector(candidate, ii);
				cached = true;
				return greyFeature[index];
			}
		}	
}
