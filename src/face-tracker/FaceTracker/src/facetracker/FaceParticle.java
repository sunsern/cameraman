package facetracker;

import java.util.Random;
import particlefilter.Particle;
import particlefilter.State;

public class FaceParticle extends Particle {

    private TrackerConfig config;
    private Random randomGenerator;
	
    public FaceParticle(State state, boolean isFixed, TrackerConfig config) {
		super(state, isFixed);
		randomGenerator = new Random();
		this.config = config;
	}
	
	public FaceParticle resample() {
	    return resample(config.sigmaSize,config.sigmaVelocity);
	}

	public FaceParticle resample(double sigmaPosition, double sigmaSize, double sigmaVelocity) {
		
		FaceState faceState = (FaceState)this.getCurrentState();
		double[] stateVector = faceState.getStateVector();

		FaceState newFaceState = new FaceState();
		double[] newStateVector = newFaceState.getStateVector();
		
		double r;

		// new x
		r = randomGenerator.nextGaussian();
		newStateVector[0] = (r*sigmaPosition + stateVector[0]);
		
		// new y
		r = randomGenerator.nextGaussian();
		newStateVector[1] = (r*sigmaPosition + stateVector[1]);
		
		// new size
		r = randomGenerator.nextGaussian();
		newStateVector[2] = (r*sigmaSize + stateVector[2]);
		
		// new vx
		r = randomGenerator.nextGaussian();
		newStateVector[3] = (r*sigmaVelocity + stateVector[3]);
		
		// new vy
		r = randomGenerator.nextGaussian();
		newStateVector[4] = (r*sigmaVelocity + stateVector[4]);
		
		return new FaceParticle(newFaceState,false,config);		
	}

	
	public FaceParticle resample(double sigmaSize, double sigmaVelocity) {
		
		FaceState faceState = (FaceState)this.getCurrentState();
		double[] stateVector = faceState.getStateVector();

		FaceState newFaceState = new FaceState();
		double[] newStateVector = newFaceState.getStateVector();
		
		double r;

		double sp = stateVector[2]/(16);

		// new x
		r = randomGenerator.nextGaussian();
		newStateVector[0] = (r*sp + stateVector[0]);
		
		// new y
		r = randomGenerator.nextGaussian();
		newStateVector[1] = (r*sp + stateVector[1]);
		
		// new size
		r = randomGenerator.nextGaussian();
		newStateVector[2] = (r*sigmaSize + stateVector[2]);
		
		// new vx
		r = randomGenerator.nextGaussian();
		newStateVector[3] = (r*sigmaVelocity + stateVector[3]);
		
		// new vy
		r = randomGenerator.nextGaussian();
		newStateVector[4] = (r*sigmaVelocity + stateVector[4]);
		
		return new FaceParticle(newFaceState,false,config);		
	}

}
