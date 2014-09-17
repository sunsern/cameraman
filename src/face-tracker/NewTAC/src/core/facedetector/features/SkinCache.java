package core.facedetector.features;

import core.facedetector.FaceCandidate;

/**
 * Used to store skin color information of a candidate window.
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class SkinCache {

	FaceCandidate candidate;
	SkinIntegralImage ii;

	public SkinCache(FaceCandidate c, SkinIntegralImage ii) {
		this.candidate = c;
		this.ii = ii;
	}

	public double getFeatureValue(int index) {
		return SkinFeature.getSkinFeatureAtIndex(candidate, ii, index);
	}	
}
