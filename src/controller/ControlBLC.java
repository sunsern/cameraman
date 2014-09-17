import com.cycling74.max.*;
import com.cycling74.jitter.*;
import java.net.*;
import java.io.*;


public class ControlBLC extends MaxObject {

    float[] d;
    int d_idx;
    //boolean isOn;
    //float threshold_dark_on = 0.3f;
    //float threshold_dark_off = 0.04f;
    int expcomp;
    boolean sending_command;

    public ControlBLC() {

	Authenticator.setDefault(new MyAuthenticator("cameraman", "cameraman"));

	//isOn = false; 
	//turnOffBLC();
	resetExpComp();

	d = new float[4];
	d_idx = 0;

	declareIO(1,0);

	sending_command = false;
    }


    private float getMean() {
	float mean = 0f;
	for (int i=0;i<d.length;i++)
	    mean += d[i];
	return (mean/d.length);
    }

    /*
    private void turnOffBLC() {
	Thread t = new SendCommandThread("BLComp=off");
	t.start();
	post("back light comp = off");
    }

    private void turnOnBLC() {
	Thread t = new SendCommandThread("BLComp=on");
	t.start();
	post("back light comp = on");
    }
    */

    private void resetExpComp() {
	expcomp = 6;
	Thread t = new SendCommandThread("ExpComp=" + expcomp);
	t.start();
	post("exp comp = " + expcomp); 
    }

    private void incExpComp() {
	/*
	if (!sending_command) {
	    expcomp++;
	    if (expcomp > 14) expcomp = 14;
	    Thread t = new SendCommandThread("ExpComp=" + expcomp);
	    t.start();
	    post("exp comp = " + expcomp); 
	}
	*/
    }

    private void decExpComp() {
	/*
	if (!sending_command) {
	    expcomp--;
	    if (expcomp < 0) expcomp = 0;
	    Thread t = new SendCommandThread("ExpComp=" + expcomp);
	    t.start();
	    post("exp comp = " + expcomp); 
	}
	*/
    }



    public void inlet(float f) {
	
	d[d_idx++] = f;	
	if (d_idx == d.length) d_idx=0;

	float m = getMean();
	
	/*
	if (!isOn && 
	    m > threshold_dark_on) {
	    isOn = true;
	    turnOnBLC();
	}
	else if (isOn &&
		 m < threshold_dark_off) {
	    isOn = false;
	    turnOffBLC();
	}
	*/


	if (m > 0.19) {
	    incExpComp();
	}
	else if (m < 0.06) {
	    decExpComp();
	}


    }

    public void user_lost() {
	//isOn = false;
	//turnOffBLC();
	resetExpComp();
    }

    
    public void anything(String msg, Atom[] args) {
	// do nothing
    }
    

    private class SendCommandThread extends Thread {
	String command;
	public SendCommandThread(String cmd) {
	    command = cmd;
	}

	public void run() {
	    sending_command = true;
	    try {
		URL inq = new URL("http://192.168.1.11/command/camera.cgi?" + command);
		URLConnection urlc = inq.openConnection();
		HttpURLConnection httpc = (HttpURLConnection)httpc;
		httpc.setRequestMethod("HEAD");
		httpc.getContentLength();
	    }
	    catch (Exception e) {
		e.printStackTrace();
	    }
	    sending_command = false;
	}
    }

    private class MyAuthenticator extends Authenticator {
	private String username, password;

	public MyAuthenticator(String user, String pass) {
	    username = user;
	    password = pass;
	}

	protected PasswordAuthentication getPasswordAuthentication() {
	    //post("Requesting Host  : " + getRequestingHost());
	    //post("Requesting Port  : " + getRequestingPort());
	    //post("Requesting Prompt : " + getRequestingPrompt());
	    //post("Requesting Protocol: " + getRequestingProtocol());
	    //post("Requesting Scheme : " + getRequestingScheme());
	    //post("Requesting Site  : " + getRequestingSite());
	    return new PasswordAuthentication(username, password.toCharArray());
	}
    }


}
