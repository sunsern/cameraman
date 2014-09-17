package shared;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

public class Util {

	private static double EPSILON = 0.000001d;

	public static void rgb2hsv(int r, int g, int b, double hsv[]) {
		
		int min; // Min. value of RGB
		int max; // Max. value of RGB
		int delMax; // Delta RGB value

		if (r > g) {
			min = g;
			max = r;
		}
		else {
			min = r;
			max = g;
		}
		if (b > max) max = b;
		if (b < min) min = b;

		delMax = max - min;
		
		double H = 0, S;
		double V = max / 255f;

		if (delMax == 0) {
			H = 0;
			S = 0;
		}
		else {
			S = (double)delMax / max;
			if (r == max) H = ((double)(g - b) / (double) delMax) * 60;
			else if (g == max) H = (2 + (double)(b - r) / (double) delMax) * 60;
			else if (b == max) H = (4 + (double)(r - g) / (double) delMax) * 60;
		}
		if (H < 0) H += 360;
		if (H > 360) H -= 360;
		
		hsv[0] = H / 360f;
		hsv[1] = S;
		hsv[2] = V;
		 
	}
	
	public static void convertHSVtoRGB(float h, float s, float v, float[] rgb) {
    float r = 0;
    float g = 0;
    float b = 0;

    if (s == 0) {
      // this color in on the black white center line <=> h = UNDEFINED
      if (Float.isNaN(h)) {
        // Achromatic color, there is no hue
        r = v;
        g = v;
        b = v;
      } else {
      	//throw new IllegalArgumentException(INVALID_H_MSG);
      	System.out.println("Invalid H");
      }
    } else {
      if (h == 360) {
        // 360 is equiv to 0
        h = 0;
      }

      // h is now in [0,6)
      h = h / 60;

      int i = (int) Math.floor(h);
      float f = h - i; //f is fractional part of h
      float p = v * (1 - s);
      float q = v * (1 - (s * f));
      float t = v * (1 - (s * (1 - f)));

      switch (i) {
        case 0:
          r = v;
          g = t;
          b = p;

          break;

        case 1:
          r = q;
          g = v;
          b = p;

          break;

        case 2:
          r = p;
          g = v;
          b = t;

          break;

        case 3:
          r = p;
          g = q;
          b = v;

          break;

        case 4:
          r = t;
          g = p;
          b = v;

          break;

        case 5:
          r = v;
          g = p;
          b = q;

          break;
      }
    }

    // now assign everything....
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
  }

