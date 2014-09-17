package facedetector;
import features.GrayCache;
import features.GrayIntegralImage;
import features.HogCache;
import features.HogFeature;
import features.HogIntegralImage;
import features.HsvSkinCache;
import features.HsvSkinIntegralImage;
import features.SkinCache;
import features.SkinIntegralImage;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

import javax.imageio.ImageIO;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import shared.Candidate;
import shared.Util;
import skindetector.SkinDetector;


public class FaceDetect {

	protected FaceScorer scorer;
	protected SkinDetector sd;

	protected BufferedImage current_bimg;
	protected GrayIntegralImage grayii; 
	protected SkinIntegralImage skinii;
	protected HsvSkinIntegralImage hsvskinii;
	protected HogIntegralImage hogii;
	
	private static double thresholdDetection = 8.0d;
	
	private static final String SKIN_XML = "/home/scheaman/workspace/Detector-3.0/xml/" +
	"skin-Nov18-newHSV-cascade.xml";

	private static final String XML = "/home/scheaman/workspace/Detector-3.0/xml/" +
	"Jan22-cascade-2.xml";

	private static final int[] sizes =
		new int[] { 32, 40, 48, 56, 64, 72, 80, 88, 
		            96, 104, 112, 120, 128, 136, 144, 152,
		            160, 168, 184, 216 };

	private static String FACE_DIR = "face"; 

	private static ArrayList<Candidate> faces;
		
	public FaceDetect(String xml_filename, String skin_xml) throws ParserConfigurationException, SAXException, IOException {
	  // load face detector XML
		scorer = new FaceScorer(xml_filename);
		// load skin detector
		sd = new SkinDetector(skin_xml);
	}
	
	public void loadImage(String image_filename) throws IOException {
		
		// read image file 
		current_bimg = ImageIO.read(new File(image_filename));

		int w = current_bimg.getWidth();
		int h = current_bimg.getHeight();
		
		int[] rgbs = new int[w * h * 3];
		int[] packedRgbs = new int[w * h];
		current_bimg.getRGB(0, 0, w, h, packedRgbs, 0, w);
		
		// change color space
		int l = w * h;
		int[] gray = new int[l];
		double[] hsv = new double[l * 3];
		double[] temp_hsv = new double[3];
		
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
			
			gray[j] = (int)(0.3d*r + 0.59d*g + 0.11d*b);
		}
		
		// detect skin
		double[] skin = sd.getScoreVector(hsv, w, h);
		
