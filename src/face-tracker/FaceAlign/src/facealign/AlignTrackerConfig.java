package facealign;

import facetracker.TrackerConfig;

public class AlignTrackerConfig extends TrackerConfig {

	/** FaceParticle.java constants */
	public double sigmaPosition = 2.0;
	public double sigmaSize = 2.0;
	public double sigmaVelocity = 1.0;
	
	/** FaceTracker.java constants */
	public final int DEFAULT_DYNAMIC_PARTICLES_NUM = 750;
	public final double DEFAULT_ALPHA = 0.2;
	public final int GRIDSIZE = 0;
	public final int[] FIXEDSIZES = new int[] {32,48,64,80,96};
	public final int NFIXED = GRIDSIZE * GRIDSIZE * FIXEDSIZES.length;
	
}
