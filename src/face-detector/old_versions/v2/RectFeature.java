public class RectFeature extends Feature {

	/**
	 * Helper class for keeping rectangle info
	 */
	private class Coords {
		private int x, y, w, h;
		private double v;

		public Coords(int xc, int yc, int width, int height, double val) {
			x = xc;
			y = yc;
			w = width;
			h = height;
			v = val;
		}

		public void printCoord() {
			System.out.println(x + " " + y + " " + w + " " + h + " " + v);
		}
	}

	private Coords[] rectCoords;
	private int num_coords;

	public double getFeatureValue(Candidate c, IntegralImage ii, int window_size,
																double stdDev) {
		double acc = 0;
		double scale_factor_w, scale_factor_h;
		for (int i = 0; i < num_coords; i++) {
			scale_factor_w =
					(double) ((rectCoords[i].w * c.size) / window_size) / rectCoords[i].w;
			scale_factor_h =
					(double) ((rectCoords[i].h * c.size) / window_size) / rectCoords[i].h;
			acc +=
					rectCoords[i].v
							* ii.getSum(c.x + rectCoords[i].x * c.size / window_size,
													c.y + rectCoords[i].y * c.size / window_size,
													rectCoords[i].w * c.size / window_size,
													rectCoords[i].h * c.size / window_size)
							/ (scale_factor_w * scale_factor_h);
		}
		return acc / (stdDev * window_size * window_size);
	}

	public boolean testCandidate(Candidate c, IntegralImage ii, int window_size,
																double stdDev) {
		double acc = 0;
		double scale_factor_w, scale_factor_h;
		for (int i = 0; i < num_coords; i++) {
			scale_factor_w =
					(double) ((rectCoords[i].w * c.size) / window_size) / rectCoords[i].w;
			scale_factor_h =
					(double) ((rectCoords[i].h * c.size) / window_size) / rectCoords[i].h;
			acc +=
					rectCoords[i].v
							* ii.getSum(c.x + (rectCoords[i].x * c.size) / window_size,
													c.y + (rectCoords[i].y * c.size) / window_size,
													(rectCoords[i].w * c.size) / window_size,
													(rectCoords[i].h * c.size) / window_size)
							/ (scale_factor_w * scale_factor_h);
		}
		return (acc < this.getThreshold() * stdDev * window_size * window_size);
	}

	public void addCoordinate(int x, int y, int w, int h, double v) {
		rectCoords[num_coords] = new Coords(x, y, w, h, v);
		num_coords++;
	}

	public RectFeature(int n) {
		num_coords = 0;
		rectCoords = new Coords[n];
	}

	public int getNumCoords() {
		return num_coords;
	}

	public String getCoord(int i) {
		String output =
				"" + rectCoords[i].x + " " + rectCoords[i].y + " " + rectCoords[i].w
						+ " " + rectCoords[i].h + " " + rectCoords[i].v;
		return output;
	}

	public void printFeature() {
		int i;
		for (i = 0; i < num_coords; i++) {
			rectCoords[i].printCoord();
		}
		System.out.println("threshold = " + getThreshold());
		System.out.println("right val = " + getRightValue());
		System.out.println("left val = " + getLeftValue());
		System.out.println("--------------------");
	}
}
