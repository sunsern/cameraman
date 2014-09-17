package core.facedetector.features;

import core.facedetector.FaceCandidate;


public class HogFeature extends AbstractFeature {

	private static final double EPSILON = 1e-7;

	// refer to http://seed.ucsd.edu/mediawiki/index.php/Image:HOG_Feature_Mask.jpg
	
	public static final int nBin = 8;     // 8 direction bins
	public static final int nRegion = 16;
	public static final int nFeature = nBin*nRegion;
	
	public static final int TYPEID = 3;
	public static final String NAME = "HOG";
	public int hogindex;
	
	public double getFeatureValue(HogCache cache) {
		return cache.getFeatureValue(hogindex);
	}
	
	public boolean testCandidate(HogCache cache) {
		double v = cache.getFeatureValue(hogindex);
		return v < this.getThreshold();
	}

	
	//------------ Static Methods ---------------//
	
	public static HogIntegralImage getHOGIntegralImage(int[] rgbs, int w, int h) {
		return getHOGIntegralImage(rgbs, w, h, 1);
	}

	public static HogIntegralImage getHOGIntegralImage(int[] rgbs, int w, int h, int resizeFactor) {
	    return getHOGIntegralImage(rgbs,w,h,0,0,w-1,h-1,resizeFactor);
	}
	
