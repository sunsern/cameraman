package facetracker;

import java.awt.Color;
import java.awt.Graphics2D;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FilenameFilter;
import java.io.IOException;
import java.text.DecimalFormat;
import java.text.NumberFormat;
import java.util.Arrays;
import java.util.Random;

import javax.imageio.ImageIO;

import particlefilter.ParticleFilter;

public class FaceTracker extends ParticleFilter {
	
	private int imageWidth, imageHeight;
	private int nParticles;
	private Random r = new Random();
	
	public FaceTracker(FaceScorer scorer, int imageWidth, int imageHeight) {
		this(scorer,imageWidth,imageHeight,Config.DEFAULT_DYNAMIC_PARTICLES_NUM,Config.DEFAULT_ALPHA);
	}
	
	public FaceTracker(FaceScorer scorer, int imageWidth, int imageHeight, 
	                   int nDynamicParticles, double alpha) {
			
		super(nDynamicParticles+Config.NFIXED, Config.NFIXED, alpha, scorer);

		FaceParticle[] fixed = new FaceParticle[Config.NFIXED];
		
		this.imageWidth = imageWidth;
		this.imageHeight = imageHeight;
		this.nParticles = nDynamicParticles + Config.NFIXED;
		
		int S = Config.GRIDSIZE;
		int k = 0;
		for (int i=0;i<S;i++) {
			for (int j=0;j<S;j++) {
				int x= (this.imageWidth/S)*j + this.imageWidth/(2*S);
				int y= (this.imageWidth/S)*i + this.imageWidth/(2*S);
				// for each size
				for (int s=0;s<Config.FIXEDSIZES.length;s++) {
					int size = Config.FIXEDSIZES[s];
					fixed[k++] = 
						new FaceParticle(new FaceState(x,y,size),true);
				}
			}
		}
		
		this.addFixedParticles(fixed);

	}
	
	public void setInitialFace(int x, int y, int size) {
		int n = nParticles - Config.NFIXED;
		FaceParticle[] dynamic = new FaceParticle[n];
		FaceParticle initial = new FaceParticle(new FaceState(x,y,size),false); 
		for (int i=0;i<n;i++) {
			dynamic[i] = initial.resample(imageHeight/4, 16.0, 0.25);
		}
		this.addDynamicParticles(dynamic);
	}
	
	public void translateParticles(int x, int y) {
		for (int i=0;i<nParticles;i++) {
			FaceParticle p = (FaceParticle)pv.get(i);
			// only for drifting particles
			if (!p.isFixedParticle()) {
				FaceState fs =  (FaceState)p.getCurrentState();
				double[] state = fs.getStateVector();
				state[0] += x;
				state[1] += y;
			}
		}
	}
	
	public void concentrateParticles(int x, int y, int size) {
		for (int i=0;i<nParticles;i++) {
			FaceParticle p = (FaceParticle)pv.get(i);
			// only for drifting particles
			if (!p.isFixedParticle()) {
				FaceState fs =  (FaceState)p.getCurrentState();
				double[] state = fs.getStateVector();
				// x
				state[0] = x + r.nextGaussian()*state[2]/16;
				// y
				state[1] = y + r.nextGaussian()*state[2]/16;
				// size
				state[2] = size + r.nextGaussian()*8;
				// vx
				state[3] = r.nextGaussian();
				// vy
				state[4] = r.nextGaussian();
			}
		}
	}


        public int[] getBoundary() {
	    int minX = Integer.MAX_VALUE;
	    int maxX = -1; 
	    int minY = Integer.MAX_VALUE;
	    int maxY = -1;
	    for (int i=0;i<nParticles;i++) {
			FaceParticle p = (FaceParticle)pv.get(i);
			// only for drifting particles
			if (!p.isFixedParticle()) {
				FaceState fs =  (FaceState)p.getCurrentState();
				double[] state = fs.getStateVector();
				int s = (int)(state[2]/2) + 1;
				int x1 = (int)state[0] - s;
				int y1 = (int)state[1] - s;
				int x2 = (int)state[0] + s;
				int y2 = (int)state[1] + s;
				if (x1 < minX) minX = x1;
				if (y1 < minY) minY = y1;
				if (x2 > maxX) maxX = x2;
				if (y2 > maxY) maxY = y2;
			}
		}

	    if (minX < 0) minX = 0;
	    if (minY < 0) minY = 0;
	    if (maxX >= imageWidth) maxX = imageWidth - 1;
	    if (maxY >= imageHeight) maxY = imageHeight - 1;
	    return new int[] {minX,minY,maxX,maxY};
        }
	
