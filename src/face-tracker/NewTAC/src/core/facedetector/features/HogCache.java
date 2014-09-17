package core.facedetector.features;

import core.facedetector.FaceCandidate;

public class HogCache {
	
		FaceCandidate candidate;
		HogIntegralImage ii;
		
	  public HogCache(FaceCandidate c, HogIntegralImage ii) {
	  	this.candidate = c;
	  	this.ii = ii;
	  }
		
		public double getFeatureValue(int index) {
			return HogFeature.computeHOGFeatureAtIndex(candidate, ii, index); 
		}	
}
