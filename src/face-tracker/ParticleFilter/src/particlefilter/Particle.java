package particlefilter;


public abstract class Particle {
	
	protected State currentState;
	protected boolean isFixed;
	protected double score;
	protected double regret;
	protected double weight;
	
	public Particle(State state, boolean isFixed) {
		this(state,isFixed,0);
	}
	
	public Particle(State state, boolean isFixed, double weight) {
		this.currentState = state;
		this.isFixed = isFixed;
		this.weight = weight;
		this.score = Double.NaN;
		this.regret = 0;
	}
	
	public boolean isFixedParticle() {
		return isFixed;
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
