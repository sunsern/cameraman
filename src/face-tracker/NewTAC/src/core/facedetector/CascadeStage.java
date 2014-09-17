package core.facedetector;

import core.facedetector.features.AbstractFeature;

public class CascadeStage {

	private AbstractFeature[] features;
	private int num_features;
	private double threshold;

	public CascadeStage(int n) {
		features = new AbstractFeature[n];
		num_features = 0;
	}

	public AbstractFeature getFeature(int idx) {
		return features[idx];
	}

	public int getNumFeatures() {
		return num_features;
	}

	public void addFeature(AbstractFeature f) {
		features[num_features] = f;
		num_features++;
	}

	public void setThreshold(double t) {
		threshold = t;
	}

	public double getThreshold() {
		return threshold;
	}

}
