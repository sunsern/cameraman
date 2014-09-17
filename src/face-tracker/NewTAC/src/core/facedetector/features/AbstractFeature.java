package core.facedetector.features;

public abstract class AbstractFeature {

	private double threshold;
	private double lValue; // cond_true_val
	private double rValue; // cond_false_val
	private String type;
	private int id, parent, parent_cond;

	public int getFeatureID() {
		return id;
	}

	public void setFeatureID(int fid) {
		id = fid;
	}

	public int getParent() {
		return parent;
	}

	public void setParent(int p) {
		parent = p;
	}

	public int getParentCondition() {
		return parent_cond;
	}

	public void setParentCondition(int pc) {
		parent_cond = pc;
	}

	public String getType() {
		return type;
	}

	public void setType(String t) {
		type = t;
	}

	public double getThreshold() {
		return threshold;
	}

	public void setThreshold(double t) {
		threshold = t;
	}

	public double getLeftValue() {
		return lValue;
	}

	public void setLeftValue(double v) {
		lValue = v;
	}

	public double getRightValue() {
		return rValue;
	}

	public void setRightValue(double v) {
		rValue = v;
	}
	
}
