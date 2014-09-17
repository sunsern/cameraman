package shared;

public class Stage {

	private Feature[] features;
	private int num_features;
	private double threshold;

	public Stage(int n) {
		features = new Feature[n];
		num_features = 0;
	}

	public Feature getFeature(int idx) {
		return features[idx];
	}

	public int getNumFeatures() {
		return num_features;
	}

	public void addFeature(Feature f) {
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
