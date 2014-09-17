package facetracker;

import java.awt.image.BufferedImage;

import features.HogCache;
import features.HogFeature;
import features.HogIntegralImage;
import features.SkinCache;
import features.SkinIntegralImage;
import features.HsvSkinCache;
import features.HsvSkinIntegralImage;

import particlefilter.Scorer;
import particlefilter.State;
import shared.Candidate;
import shared.Util;

/**
 *  Default scorer using the score from face detector 
 *  
 * @author scheaman
 *
 */
public class FaceScorer implements Scorer {

	private SkinIntegralImage skinii;
	private HogIntegralImage hogii;
        private HsvSkinIntegralImage hsvskinii;
	public double[] skinScore;

	final int MIN_FACE_SIZE = 32;
	final double MIN_SCORE = -1000;
	
	int imageWidth;
	int imageHeight;

	facedetector.FaceScorer scorer;
	skindetector.SkinDetector skindetector;

	public FaceScorer(String face_xml, String skin_xml)  {
		try {
			scorer = new facedetector.FaceScorer(face_xml);
			skindetector = new skindetector.SkinDetector(skin_xml);
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		imageWidth = 0;
		imageHeight = 0;
	}

	public int getWidth() {
		return imageWidth;
	}

	public int getHeight() {
		return imageHeight;
	}

	public void loadImage(BufferedImage bimg, int resizeFactor) {
		loadImage(bimg,resizeFactor,0,0,bimg.getWidth()-1,bimg.getHeight()-1);
	}

	public void loadImage(BufferedImage bimg, int resizeFactor, 
	                      int x1, int y1, int x2, int y2) {
		
		int w = bimg.getWidth();
		int h = bimg.getHeight();

		int[] packedRgbs = new int[w*h];
		bimg.getRGB(0, 0, w, h, 
		            packedRgbs, 0, w);
		
		imageWidth = w;
		imageHeight = h;

		int newW = w/resizeFactor;
		int newH = h/resizeFactor;

		int l = newW * newH;
		int[] rgbs = new int[l * 3];
		double[] hsv = new double[l * 3];
		double[] temp_hsv = new double[3];
		skinScore = new double[l];
		
		for (int i=0; i<newH; i++) {
			for (int j=0; j<newW; j++) {

				int idx = i*newW+j;
				int oldIdx = (i*resizeFactor)*w+j*resizeFactor;

				// sampling a pixel based on resizeFactor;
				int b = (packedRgbs[oldIdx])       & 0xff;
				int g = (packedRgbs[oldIdx] >> 8)  & 0xff;
				int r = (packedRgbs[oldIdx] >> 16) & 0xff;

				Util.rgb2hsv(r, g, b, temp_hsv);

				rgbs[idx*3]   = r;
				rgbs[idx*3+1] = g;
				rgbs[idx*3+2] = b;

				hsv[idx*3]     = temp_hsv[0];
				hsv[idx*3 + 1] = temp_hsv[1];
				hsv[idx*3 + 2] = temp_hsv[2];	
			}
		}

		// scale
		x1 = x1/resizeFactor;
		y1 = y1/resizeFactor;
		x2 = x2/resizeFactor;
		y2 = y2/resizeFactor;
		
		// detect skin
		skinScore = skindetector.getScoreVector(hsv, newW, newH);

		//skinii = new SkinIntegralImage(skinScore, newW, newH, 
		//                               x1,y1,x2,y2,resizeFactor);

		hogii = HogFeature.getHOGIntegralImage(rgbs, newW, newH, 
		                                       x1,y1,x2,y2,resizeFactor);

		hsvskinii = new HsvSkinIntegralImage(hsv, newW, newH,
						     x1,y1,x2,y2,resizeFactor);
	}


	public double getScore(State state) {

		int minSize = this.MIN_FACE_SIZE;
		int maxSize = imageHeight;

		FaceState fs = (FaceState)state;
		double[] s = fs.getStateVector();
		int x = (int)(s[0]-(s[2]/2));
		int y = (int)(s[1]-(s[2]/2));
		int size = (int)(s[2]);

		if (s[0] < 0 || s[0] >= imageWidth ||
				s[1] < 0 || s[1] >= imageHeight ||
				s[2] < minSize || s[2] >= maxSize) {
			return this.MIN_SCORE;
		}
		else {
			if (size < minSize) size = minSize;
			else if (size >= maxSize) size = maxSize;
			if (x+size >= imageWidth) x = imageWidth-size-1;
			if (x < 0) x = 0;
			if (y+size >= imageHeight) y = imageHeight-size-1;
			if (y < 0) y = 0;
			return getScore(x, y, size);
		}
	}

	public double getScore(Candidate c) {
		HogCache hogcache = new HogCache(c,hogii);
		SkinCache skincache = new SkinCache(c,skinii);
		HsvSkinCache hsvskincache = new HsvSkinCache(c,hsvskinii);

		double s = scorer.getCandidateScore(c, skincache, hogcache, null,hsvskincache);
		if (s < 3) s = -10;
		return s;
	}

	public double getScore(int x, int y, int size) {

		Candidate c = new Candidate(x,y,size); 
		HogCache hogcache = new HogCache(c,hogii);
		SkinCache skincache = new SkinCache(c,skinii);
		HsvSkinCache hsvskincache = new HsvSkinCache(c,hsvskinii);

		double s = scorer.getCandidateScore(c, skincache, hogcache, null,hsvskincache);
		if (s < 3) s = -10;
		return s;
	} 	
		
}
