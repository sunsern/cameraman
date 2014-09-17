import java.util.*;
import java.io.*;

import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class MaxVideoRecorder extends MaxObject {

	private BufferedWriter fout;
	private String path;
	private String prefix, current_path, recordtime;

	private JitterMatrix jm_video = new JitterMatrix();
	private JitterMatrix jm_buf1 = new JitterMatrix();
	private JitterMatrix jm_buf2 = new JitterMatrix();
	
	private int cur_frame_num;
	private int frames_to_record;
	private boolean recording, enable;


	public MaxVideoRecorder(String p, int num_record) {

		declareIO(2,1);

		/* create path prefix */
		Calendar rightNow = Calendar.getInstance();
		String d = Integer.toString(rightNow.get(Calendar.DAY_OF_MONTH));
		if (d.length() < 2) d = "0"+d;
		String m = Integer.toString(rightNow.get(Calendar.MONTH)+1);
		if (m.length() < 2) m = "0"+m;
		String y = Integer.toString(rightNow.get(Calendar.YEAR));
		prefix = new String(y+m+d+"_");
		
		/* initialization */
		path = p;
		recording = false;
		enable = true;

		frames_to_record = num_record;
		
		/* if frames_to_record is 0, set it to infinity */
		if (frames_to_record == 0)
		    frames_to_record = 999999999;
		
	}

	public void enable() {
		enable = true;
		post("video recording enabled");
	}

	public void disable() {
		enable = false;
  		post("video recording disabled");
	}
	
	
	public void start() {
		if (!recording && enable) {
			startNewRecording();
		}
	}

	public void stop() {
		if (recording) {
			stopRecording();	
		}
	}

	public void jit_matrix(String s) {
		int inlet_num = getInlet();
		
		if (inlet_num == 0) {
			jm_buf2.frommatrix(jm_buf1);
			jm_buf1.frommatrix(jm_video);
			jm_video.frommatrix(s);
		}
		
		if (recording) {
			if (cur_frame_num <= frames_to_record) {
				saveCurrentFrame();
				cur_frame_num++;
			}
			else {
				stopRecording();
			}
		}
		
	}
		
	private void saveCurrentFrame() {
		String fn = Integer.toString(cur_frame_num);
		while (fn.length() < 4) { fn = "0" + fn; }
		jm_video.exportimage(current_path+File.separator+prefix+recordtime+"_"+fn);
		
		//post("-> Processing frame "+cur_frame_num);
	}

	private void startNewRecording() {
		
		recording = true;
		outlet(0,"start");
		
		Calendar rightNow = Calendar.getInstance();
		String h = Integer.toString(rightNow.get(Calendar.HOUR_OF_DAY));
		if (h.length() < 2) h = "0"+h;
		String m = Integer.toString(rightNow.get(Calendar.MINUTE));
		if (m.length() < 2) m = "0"+m;
		String s = Integer.toString(rightNow.get(Calendar.SECOND));
		if (s.length() < 2) s = "0"+s;
		
		recordtime = h+m+s; 
		current_path = new String(path+prefix+recordtime);
		File dir = new File(current_path);
		dir.mkdir();
		
		post("video recording: writing "+path+prefix+recordtime);
		
		cur_frame_num = 1;
		
		/* write buffer */
		String fn = Integer.toString(cur_frame_num);
		while (fn.length() < 4) { fn = "0" + fn; }
		jm_buf2.exportimage(current_path+File.separator+prefix+recordtime+"_"+fn);
		cur_frame_num++;
		
		fn = Integer.toString(cur_frame_num);
		while (fn.length() < 4) { fn = "0" + fn; }
		jm_buf1.exportimage(current_path+File.separator+prefix+recordtime+"_"+fn);
		cur_frame_num++;
		
	}

	private void stopRecording() {
		recording = false;
		outlet(0,"stop");
		post("video recording: done writing "+path+prefix+recordtime);
	}

	
}
