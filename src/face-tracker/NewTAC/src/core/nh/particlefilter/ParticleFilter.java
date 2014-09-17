package core.nh.particlefilter;

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
	
	public ParticleFilter(int n, double alpha, Scorer scorer) {
		Particle[] particles = new Particle[n];
		pv = new ParticleVector(particles);
		this.alpha = alpha;
		this.scorer = scorer;
	}

	public void addParticles(Particle[] particles) {
		double defaultWeight = 1.0/pv.nParticles;
		for (int i=0;i<pv.nParticles;i++) {
			particles[i].setWeight(defaultWeight); 
			pv.particles[i] = particles[i];
		}
	}
		
	// return indices of particle vector sampled according weights
	public int[] sampleByWeight(int n) {
		int[] ret = new int[n];
		double[] weight = pv.getWeightVector();
		double[] cumWeight = new double[weight.length];
		cumWeight[0] = weight[0];
		for (int i=1;i<weight.length;i++) {
			cumWeight[i] = cumWeight[i-1] + weight[i];
		}
		for (int i=0;i<n;i++) {
			double r = Math.random()*cumWeight[cumWeight.length-1];
			ret[i] = bsearch(cumWeight, r);
		}
		return ret;
	}
	
	// return indices of particle vector sampled according inverted (1-x) weight
	public int[] sampleByWeightInverse(int n) {
		int[] ret = new int[n];
		double[] weight = pv.getWeightVector();
		for (int i=0;i<weight.length;i++) {
			weight[i] = 1.0-weight[i];
		}
		double[] cumWeight = new double[weight.length];
		cumWeight[0] = weight[0];
		for (int i=1;i<weight.length;i++) {
			cumWeight[i] = cumWeight[i-1] + weight[i];
		}
		for (int i=0;i<n;i++) {
			double r = Math.random()*cumWeight[cumWeight.length-1];
			ret[i] = bsearch(cumWeight, r);
		}
		return ret;
	}
	
	public void updateScoresAndRegret() {
		pv.updateScores(scorer);
		double avgScore = pv.getAverageScore();
		pv.updateRegrets(alpha, avgScore);	
	}
	
	// return c_t
	public double resampleAndUpdateWeights() {
		
		// count number of particles to replace
		int numReplace = 0;
		for (int i=0;i<pv.nParticles;i++) {
			if (pv.getRegret(i) <= 0) numReplace++;
		}

		// replace if necessary
		if (numReplace > 0) {
			int[] replaceBy = sampleByWeight(numReplace);
			int k=0;
			for (int i=0;i<pv.nParticles;i++) {
				if (pv.getRegret(i) <= 0) {
					Particle old = pv.get(replaceBy[k++]);
					Particle newParticle = old.resample();
					pv.replaceWith(i, newParticle);
				}
			}
		}
		
		// update scores and regrets after resampling
		updateScoresAndRegret();
		
		// update weights and return c_t
		return pv.updateWeights();
	}

	public void updateStates() {
		pv.updateParticleStates();
	}
	
	private static int bsearch(double[] vector, double key) {
		double EPS = 1e-9;
		int l=0, r=vector.length, c = -1;
		while (r-l > 1) {
			c = (l+r) >>> 1;
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
