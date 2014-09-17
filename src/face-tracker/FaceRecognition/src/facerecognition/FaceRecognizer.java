package facerecognition;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.util.StringTokenizer;

import javax.imageio.ImageIO;


public class FaceRecognizer {

	final int K = 200;
	final int SIZE = 120;
		
	double[] mean;
	double[][] eigenfaces;
        int[] count;
	
        public FaceRecognizer(String meanFile, String eigenfaceFile) {
		
		System.out.println("Loading eigenfaces...");
		mean = loadMean(meanFile);
		eigenfaces = loadEigenfaces(eigenfaceFile);
		System.out.println("Done!");

		count = new int[4];

	}

        public void reset() {
	    count[0] = 0;
	    count[1] = 0;
	    count[2] = 0;
	    count[3] = 0;
        }
	
	public String recognize(BufferedImage bimg) {
		return recognize(bimg,0,0,bimg.getHeight()); 
	}
	
	public String recognize(BufferedImage frame, int x, int y, int size) {
		
		BufferedImage bimg = frame.getSubimage(x, y, size, size);
		
		// resize to SIZE x SIZE by sampling 
		// and convert to grayscale
		
		int w = bimg.getWidth();
		int h = bimg.getHeight();

		double scale = (double)w / SIZE;
	
		int[] rgbsPacked = new int[w * h];
		bimg.getRGB(0, 0, w, h, 
		            rgbsPacked, 0, w);
		
		int[] gray = new int[SIZE * SIZE];
		int c = 0;
		// need same order as Matlab
		for (int i=0;i<SIZE;i++) {
			for (int j=0;j<SIZE;j++) {
				
				int tx = (int)(i*scale);
				int ty = (int)(j*scale);
				
				int idx = ty*w+tx;		
				int b = (rgbsPacked[idx])       & 0xff;
				int g = (rgbsPacked[idx] >> 8)  & 0xff;
				int r = (rgbsPacked[idx] >> 16) & 0xff;
				
				gray[c++]  = (int)(0.3*r + 0.58*g + 0.12*b);
			}
		}
		
		// normalize
		double[] normalized = normalize(gray);
		
		// substract mean
		for (int i=0;i<SIZE*SIZE;i++) {
			normalized[i] = normalized[i] - mean[i];
		}
		
		// project on eigenfaces
		Double[] features = new Double[K];
		for (int k=0;k<K;k++) {
			double sum = 0;
			for (int i=0;i<SIZE*SIZE;i++) {
				sum += normalized[i]*eigenfaces[k][i];
			}
			features[k] = new Double(sum);
		}
		
		// run through classifiers
		double[] yoav = facerecognition.yoav.Predict.predict(features);
		double[] sunsern = facerecognition.sunsern.Predict.predict(features);
		double[] evan = facerecognition.evan.Predict.predict(features);
		
		System.out.println("yoav scrore = " + yoav[1]);
		System.out.println("sunsern score = " + sunsern[1]);
		System.out.println("evan scrore = " + evan[1]);

		if (yoav[1] > 3) { 
		    count[0]++;
 		    System.out.println("I think you're Yoav...[" + 
				       yoav[1] + "]");
		}
		if (sunsern[1] > 3) {
		    count[1]++;
 		    System.out.println("I think you're Sunsern...[" + 
				       sunsern[1] + "]");
		}
		if (evan[1] > 5) {
		    count[2]++;
 		    System.out.println("I think you're Evan...[" +
				       evan[1] + "]");
		}

		if (yoav[1] < -8 && sunsern[1] < -8 && evan[1] < -8) {
		    count[3]++;
		    System.out.println("I think I don't know you...");
		}

		if (count[0] > 10)
		    return "Hi, Yoav!";
		
		if (count[1] > 10)
		    return "Hi, Sunsern!";

		if (count[2] > 10)
		    return "Hi, Evan!";

		if (count[3] > 100)
		    return "Do I know you?";
		
		return null;

	}
	
	private double[] normalize(int[] gray) {
		
		double mean = 0;
		double stDev = 0;
		
		int l = gray.length;
		for (int i=0;i<l;i++) {
			mean += gray[i];
		}
		mean /= l;
		
		for (int i=0;i<l;i++) {
			stDev += Math.pow((double)gray[i] - mean,2);
		}

		stDev = Math.sqrt(stDev/l);
		
		double[] normalized = new double[l];
		
		if (stDev > 1e-7) {
			for (int i=0;i<l;i++) {
				normalized[i] = ((double)gray[i] - mean)/stDev;
			}
		}
		return normalized;
	}

	private double[] loadMean(String fileName) {
		double[] mean = new double[SIZE * SIZE];
		int counter = 0;
		try {
			BufferedReader br = new BufferedReader(new FileReader(new File(fileName)));
			String aline = br.readLine();
			while (aline != null) {
				StringTokenizer st = new StringTokenizer(aline);
				while (st.hasMoreTokens()) {
					String token = st.nextToken();
					mean[counter++] = Double.parseDouble(token);
				}
				aline = br.readLine();
			}
			br.close();
		}
		catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return mean;
	}

	private double[][] loadEigenfaces(String fileName) {
		double[][] eigenfaces = new double[K][SIZE * SIZE];
		try {
			BufferedReader br = new BufferedReader(new FileReader(new File(fileName)));
			String aline = br.readLine();
			int j = 0;
			while (aline != null) {
				StringTokenizer st = new StringTokenizer(aline);
				int i = 0;
				while (st.hasMoreTokens()) {
					String token = st.nextToken();
					eigenfaces[i++][j] = Double.parseDouble(token);
				}
				j++;
				aline = br.readLine();
			}
			br.close();
		}
		catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		return eigenfaces;
	}
	
	public static void main(String[] args) {
		
		FaceRecognizer fr = new FaceRecognizer("/home/scheaman/Desktop/mean.csv",
		                                       "/home/scheaman/Desktop/eigenfaces.csv");
		
		try {
			String output;
			output = fr.recognize(ImageIO.read(new File("/home/scheaman/Desktop/test2.png")));
			System.out.println(output);
			output = fr.recognize(ImageIO.read(new File("/home/scheaman/Desktop/test3.png")));
			System.out.println(output);
			output = fr.recognize(ImageIO.read(new File("/home/scheaman/Desktop/test4.png")));
			System.out.println(output);
			output = fr.recognize(ImageIO.read(new File("/home/scheaman/Desktop/test.png")));
			System.out.println(output);
		}
		catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	
}