		grayii = new GrayIntegralImage(gray, w, h);
		skinii = new SkinIntegralImage(skin, w, h);
		hogii = HogFeature.getHOGIntegralImage(rgbs, w, h);
		hsvskinii = new HsvSkinIntegralImage(hsv, w, h);
		
	}
	
	public double getScore(int x, int y, int size) {
		Candidate c = new Candidate(x,y,size); 
		
		HogCache hogcache = new HogCache(c,hogii);
		SkinCache skincache = new SkinCache(c,skinii);
		GrayCache graycache = new GrayCache(c,grayii);
		HsvSkinCache hsvskincache = new HsvSkinCache(c,hsvskinii);
		return scorer.getCandidateScore(c, skincache, hogcache, graycache,hsvskincache);
	}
	
	public double[] getScoreVector(int x, int y, int size) {
		Candidate c = new Candidate(x,y,size); 
		
		HogCache hogcache = new HogCache(c,hogii);
		SkinCache skincache = new SkinCache(c,skinii);
		GrayCache graycache = new GrayCache(c,grayii);
		HsvSkinCache hsvskincache = new HsvSkinCache(c,hsvskinii);
		return scorer.getCandidateScoreVector(c, skincache, hogcache, graycache,hsvskincache);	
	}

	public void unloadImage() {
		grayii = null;
		skinii = null;
		hogii = null;
	}
	
	public Candidate getBestFace(ArrayList<Candidate> faces) {
		double bestScore = -1;
		Candidate bestFace = null;
		for (int i=0;i<faces.size();i++) {
			Candidate c = faces.get(i);
			double s = getScore(c.x, c.y, c.size);
			if (bestScore < s) {
				bestScore = s;
				bestFace = c;
			}
		}
		return bestFace;
	}
	
	public ArrayList<Candidate> localizeFace(double threshold, boolean merge) {

		ArrayList<Candidate> output = new ArrayList<Candidate>();
		
		int w = current_bimg.getWidth();
		int h = current_bimg.getHeight();

		for (int j=0;j<sizes.length;j++) {
			int size = sizes[j];
			for (int y = 0; y + size < h; y += 1 + (size / sizes[0])) {
				for (int x = 0; x + size < w; x += 1 + (size / sizes[0])) {
					double score = getScore(x, y, size);
					if (score > threshold) {
						
						Candidate c = new Candidate(x,y,size);
						
						if (merge) {
							boolean found = false; 
							for (int k=0;k<output.size();k++) {
								Candidate c2 = output.get(k);
								if (c2.isSameDetection(c)) {
									// union
									int t1 = Math.max(c2.x+c2.size, c.x+c.size)-Math.min(c2.x,c.x);
									int t2 = Math.max(c2.y+c2.size, c.y+c.size)-Math.min(c2.y,c.y);
									c2.x = Math.min(c2.x,c.x);
									c2.y = Math.min(c2.y,c.y);
									c2.size = Math.min(t1, t2);

									found = true;
									break;
								}	
							}
							if (!found) output.add(c);
						}
						else	output.add(c);
						
					}					
				}
			}
		}
		
		return output;
	}
	
	// -------------------------------------------------------------------------//
	
	private static void make_dir(String path) {
		File dir = new File(path);
		if (!dir.exists()) {
			dir.mkdir();
		}
	}
	
	public static void main(String[] args) {

		if (args.length != 2) {
			System.out.println("usage: java Face_Detect <input dir> <framenumber>");
			return;
		}

		try {

			String xml_filename = XML;
			String input_dir    = args[0];
			int frame_no        = Integer.parseInt(args[1]);
			
			FilenameFilter filter = new FilenameFilter() {
				public boolean accept(File dir, String name) {
					return (name.toLowerCase().endsWith(".png") && 
							!(name.toLowerCase().startsWith("face") || 
									name.toLowerCase().startsWith("nonface")));
				}
			};

			File dir = new File(input_dir);
			if (!dir.exists()) {
				System.out.println("Error: cannot find directory " + args[1]);
				return;
			}

			// create a file list
			String[] children = dir.list(filter);
			
			if (children == null) {
				System.out.println("Error: make sure " + args[1] + " contains png files");
				return;
			}
			
			// sort by file name
			Arrays.sort(children);
			
			// create output dir if not exist
			String facePath = input_dir + File.separator + FACE_DIR + File.separator;
			make_dir(facePath);
				
			FaceDetect fd = new FaceDetect(xml_filename, SKIN_XML);
			
			// for each image 
			for (int i = 0; i < children.length; i++) {
			 
				if (i != frame_no && frame_no != -1) continue;
				
				String image_filename = children[i];
				String abs_filename = input_dir + File.separator	+ image_filename;
				System.out.println("Processing " + abs_filename);
				
				// start timer
				long start = System.currentTimeMillis();

				// load new image file 
				fd.loadImage(abs_filename);
								
				// localize face
				faces = fd.localizeFace(thresholdDetection, true);
			
				long elapsedTimeMillis = System.currentTimeMillis() - start;
				System.out.println("Elapsed Time: " + elapsedTimeMillis);
				System.out.println("face count: " + faces.size());
				
				Candidate c = fd.getBestFace(faces);
				
				if (c != null) {
					double score = fd.getScore(c.x, c.y, c.size); 
					// output face
					String faceName = input_dir + "_" + image_filename + "_" + 
					(int)(Math.random()*10000) + "-" + score + ".png"; 

					int x = c.x; // + c.size/8;
					int y = c.y;// + c.size/8;
					int s = c.size; // - 2*c.size/8;
					
					System.out.println(">" + c.x);
					System.out.println(">" + c.y);
					System.out.println(">" + c.size);
					
					
					BufferedImage bout = fd.current_bimg.getSubimage(x, y, s, s);
					ImageIO.write(bout, "png", new File(facePath + faceName));
				}
			}
			
			// free mem
			fd.unloadImage();
			
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}


	
}
