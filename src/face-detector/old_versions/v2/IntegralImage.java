public class IntegralImage {

	private int _height, _width;
	private int ii[][]; /* [x][y] */
	private long iis[][]; /* [x][y] */
	private boolean ss;

	private int getIndex(int x, int y) {
		return (y * _width) + x;
	}

	public int getWidth() {
		return _width;
	}

	public int getHeight() {
		return _height;
	}

	public long getSum(int x, int y, int w, int h) {
		long A = 0, B = 0, C = 0, D = 0;

		if (x + w - 1 < _width && y + h - 1 < _height) {
			A = ii[x + w - 1][y + h - 1];
		}

		if (x > 0 && y > 0) {
			B = ii[x - 1][y - 1];
		}

		if (x > 0 && y + h - 1 < _height) {
			C = ii[x - 1][y + h - 1];
		}

		if (y > 0 && x + w - 1 < _width) {
			D = ii[x + w - 1][y - 1];
		}

		return (A - C + B - D);
	}

	public long getSumSquare(int x, int y, int w, int h) {
		long A = 0, B = 0, C = 0, D = 0;

		if (x + w - 1 < _width && y + h - 1 < _height) {
			A = iis[x + w - 1][y + h - 1];
		}

		if (x > 0 && y > 0) {
			B = iis[x - 1][y - 1];
		}

		if (x > 0 && y + h - 1 < _height) {
			C = iis[x - 1][y + h - 1];
		}

		if (y > 0 && x + w - 1 < _width) {
			D = iis[x + w - 1][y - 1];
		}

		return (A - C + B - D);
	}

	public double getMean(int x, int y, int size) {
		double s = this.getSum(x, y, size, size);
		return s / ((double) (size * size));
	}

	public double getMeanSquare(int x, int y, int size) {
		double s = this.getSumSquare(x, y, size, size);
		return s / ((double) (size * size));
	}

	public double getStdDev(int x, int y, int size) {
		double mean = this.getMean(x, y, size);
		double meanSq = this.getMeanSquare(x, y, size);
		return Math.sqrt(meanSq - mean * mean);
	}

	private void compute_ii_with_iis(byte[] buf) {

		ii = new int[_width][_height];
		iis = new long[_width][_height];

		ii[0][0] = buf[getIndex(0, 0)] & (0xff);
		iis[0][0] = ii[0][0] * ii[0][0];

		for (int i = 1; i < _width; i++) {
			int t = buf[getIndex(i, 0)] & (0xff);
			ii[i][0] = ii[i - 1][0] + t;
			iis[i][0] = iis[i - 1][0] + t * t;
		}

		int[] tmp = new int[_width];
		long[] tmp2 = new long[_width];

		for (int j = 1; j < _height; j++) {
			int t = buf[getIndex(0, j)] & (0xff);
			ii[0][j] = ii[0][j - 1] + t;
			iis[0][j] = iis[0][j - 1] + t * t;
			tmp[0] = t;
			tmp2[0] = t * t;
			for (int i = 1; i < _width; i++) {
				int s = buf[getIndex(i, j)] & (0xff);
				tmp[i] = tmp[i - 1] + s;
				tmp2[i] = tmp2[i - 1] + s * s;
				ii[i][j] = ii[i][j - 1] + tmp[i];
				iis[i][j] = iis[i][j - 1] + tmp2[i];
			}
		}
	}

	private void compute_ii_without_iis(byte[] buf) {

		ii = new int[_width][_height];
		ii[0][0] = buf[getIndex(0, 0)] & (0xff);
		for (int i = 1; i < _width; i++) {
			int t = buf[getIndex(i, 0)] & (0xff);
			ii[i][0] = ii[i - 1][0] + t;
		}

		int[] tmp = new int[_width];
		for (int j = 1; j < _height; j++) {
			int t = buf[getIndex(0, j)] & (0xff);
			ii[0][j] = ii[0][j - 1] + t;
			tmp[0] = t;
			for (int i = 1; i < _width; i++) {
				int s = buf[getIndex(i, j)] & (0xff);
				tmp[i] = tmp[i - 1] + s;
				ii[i][j] = ii[i][j - 1] + tmp[i];
			}
		}
	}

	public IntegralImage(byte[] buf, int w, int h, boolean sumSquareNeeded) {

		ss = sumSquareNeeded;

		_height = h;
		_width = w;

		if (ss) compute_ii_with_iis(buf);
		else compute_ii_without_iis(buf);
	}

}
