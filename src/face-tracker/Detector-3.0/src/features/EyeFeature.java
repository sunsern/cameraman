package features;

import shared.*;

/**
 * Used to store skin color feature 
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class EyeFeature extends AbstractFeature {

	public static final int nFeature = 1;
	
	public static final int TYPEID = 5;
	public static final String NAME = "eye";
	public int eyeindex;
	
	public double getFeatureValue(Candidate c, GrayIntegralImage ii) {
		return getEyeFeature(c, ii);
	}

	public boolean testCandidate(Candidate c, GrayIntegralImage ii) {
		double v = getEyeFeature(c, ii);
		return v < this.getThreshold();
	}

	//--------- Static Methods -----------//
	
	public static double getEyeFeature(Candidate c, GrayIntegralImage ii) {
		
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
		
		int box0_x = x + 2 * size8;
		int box0_y = y + 3 * size8;
		int box0_w = size / 4;
		int box0_h = size / 4;
		
		int box1_x = x + size / 2;
		int box1_y = box0_y;
		int box1_w = size / 4;
		int box1_h = size / 4;
		
		int box2_x = box0_x + size / 16;
		int box2_y = box0_y + size / 16;
		int box2_w = size8;
		int box2_h = size8;
		
		int box3_x = box1_x + size / 16;
		int box3_y = box1_y + size / 16;
		int box3_w = size8;
		int box3_h = size8;
		
		double b0 = (double)ii.getSum(box0_x, box0_y, box0_w, box0_h);
		double b1 = (double)ii.getSum(box1_x, box1_y, box1_w, box1_h);		
		double b2 = (double)ii.getSum(box2_x, box2_y, box2_w, box2_h);
		double b3 = (double)ii.getSum(box3_x, box3_y, box3_w, box3_h);

		double temp3 = (b0-b2)/(box0_w*box0_h - box2_w*box2_h) - b2/(box2_w * box2_h);
		double temp4 = (b1-b3)/(box1_w*box1_h - box3_w*box3_h) - b3/(box3_w * box3_h);
		
		//System.out.println("t1: " + temp1);
		//System.out.println("t2: " + temp2);
		
		return temp3 + temp4;
		
	}		
	
	
}
