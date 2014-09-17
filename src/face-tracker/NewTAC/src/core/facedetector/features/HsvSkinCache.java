package core.facedetector.features;

import core.facedetector.FaceCandidate;

/**
 * Used to store skin color information of a candidate window.
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class HsvSkinCache {

	FaceCandidate candidate;
	HsvSkinIntegralImage ii;

	public HsvSkinCache(FaceCandidate c, HsvSkinIntegralImage ii) {
		this.candidate = c;
		this.ii = ii;
	}

	public double getFeatureValue(int index) {
		return HsvSkinFeature.getSkinFeatureAtIndex(candidate, ii, index);
	}	
}
