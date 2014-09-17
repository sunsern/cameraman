package core.nh.videotracker;

import java.awt.image.BufferedImage;

import core.nh.particlefilter.Scorer;

/**
 *  Default scorer
 *  
 * @author scheaman
 *
 */
public abstract class TargetScorer implements Scorer {
	
  BufferedImage image;
  int imageWidth, imageHeight;
  
  public void loadImage(BufferedImage bimg) {
    image = bimg;
    imageHeight = bimg.getHeight();
    imageWidth = bimg.getWidth();
  }
  
  public abstract int getMinSize();
  public abstract int getMaxSize();
  
  
}