	public static void outputEdgeImage(BufferedImage bimg, String filename) {
		
		int[] rgbs = new int[bimg.getWidth() * bimg.getHeight()];
		bimg.getRGB(0, 0, bimg.getWidth(), bimg.getHeight(), rgbs, 0,
								bimg.getWidth());
		
		int[][] rgb2d = new int[bimg.getWidth() * bimg.getHeight()][3];
		
		for (int j=0; j < rgbs.length; j++) {
			rgb2d[j][0] = rgbs[j] & 0xff;
			rgb2d[j][1] = (rgbs[j] >> 8) & 0xff;
			rgb2d[j][2] = (rgbs[j] >> 16) & 0xff;
		}
		
		/* compute gradient using [-1 0 1] and its transpose */
		
		/* [-1 0 1]*/
		double[][] Gx = new double[bimg.getWidth() * bimg.getHeight()][3];
		
		/* [-1 0 1]' */
		double[][] Gy = new double[bimg.getWidth() * bimg.getHeight()][3];

		for (int j=0;j<3;j++) {
			for (int y=0;y<bimg.getHeight();y++) {
				for (int x=0;x<bimg.getWidth();x++) {	
					if (x-1 >= 0 && x+1 < bimg.getWidth()) {
						Gx[y*bimg.getWidth() + x][j] = 
							-1 * rgb2d[y*bimg.getWidth() + (x-1)][j] +
							1 * rgb2d[y*bimg.getWidth() + (x+1)][j];
					}
					else 
						Gx[y*bimg.getWidth() + x][j] = 0;

					if (y-1 >= 0 && y+1 < bimg.getHeight()) {
						Gy[y*bimg.getWidth() + x][j] = 
							-1 * rgb2d[(y-1)*bimg.getWidth() + x][j] +
							1 * rgb2d[(y+1)*bimg.getWidth() + x][j];
					}
					else
						Gy[y*bimg.getWidth() + x][j] = 0;
				}
			}
		}
		
		/* sqrt(Gx^2 + Gy^2) */
		double[][] G = new double[bimg.getWidth() * bimg.getHeight()][3];
		for (int j=0;j<3;j++) {
			for (int k=0;k < bimg.getWidth() * bimg.getHeight(); k++)
				G[k][j] = Math.sqrt(Gx[k][j]*Gx[k][j] + Gy[k][j]*Gy[k][j]);
		}
	
		double[] Gmax = new double[bimg.getWidth() * bimg.getHeight()];
		double tGx, tGy;
		
		/* calculate orientation
		 * -1 = undefined 
		 *  0 = 0 	
		 *  1 = 45 
		 *  2 = 90 
		 *  3 = 135 
		 *  4 = 180 
		 *  5 = 225 
		 *  6 = 270 
		 *  7 = 315 
		 */ 
		int[] ot = new int[bimg.getWidth() * bimg.getHeight()];
		
		for (int j=0;j < ot.length; j++) {
			
			// find maximum
			if (G[j][0] > G[j][1] && G[j][0] > G[j][2]) {
				Gmax[j] = G[j][0];
				tGx = Gx[j][0];
				tGy = Gy[j][0];
			}
			else if (G[j][1] > G[j][0] && G[j][1] > G[j][2]) {
				Gmax[j] = G[j][1];
				tGx = Gx[j][1];
				tGy = Gy[j][1];
			}
			else {
				Gmax[j] = G[j][2];
				tGx = Gx[j][2];
				tGy = Gy[j][2];
			}
			
			// find corresponding orientation
			if (Math.abs(tGx) < EPSILON) {
				if (Math.abs(tGy) < EPSILON) ot[j] = -1; // undefined
				else if (tGy > 0)	ot[j] = 2; // 90
				else if (tGy < 0)	ot[j] = 6; // 270
				
			}
			else if (tGx > 0) {
				if (Math.abs(tGy) < EPSILON) {
					ot[j] = 0; // 0
				}
				else if (tGy > 0) {
					if (tGy > 2.5 * tGx) ot[j] = 2; // 90
					else if (tGy < 0.4 * tGx) ot[j] = 0; // 0
					else ot[j] = 1; // 45
				}
				else if (tGy < 0) {
					if (-tGy > 2.5 * tGx) ot[j] = 6; // 270
					else if (-tGy < 0.4 * tGx) ot[j] = 0; // 0
					else ot[j] = 7; // 315
				}
			}
			else if (tGx < 0) {
				if (Math.abs(tGy) < EPSILON) {
					ot[j] = 4; // 180
				}
				else if (tGy > 0) {
					if (tGy > 2.5 * -tGx) ot[j] = 2; // 90
					else if (tGy < 0.4 * -tGx) ot[j] = 4; // 180
					else ot[j] = 3; // 135
				}
				else if (tGy < 0) {
					if (-tGy > 2.5 * -tGx) ot[j] = 6; // 270
					else if (-tGy < 0.4 * -tGx) ot[j] = 4; // 180
					else ot[j] = 5; // 225
				}
			}
		}
		
		// output edge image
		BufferedImage edge = new BufferedImage(bimg.getWidth(), bimg.getHeight(),	BufferedImage.TYPE_INT_RGB);
		float max_v = -1;
		float[] h = new float[bimg.getHeight()*bimg.getWidth()];
		float[] v = new float[bimg.getHeight()*bimg.getWidth()];
		for (int idx=0; idx<bimg.getHeight()*bimg.getWidth(); idx++) {
			switch (ot[idx]) {
				case 0: h[idx] = 0f; break;
				case 1: h[idx] = 45f; break;
				case 2: h[idx] = 90f; break;
				case 3: h[idx] = 135f; break;
				case 4: h[idx] = 180f; break;
				case 5: h[idx] = 225f; break;
				case 6: h[idx] = 270f; break;
				case 7: h[idx] = 315f; break;
				default: h[idx] = 0f; break;
			}
			v[idx] = (float)Gmax[idx];
			if (v[idx] > max_v) max_v = v[idx];
		}
		float[] rgb = new float[3];
		for (int i=0; i<bimg.getHeight(); i++) {
			for (int j=0; j<bimg.getWidth(); j++) {
				int idx = i*bimg.getWidth() + j;
				convertHSVtoRGB(h[idx],1.0f,v[idx]/max_v,rgb);
				int r = (int)(rgb[0]*255f);
				int g = (int)(rgb[1]*255f);
				int b = (int)(rgb[2]*255f);
				edge.setRGB(j,i,((r<<16)|(g<<8)|b));
			}
		}
		
		try {
			ImageIO.write(edge, "png",
			              new File(filename));
		}
		catch (IOException e) {
			e.printStackTrace();
		}
		
	}

	
}
