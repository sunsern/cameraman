public class SkinColorFeature extends Feature {

	public double getFeatureValue(Candidate c, IntegralImage skin) {
		double fskin = (double) skin.getSum(c.x, c.y, c.size, c.size)
				/ (c.size * c.size);
		return fskin;
	}

	public boolean testCandidate(Candidate c, IntegralImage skin) {
		double fskin = (double) skin.getSum(c.x, c.y, c.size, c.size)
				/ (c.size * c.size);
		return fskin > this.getThreshold();
	}

	public SkinColorFeature() {
		/* nothing to do here */
	}

}
