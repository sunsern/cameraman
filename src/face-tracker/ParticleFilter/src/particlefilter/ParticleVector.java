package particlefilter;

public class ParticleVector  {

	protected Particle[] particles;
	protected int nParticles, nFixed;
	

	public ParticleVector(Particle[] particles, int nFixed) { 		
		this.particles = particles;
		this.nParticles = particles.length;
		this.nFixed = nFixed;
	}
	
	
	public void updateScores(Scorer scorer) {
		for (int i=0;i<nParticles;i++) {
			particles[i].updateScore(scorer);
		}
	}
	
	public double getWeightedAverageScore() {
		double sum = 0;
		//for (int i=0;i<nParticles;i++) { 
		//    sum += (particles[i].getScore() * 
		//			particles[i].getWeight());
		//}
		//return sum;
		for (int i=0;i<nParticles;i++) { 
		    sum += particles[i].getScore();
		}
		return sum/nParticles;
		
	}
	
	public void updateRegrets(double alpha, double avgScore) {
		for (int i=0;i<nParticles;i++) 
			particles[i].updateRegret(alpha, avgScore);
		
	}
	
	public double[] getPositiveRegretWeightVector() {
		double[] retVal = new double[nParticles];
		for (int i=0;i<nParticles;i++) {
			retVal[i] = (particles[i].isPositiveRegret() ? particles[i].getWeight() : 0);
		}
		return retVal;
	}
	
	public double getRegret(int i) {
		return particles[i].getRegret(); 
	}
	
	public double[] getRegretVector() {
		double[] retVal = new double[nParticles];
		for (int i=0;i<nParticles;i++) {
			retVal[i] = particles[i].getRegret();
		}
		return retVal;
	}
	 
	public void replaceWith(int i, Particle p) {
		if (i >= nFixed) particles[i] = p;
	}

	public void updateWeights() {
		
		double minC = 0, maxC = 0;
		double[] hedgedRegretVector = new double[nParticles];
		for (int i=0;i<nParticles;i++) {
			double regret = particles[i].getRegret();
			hedgedRegretVector[i] = (regret > 0 ? regret : 0);
			maxC += hedgedRegretVector[i] * hedgedRegretVector[i];
		}
		
		// compute c by binary search
		double EPS = 1e-9;
		double RHS = nParticles * Math.E;
		double LHS, c = 0;
		while (maxC - minC > EPS) {
			c = (maxC + minC) / 2;
			LHS = 0;
			for (int i=0;i<nParticles;i++) {
				LHS += Math.exp((hedgedRegretVector[i]*hedgedRegretVector[i])/(2*c));
			}
			if (Math.abs(LHS - RHS) < EPS) {
				// done
				break;
			}

			if (LHS < RHS) {
				// c too high, decrease c
				maxC = c;
			}
			else if (RHS < LHS) {
				// c too low, increase c
				minC = c;
			}
		}
		
		// normalization
		double[] weights = new double[nParticles];
		double sum = 0;

		if (c < EPS) c = 1e-4;

		for (int i=0;i<nParticles;i++) {
			double r = hedgedRegretVector[i];

			// avoid numerical instability
			weights[i] = r*Math.exp((r*r)/(2*c));
			
			sum += weights[i];
		}
		
		// avoid numerical instability
		if (sum < EPS) {
			double w = 1.0/nParticles;
			for (int i=0;i<nParticles;i++) {
				particles[i].setWeight(w);
			}
		}
		else {
			for (int i=0;i<nParticles;i++) {
				particles[i].setWeight(weights[i]/sum);
			}
		}
	}
	
	public double[] getWeightVector() {
		double[] retVal = new double[nParticles];
		for (int i=0;i<nParticles;i++) {
			retVal[i] = particles[i].getWeight();
		}
		return retVal;
	}
	
	
	public void updateParticleStates() {
		for (int i=0;i<nParticles;i++) {
			particles[i].updateState();
		}
	}
	
	public Particle get(int idx) {
		return particles[idx];
	}
	
	public Particle[] getParticles() {
		return particles;
	}
	
}
