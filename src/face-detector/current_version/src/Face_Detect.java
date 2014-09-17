import gray.Gray_Cache;
import gray.Gray_IntegralImage;

import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

import javax.imageio.ImageIO;
import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import shared.Candidate;
import shared.Util;
import skin.Skin_Cache;
import skin.Skin_Detector;
import skin.Skin_IntegralImage;

import HOG.HOG_Cache;
import HOG.HOG_Core;
import HOG.HOG_IntegralImage;

public class Face_Detect {

	protected Face_Scorer scorer;
	protected Skin_Detector sd;
	protected int[] rgbs;
	protected double[] skin;
	protected int[] gray;
	protected double[] hsv;
	protected BufferedImage current_bimg; 
	protected Gray_IntegralImage grayii; 
	protected Skin_IntegralImage skinii;
	protected HOG_IntegralImage hogii;
	
	
	private static double thresholdDetection = 10.0d;
	private static double hardMargin = 5d;
	
	private static boolean mergeFace = true;
	private static boolean calibrateFace = true;
	
	private static boolean outputSkin = false;
	private static boolean outputEdge = false;
	private static boolean outputScoreVector = false;	
	private static boolean outputDetection = false;
	private static boolean outputDetailedDetection = false;
	private static boolean outputHardExamples = false;	
	
	private static final String SKIN_XML = "/home/scheaman/workspace/FaceDetector/xml/" +
			"skin-Nov18-newHSV-cascade.xml";
	
	private static final int[] sizes =
			new int[] { 32, 40, 48, 56, 64, 72, 80, 96, 104, 120, 144, 160, 184, 216 };
	
	private static String OUTPUT_DIR = "output"; 
	private static String FACE_DIR = "face"; 
	private static String DETECTION_DIR = "detection";
	private static String HARD_DIR = "hard";

	private static String DETECTION_DIR_0 = "LessThanNegative10"; // < -10
	private static String DETECTION_DIR_1 = "Negative10ToNegative5"; // [-10 , -5]
	private static String DETECTION_DIR_2 = "Negative5To0"; // [-5 , 0 ]
	private static String DETECTION_DIR_3 = "0ToPositive5"; // [0 , 5 ]
	private static String DETECTION_DIR_4 = "Positive5ToPositive10"; // [5 , 10]
	private static String DETECTION_DIR_5 = "Positive10ToPositive15"; // [10,15]
	private static String DETECTION_DIR_6 = "MoreThanPositive15"; // > 15
	
	private static ArrayList<ExtendedCandidate> faces, candidates;
	private static BufferedWriter scoreVectorFile = null;
	
	
	public Face_Detect(String xml_filename, String skin_xml) throws ParserConfigurationException, SAXException, IOException {
	  // load face detector XML
		scorer = new Face_Scorer(xml_filename);
		// load skin detector
		sd = new Skin_Detector(skin_xml);
	}
	
	public void loadImage(String image_filename) throws IOException {
		
		// read image file 
		current_bimg = ImageIO.read(new File(image_filename));

		int w = current_bimg.getWidth();
		int h = current_bimg.getHeight();
		
		rgbs = new int[w * h];
		current_bimg.getRGB(0, 0, w, h, rgbs, 0, w);
		
		// change color space
		int l = w * h;
		gray = new int[l];
		hsv = new double[l * 3];
		double[] temp_hsv = new double[3];
		
		for (int j=0; j<l; j++) {
			int b = (rgbs[j])       & 0xff;
			int g = (rgbs[j] >> 8)  & 0xff;
			int r = (rgbs[j] >> 16) & 0xff;

			Util.rgb2hsv(r, g, b, temp_hsv);

			hsv[j*3]     = temp_hsv[0];
			hsv[j*3 + 1] = temp_hsv[1];
			hsv[j*3 + 2] = temp_hsv[2];	
			
			gray[j] = (int)(0.3d*r + 0.59d*g + 0.11d*b);
		}
		
		// detect skin
		skin = sd.getScoreVector(hsv, w, h);

		grayii = new Gray_IntegralImage(gray, w, h);
		skinii = new Skin_IntegralImage(skin, w, h);
		hogii = HOG_Core.getHOGIntegralImage(current_bimg);
		
	}
	
	public double getScore(int x, int y, int size) {
		Candidate c = new Candidate(x,y,size); 
		
		HOG_Cache hogcache = new HOG_Cache(c,hogii);
		Skin_Cache skincache = new Skin_Cache(c,skinii);
		Gray_Cache graycache = new Gray_Cache(c,grayii);
		
		return scorer.getCandidateScore(c, skincache, hogcache, graycache);
	}
	
