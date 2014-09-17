package features;


/**
 * Stores and provides methods for skin integral images
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class HsvSkinIntegralImage {

	private final int nBin = HsvSkinFeature.nBin;
	private int height, width, resizeFactor;
	private double[] ii; // [bin#][x][y]
	private int x1,y1,x2,y2;

	public HsvSkinIntegralImage(double[] hsv, int w, int h, 
	                         int x1, int y1, int x2, int y2, int resizeFactor) {
		height = h;
		width = w;
		this.x1 = x1;
		this.y1 = y1;
		this.x2 = x2;
		this.y2 = y2;
		this.resizeFactor = resizeFactor;
		computeIntegralImage(hsv);
	}

	public HsvSkinIntegralImage(double[] score, int w, int h, int resizeFactor) {
		this(score,w,h,0,0,w-1,h-1,resizeFactor);
	}

	public HsvSkinIntegralImage(double[] score, int w, int h) {
		this(score,w,h,1);
	}

	public int getWidth() {
		return width*resizeFactor;
	}

	public int getHeight() {
		return height*resizeFactor;
	}

	public double getSum(int b, int x, int y, int w, int h) {
		double A = 0, B = 0, C = 0, D = 0;

		x = x/resizeFactor;
		y = y/resizeFactor;
		w = w/resizeFactor;
		h = h/resizeFactor;

		if (x + w - 1 < width && y + h - 1 < height)
			A = ii[getIndex(b,x+w-1,y+h-1)];

		if (x > 0 && y > 0)
			B = ii[getIndex(b,x-1,y-1)];

		if (x > 0 && y + h - 1 < height)
			C = ii[getIndex(b,x-1,y+h-1)];

		if (y > 0 && x + w - 1 < width)
			D = ii[getIndex(b,x+w-1,y-1)];

		return (A - C + B - D) * resizeFactor * resizeFactor;
	}

	private void computeIntegralImage(double[] hsv) {

		ii = new double[nBin * width * height];

		double h,s,v;

		for (int i=y1; i<=y2; i++) {
			for (int j=x1; j<=x2; j++) {	
				int idx = (i*width+j)*3; 
				h = hsv[idx];
				s = hsv[idx+1];
				v = hsv[idx+2];	
				ii[getIndex(0,j,i)] = h;
				ii[getIndex(1,j,i)] = s;
				ii[getIndex(2,j,i)] = v;
			}
		}

		for (int b=0; b<nBin; b++) {
			for (int j=x1+1; j<=x2; j++) {
				ii[getIndex(b,j,y1)] += ii[getIndex(b,j-1,y1)];
			}

			double[] tmp = new double[width];
			for (int i = y1+1; i <= y2; i++) {
				tmp[x1] = ii[getIndex(b,x1,i)];
				ii[getIndex(b,x1,i)] += ii[getIndex(b,x1,i-1)];
				for (int j = x1+1; j <= x2; j++) {
					tmp[j] = tmp[j-1] + ii[getIndex(b,j,i)];
					ii[getIndex(b,j,i)] = ii[getIndex(b,j,i-1)] + tmp[j];
				}
			}
		}
	}

	private final int getIndex(int b, int x, int y) {
		return ((y*width+x)*nBin+b);
	}

}
