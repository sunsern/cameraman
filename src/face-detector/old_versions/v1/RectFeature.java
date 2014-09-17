
public class RectFeature extends Feature {

	/** 
	 * Helper class for keeping rectangle info
	 */
    private class Coords {
    	private int x, y, w, h, v;
    	public Coords(int xc, int yc, int width, int height, int val) {
    		x = xc; y = yc; w = width; h = height; v = val;
    	}
    	public void printCoord() { System.out.println(x+" "+y+" "+w+" "+h+" "+v); }
    }
    
    private Coords[] rectCoords;
    private int num_coords;
    
  
    public double getFeatureValue(Candidate c, IntegralImage ii, int window_size) {
    	long acc = 0;
    	for (int i = 0; i < num_coords; i++) {
    		acc += rectCoords[i].v * ii.getSum(c.x + rectCoords[i].x * c.size / window_size, 
    			 	  		 		           c.y + rectCoords[i].y * c.size / window_size, 
    			 	  		 		           rectCoords[i].w * c.size / window_size, 
    			 	  		 	               rectCoords[i].h * c.size / window_size);
    	}
    	/* normalize by variance and size */
		return acc / (ii.getVariance(c.x, c.y, c.size)*(c.size)*(c.size));
	}
    
    public double testCandidate(Candidate c, IntegralImage ii, int window_size) {
    	long acc = 0;
    	for (int i = 0; i < num_coords; i++) {
    		acc += rectCoords[i].v * ii.getSum(c.x + rectCoords[i].x * c.size / window_size, 
	  		 		   						   c.y + rectCoords[i].y * c.size / window_size, 
	  		 		                           rectCoords[i].w * c.size / window_size, 
	  		 		                           rectCoords[i].h * c.size / window_size);
    	}
    	if (acc < this.threshold * ii.getVariance(c.x, c.y, c.size)*(c.size)*(c.size)) 
    		return this.lValue;
    	else
    		return this.rValue;
	}
    
    public void addCoordinate(int x, int y, int w, int h, int v) {
    	rectCoords[num_coords] = new Coords(x,y,w,h,v);
    	num_coords++;
    }
	
 
    public RectFeature(int n) {
    	num_coords = 0;
    	rectCoords = new Coords[n];   
    }
    
   
    public void printFeature() {
    	int i; 
    	for(i = 0; i < num_coords; i++) {
    		rectCoords[i].printCoord();
    	}
    	System.out.println("threshold = " + getThreshold());
    	System.out.println("right val = " + getRightValue());
    	System.out.println("left val = " + getLeftValue());
    	System.out.println("--------------------");
    }
}