package facealign;


import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Arrays;

import javax.imageio.ImageIO;

import particlefilter.Particle;

import shared.Candidate;

import facetracker.FaceParticle;
import facetracker.FaceScanner;
import facetracker.FaceScorer;
import facetracker.FaceState;
import facetracker.FaceTracker;
import facetracker.TrackerConfig;


public class FaceAlign  {
	
	public static void main(String[] args) {

		final double LOW_FACE_SCORE = 3.0;
		final double NON_FACE_SCORE = -3.0;
		final int NUM_FRAME_BEFORE_UPDATE = 5;
		
		final int RESIZE_FACTOR = 2;
		final String face_xml = "/home/scheaman/workspace/Detector-3.0/xml/Jan22-cascade-2.xml";
		final String skin_xml = "/home/scheaman/workspace/Detector-3.0/xml/skin-Nov18-newHSV-cascade.xml";
		
		
		String input_dir = args[0];
		String mask_file = args[1];
		double faceThreshold = Double.parseDouble(args[2]);
		double outputThreshold = Double.parseDouble(args[3]);
		
		FilenameFilter filter = new FilenameFilter() {
			public boolean accept(File dir, String name) {
				return (name.toLowerCase().endsWith(".png") && 
							!(name.toLowerCase().endsWith(".out.png")));
			}
		};

		File dir = new File(input_dir);
		if (!dir.exists()) {
			System.out.println("Error: cannot find directory " + input_dir);
			return;
		}

		// create the file list
		String[] children = dir.list(filter);
		if (children == null) {
			System.out.println("Error: make sure " + args[1] + " contains png files");
			return;
		}
		// sort by file name
		Arrays.sort(children);


		// read image dimenstion
		int imgW = -1;
		int imgH = -1;

		try {
			BufferedImage bimg = ImageIO.read(new File(input_dir + File.separator	+ children[0]));
			imgW = bimg.getWidth();
			imgH = bimg.getHeight();
		}
		catch (Exception e) {
			e.printStackTrace();
			return;
		}

		// create face dir
		File faceDir = new File(input_dir + "/face");
		if (!faceDir.exists()) {
			faceDir.mkdir();
		}

		// setting up scorer
		FaceScorer scorer = new FaceScorer(face_xml, skin_xml);
		FaceScorerMask scorerMask = new FaceScorerMask(mask_file);
		FaceScorerCombine scorerComb = new FaceScorerCombine(scorer,scorerMask);
		
		
		// setting up config
		TrackerConfig config = new AlignTrackerConfig();
		
		// setting up tracker
		
		FaceTracker tracker = new FaceTracker(scorerMask, imgW, imgH, config);
		//FaceTracker tracker = new FaceTracker(scorerComb, imgW, imgH, config);
		tracker.setInitialFace(imgW/2, imgH/2, imgW/8);
		
		// setting up scanner
		FaceScanner scanner = new FaceScanner(scorer,imgW,imgH);
		
		int counter = 0;
		long totalTime = 0;
		
		// for each image 
		for (int i = 0; i < children.length; i++) {

			String image_filename = children[i];
			String abs_filename = input_dir + File.separator	+ image_filename;
			System.out.println("Processing " + abs_filename);

			try {
				
				// read frame
				BufferedImage bimg = ImageIO.read(new File(abs_filename));

				// start timer
				long start = System.currentTimeMillis();

				scorer.loadImage(bimg,RESIZE_FACTOR);
				scorerMask.loadImage(bimg);
				
				// If 1st image
				if (i == 0) {
					Candidate c = scanner.getBestFace(faceThreshold);
					if (c != null) {
						int x = c.x + c.size/2;
						int y = c.y + c.size/2;
						tracker.concentrateParticles(x, y, c.size);						
					}
				}
				
				tracker.resampleAndUpdateWeights();				
				
				FaceState fs = tracker.getWeightedAverageFaceState();
				
				long elapsed =  System.currentTimeMillis() - start;

				double variance = tracker.getVariance();
				double[] fsvec = fs.getStateVector();
				
				double x = fsvec[0];
				double y = fsvec[1];
				double size = fsvec[2];
				double faceScore = scorer.getScore(fs);
				double maskScore = scorerMask.getScore(fs);

				System.out.println("FACE: [" + x + "," + y + "," + size + "]");
				System.out.println("Score: " + faceScore);
				System.out.println("Score2: " + maskScore);
				System.out.println("Variance: " + variance);
				System.out.println("Time: " + elapsed);
				totalTime += elapsed;

				// output perfect face
				if (variance < 5 && maskScore > outputThreshold && faceScore > LOW_FACE_SCORE) {
					String faceName = input_dir + "/face/" + image_filename + "_" + 
					(int)(Math.random()*10000) + "_" + faceScore + "_"+ maskScore + ".png"; 

					int tx = (int)(x - size/2);
					int ty = (int)(y - size/2);
					int s = (int)size;
					
					try {
						BufferedImage bout = bimg.getSubimage(tx, ty, s, s);
						ImageIO.write(bout, "png", new File(faceName));
					}
					catch (Exception e) {
						e.printStackTrace();
					}
				}

//				
//				Graphics2D g2d = bimg.createGraphics();
//
//				if (faceScore > 8d) {
//					g2d.setColor(Color.RED);
//				}
//				else if (faceScore > 3d) {
//					g2d.setColor(Color.GREEN);
//				}
//				else {
//					g2d.setColor(Color.YELLOW);
//				}
//
//				// draw rectangle
//				if (faceScore > 0) {
//					g2d.drawRect((int)(x - size/2),
//					             (int)(y - size/2),
//					             (int)(size),
//					             (int)(size));
//				}
//				
//				// draw particles
//				g2d.setColor(Color.RED);
//				// plot all drifting particles
//				Particle[] particleVector = tracker.pv.getParticles();
//
//
//				for (int j=0;j<particleVector.length;j++) {
//					FaceParticle fp = (FaceParticle)particleVector[j];
//					if (!fp.isFixedParticle()) {
//						FaceState s = (FaceState)fp.getCurrentState();
//						double[] temp = s.getStateVector();
//						g2d.drawRect((int)temp[0], (int)temp[1], 3, 3);
//					}
//				}
//
//				// draw the average particle
//				g2d.setColor(Color.GREEN);
//				g2d.drawRect((int)fsvec[0], (int)fsvec[1], 3, 3);
//
//				// draw text
//				NumberFormat f = new DecimalFormat("0.000");
//				String str = "Variance = " + f.format(variance);
//				g2d.setColor(Color.black);
//				g2d.drawChars(str.toCharArray(), 0, str.length(), 10, 20);
//				
//				str = "FaceScore = " + f.format(faceScore);
//				g2d.setColor(Color.black);
//				g2d.drawChars(str.toCharArray(), 0, str.length(), 200, 20);
//				
//				str = "MaskScore = " + f.format(maskScore);
//				g2d.setColor(Color.black);
//				g2d.drawChars(str.toCharArray(), 0, str.length(), 400, 20);
//
//				// done drawing
//				g2d.dispose();

				if (faceScore < NON_FACE_SCORE) {
					counter++;
					if (counter > NUM_FRAME_BEFORE_UPDATE) {
						Candidate c = scanner.getBestFace(faceThreshold);
						if (c != null) {
							tracker.concentrateParticles(c.x+c.size/2, c.y+c.size/2, c.size);
							counter = 0;
						}
						else {
							tracker.updateStates();
						}	
					}
					else
						tracker.updateStates();
				}
				else {
					counter = 0;
					tracker.updateStates();
				}
				
				// output
				//ImageIO.write(bimg, "png", new File(abs_filename + ".out.png"));
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		}		
		
		System.out.println("TotalTime = " + totalTime);
		System.out.println("FPS = " + (1000/(totalTime / children.length)));
		
	}
}
