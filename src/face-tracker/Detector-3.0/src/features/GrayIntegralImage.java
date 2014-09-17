package features;

/**
 * Stores and provides methods for skin integral images
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class GrayIntegralImage {
	
	private int height, width;
	private long ii[][]; 

	public GrayIntegralImage(int[] grayvalue, int w, int h) {
		height = h;
		width = w;
		computeIntegralImage(grayvalue);
	}

	public int getWidth() {
		return width;
	}

	public int getHeight() {
		return height;
	}

	public long getSum(int x, int y, int w, int h) {
		long A = 0, B = 0, C = 0, D = 0;

		if (x + w - 1 < width && y + h - 1 < height)
			A = ii[x+w-1][y+h-1];

		if (x > 0 && y > 0)
			B = ii[x-1][y-1];

		if (x > 0 && y + h - 1 < height)
			C = ii[x-1][y+h-1];

		if (y > 0 && x + w - 1 < width)
			D = ii[x+w-1][y-1];

		return (A - C + B - D);
	}

	private void computeIntegralImage(int[] greyvalue) {

		ii = new long[width][height];
		
		for (int i=0; i<height; i++) {
			for (int j=0; j<width; j++) {
				ii[j][i] = greyvalue[i*width+j];
			}
		}
		
		for (int j=1; j<width; j++) {
			ii[j][0] = ii[j-1][0] + ii[j][0];
		}

		long[] tmp = new long[width];
		for (int i = 1; i < height; i++) {
			tmp[0] = ii[0][i];
			ii[0][i] = ii[0][i-1] + ii[0][i];
			for (int j = 1; j < width; j++) {
				tmp[j] = tmp[j-1] + ii[j][i];
				ii[j][i] = ii[j][i-1] + tmp[j];
			}
		}

	}
}
