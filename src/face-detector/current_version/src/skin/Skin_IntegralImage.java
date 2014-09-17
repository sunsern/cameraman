package skin;

/**
 * Stores and provides methods for skin integral images
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Skin_IntegralImage {
	
	private final int nBin = Skin_Core.nBin;
	private int _height, _width;
	private double ii[][][]; // [bin#][x][y]

	public Skin_IntegralImage(double[] score, int w, int h) {
		_height = h;
		_width = w;
		compute_ii(score);
	}

	public int getWidth() {
		return _width;
	}

	public int getHeight() {
		return _height;
	}

	public double getSum(int b, int x, int y, int w, int h) {
		double A = 0, B = 0, C = 0, D = 0;

		if (x + w - 1 < _width && y + h - 1 < _height)
			A = ii[b][x+w-1][y+h-1];

		if (x > 0 && y > 0)
			B = ii[b][x-1][y-1];

		if (x > 0 && y + h - 1 < _height)
			C = ii[b][x-1][y+h-1];

		if (y > 0 && x + w - 1 < _width)
			D = ii[b][x+w-1][y-1];

		return (A - C + B - D);
	}

	private void compute_ii(double[] score) {

		ii = new double[nBin][_width][_height];

		double s;
		
		for (int i=0; i<_height; i++) {
			for (int j=0; j<_width; j++) {
				s = score[i*_width+j];
				if (s <= 0d) {
					ii[0][j][i] = s;
					ii[1][j][i] = 0d;
				}
				else {
					ii[0][j][i] = 0d;
					ii[1][j][i] = s;
				}
			}
		}
		
		for (int b=0; b<nBin; b++) {
			for (int j=1; j<_width; j++) {
				ii[b][j][0] = ii[b][j-1][0] + ii[b][j][0];
			}

			double[] tmp = new double[_width];
			for (int i = 1; i < _height; i++) {
				tmp[0] = ii[b][0][i];
				ii[b][0][i] = ii[b][0][i-1] + ii[b][0][i];
				for (int j = 1; j < _width; j++) {
					tmp[j] = tmp[j-1] + ii[b][j][i];
					ii[b][j][i] = ii[b][j][i-1] + tmp[j];
				}
			}
		}
	}
}
