package app.facetracker;

import java.awt.image.BufferedImage;

import core.facedetector.FaceCandidate;
import core.facedetector.FaceXMLReader;
import core.facedetector.SkinDetector;
import core.facedetector.Util;
import core.facedetector.features.HogCache;
import core.facedetector.features.HogFeature;
import core.facedetector.features.HogIntegralImage;
import core.facedetector.features.HsvSkinCache;
import core.facedetector.features.HsvSkinIntegralImage;
import core.facedetector.features.SkinCache;
import core.facedetector.features.SkinIntegralImage;
import core.nh.particlefilter.State;
import core.nh.videotracker.TargetScorer;
import core.nh.videotracker.TargetState;


/**
 *  Default scorer using the score from face detector 
 *  
 * @author scheaman
 *
 */
public class FaceScorer extends TargetScorer {

	private SkinIntegralImage skinii;
	private HogIntegralImage hogii;
	private HsvSkinIntegralImage hsvskinii;
	public double[] skinScore;

	// Assuming 640x480 
	final int MIN_FACE_SIZE = 48;
	final int MAX_FACE_SIZE = 320;
	final double MIN_SCORE = -50;

	int imageWidth;
	int imageHeight;
	int resizeFactor;

	FaceXMLReader scorer;
	SkinDetector skindetector;

	public FaceScorer(String face_xml, String skin_xml)  {
		try {
			scorer = new FaceXMLReader(face_xml);
			skindetector = new SkinDetector(skin_xml);
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		imageWidth = 0;
		imageHeight = 0;
		resizeFactor = 1;
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
				int r = (packedRgbs[oldIdx] >> 16) & 0xff;
				int g = (packedRgbs[oldIdx] >> 8)  & 0xff;
				int b = (packedRgbs[oldIdx])       & 0xff;

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

		skinii = new SkinIntegralImage(skinScore, newW, newH, 
				x1,y1,x2,y2,resizeFactor);
		hogii = HogFeature.getHOGIntegralImage(rgbs, newW, newH, 
				x1,y1,x2,y2,resizeFactor);
		hsvskinii = new HsvSkinIntegralImage(hsv, newW, newH, 
				x1,y1,x2,y2,resizeFactor);

	}

	public double getScore(State state) {

		int minSize = MIN_FACE_SIZE;
		int maxSize = MAX_FACE_SIZE; 

		TargetState fs = (TargetState)state;
		double[] s = fs.getStateVector();
		int x = (int)(s[0]-(s[2]/2));
		int y = (int)(s[1]-(s[2]/2));
		int size = (int)(s[2]);

		if (x < 0 || 
			x+size >= imageWidth ||
			y < 0 || 
			y+size >= imageHeight ||
			size < minSize || 
			size > maxSize) {
			return this.MIN_SCORE;
		}
		else {
			return getScore(x, y, size);
		}
	}

	public double getScore(int x, int y, int size) {
		FaceCandidate c = new FaceCandidate(x,y,size); 
		HogCache hogcache = new HogCache(c,hogii);
		SkinCache skincache = new SkinCache(c,skinii);
		HsvSkinCache hsvskincache = new HsvSkinCache(c,hsvskinii);
		return scorer.getCandidateScore(c, skincache, hogcache, null, hsvskincache);
	}

	@Override
	public int getMaxSize() {
		return MAX_FACE_SIZE;
	}

	@Override
	public int getMinSize() {
		return MIN_FACE_SIZE;
	}

	public double getMinScore() {
		return MIN_SCORE;
	}
	
}
