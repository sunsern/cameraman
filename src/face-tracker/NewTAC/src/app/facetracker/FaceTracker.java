package app.facetracker;


import java.awt.AlphaComposite;
import java.awt.BasicStroke;
import java.awt.Color;
import java.awt.Composite;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.Arrays;

import javax.imageio.ImageIO;


import core.nh.particlefilter.Particle;
import core.nh.videotracker.TargetDetector;
import core.nh.videotracker.TargetParticle;
import core.nh.videotracker.TargetState;
import core.nh.videotracker.TargetTracker;


public class FaceTracker  {

	public static void main(String[] args) throws IOException {

		final double LOW_FACE_SCORE = 15.0;
		final double NON_FACE_SCORE = 5.0;
		final int NUM_FRAME_BEFORE_UPDATE = 10;

		final int RESIZE_FACTOR = 2;
		
		final String face_xml = "/home/scheaman/research/TAC/xml/Jul15-cascade.xml";
		final String skin_xml = "/home/scheaman/research/TAC/xml/skin-Nov18-newHSV-cascade.xml";


		String input_dir = args[0];
		String output = args[1];

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

		// read image dimension
		int imgW = -1;
		int imgH = -1;

		try {
			BufferedImage bimg = ImageIO.read(new File(input_dir + File.separator + children[0]));
			imgW = bimg.getWidth();
			imgH = bimg.getHeight();
		}
		catch (Exception e) {
			e.printStackTrace();
			return;
		}


		// setting up scorer
		FaceScorer scorer = new FaceScorer(face_xml, skin_xml);
		
		// setting up config
		FaceTrackerConfig config = new FaceTrackerConfig();

		// setting up tracker
		TargetTracker tracker = new TargetTracker(scorer, imgW, imgH, config);
				
		// setting up scanner
		TargetDetector scanner = new TargetDetector(scorer);

		int counter = 0;
		long totalTime = 0;
		double ct = Double.NaN;

		// output text
		BufferedWriter bw = new BufferedWriter(new FileWriter(output));

		// for each image 
		for (int i = 0; i < children.length; i++) {

			String image_filename = children[i];
			String abs_filename = input_dir + File.separator + image_filename;
			System.out.println("Processing " + abs_filename);

			try {

				// read frame
				BufferedImage bimg = ImageIO.read(new File(abs_filename));

				// start timer
				long start = System.currentTimeMillis();

				scorer.loadImage(bimg,RESIZE_FACTOR);

				// If 1st image
				if (i == 0) {

					TargetState t = scanner.localizeTarget(bimg, LOW_FACE_SCORE);
					if (t != null) {
						double[] state = t.getStateVector();
						tracker.initialParticlesAt((int)state[0], (int)state[1], (int)state[2]);						
					}
					else {
						// initialize particles to somewhere
						tracker.initialParticlesAt(imgW/2, imgH/2, imgW/8);
					}
				}

				ct = tracker.resampleAndUpdateWeights();
				TargetState fs = tracker.getWeightedAverageState();
				
				// stop timer
				long elapsed =  System.currentTimeMillis() - start;

				double variance = tracker.getVariance();
				double[] fsvec = fs.getStateVector();

				double x = fsvec[0];
				double y = fsvec[1];
				double size = fsvec[2];
				double faceScore = scorer.getScore(fs);

				System.out.println("  FACE: [" + x + "," + y + "," + size + "]");
				System.out.println("  Score: " + faceScore);
				System.out.println("  c_t: " + ct);
				System.out.println("  Variance: " + variance);
				System.out.println("  Time: " + elapsed);
				totalTime += elapsed;

				if (ct < 50 && faceScore > LOW_FACE_SCORE) {
					int tx = (int)(x - size/2);
					int ty = (int)(y - size/2);
					int s = (int)size;
					System.out.println("  (GOODFACE) " + i + ":" + tx + "," + ty + "," + s + "\n");
					bw.write("(GOODFACE) " + i + ":" + tx + "," + ty + "," + s + "\n");
					bw.flush();
				}

				Graphics2D g2d = bimg.createGraphics();
				
				// draw rectangle
				g2d.setColor(Color.RED);
				g2d.setStroke(new BasicStroke(6,BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));			
				g2d.drawRect((int)(x - size/2),
						(int)(y - size/2),
						(int)(size),
						(int)(size));

				// plot all drifting particles
				Particle[] particleVector = tracker.pv.getParticles();
				Composite originalComposite = g2d.getComposite();
				g2d.setComposite(AlphaComposite.getInstance(AlphaComposite.SRC_OVER,0.5f));
				g2d.setColor(Color.BLUE);
				g2d.setStroke(new BasicStroke(6,BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
				for (int j=0;j<particleVector.length;j++) {
					TargetParticle fp = (TargetParticle)particleVector[j];
					TargetState s = (TargetState)fp.getCurrentState();
					double[] temp = s.getStateVector();					
					g2d.drawOval((int)temp[0], (int)temp[1], 2, 2);
				}
				g2d.setComposite(originalComposite);
				
				// draw the average particle
				g2d.setColor(Color.GREEN);
				g2d.setStroke(new BasicStroke(6,BasicStroke.CAP_ROUND, BasicStroke.JOIN_ROUND));
				g2d.drawOval((int)fsvec[0], (int)fsvec[1], 2, 2);

				// done drawing
				g2d.dispose();
				
				if (faceScore < NON_FACE_SCORE) {
					counter++;
					if (counter > NUM_FRAME_BEFORE_UPDATE) {
						TargetState t = scanner.localizeTarget(bimg, LOW_FACE_SCORE);
						if (t != null) {
							double[] state = t.getStateVector();
							tracker.concentrateParticles((int)state[0], (int)state[1], (int)state[2]);
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
				ImageIO.write(bimg, "jpg", new File(abs_filename + ".out.jpg"));
				
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		}		


		System.out.println("TotalTime = " + totalTime);
		System.out.println("FPS = " + (1000/(totalTime / children.length)));

		bw.close();

	}
}
