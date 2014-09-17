package features;

public class HogIntegralImage {

	private final int nBin = HogFeature.nBin;
	private int height, width, resizeFactor;
	private double[] ii;
	private int x1,y1,x2,y2;

	public HogIntegralImage(int w, int h, int x1, int y1, int x2, int y2, 
	                        double[] Gmax, int[] ot, int resizeFactor) {

		width = w;
		height = h;
		this.x1 = x1;
		this.y1 = y1;
		this.x2 = x2;
		this.y2 = y2;
		this.resizeFactor = resizeFactor;

		ii = new double[nBin * width * height];

		for (int i=y1;i<=y2;i++) {
			for (int j=x1;j<=x2;j++) {
				int idx = i*width + j;
				if (ot[idx] >= 0 && 
						ot[idx] < nBin) {
					ii[getIndex(ot[idx],j,i)] = Gmax[idx];
				}
			}
		}

		for (int i=0;i<nBin;i++) computeIntegralImage(i);
	}



	public HogIntegralImage(int w, int h, double[] Gmax, int[] ot, int resizeFactor) {
		this(w,h,0,0,w-1,h-1,Gmax,ot,resizeFactor);
	}

	public HogIntegralImage(int w, int h, double[] Gmax, int[] ot) {
		this(w,h,Gmax,ot,1);
	}


	private void computeIntegralImage(int b) {

		for (int j = x1+1; j <= x2; j++) {
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

	private final int getIndex(int b, int x, int y) {
		return ((y*width+x)*nBin+b);
	}

	public int getWidth() {
		return width * resizeFactor;
	}

	public int getHeight() {
		return height * resizeFactor;
	}


}
