package facetracker;

public class TrackerConfig {

	/** FaceParticle constants */
	public double sigmaPosition = 4.0;
	public double sigmaSize = 4.0;
	public double sigmaVelocity = 0.0;
	
	/** FaceTracker constants */
	public final int DEFAULT_DYNAMIC_PARTICLES_NUM = 200;
	public final double DEFAULT_ALPHA = 0.50;
	public final int GRIDSIZE = 0;
	public final int[] FIXEDSIZES = new int[] {32,48,64,80,96};
	public final int NFIXED = GRIDSIZE * GRIDSIZE * FIXEDSIZES.length;
}
