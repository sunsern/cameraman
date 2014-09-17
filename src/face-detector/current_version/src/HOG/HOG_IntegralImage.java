package HOG;

public class HOG_IntegralImage {

	private int _height, _width, nBin;
	private double ii[][][]; /* [bin#][x][y] */
	
	public HOG_IntegralImage(int w, int h, int b, double[] Gmax, int[] ot) {

		_width = w;
		_height = h;
		nBin = b;

		compute_ii(Gmax,ot);

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


	private void compute_ii(double[] Gmax, int[] ot) {

		ii = new double[nBin][_width][_height];
		
		for (int i=0;i < _height;i++) {
			for (int j=0;j < _width;j++) {
				if (ot[i*_width + j] >= 0 && 
						ot[i*_width + j] < nBin) {
					ii[ot[i*_width + j]][j][i] = Gmax[i*_width + j];
				}
			}
		}
		
		for (int b=0; b < nBin; b++) {
			for (int j = 1; j < _width; j++) {
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
