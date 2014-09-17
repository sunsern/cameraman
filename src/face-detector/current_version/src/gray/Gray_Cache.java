package gray;

import shared.*;

/**
 * Used to store skin color information of a candidate window.
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Gray_Cache {
	
		Candidate _c;
		Gray_IntegralImage _ii;
		double[] greyfeature; 
		
	  public Gray_Cache(Candidate c, Gray_IntegralImage ii) {
	  	_c = c;
	  	_ii = ii;
	  	greyfeature = Gray_Core.getGreyFeatureVector(c, ii);
		}
		
		public double getFeatureValue(int index) {
			if ((index >= 0) && (index < greyfeature.length)) {
				return greyfeature[index];
			}
			else {
				throw new RuntimeException("Invalid skin index");
			}
		}	
}
