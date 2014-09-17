package HOG;


import java.awt.image.BufferedImage;

import shared.Candidate;


public class HOG_Core {

	private static final double EPSILON = 0.000001d;

	// refer to http://seed.ucsd.edu/mediawiki/index.php/Image:HOG_Feature_Mask.jpg
	
	public static final int nBin = 8;   // 8 direction bins
	public static final int nSection = 16;
	public static final int nFeature = nBin*nSection;
	
  
	public static HOG_IntegralImage getHOGIntegralImage(BufferedImage bimg) {
		
		int[] rgbs = new int[bimg.getWidth() * bimg.getHeight()];
		bimg.getRGB(0, 0, bimg.getWidth(), bimg.getHeight(), rgbs, 0,
								bimg.getWidth());
		
		int[][] rgb2d = new int[bimg.getWidth() * bimg.getHeight()][3];
		
		for (int j=0; j < rgbs.length; j++) {
			rgb2d[j][0] = rgbs[j] & 0xff;
			rgb2d[j][1] = (rgbs[j] >> 8) & 0xff;
			rgb2d[j][2] = (rgbs[j] >> 16) & 0xff;
		}
		
		// compute gradient using [-1 0 1] and its transpose

		// [-1 0 1]
		double[][] Gx = new double[bimg.getWidth() * bimg.getHeight()][3];
		
		// [-1 0 1]' 
		double[][] Gy = new double[bimg.getWidth() * bimg.getHeight()][3];

		for (int j=0;j<3;j++) {
			for (int y=0;y<bimg.getHeight();y++) {
				for (int x=0;x<bimg.getWidth();x++) {	
					if (x-1 >= 0 && x+1 < bimg.getWidth()) {
						Gx[y*bimg.getWidth() + x][j] = 
							-1 * rgb2d[y*bimg.getWidth() + (x-1)][j] +
							1 * rgb2d[y*bimg.getWidth() + (x+1)][j];
					}
					else 
						Gx[y*bimg.getWidth() + x][j] = 0;

					if (y-1 >= 0 && y+1 < bimg.getHeight()) {
						Gy[y*bimg.getWidth() + x][j] = 
							-1 * rgb2d[(y-1)*bimg.getWidth() + x][j] +
							1 * rgb2d[(y+1)*bimg.getWidth() + x][j];
					}
					else
						Gy[y*bimg.getWidth() + x][j] = 0;
				}
			}
		}
		
		/* sqrt(Gx^2 + Gy^2) */
		double[][] G = new double[bimg.getWidth() * bimg.getHeight()][3];
		for (int j=0;j<3;j++) {
			for (int k=0;k < bimg.getWidth()*bimg.getHeight();k++)
				G[k][j] = Math.sqrt(Gx[k][j]*Gx[k][j] + Gy[k][j]*Gy[k][j]);
		}
	
		double[] Gmax = new double[bimg.getWidth() * bimg.getHeight()];
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
		int[] ot = new int[bimg.getWidth() * bimg.getHeight()];
		
		for (int j=0;j < ot.length; j++) {
			
			// find maximum
			if (G[j][0] > G[j][1] && G[j][0] > G[j][2]) {
				Gmax[j] = G[j][0];
				tGx = Gx[j][0];
				tGy = Gy[j][0];
			}
			else if (G[j][1] > G[j][0] && G[j][1] > G[j][2]) {
				Gmax[j] = G[j][1];
				tGx = Gx[j][1];
				tGy = Gy[j][1];
			}
			else {
				Gmax[j] = G[j][2];
				tGx = Gx[j][2];
				tGy = Gy[j][2];
			}
			
			// find corresponding orientation
			if (Math.abs(tGx) < EPSILON) {
				if (Math.abs(tGy) < EPSILON) ot[j] = -1; // undefined
				else if (tGy > 0)	ot[j] = 2; // 90
				else if (tGy < 0)	ot[j] = 6; // 270
				
			}
			else if (tGx > 0) {
				if (Math.abs(tGy) < EPSILON) {
					ot[j] = 0; // 0
				}
				else if (tGy > 0) {
					if (tGy > 2.5 * tGx) ot[j] = 2; // 90
					else if (tGy < 0.4 * tGx) ot[j] = 0; // 0
					else ot[j] = 1; // 45
				}
				else if (tGy < 0) {
					if (-tGy > 2.5 * tGx) ot[j] = 6; // 270
					else if (-tGy < 0.4 * tGx) ot[j] = 0; // 0
					else ot[j] = 7; // 315
				}
			}
			else if (tGx < 0) {
				if (Math.abs(tGy) < EPSILON) {
					ot[j] = 4; // 180
				}
				else if (tGy > 0) {
					if (tGy > 2.5 * -tGx) ot[j] = 2; // 90
					else if (tGy < 0.4 * -tGx) ot[j] = 4; // 180
					else ot[j] = 3; // 135
				}
				else if (tGy < 0) {
					if (-tGy > 2.5 * -tGx) ot[j] = 6; // 270
					else if (-tGy < 0.4 * -tGx) ot[j] = 4; // 180
					else ot[j] = 5; // 225
				}
			}
		}
		return new HOG_IntegralImage(bimg.getWidth(),bimg.getHeight(),nBin,Gmax,ot);

	}

		
	public static double getHOGFeatureAtIndex(Candidate c, HOG_IntegralImage ii, int index) {

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
		
		int[] sectionInfo  = new int[] { x        ,y        ,size2,size2,
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
				                      sectionInfo[region*4], 
				                      sectionInfo[region*4+1],
				                      sectionInfo[region*4+2],
				                      sectionInfo[region*4+3]);
				double b1 = ii.getSum(b, 
				                      sectionInfo[(4+region)*4], 
				                      sectionInfo[(4+region)*4+1],
				                      sectionInfo[(4+region)*4+2],
				                      sectionInfo[(4+region)*4+3]);
				double area = sectionInfo[region*4+2]*sectionInfo[region*4+3] -
				              sectionInfo[(4+region)*4+2]*sectionInfo[(4+region)*4+3];
				v = (b0 - b1) / area; 
		}
		else {
				double b0 = ii.getSum(b, 
				                      sectionInfo[region*4], 
				                      sectionInfo[region*4+1],
				                      sectionInfo[region*4+2],
				                      sectionInfo[region*4+3]);
				double area = sectionInfo[region*4+2]*sectionInfo[region*4+3];
				v = b0 / area; 	
		}
				
		return v;
	}
	
	public static double[] getHOGFeatureVector(Candidate c, HOG_IntegralImage ii) {
		
		double[] HOG = new double[nFeature];
		
		for (int i=0;i<HOG.length;i++) {
			HOG[i] = getHOGFeatureAtIndex(c,ii,i);
		}
						
		return HOG;
		
	}			
}
