import com.cycling74.max.*;
import com.cycling74.jitter.*;
import java.net.*;


public class ExpComp extends MaxObject {

    final String userpasswd = "cameraman:cameraman";
    final String encoding = new sun.misc.BASE64Encoder().encode(userpasswd.getBytes());

    final double UPPER_THRESHOLD = 0.8;
    final double LOWER_THRESHOLD = 0.2;

    final int SLEEP_TIME_AFTER_CMD = 800;
    final int SLEEP_TIME = 400;

    float[] shadow, light;
    int shadow_idx, light_idx;

    JitterMatrix jm;
    int jmWidth, jmHeight;
    int[] v;
    float s,l;
    boolean running, reset;
    Thread t = new ExpCompThread();
    

    public ExpComp() {

	jm = new JitterMatrix();
	jmWidth = 240;
	jmHeight = 240;
	v = new int[jmWidth *jmHeight];

	shadow = new float[2];
	shadow_idx = 0;

	light = new float[2];
	light_idx = 0;

	declareIO(1,1);

	running = false;
	reset = false;

	t.start();
    }


   
    public void jit_matrix(String m) {

	jm.frommatrix(m);
	int[] dim = jm.getDim();
	if (dim[0] != jmWidth || dim[1] != jmHeight) {
	    jmWidth = dim[0];
	    jmHeight = dim[1];
	    v = new int[jmWidth *jmHeight];	
	}

	if (!running) {
	    synchronized(t) {
		t.notify();
	    }
	}

    }


    public void user_lost() {
	reset = true;
    }

    
    public void anything(String msg, Atom[] args) {
	// do nothing
    }


    private class ExpCompThread extends Thread {

	private void resetExpComp() {
	    post("Reset Exposure");
	    sendCommand("8101040E00FF");
	}

	private void incExpComp() {
	    post("Increase Exposure");
	    sendCommand("8101040E02FF");
	}
	
	private void decExpComp() {
	    post("Decrease Exposure");
	    sendCommand("8101040E03FF");
	}
	
	private void sendCommand(String cmd) {
	    try {
		URL inq = new URL("http://cameraman:cameraman@192.168.1.11/command/visca-gen.cgi?visca=" + cmd); 
		URLConnection urlc = inq.openConnection();
		urlc.setRequestProperty ("Authorization", "Basic " + encoding);
		urlc.getContentLength();
	    } 
	    catch (Exception e) {
		// do nothing
	    }
	}


	private void trySleep(int duration) {
	    //post("going to sleep");
	    try {
		sleep(duration);
	    }
	    catch (Exception e) { }
	    //post("wake up!");
	}

	private float getMean(float[] d) {
	    float mean = 0f;
	    for (int i=0;i<d.length;i++)
		mean += d[i];
	    return (mean/d.length);
	}

	public void run() {

	    while (true) {
		
		try {
		    synchronized(this) {
			wait();
		    }
		}
		catch (Exception e) { }
		
		running = true;

		// count pixels
		jm.copyMatrixToArray(v);
		s = 0; l = 0;
		for (int i=0;i<v.length;i++) {
		    if ((double)v[i] / 255d < LOWER_THRESHOLD) {
			s += 1;
		    }
		    else if ((double)v[i] / 255d > UPPER_THRESHOLD) {
			l += 1;
		    } 
		}
		s /= v.length;
		l /= v.length;

		// calculation

		shadow[shadow_idx++] = s;
		if (shadow_idx == shadow.length) shadow_idx = 0;
	    
		light[light_idx++] = l;
		if (light_idx == light.length) light_idx = 0;
	    
		float mShadow = getMean(shadow);
		float mLight = getMean(light);
	    
		//post("shadow = " + mShadow);
		//post("light = " + mLight);
	    
		if (reset) {
		    resetExpComp();
		    trySleep(SLEEP_TIME_AFTER_CMD);
		    reset = false;
		}
		else if (mLight + mShadow < 0.5) {
		    trySleep(SLEEP_TIME);
		}
		else if (mLight > mShadow){
		    decExpComp();
		    trySleep(SLEEP_TIME_AFTER_CMD);
		}
		else {
		    incExpComp();
		    trySleep(SLEEP_TIME_AFTER_CMD);
		}

		running = false;
	    }
	}
    }

    public void notifyDeleted() {
	t = null;
    }

}