	public double getVariance() {
		double meanX = 0, meanY = 0, meanSize = 0;
		double totalWeight = 0;
		double[] weights = pv.getWeightVector();
		for (int i=0;i<nParticles;i++) {
			FaceParticle p = (FaceParticle)pv.get(i);
			// only for drifting particles
			if (!p.isFixedParticle()) {
				FaceState fs =  (FaceState)p.getCurrentState();
				double[] state = fs.getStateVector();
				meanX += weights[i]*state[0];
				meanY += weights[i]*state[1];
				meanSize += weights[i]*state[2];
				totalWeight += weights[i];
			}
		}

		meanX /= totalWeight;
		meanY /= totalWeight;
		meanSize /= totalWeight;

		double v = 0;
		for (int i=0;i<nParticles;i++) {
			FaceParticle p = (FaceParticle)pv.get(i);
			// only for drifting particles
			if (!p.isFixedParticle()) {
				FaceState fs =  (FaceState)p.getCurrentState();
				double[] state = fs.getStateVector();
				double dx = state[0] - meanX;
				double dy = state[1] - meanY;
				double dsize = state[2] - meanSize;
				v += (dx*dx + dy*dy + dsize*dsize) * weights[i];
			}
		}

		return v/totalWeight;
	}
	
	public FaceState getWeightedAverageFaceState() {
		double x = 0, y = 0, size = 0; 
		double[] weights = pv.getWeightVector();
		for (int i=0;i<nParticles;i++) {
			FaceParticle p = (FaceParticle)pv.get(i);
			FaceState fs =  (FaceState)p.getCurrentState();
			double[] state = fs.getStateVector();
			x += weights[i]*state[0];
			y += weights[i]*state[1];
			size += weights[i]*state[2];
		}
		return new FaceState(x, y, size);
	}
	
	public void updateStates() {
		super.updateStates();
	}
	
	public static void main(String[] args) {

		final int SKIP_FACTOR = 2;
		final int RESIZE_FACTOR = 2;
		
		int imgW = 320;
		int imgH = 240;
		
		String input_dir = "/home/scheaman/workspace/ParticleFilter/data/Boyko/smaller";
			
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
		
		String face_xml = "/home/scheaman/workspace/Detector-3.0/xml/Jan22-cascade-2.xml";
		String skin_xml = "/home/scheaman/workspace/Detector-3.0/xml/skin-Nov18-newHSV-cascade.xml";
		FaceScorer scorer = new FaceScorer(face_xml, skin_xml);
	
		FaceTracker faceTracker = new FaceTracker(scorer, imgW, imgH);
		faceTracker.setInitialFace(imgW/2, imgH/2, imgW/8);
		
		int counter = 0;
		long totalTime = 0;
		// for each image 
		for (int i = 0; i < children.length; i++) {
			
			String image_filename = children[i];
			String abs_filename = input_dir + File.separator	+ image_filename;
			System.out.println("Processing " + abs_filename);
			

			try {
					BufferedImage bimg = ImageIO.read(new File(abs_filename));

					// start timer
					long start = System.currentTimeMillis();

					if (counter % SKIP_FACTOR == 0) {
						scorer.loadImage(bimg,RESIZE_FACTOR);
					}

					faceTracker.resampleAndUpdateWeights();
					FaceState fs = faceTracker.getWeightedAverageFaceState();

					long elapsed =  System.currentTimeMillis() - start;
					
					double[] fsvec = fs.getStateVector();
					
					double x = fsvec[0];
					double y = fsvec[1];
					double size = fsvec[2];
					System.out.println("FACE: [" + x + "," + y + "," + size + "]");
					double score = scorer.getScore(fs);
					System.out.println("Score: " + score);

					
					System.out.println("Time: " + elapsed);
					totalTime += elapsed;
					
					Graphics2D g2d = bimg.createGraphics();
					
					if (score > 8d) {
						g2d.setColor(Color.RED);
					}
					else if (score > 3d) {
						g2d.setColor(Color.GREEN);
					}
					else {
						g2d.setColor(Color.YELLOW);
					}
					
					double variance = faceTracker.getVariance();
					
					if (variance < 100) {
					g2d.drawRect((int)(x - size/2),
					             (int)(y - size/2),
					             (int)(size),
					             (int)(size));
					//g2d.dispose();
					}
					
					/*
					g2d.setColor(Color.RED);
					// plot all drifting particles
					Particle[] particleVector = faceTracker.pv.getParticles();
					
					
					for (int j=0;j<particleVector.length;j++) {
						FaceParticle fp = (FaceParticle)particleVector[j];
						if (!fp.isFixedParticle()) {
							FaceState s = (FaceState)fp.getCurrentState();
							double[] temp = s.getStateVector();
							g2d.drawRect((int)temp[0], (int)temp[1], 3, 3);
						}
					}
					
					// draw the average particle
					g2d.setColor(Color.GREEN);
					g2d.drawRect((int)fsvec[0], (int)fsvec[1], 3, 3);
					 */
					
					NumberFormat f = new DecimalFormat("0.000");
					String var = "Variance = " + f.format(faceTracker.getVariance());
					g2d.setColor(Color.black);
					g2d.drawChars(var.toCharArray(), 0, var.length(), 10, 20);
					g2d.dispose();
					
					faceTracker.updateStates();
			
					counter++;
					
					ImageIO.write(bimg, "png", new File(abs_filename + ".out.png"));
			}
			catch (IOException e) {
				e.printStackTrace();
			}
		}		
		System.out.println("TotalTime = " + totalTime);
		System.out.println("FPS = " + (1000/(totalTime / children.length)));
	}
}
