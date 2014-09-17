package core.huemap;

import java.awt.Color;
import java.awt.image.BufferedImage;

public class HueMap {

	private final int MAXBIN = 60;
	private final float B_THRESHOLD = 0.05f;
	private final float S_THRESHOLD = 0.05f;
	private final float[] smoothingFilter = {1f,1f,1f,1f,1f};
	private int[] map;
	private int total;
	
	public HueMap() {
		map = new int[MAXBIN];
		total = 0;
	}
	
	public HueMap(BufferedImage rgbImg) {
		this();
		int[] rgbArray = rgbImg.getRGB(0, 0, rgbImg.getWidth(), rgbImg.getHeight(), null, 0, rgbImg.getWidth());
		for (int i=0;i<rgbArray.length;i++) {
			
			int pixel = rgbArray[i];
			int r = (pixel >> 16) & 0xff;
			int g = (pixel >> 8)  & 0xff;
			int b = pixel & 0xff;
			float[] hsb = Color.RGBtoHSB(r, g, b, null);
			
			// good lighting condition
			if (hsb[1] > S_THRESHOLD && hsb[2] > B_THRESHOLD) {
				this.addHue(hsb[0]);
			}
		}	
	}
	
	public void addHue(float h) {
		int b = Math.round(h*MAXBIN)%MAXBIN;
		map[b]++;
		total++;
	}
	
	public int[] getMap() {
		return map;
	}
	
	public float[] getNormalizedMap() {
		float[] ret = new float[MAXBIN];
		for (int i=0;i<MAXBIN;i++) {
			ret[i] = (float)map[i] / (float)total;
		}
		return ret;
	}
	
	public float[] getSmoothNormalizedMap() {
		float[] ret = new float[MAXBIN];
		int fw = (smoothingFilter.length-1)/2; 
			
		float sum = 0f;
		for (int i=0;i<MAXBIN;i++) {
			float k = 0;
			for (int j=-fw;j<=fw;j++) {
				int idx=(i+j+MAXBIN)%MAXBIN;
				k += map[idx]*smoothingFilter[j+fw];
			}
			ret[i] = k;
			sum += k;
		}
		for (int i=0;i<MAXBIN;i++) {
			ret[i] = ret[i] / sum;
		}
		return ret;
	}
	
	public BufferedImage createMapImage(BufferedImage rgbImg) {
		
		// get normalized map
		float[] normalizedMap = getNormalizedMap();
		
		// compute max
		float max = Float.MIN_VALUE;
		for (int i=0;i<MAXBIN;i++) {
			if (normalizedMap[i] > max) {
				max = normalizedMap[i];
			}
		}
		
		BufferedImage out = new BufferedImage(rgbImg.getWidth(),rgbImg.getHeight(),BufferedImage.TYPE_INT_RGB);
		int[] rgbArray = rgbImg.getRGB(0, 0, rgbImg.getWidth(), rgbImg.getHeight(), null, 0, rgbImg.getWidth());
		int[] mapImage = new int[rgbArray.length];
		
		for (int i=0;i<rgbArray.length;i++) {
			int pixel = rgbArray[i];
			int r = (pixel >> 16) & 0xff;
			int g = (pixel >> 8) & 0xff;
			int b = pixel & 0xff;
			float[] hsb = Color.RGBtoHSB(r, g, b, null);
						
			// good lighting condition
			if (hsb[1] > S_THRESHOLD && hsb[2] > B_THRESHOLD) {
				int bin = Math.round(hsb[0]*MAXBIN)%MAXBIN;
				int v = Math.round((normalizedMap[bin]/max)*255);
				mapImage[i] = v | (v << 8) | (v << 16);
			}
		}
		
		out.setRGB(0, 0, out.getWidth(), out.getHeight(), mapImage, 0, out.getWidth());
		return out;
	}

	
	public BufferedImage createSmoothMapImage(BufferedImage rgbImg) {
		
		// get smooth normalized map
		float[] normalizedMap = getSmoothNormalizedMap();
		
		// compute max
		float max = Float.MIN_VALUE;
		for (int i=0;i<MAXBIN;i++) {
			System.out.println(normalizedMap[i]);
			if (normalizedMap[i] > max) {
				max = normalizedMap[i];
			}
		}
		
		BufferedImage out = new BufferedImage(rgbImg.getWidth(),rgbImg.getHeight(),BufferedImage.TYPE_INT_RGB);
		int[] rgbArray = rgbImg.getRGB(0, 0, rgbImg.getWidth(), rgbImg.getHeight(), null, 0, rgbImg.getWidth());
		int[] mapImage = new int[rgbArray.length];
		
		for (int i=0;i<rgbArray.length;i++) {
			int pixel = rgbArray[i];
			int r = (pixel >> 16) & 0xff;
			int g = (pixel >> 8) & 0xff;
			int b = pixel & 0xff;
			float[] hsb = Color.RGBtoHSB(r, g, b, null);
						
			// good lighting condition
			if (hsb[1] > S_THRESHOLD && hsb[2] > B_THRESHOLD) {
				int bin = Math.round(hsb[0]*MAXBIN)%MAXBIN;
				int v = Math.round((normalizedMap[bin]/max)*255);
				mapImage[i] = v | (v << 8) | (v << 16);
			}
		}
		
		out.setRGB(0, 0, out.getWidth(), out.getHeight(), mapImage, 0, out.getWidth());
		return out;
	}
	
}
