package skin;

import shared.Candidate;

/**
 * Contains a collection of static methods for skin detection
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Skin_Core {

  // refer to http://seed.ucsd.edu/mediawiki/index.php/Image:HOG_Feature_Mask.jpg
	public static final int nBin = 2;  // negative bin and positive bin
	public static final int nSection = 16;
	public static final int nFeature = nSection * nBin;
		
	
	public static double getSkinFeatureAtIndex(Candidate c, Skin_IntegralImage skinii, int index) {
		
		int x = c.x, y = c.y, size = c.size;
		
		// check if the window is entirely inside the image
		if (x < 0 || x+size < 0 ||
				x > skinii.getWidth() || x+size > skinii.getWidth() || 
				y < 0 || y+size < 0 ||
				y > skinii.getHeight() || y+size > skinii.getHeight()) {
			throw new RuntimeException("Invalid detection window");
		}
		
		// adjust the window so that its size is a multiple of 8 and it's at the center
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
			double t0 = skinii.getSum(b,sectionInfo[region*4], 
			                          sectionInfo[region*4+1],
			                          sectionInfo[region*4+2],
			                          sectionInfo[region*4+3]);
			double t1 = skinii.getSum(b,sectionInfo[(4+region)*4], 
			                          sectionInfo[(4+region)*4+1],
			                          sectionInfo[(4+region)*4+2],
			                          sectionInfo[(4+region)*4+3]);
			double area = sectionInfo[region*4+2]*sectionInfo[region*4+3] -
                    sectionInfo[(4+region)*4+2]*sectionInfo[(4+region)*4+3];
			v = (t0 - t1) / area;		
		}
		else {
			double t0 = skinii.getSum(b,sectionInfo[region*4], 
			                          sectionInfo[region*4+1],
			                          sectionInfo[region*4+2],
			                          sectionInfo[region*4+3]);
			double area = sectionInfo[region*4+2]*sectionInfo[region*4+3];
			v = t0 / area;
		}
						
		return v;		
	}
	
	
	public static double[] getSkinFeatureVector(Candidate c, Skin_IntegralImage skinii) {
		
		double[] score = new double[nFeature];
		
		for (int i=0;i<score.length;i++) {
			score[i] = getSkinFeatureAtIndex(c,skinii,i);
		}
		
		return score;		
	}		
	
}
