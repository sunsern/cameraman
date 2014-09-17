package facealign;


import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FilenameFilter;

import javax.imageio.ImageIO;
import javax.swing.filechooser.FileFilter;

import facetracker.FaceState;

import particlefilter.Scorer;
import particlefilter.State;
import shared.Candidate;

public class FaceScorerMask implements Scorer {

	BufferedImage meanImg;	
	
	int[] grayMean;
	double[] normalizedMean;

	int[] grayCurrent;
	    	
	int imageWidth;
	int imageHeight;
	
	double[] weights; 
	
	public FaceScorerMask(String meanImageFile)  {
	    
	    try {
		meanImg = ImageIO.read(new File(meanImageFile));
		int w = meanImg.getWidth();
		int h = meanImg.getHeight();
		int[] rgbsMeanPacked = new int[w*h];
		meanImg.getRGB(0, 0, w, h, 
			       rgbsMeanPacked, 0, w);
		
		grayMean = new int[w * h];
		    
		for (int i=0;i<rgbsMeanPacked.length;i++) {
		    grayMean[i]   = rgbsMeanPacked[i] & 0xff;
		}
		    
		    
		normalizedMean = normalize(grayMean);
		    
		weights = new double[w * h];		
		for (int i=0;i<h;i++) {
		    for (int j=0;j<w;j++) {
			weights[i*w+j] = 
			    Math.exp(-(Math.pow(i-h/2.0,2)/(h*h/(6.25))+
				       Math.pow(j-w/2.0,2)/(w*w/6.25))); 
		    }
		}			
		    
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

	public void loadImage(BufferedImage bimg) {
		
	    imageWidth = bimg.getWidth();
	    imageHeight = bimg.getHeight();
		
	    int[] rgbsCurrentPacked = new int[bimg.getWidth() * bimg.getHeight()];
	    bimg.getRGB(0, 0, bimg.getWidth(), bimg.getHeight(), 
			rgbsCurrentPacked, 0, bimg.getWidth());
		
	    grayCurrent = new int[bimg.getWidth() * bimg.getHeight()];
		
	    for (int i=0;i<rgbsCurrentPacked.length;i++) {
		grayCurrent[i] = rgbsCurrentPacked[i] & 0xff;
	    }
	    
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
		return -1e10;
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
	    return getScore(c.x,c.y,c.size);
	}

	
	public double getScore(int x, int y, int size) {

	    int w = meanImg.getWidth();
	    int h = meanImg.getHeight();
		
	    double scale = (double)size/w;
		
	    int[] gray = new int[w*h];
		
	    // sampling pixels
	    for (int i=0;i<h;i++) {
		for (int j=0;j<w;j++) {
		
		    int tx = (int)(x+Math.floor(scale*j));
		    int ty = (int)(y+Math.floor(scale*i));
			
		    gray[i*w+j]   = grayCurrent[((ty*imageWidth)+tx)];
		}
	    }
				
	    double[] norm_gray = normalize(gray);

	    double score = 0;
	    for (int i=0;i<norm_gray.length;i++) {
		double g1 = normalizedMean[i];
		double g2 = norm_gray[i];
		score += weights[i] * g1 * g2;
	    }
	    
	    return score / (w*h);
	
	} 	
	
	
	private double[] normalize(int[] gray) {

	    double mean = 0;
	    int l = gray.length;
	    for (int i=0;i<l;i++) {
		mean += gray[i];
	    }
	    mean /= l;
		
	    double stDev = 0;					
	    for (int i=0;i<l;i++) {
		stDev += Math.pow((double)gray[i] - mean,2);
	    }
	    stDev = Math.sqrt(stDev/l);
	    
	    double[] normalized = new double[gray.length];
	    if (stDev > 0) {
		for (int i=0;i<l;i++) {
		    normalized[i] = ((double)gray[i] - mean)/stDev;
		}
	    }
	    return normalized;
	}
	
}
