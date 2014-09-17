package skin;

import shared.*;

/**
 * Used to store skin color information of a candidate window.
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Skin_Cache {
	
		Candidate _c;
		Skin_IntegralImage _ii;
		double[] skinvalue; 
		boolean[] cached;
		
	  public Skin_Cache(Candidate c, Skin_IntegralImage ii) {
	  	_c = c;
	  	_ii = ii;
	  	skinvalue = new double[Skin_Core.nFeature];
	  	cached = new boolean[Skin_Core.nFeature];
		}
		
		public double getFeatureValue(int index) {
			if ((index >= 0) && (index < skinvalue.length)) {
				if (!cached[index]) {
					skinvalue[index] = Skin_Core.getSkinFeatureAtIndex(_c, _ii, index);
					cached[index] = true;
				}
				return skinvalue[index];
			}
			else {
				throw new RuntimeException("Invalid skin index");
			}
		}	
}
