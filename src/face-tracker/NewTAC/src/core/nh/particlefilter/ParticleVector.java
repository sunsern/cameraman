package core.nh.particlefilter;

public class ParticleVector  {

	public Particle[] particles;
	public int nParticles;

	public ParticleVector(Particle[] particles) { 		
		this.particles = particles;
		this.nParticles = particles.length;
	}


	public void updateScores(Scorer scorer) {
		for (int i=0;i<nParticles;i++) {
			particles[i].updateScore(scorer);
		}
	}

	public double getAverageScore() {
		double sum = 0;
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
		particles[i] = p;
	}

	// return c_t
	public double updateWeights() {

		double minC = 1e-4, maxC = 0;
		double[] hedgedRegretVector = new double[nParticles];
		for (int i=0;i<nParticles;i++) {
			double regret = particles[i].getRegret();			
			hedgedRegretVector[i] = (regret > 0 ? regret : 0);
			maxC += hedgedRegretVector[i] * hedgedRegretVector[i];
		}
		
		// equally bad? assign uniform weight
		if (maxC < minC) {
			double w = 1.0/nParticles;
			for (int i=0;i<nParticles;i++) {
				particles[i].setWeight(w);
			}
			return Double.NaN;
		}
		else {

			// compute c by binary search
			double EPS = 1e-9;
			double RHS = nParticles * Math.E;
			double LHS = 0, c = 0;

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

			// check c
			if (Math.abs(LHS - RHS) > 1e-1) {
				System.out.println("<<<<<<<<<<<<<<<");
				System.out.println("Error: finding C");
				System.out.println("LHS=" + LHS );
				System.out.println("RHS=" + RHS );
				System.out.println("<<<<<<<<<<<<<<<");
			}


			// c is too low, assign uniform distribution
			if (c < 1e-3) {
				System.out.println("Warning: C is too small.. setting C = 1e-4");
				c = 1e-3;
			}

			double[] weights = new double[nParticles];
			double sum = 0;

			for (int i=0;i<nParticles;i++) {
				double r = hedgedRegretVector[i];
				weights[i] = r*Math.exp((r*r)/(2*c));
				sum += weights[i];
			}

			// normalization
			if (sum < 1e-7) {
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

			return c;
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
