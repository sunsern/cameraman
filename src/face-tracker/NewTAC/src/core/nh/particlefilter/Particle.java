package core.nh.particlefilter;


public abstract class Particle {
	
	protected State currentState;
	protected double score;
	protected double regret;
	protected double weight;
	
	public Particle(State state) {
		this(state,0);
	}
	
	public Particle(State state, double weight) {
		this.currentState = state;
		this.weight = weight;
		this.score = Double.NaN;
		this.regret = 0;
	}
	
	public State getCurrentState() {
		return currentState;
	}
	 
	public void updateState() {
		currentState.update();
	}
	
	public void updateScore(Scorer scorer) {
		score = scorer.getScore(currentState);
	}

	public double getScore() {
		return score;
	}

	public void updateRegret(double alpha, double avgScore) {
		regret = (1-alpha)*regret + (score-avgScore);
	}

	public void setRegret(double r) {
		this.regret = r;
	}

	public double getRegret() {
		return regret;
	}
	
	public boolean isPositiveRegret() {
		return (this.regret > 0);
	}
	
    public abstract Particle resample();
    
	public void setWeight(double weight) {
		this.weight = weight;
	}

	public double getWeight() {
		return weight;
	}

}
