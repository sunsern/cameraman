package particlefilter;

/**
 * Main class 
 * 
 * @author scheaman
 *
 */
public abstract class ParticleFilter {

	protected Scorer scorer;
	protected double alpha;
	public ParticleVector pv;
	
	public ParticleFilter(int n, int nFixed, double alpha, Scorer scorer) {
		Particle[] particles = new Particle[n];
		pv = new ParticleVector(particles, nFixed);
		this.alpha = alpha;
		this.scorer = scorer;
	}

	public void addDynamicParticles(Particle[] dynamicParticles) {

		double defaultWeight = 1.0/pv.nParticles;

		int k = pv.nParticles - pv.nFixed;
		for (int i=0;i<k;i++) {
			dynamicParticles[i].setWeight(defaultWeight); 
			pv.particles[i+pv.nFixed] = dynamicParticles[i];
		}
	}
	
	
	public void addFixedParticles(Particle[] fixedParticles) { 

		double defaultWeight = 1.0/pv.nParticles;
		
		for (int i=0;i<fixedParticles.length;i++) {
			fixedParticles[i].setWeight(defaultWeight);
			pv.particles[i] = fixedParticles[i];
		}
	}

	
	public void resampleAndUpdateWeights() {

		pv.updateScores(scorer);
		double avgScore = pv.getWeightedAverageScore();
		pv.updateRegrets(alpha, avgScore);

		// count number of particles to replace
		int numReplace = 0;
		for (int i=pv.nFixed;i<pv.nParticles;i++) {
			if (pv.getRegret(i) <= 0) numReplace++;
		}

		if (numReplace > 0) {

			// resample
			double[] weight = pv.getPositiveRegretWeightVector();
			double[] cumWeight = new double[weight.length];
			cumWeight[0] = weight[0];
			for (int i=1;i<weight.length;i++) {
				cumWeight[i] = cumWeight[i-1] + weight[i];
			}

			Particle[] newParticles = new Particle[numReplace];

			for (int i=0;i<numReplace;i++) {
				// random with prob proportional to weight
				double r = Math.random()*cumWeight[cumWeight.length-1];
				int idx = bsearch(cumWeight, r);
				Particle old = pv.get(idx);
				newParticles[i] = old.resample();
				newParticles[i].updateScore(scorer);
				newParticles[i].updateRegret(alpha,avgScore);
			}
			
			// replace particles
			for (int i=pv.nFixed;i<pv.nParticles;i++) {
				if (pv.getRegret(i) <= 0) {
					pv.replaceWith(i, newParticles[--numReplace]);
				}
			}

		}
		// update weights
		pv.updateWeights();
	}

	public void updateStates() {
		pv.updateParticleStates();
	}
	
	private static int bsearch(double[] vector, double key) {
		double EPS = 1e-9;
		int l=0, r=vector.length, c = -1;
		while (r-l > 1) {
			c = (l+r)/2;
			if (vector[c] > key) r = c;
			else if (vector[c] < key) l = c;
			else break;
		}
 
		while (c > 0 && vector[c] > key - EPS) c--;
		while (c+1 < vector.length && vector[c] < key - EPS) c++;
		return c;
	}
	
	public static void main(String[] args) {

		// bsearch test
		double[] v = new double[] {0,1,2,3,4,5,5,6,6,6,7,7,7,8,9};
		for (int i=0;i<v.length;i++)
			System.out.println(bsearch(v,v[i]));
		
		System.out.println(bsearch(v,10));
		System.out.println(bsearch(v,-1));
		System.out.println(bsearch(v,5.5));

		return;
	}
	
	
}
