package gray;

/**
 * Stores and provides methods for skin integral images
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Gray_IntegralImage {
	
	private int _height, _width;
	private long ii[][]; // [x][y]

	public Gray_IntegralImage(int[] greyvalue, int w, int h) {
		_height = h;
		_width = w;
		compute_ii(greyvalue);
	}

	public int getWidth() {
		return _width;
	}

	public int getHeight() {
		return _height;
	}

	public long getSum(int x, int y, int w, int h) {
		long A = 0, B = 0, C = 0, D = 0;

		if (x + w - 1 < _width && y + h - 1 < _height)
			A = ii[x+w-1][y+h-1];

		if (x > 0 && y > 0)
			B = ii[x-1][y-1];

		if (x > 0 && y + h - 1 < _height)
			C = ii[x-1][y+h-1];

		if (y > 0 && x + w - 1 < _width)
			D = ii[x+w-1][y-1];

		return (A - C + B - D);
	}

	private void compute_ii(int[] greyvalue) {

		ii = new long[_width][_height];
		
		for (int i=0; i<_height; i++) {
			for (int j=0; j<_width; j++) {
				ii[j][i] = greyvalue[i*_width+j];
			}
		}
		
		for (int j=1; j<_width; j++) {
			ii[j][0] = ii[j-1][0] + ii[j][0];
		}

		long[] tmp = new long[_width];
		for (int i = 1; i < _height; i++) {
			tmp[0] = ii[0][i];
			ii[0][i] = ii[0][i-1] + ii[0][i];
			for (int j = 1; j < _width; j++) {
				tmp[j] = tmp[j-1] + ii[j][i];
				ii[j][i] = ii[j][i-1] + tmp[j];
			}
		}

	}
}
