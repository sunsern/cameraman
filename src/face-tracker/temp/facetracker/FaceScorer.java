package facetracker;

import java.awt.Graphics2D;
import java.awt.RenderingHints;
import java.awt.image.BufferedImage;

import features.HogCache;
import features.HogFeature;
import features.HogIntegralImage;
import features.SkinCache;
import features.SkinIntegralImage;

import particlefilter.Scorer;
import particlefilter.State;
import shared.Candidate;
import shared.Util;

public class FaceScorer implements Scorer {

	SkinIntegralImage skinii;
	HogIntegralImage hogii;
	public double[] skinScore;
	
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
	   
		// resize
		int newH = bimg.getHeight()/resizeFactor;
		int newW = bimg.getWidth()/resizeFactor;

		x1 /= resizeFactor;
		y1 /= resizeFactor;
		x2 /= resizeFactor;
		y2 /= resizeFactor;

		BufferedImage dimg = new BufferedImage(newW, 
		                                       newH,
		                                       BufferedImage.TYPE_INT_ARGB);
		Graphics2D g2d = dimg.createGraphics();
		g2d.setRenderingHint(RenderingHints.KEY_INTERPOLATION, 
		                     RenderingHints.VALUE_INTERPOLATION_NEAREST_NEIGHBOR);
		g2d.drawImage(bimg, 0, 0, newW, newH, 0, 0, bimg.getWidth(), bimg.getHeight(), null);
		g2d.dispose();
		
		imageWidth = bimg.getWidth();
		imageHeight = bimg.getHeight();
		
		int w = dimg.getWidth();
		int h = dimg.getHeight();
		
		int l = w * h;
		int[] rgbs = new int[l * 3];
		int[] packedRgbs = new int[l];
		double[] hsv = new double[l * 3];
		skinScore = new double[l];
		double[] temp_hsv = new double[3];
		dimg.getRGB(0, 0, w, h, packedRgbs, 0, w);
		
		for (int j=0; j<l; j++) {
			int b = (packedRgbs[j])       & 0xff;
			int g = (packedRgbs[j] >> 8)  & 0xff;
			int r = (packedRgbs[j] >> 16) & 0xff;

			Util.rgb2hsv(r, g, b, temp_hsv);

			rgbs[j*3]   = r;
			rgbs[j*3+1] = g;
			rgbs[j*3+2] = b;
			
			hsv[j*3]     = temp_hsv[0];
			hsv[j*3 + 1] = temp_hsv[1];
			hsv[j*3 + 2] = temp_hsv[2];	
		}
		
		// detect skin
		skinScore = skindetector.getScoreVector(hsv, w, h);
		
		skinii = new SkinIntegralImage(skinScore, w, h, 
					       x1,y1,x2,y2,resizeFactor);
		hogii = HogFeature.getHOGIntegralImage(rgbs, w, h, 
						       x1,y1,x2,y2,resizeFactor);
			
	}


	public double getScore(State state) {
		
		int MINSIZE = 32;
		int MAXSIZE = imageHeight;
			
		FaceState fs = (FaceState)state;
 		double[] s = fs.getStateVector();
 		int x = (int)(s[0]-(s[2]/2));
 		int y = (int)(s[1]-(s[2]/2));
 		int size = (int)(s[2]);
 		
		if (s[0] < 0 || s[0] >= imageWidth ||
		    s[1] < 0 || s[1] >= imageHeight ||
		    s[2] < MINSIZE || s[2] >= MAXSIZE) {
		    return -100;
		}
		else {
			if (size < MINSIZE) size = MINSIZE;
			else if (size >= MAXSIZE) size = MAXSIZE;
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

		return scorer.getCandidateScore(c, skincache, hogcache, null);
	}
	
	private double getScore(int x, int y, int size) {
	
		Candidate c = new Candidate(x,y,size); 
		HogCache hogcache = new HogCache(c,hogii);
		SkinCache skincache = new SkinCache(c,skinii);

		return scorer.getCandidateScore(c, skincache, hogcache, null);
	} 	
}
