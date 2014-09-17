package gray;

import shared.Candidate;


/**
 * Contains a collection of static methods for skin detection
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Gray_Core {

	public static final int nBlock = 6;  
	public static final int nFeature = nBlock * nBlock;
		
	
	public static double[] getGreyFeatureVector(Candidate c, Gray_IntegralImage ii) {
		
		int x = c.x, y = c.y, size = c.size;
		
		// check if the window is entirely inside the image
		if (x < 0 || x+size < 0 ||
				x > ii.getWidth() || x+size > ii.getWidth() || 
				y < 0 || y+size < 0 ||
				y > ii.getHeight() || y+size > ii.getHeight()) {
			throw new RuntimeException("Invalid detection window");
		}
		
		// adjust the window so that its size is a multiple of 8 and it's at the center
		int pad = (size % 8) / 2;
		x = x + pad;
		y = y + pad;
		size = size - (size % 8);
		
		int size8 = size / 8;
		
		int box_x = x + size8;
		int box_y = y + size8;
		
		double[] score = new double[nFeature]; 
		
		for (int row=0;row<nBlock;row++) {
			for (int col=0;col<nBlock;col++) {				
				int xx = box_x + col * size8;
				int yy = box_y + row * size8;
				score[row*nBlock + col] = (double)ii.getSum(xx, yy, size8, size8) / (size8*size8);		
			}
		}
		
		// center at 0 and normalize by its variance
		double mean = 0;
		for (int i=0;i<nFeature;i++) {
			mean += score[i];
		}
		mean /= nFeature;
		
		double var = 0;
		for (int i=0;i<nFeature;i++) {
				var += (score[i] - mean) * (score[i] - mean);
		}
		var /= nFeature;
		
		double[] normalized = new double[nFeature];
		
		for (int i=0;i<nFeature;i++) {
			normalized[i] = (score[i] - mean) / var;
		}
		
		return normalized;
				
	}		
	
}