        public static HogIntegralImage getHOGIntegralImage(int[] rgbs, int w, int h, 
							   int x1, int y1, int x2, int y2, int resizeFactor) {
		
		// compute gradient using [-1 0 1] and its transpose

		double[] Gmax = new double[w * h];
		double tGx, tGy;
		
		/* calculate orientation
		 * -1 = undefined 
		 *  0 = 0 	
		 *  1 = 45 
		 *  2 = 90 
		 *  3 = 135 
		 *  4 = 180 
		 *  5 = 225 
		 *  6 = 270 
		 *  7 = 315 
		 */ 
		int[] ot = new int[w * h];


		// [-1 0 1]
		double[] Gx = new double[3];
		
		// [-1 0 1]' 
		double[] Gy = new double[3];

		/* sqrt(Gx^2 + Gy^2) */
		double[] G = new double[3];


		for (int y=y1;y<=y2;y++) {
		    for (int x=x1;x<=x2;x++) {	
			
			for (int j=0;j<3;j++) {
			    if (x-1 >= 0 && x+1 < w) {
				Gx[j] = 
				    -1 * rgbs[(y*w + (x-1))*3+j] +
				     1 * rgbs[(y*w + (x+1))*3+j];
			    }
			    else 
				Gx[j] = 0;
			    
			    if (y-1 >= 0 && y+1 < h) {
				Gy[j] = 
				    -1 * rgbs[((y-1)*w + x)*3+j] +
			 	     1 * rgbs[((y+1)*w + x)*3+j];
			    }
			    else
				Gy[j] = 0;
			    
			    G[j] = Math.sqrt(Gx[j]*Gx[j] + Gy[j]*Gy[j]);
			}

			int idx = y*w + x;
  
			// find maximum
			if (G[0] > G[1] && G[0] > G[2]) {
				Gmax[idx] = G[0];
				tGx = Gx[0];
				tGy = Gy[0];
			}
			else if (G[1] > G[0] && G[1] > G[2]) {
				Gmax[idx] = G[1];
				tGx = Gx[1];
				tGy = Gy[1];
			}
			else {
				Gmax[idx] = G[2];
				tGx = Gx[2];
				tGy = Gy[2];
			}
			
			// find corresponding orientation
			if (Math.abs(tGx) < EPSILON) {
				if (Math.abs(tGy) < EPSILON) ot[idx] = -1; // undefined
				else if (tGy > 0)	ot[idx] = 2; // 90
				else if (tGy < 0)	ot[idx] = 6; // 270
				
			}
			else if (tGx > 0) {
				if (Math.abs(tGy) < EPSILON) {
					ot[idx] = 0; // 0
				}
				else if (tGy > 0) {
					if (tGy > 2.5 * tGx) ot[idx] = 2; // 90
					else if (tGy < 0.4 * tGx) ot[idx] = 0; // 0
					else ot[idx] = 1; // 45
				}
				else if (tGy < 0) {
					if (-tGy > 2.5 * tGx) ot[idx] = 6; // 270
					else if (-tGy < 0.4 * tGx) ot[idx] = 0; // 0
					else ot[idx] = 7; // 315
				}
			}
			else if (tGx < 0) {
				if (Math.abs(tGy) < EPSILON) {
					ot[idx] = 4; // 180
				}
				else if (tGy > 0) {
					if (tGy > 2.5 * -tGx) ot[idx] = 2; // 90
					else if (tGy < 0.4 * -tGx) ot[idx] = 4; // 180
					else ot[idx] = 3; // 135
				}
				else if (tGy < 0) {
					if (-tGy > 2.5 * -tGx) ot[idx] = 6; // 270
					else if (-tGy < 0.4 * -tGx) ot[idx] = 4; // 180
					else ot[idx] = 5; // 225
				}
			}

		    }
		}
	
		return new HogIntegralImage(w,h,Gmax,ot,resizeFactor);

	}

		
	public static double computeHOGFeatureAtIndex(FaceCandidate c, HogIntegralImage ii, int index) {

		int x = c.x, y = c.y, size = c.size;
		
		// check if the window is entirely inside the image
		if (x < 0 || x+size < 0 ||
				x > ii.getWidth() || x+size > ii.getWidth() || 
				y < 0 || y+size < 0 ||
				y > ii.getHeight() || y+size > ii.getHeight()) {
			throw new RuntimeException("Invalid detection window");
		}
		
		// adjust the window so that its size is multiple of 8 and it's at the center
		int pad = (size % 8) / 2;
		x = x + pad;
		y = y + pad;
		size = size - (size % 8);
		
		int size8 = size / 8;
		int size2 = size / 2;		
		
		int[] regionInfo  = new int[] { x        ,y        ,size2,size2,
		                                x+size2  ,y        ,size2,size2,
		                                x        ,y+size2  ,size2,size2,
		                                x+size2  ,y+size2  ,size2,size2,
		                                x+size8  ,y+size8  ,3*size8,3*size8,
		                                x+size2  ,y+size8  ,3*size8,3*size8,
		                                x+size8  ,y+size2  ,3*size8,3*size8,
		                                x+size2  ,y+size2  ,3*size8,3*size8,
		                                x+2*size8,y+2*size8,2*size8,2*size8,
		                                x+size2  ,y+2*size8,2*size8,2*size8,
		                                x+2*size8,y+size2  ,2*size8,2*size8,
		                                x+size2  ,y+size2  ,2*size8,2*size8,
		                                x+3*size8,y+3*size8,size8,size8,
		                                x+size2  ,y+3*size8,size8,size8,
		                                x+3*size8,y+size2  ,size8,size8,
		                                x+size2  ,y+size2  ,size8,size8 };
		

		int region = index / nBin;
		int b = index % nBin;
		double v;
		
		if (region < 12) {	
				double b0 = ii.getSum(b, 
				                      regionInfo[region*4], 
				                      regionInfo[region*4+1],
				                      regionInfo[region*4+2],
				                      regionInfo[region*4+3]);
				double b1 = ii.getSum(b, 
				                      regionInfo[(4+region)*4], 
				                      regionInfo[(4+region)*4+1],
				                      regionInfo[(4+region)*4+2],
				                      regionInfo[(4+region)*4+3]);
				double area = regionInfo[region*4+2]*regionInfo[region*4+3] -
				              regionInfo[(4+region)*4+2]*regionInfo[(4+region)*4+3];
				v = (b0 - b1) / area; 
		}
		else {
				double b0 = ii.getSum(b, 
				                      regionInfo[region*4], 
				                      regionInfo[region*4+1],
				                      regionInfo[region*4+2],
				                      regionInfo[region*4+3]);
				double area = regionInfo[region*4+2]*regionInfo[region*4+3];
				v = b0 / area; 	
		}
				
		return v;
	}
	
	public static double[] getHOGFeatureVector(FaceCandidate c, HogIntegralImage ii) {		
		double[] HOG = new double[nFeature];
		
		for (int i=0;i<HOG.length;i++) {
			HOG[i] = computeHOGFeatureAtIndex(c,ii,i);
		}
						
		return HOG;
	}			
	
}