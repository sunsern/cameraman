package core.nh.videotracker;

import java.awt.image.BufferedImage;
import java.util.ArrayList;


public class TargetDetector {

	final int STEPSIZE = 8;
	TargetScorer scorer; 

	public TargetDetector(TargetScorer scorer) {
		this.scorer = scorer;
	}

	// no rotation
	public TargetState localizeTarget(BufferedImage bimg, double threshold) {

		ArrayList<TargetState> temp = new ArrayList<TargetState>();

		int w = bimg.getWidth();
		int h = bimg.getHeight();

		int minSize = scorer.getMinSize();
		int maxSize = scorer.getMaxSize();

		for (int size=maxSize;size>minSize; size -= STEPSIZE) {
			int step = 1 + size/(minSize);
			for (int y = 0; y + size < h; y += step) {
				for (int x = 0; x + size < w; x += step) {

					TargetState t = new TargetState(x,y,size,0);
					double score = scorer.getScore(t);

					if (score > threshold) {

						boolean found = false;
						for (int k=0;k<temp.size();k++) {
							TargetState t_old = temp.get(k);
							if (t.isSimilar(t_old)) {
								// merge
								double[] ns = t.getStateVector();
								double[] os = t_old.getStateVector(); 
								os[0] = (os[0] + ns[0])/2;
								os[1] = (os[1] + ns[1])/2;
								os[2] = (os[2] + ns[2])/2;
								found = true;
								break;
							}	
						}
						if (!found) temp.add(t);
					}					
				}
			}
		}
		
		double bestScore = Double.NEGATIVE_INFINITY;
		TargetState best = null;
		for (int i=0;i<temp.size();i++) {
			TargetState t = temp.get(i);
			double s = scorer.getScore(t);
			if (bestScore < s) {
				bestScore = s;
				best = t;
			}
		}

		if (bestScore < 0) return null;
		else return best;
	}

}
