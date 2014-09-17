package facetracker;

import java.util.Random;

import particlefilter.ParticleFilter;
import particlefilter.Scorer;

public class FaceTracker extends ParticleFilter {

	TrackerConfig config;
	private int imageWidth, imageHeight;
	private int nParticles;
	private Random r = new Random();

	public FaceTracker(Scorer scorer, 
	                   int imageWidth, int imageHeight,
	                   TrackerConfig config) {
		this(scorer,imageWidth,imageHeight,
		     config.DEFAULT_DYNAMIC_PARTICLES_NUM,
		     config.DEFAULT_ALPHA, config);
	}

	public FaceTracker(Scorer scorer, 
	                   int imageWidth, int imageHeight, 
	                   int nDynamicParticles, double alpha,
	                   TrackerConfig config) {

		super(nDynamicParticles+config.NFIXED, config.NFIXED, alpha, scorer);

		this.config = config;
		
		FaceParticle[] fixed = new FaceParticle[config.NFIXED];

		this.imageWidth = imageWidth;
		this.imageHeight = imageHeight;
		this.nParticles = nDynamicParticles + config.NFIXED;

		int S = config.GRIDSIZE;
		int k = 0;
		for (int i=0;i<S;i++) {
			for (int j=0;j<S;j++) {
				int x= (this.imageWidth/S)*j + this.imageWidth/(2*S);
				int y= (this.imageWidth/S)*i + this.imageWidth/(2*S);
				// for each size
				for (int s=0;s<config.FIXEDSIZES.length;s++) {
					int size = config.FIXEDSIZES[s];
					fixed[k++] = 
						new FaceParticle(new FaceState(x,y,size),true,config);
				}
			}
		}

		this.addFixedParticles(fixed);
	}

	public void setInitialFace(int x, int y, int size) {
		int n = nParticles-config.NFIXED;
		FaceParticle[] dynamic = new FaceParticle[n];
		FaceParticle initial = new FaceParticle(new FaceState(x,y,size),
		                                        false,config); 
		for (int i=0;i<n;i++) {
			dynamic[i] = initial.resample(imageHeight/4, 16.0, 0.1);
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
				state[3] = r.nextGaussian();
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
			}
		}

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
		
		return v;
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

}
