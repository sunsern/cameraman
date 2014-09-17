package HOG;

import shared.*;

public class HOG_Cache {
	
		Candidate _c;
		HOG_IntegralImage _ii;
		double[] HOGvalue; 
		boolean[] cached;
	  
	  public HOG_Cache(Candidate c, HOG_IntegralImage ii) {
	  	_c = c;
	  	_ii = ii;
	  	HOGvalue = new double[HOG_Core.nFeature];
	  	cached = new boolean[HOG_Core.nFeature];
		}
		
		public double getFeatureValue(int index) {
			if (index >= 0 && index < HOGvalue.length) {
				if (!cached[index]) {
					HOGvalue[index] = HOG_Core.getHOGFeatureAtIndex(_c, _ii, index); 
					cached[index] = true;
				}
				return HOGvalue[index];
			}
			else 
				throw new RuntimeException("Invalid HOG index");
		}	
}
