package core.nh.videotracker;

import core.nh.particlefilter.ParticleFilter;


public class TargetTracker extends ParticleFilter {

	TargetTrackerConfig config;
	private int nParticles;

	public TargetTracker(TargetScorer scorer, int imageWidth, int imageHeight, TargetTrackerConfig config) {
		super(config.getParticlesNum(),config.getAlpha(),scorer);
		this.config = config;
		this.nParticles = config.getParticlesNum();
	}

	public void initialParticlesAt(int x, int y, int size) {		
		TargetParticle[] particles = new TargetParticle[nParticles];
		particles[0] = new TargetParticle(new TargetState(x,y,size,0),config);
		for (int i=1;i<nParticles;i++) {
			particles[i] = (TargetParticle)particles[0].resample();
		}
		this.addParticles(particles);
		this.updateScoresAndRegret();
	}

	public void spawnSomeAtNewLocation(int x, int y, int size, double percentage) {
		int numToReplace = (int) Math.round(percentage*nParticles);
		int[] replaceAt = this.sampleByWeightInverse(numToReplace);
		TargetParticle p = new TargetParticle(new TargetState(x,y,size,0),config);
		for (int i=0;i<numToReplace;i++) {
			pv.replaceWith(replaceAt[i], p.resample());
		}
		this.updateScoresAndRegret();
	}

	public void translateParticles(int x, int y) {
		for (int i=0;i<nParticles;i++) {
			TargetParticle p = (TargetParticle)pv.get(i);
			TargetState fs = (TargetState)p.getCurrentState();
			double[] state = fs.getStateVector();
			state[0] += x;
			state[1] += y;
		}
		this.updateScoresAndRegret();
	}

	public void concentrateParticles(int x, int y, int size) {
		TargetParticle p = new TargetParticle(new TargetState(x,y,size,0), config);
		for (int i=0;i<nParticles;i++) {
			pv.replaceWith(i, p.resample());
		}
		this.updateScoresAndRegret();
	}

	// now only x, y, size
	public double getVariance() {
		double meanX = 0, meanY = 0, meanSize = 0;
		double[] weights = pv.getWeightVector();
		for (int i=0;i<nParticles;i++) {
			TargetParticle p = (TargetParticle)pv.get(i);
			TargetState fs =  (TargetState)p.getCurrentState();
			double[] state = fs.getStateVector();
			meanX += weights[i]*state[0];
			meanY += weights[i]*state[1];
			meanSize += weights[i]*state[2];
		}

		double v = 0;
		for (int i=0;i<nParticles;i++) {
			TargetParticle p = (TargetParticle)pv.get(i);
			TargetState fs =  (TargetState)p.getCurrentState();
			double[] state = fs.getStateVector();
			double dx = (state[0] - meanX)/config.getSigmaPosition();
			double dy = (state[1] - meanY)/config.getSigmaPosition();
			double dsize = (state[2] - meanSize)/config.getSigmaSize();
			v += (dx*dx + dy*dy + dsize*dsize) * weights[i];
		}
		return v;
	}

	public TargetState getWeightedAverageState() {
		double x = 0, y = 0, size = 0, theta = 0;
		for (int i=0;i<nParticles;i++) {
			TargetParticle p = (TargetParticle)pv.get(i);
			TargetState fs =  (TargetState)p.getCurrentState();
			double[] state = fs.getStateVector();
			double weight = p.getWeight(); 
			x += weight*state[0];
			y += weight*state[1];
			size += weight*state[2];
			theta += weight*state[6];
		}
		return new TargetState(x, y, size, theta);
	}


	public void updateStates() {
		super.updateStates();
	}

}
