package facetracker;

public class Config {

	/** FaceParticle.java constants */
	public static double sigmaPosition = 3.0;
	public static double sigmaSize = 2.0;
	public static double sigmaVelocity = 1.0;
	
	/** FaceTracker.java constants */
	public static final int DEFAULT_DYNAMIC_PARTICLES_NUM = 500;
	public static final double DEFAULT_ALPHA = 0.2;
	public static final int GRIDSIZE = 0;
	public static final int[] FIXEDSIZES = new int[] {32,48,64,80,96};
	public static final int NFIXED = GRIDSIZE * GRIDSIZE * FIXEDSIZES.length;
	
}
