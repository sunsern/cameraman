package core.nh.videotracker;

import java.util.Random;

import core.nh.particlefilter.Particle;


/**
 *  
 * @author scheaman
 *
 */
public class TargetParticle extends Particle {

  Random r;
  TargetTrackerConfig config;
  double sigmaPos,sigmaSize,sigmaVelocityXY,sigmaVelocitySize,sigmaTheta;

  public TargetParticle(TargetState state, TargetTrackerConfig config) {
    super(state);
    r = new Random();
    sigmaPos = config.getSigmaPosition();
    sigmaSize = config.getSigmaSize();
    sigmaVelocityXY = config.getSigmaVelocityXY();
    sigmaVelocitySize = config.getSigmaVelocitySize();
    sigmaTheta = config.getSigmaTheta();
    this.config = config;
  }

  @Override
  public Particle resample() {
	  TargetState s1 = (TargetState)this.getCurrentState();
	  double[] v1 = s1.getStateVector();
	  double[] output = new double[v1.length]; 
	  
	  output[0] = v1[0] + r.nextGaussian()*sigmaPos;
	  output[1] = v1[1] + r.nextGaussian()*sigmaPos;
	  output[2] = v1[2] + r.nextGaussian()*sigmaSize;
	  output[3] = v1[3] + r.nextGaussian()*sigmaVelocityXY;
	  output[4] = v1[4] + r.nextGaussian()*sigmaVelocityXY;
	  output[5] = v1[5] + r.nextGaussian()*sigmaVelocitySize;
	  output[6] = v1[6] + + r.nextGaussian()*sigmaTheta;
	  
	  TargetParticle p = new TargetParticle(new TargetState(output), config);
	  p.setWeight(this.weight);
	  p.setRegret(this.regret);
	  
	  return p;
  }

}
