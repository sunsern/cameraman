package facetracker;


import java.util.ArrayList;


import shared.Candidate;

public class FaceScanner {
	
	int imageWidth;
	int imageHeight;
	
	FaceScorer scorer;
		
	public FaceScanner(FaceScorer scorer, int imageWidth, int imageHeight) {
		this.scorer = scorer;
		this.imageHeight = imageHeight;
		this.imageWidth = imageWidth;
	}
	
	public Candidate getBestFace(double threshold) {

		ArrayList<Candidate> temp = new ArrayList<Candidate>();
		
		int w = imageWidth;
		int h = imageHeight;
	
		int minSize = imageHeight / 8;
		int maxSize = imageHeight / 2;
		
		
		for (int size=maxSize;size>minSize; size -= 8) {
			int step = 1 + size/(minSize);
			for (int y = 0; y + size < h; y += step) {
				for (int x = 0; x + size < w; x += step) {
					
					double score = scorer.getScore(x, y, size);
					
					if (score > threshold) {
						
						Candidate c = new Candidate(x,y,size);
						//c.weight = 1;
						
						boolean found = false; 
						for (int k=0;k<temp.size();k++) {
							Candidate c2 = temp.get(k);
							if (c2.isSameDetection(c)) {
								
							// union
								int t1 = Math.max(c2.x+c2.size, c.x+c.size)-Math.min(c2.x,c.x);
								int t2 = Math.max(c2.y+c2.size, c.y+c.size)-Math.min(c2.y,c.y);
								c2.x = Math.min(c2.x,c.x);
								c2.y = Math.min(c2.y,c.y);
								c2.size = Math.min(t1, t2);
								
//								c2.x = (c2.weight * c2.x + c.x) / (c2.weight+1);
//								c2.y = (c2.weight * c2.y + c.y) / (c2.weight+1);
//								c2.size = (c2.weight * c2.size + c.size) / (c2.weight+1);
//								c2.weight++;
								found = true;
								break;
							}	
						}
						
						if (!found) temp.add(c);
						
					}					
				}
			}
		}
		
		System.out.println("temp size = " + temp.size() );
		
		double bestScore = Double.NEGATIVE_INFINITY;
		Candidate bestFace = null;
		for (int i=0;i<temp.size();i++) {
			Candidate c = temp.get(i);
			double s = scorer.getScore(c.x, c.y, c.size);
			System.out.printf("[%d,%d,%d,%f]\n",c.x,c.y,c.size,s);
			if (bestScore < s) {
				bestScore = s;
				bestFace = c;
			}
		}
		
		if (bestScore < 0) return null;
		else return bestFace;
		
	}
	
}
