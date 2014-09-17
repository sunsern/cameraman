package features;

import shared.*;

public class HogCache {
	
		Candidate candidate;
		HogIntegralImage ii;
		
	  public HogCache(Candidate c, HogIntegralImage ii) {
	  	this.candidate = c;
	  	this.ii = ii;
	  }
		
		public double getFeatureValue(int index) {
			return HogFeature.computeHOGFeatureAtIndex(candidate, ii, index); 
		}	
}