	public double[] getScoreVector(int x, int y, int size) {
		Candidate c = new Candidate(x,y,size); 
		
		HOG_Cache hogcache = new HOG_Cache(c,hogii);
		Skin_Cache skincache = new Skin_Cache(c,skinii);
		Gray_Cache graycache = new Gray_Cache(c,grayii);
		
		return scorer.getCandidateScoreVector(c, skincache, hogcache, graycache);	
	}

	public void unloadImage() {
		grayii = null;
		skinii = null;
		hogii = null;
	}
	
	// -------------------------------------------------------------------------//
	
	private static void make_dir(String path) {
		File dir = new File(path);
		if (!dir.exists()) {
			dir.mkdir();
		}
	}
	
	public static void main(String[] args) {

		if (args.length != 3) {
			System.out.println("usage: java Face_Detect <xml file> <input dir> <framenumber>");
			return;
		}

		try {

			String xml_filename = args[0];
			String input_dir    = args[1];
			int frame_no        = Integer.parseInt(args[2]);
			
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
			String outputPath = input_dir + File.separator + OUTPUT_DIR + File.separator;
			make_dir(outputPath);
			String detectionPath = input_dir + File.separator + DETECTION_DIR + File.separator;
			make_dir(detectionPath);
			String facePath = input_dir + File.separator + FACE_DIR + File.separator;
			make_dir(facePath);
			String hardPath = input_dir + File.separator + HARD_DIR + File.separator;
			make_dir(hardPath);
				
			if (outputDetailedDetection) {
				make_dir(detectionPath + DETECTION_DIR_0 + File.separator);
				make_dir(detectionPath + DETECTION_DIR_1 + File.separator);
				make_dir(detectionPath + DETECTION_DIR_2 + File.separator);
				make_dir(detectionPath + DETECTION_DIR_3 + File.separator);
				make_dir(detectionPath + DETECTION_DIR_4 + File.separator);
				make_dir(detectionPath + DETECTION_DIR_5 + File.separator);
				make_dir(detectionPath + DETECTION_DIR_6 + File.separator);
			}
		
			Face_Detect fd = new Face_Detect(xml_filename, SKIN_XML);
			
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
								
				if (outputSkin) {
					int w = fd.current_bimg.getWidth();
					int h = fd.current_bimg.getHeight();
					BufferedImage skin_bimg = new BufferedImage(w, h,	BufferedImage.TYPE_INT_RGB);
					for (int y=0;y<h;y++) {
						for (int x=0;x<w;x++) {
							double temp = fd.skin[y*w + x];
							
							// clip score
							if (temp < -10d) temp = -10d;
							if (temp > 10d) temp = 10d;
							
							// normalize to [0,255]
							temp = (temp + 10d) * 255d / 20d;
							int v = (int)temp & 0xff;
						
							skin_bimg.setRGB(x, y, (v | (v << 8) | (v << 16)));
						}
					}
					ImageIO.write(skin_bimg, "png", new File(input_dir + File.separator + 
					                                         OUTPUT_DIR + File.separator + image_filename + ".skin.png"));
				}
				
				if (outputEdge) {
					Util.outputEdgeImage(fd.current_bimg, input_dir + File.separator + 
                               OUTPUT_DIR + File.separator + image_filename + ".edge.png");
				}
				
				// score vector: for building cascade
				if (outputScoreVector) {
					scoreVectorFile =
						new BufferedWriter(new FileWriter(input_dir + File.separator + 
						                                  OUTPUT_DIR + File.separator + image_filename + ".score"));		
				}	
				
				candidates = new ArrayList<ExtendedCandidate>();
				faces = new ArrayList<ExtendedCandidate>();
				
				// start detecting
				FaceThread[] f = new FaceThread[sizes.length];
				for (int j=0;j<sizes.length;j++) {
						f[j] = new FaceThread(fd,sizes[j],fd.current_bimg.getWidth(),fd.current_bimg.getHeight());
					f[j].start();
				}
				
				// wait for all threads to finish
				for (int j=0;j<sizes.length;j++) {
					f[j].join();
				}
				
				// stop the clock
				long elapsedTimeMillis = System.currentTimeMillis() - start;

				System.out.println("Number of Candidates: " + candidates.size());	
				System.out.println("Elapsed Time: " + elapsedTimeMillis);
				
				int detection_count = 0;
				
				// begin post-processing. 
				for (int j=0;j<candidates.size();j++) {
					
					ExtendedCandidate c = candidates.get(j);
				
					// hard examples 
					if (outputHardExamples) {
						if (c.score > -hardMargin && c.score < hardMargin) {
							String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" + 
							(int)(Math.random()*10000) + "-" + c.score + ".png"; 

							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(hardPath  + detectionFileName));
						}
					}
					
					// output to separate folders
					if (outputDetailedDetection) {
						if (c.score > 15) {
							String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" + 
							(int)(Math.random()*10000) + "-" + c.score + ".png"; 

							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(detectionPath + DETECTION_DIR_6 + File.separator + detectionFileName));
						}
						else if (c.score > 10) {
							String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" + 
							(int)(Math.random()*10000) + "-" + c.score + ".png"; 

							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(detectionPath + DETECTION_DIR_5 + File.separator + detectionFileName));
						}
						else if (c.score > 5) {
							String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" + 
							(int)(Math.random()*10000) + "-" + c.score + ".png"; 

							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(detectionPath + DETECTION_DIR_4 + File.separator + detectionFileName));
						}
						else if (c.score > 0) {
							String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" + 
							(int)(Math.random()*10000) + "-" + c.score + ".png"; 

							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(detectionPath + DETECTION_DIR_3 + File.separator + detectionFileName));
						}

						else if (c.score > -5) {
							String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" + 
							(int)(Math.random()*10000) + "-" + c.score + ".png"; 

							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(detectionPath + DETECTION_DIR_2 + File.separator + detectionFileName));
						}
						else if (c.score > -10) {
							String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" +
							(int)(Math.random()*10000) + "-" + c.score + ".png"; 

							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(detectionPath + DETECTION_DIR_1 + File.separator + detectionFileName));
						}
						else {
							String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" +
							(int)(Math.random()*10000) + "-" + c.score + ".png"; 

							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(detectionPath + DETECTION_DIR_0 + File.separator + detectionFileName));

						}
					}
					
					if (c.score > thresholdDetection) {

						detection_count++;
						
						String detectionFileName = input_dir + "_" + image_filename + "_" + i + "_" + detection_count + "_" + 
            (int)(Math.random()*10000) + "-" + c.score + ".png"; 

						// output the detection to a separate file
						if (outputDetection) {
							BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
							ImageIO.write(bout, "png",
							              new File(detectionPath + detectionFileName));
						}
						
						if (mergeFace) {
							boolean merged = false;
							for (int k=0;k<faces.size();k++) {
								ExtendedCandidate ec = faces.get(k);

								if (ec.isSameDetection(c)) {

									// average
									//ec.x = (ec.weight*ec.x + c.x) / (ec.weight + 1);
									//ec.y = (ec.weight*ec.y + c.y) / (ec.weight + 1);
									//ec.size = (ec.weight*ec.size + c.size) / (ec.weight + 1);
									//ec.weight += c.weight;


									// union
									int t1 = Math.max(ec.x+ec.size, c.x+c.size)-Math.min(ec.x,c.x);
									int t2 = Math.max(ec.y+ec.size, c.y+c.size)-Math.min(ec.y,c.y);
									ec.x = Math.min(ec.x,c.x);
									ec.y = Math.min(ec.y,c.y);
									ec.size = Math.min(t1, t2);

									merged = true;
									break;

								}	
							}

							if (!merged) faces.add(new ExtendedCandidate(c.x,c.y,c.size));
						}
						else {
							faces.add(c);
						}
					}
				}
				
				
				if (calibrateFace) calibrate_face(fd); 
				
				System.out.println("detection count: " + detection_count);
				System.out.println("face count: " + faces.size());
				
				for (int j=0;j<faces.size();j++) {

					ExtendedCandidate c = faces.get(j);
					
					int w = fd.current_bimg.getWidth();

					// add to it a green transparent box
					for (int yy=c.y;yy < c.y+c.size; yy++) {
						for (int xx=c.x;xx < c.x+c.size; xx++) {
							int org = fd.rgbs[yy*w+xx];
							int b = org & 0xff;
							int g = (org >> 8) & 0xff;
							int r = (org >> 16) & 0xff;

							g = (int)((0.40d)*(double)g + (0.60d)*255d); 

							fd.rgbs[yy*w+xx] = ((r<<16) | (g<<8) | b);
						}
					}

					// add red box in the center
					int center_x = c.x + c.size/8;
					int center_y = c.y + c.size/8;
					int center_size = 6 * c.size / 8;

					for (int yy=center_y;yy < center_y+center_size; yy++) {
						for (int xx=center_x;xx < center_x+center_size; xx++) {
							int org = fd.rgbs[yy*w+xx];
							int b = org & 0xff;
							int g = (org >> 8) & 0xff;
							int r = (org >> 16) & 0xff;

							r = (int)((0.40d)*(double)r + (0.60d)*255d); 

							fd.rgbs[yy*w+xx] = ((r<<16) | (g<<8) | b);
						}
					}

					String faceName = input_dir + "_" + image_filename + "_" + i + "_" + j + "_" + 
          (int)(Math.random()*10000) + "-" + c.score + ".png"; 

					// output face
					BufferedImage bout = fd.current_bimg.getSubimage(c.x, c.y, c.size, c.size);
					ImageIO.write(bout, "png",
					              new File(facePath + faceName));
					
				}

				// output
				int w = fd.current_bimg.getWidth();
				int h = fd.current_bimg.getHeight();
				fd.current_bimg.setRGB(0, 0, w, h, fd.rgbs, 0, w);
				ImageIO.write(fd.current_bimg, "png",
				              new File(outputPath + image_filename + ".out.png"));

				if (scoreVectorFile != null) scoreVectorFile.close();
			}
			
			fd.unloadImage();
			
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}

	private static void calibrate_face(Face_Detect fd) {
		
		ArrayList<ExtendedCandidate> output = new ArrayList<ExtendedCandidate>();
		ArrayList<ExtendedCandidate> high_score = new ArrayList<ExtendedCandidate>();
		
		
		for (int i=0;i<faces.size();i++) {
			
			high_score.clear();
			
			ExtendedCandidate ec = faces.get(i);

			int size_range = ec.size / 2; 
			int size_step = 8;
			
			for (int size=ec.size + size_range;size >= ec.size - size_range; size -= size_step)	{
				if (size > fd.skinii.getHeight()) continue;
				if (size < 32) break;
				
				for (int y=ec.y-ec.size/4;y<=ec.y+ec.size/4;y++) {
					if (y < 0) continue;
					if (y+size > fd.skinii.getHeight()) break;
					
					for (int x=ec.x-ec.size/4;x<=ec.x+ec.size/4;x++) {
						if (x < 0) continue;
						if (x+size > fd.skinii.getWidth()) break;

						double score = fd.getScore(x, y, size);
						
						if (high_score.size() < 4) {
							ExtendedCandidate new_ec = new ExtendedCandidate(x,y,size);
							new_ec.score = score;
							high_score.add(new_ec);
						}
						else {
							ExtendedCandidate min = high_score.get(0); 
							for (int k=0;k<high_score.size();k++) {
								ExtendedCandidate temp = high_score.get(k);
								if (temp.score < min.score) {
									min = temp; 
								}
							}
							if (score > min.score) {
								min.x = x;
								min.y = y;
								min.size = size;
								min.score = score;
							}
						} 
						
					}
				}
			}
			
			ExtendedCandidate average = high_score.get(0);
			for (int k=1;k<high_score.size();k++) {
				ExtendedCandidate temp = high_score.get(k);
				average.x = (average.weight * average.x + temp.x) / (average.weight + 1);
				average.y = (average.weight * average.y + temp.y) / (average.weight + 1);
				average.size = (average.weight * average.size + temp.size) / (average.weight + 1);
				average.score = (average.weight * average.score + temp.score) / (average.weight + 1);
				average.weight++;
			}

			if (average.score > thresholdDetection)
				output.add(average);
			
		}
		
		faces = output;
			
	}
	
	
	private synchronized static void addToList(ArrayList<ExtendedCandidate> l, ExtendedCandidate c) {
		l.add(c);
	}
	
	private synchronized static void writeScoreVectorFile(double[] scoreVector, BufferedWriter scoreVectorFile) {

		try {
			if (scoreVectorFile != null) {
				scoreVectorFile.write(Double.toString(scoreVector[0]));
				for (int i = 0; i < scoreVector.length; i++) {
					scoreVectorFile.write("," + scoreVector[i]);
				}
				scoreVectorFile.write("\n");
			}
		}
		catch (Exception e) {
			e.printStackTrace();
		}
		
	}
	
	private static class FaceThread extends Thread {
		
		Face_Detect fd;
		int size, fw, fh;
		
		public FaceThread(Face_Detect fd, int s, int w, int h) {
			this.fd = fd;
			size = s;
			fw = w;
			fh = h;
		}
		
		public void run() {
			// detect faces of size "size"
			for (int y = 0; y + size < fh; y += 1 + (size / sizes[0])) {
				for (int x = 0; x + size < fw; x += 1 + (size / sizes[0])) {
					
					ExtendedCandidate ec = new ExtendedCandidate(x,y,size);
				
					// skip windows with low skin
					/*
					double avg_skin = skinii.getSum(1, c.x, c.y, c.size, c.size) / (c.size*c.size);
					if (avg_skin < 0.1d) {
						ec.score = -10d;
					}
					else 
					*/
				
					ec.score = fd.getScore(x, y, size);

				  // speed hack
					/*
					if (ec.score > 0d) {
						addToList(candidates,ec);
					}
					else if (outputDetailedDetection) {
						if (Math.random() > 0.9)
							addToList(candidates,ec);
					}
					*/
					addToList(candidates,ec);
					
					if (outputScoreVector) {
						double[] scoreVector = fd.getScoreVector(x, y, size);
						writeScoreVectorFile(scoreVector, scoreVectorFile);
					}
				}
			}
		}
	}
	
	private static class ExtendedCandidate extends Candidate {
		public double score; 
		public ExtendedCandidate(int x, int y, int s) {
			super(x,y,s);
		}
	}
	

	
}
