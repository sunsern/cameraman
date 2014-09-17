package core.nh.videotracker;

public class TargetTrackerConfig {

  public double getSigmaPosition() { return 3.0; }
  public double getSigmaSize() { return 1.0; }
  public double getSigmaVelocityXY() { return 0.0; }
  public double getSigmaVelocitySize() { return 0.0; }
  public double getSigmaTheta() { return 0.0; }
  
  public int getParticlesNum() { return 50; }
  public double getAlpha() { return 0.75; }	
}
