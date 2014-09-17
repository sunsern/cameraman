package app.facetracker;

import core.nh.videotracker.TargetTrackerConfig;

public class FaceTrackerConfig extends TargetTrackerConfig {

	@Override
	public double getSigmaPosition() { return 8.0; }
	
	@Override
	public double getSigmaSize() { return 8.0; }
	
	@Override
	public double getSigmaVelocityXY() { return 0.0; }
	
	@Override
	public double getSigmaVelocitySize() { return 0.0; }
	
	@Override
	public double getSigmaTheta() { return 0.0; }

	@Override
	public int getParticlesNum() { return 50; }

	@Override
	public double getAlpha() { return 0.75; }
  
}
