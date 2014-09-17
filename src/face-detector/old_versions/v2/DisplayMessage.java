import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class DisplayMessage extends MaxObject {


    public DisplayMessage() {
	declareIO(1,1);
    }
    
    
    public void start_recording() {
	outlet(0,"set", new Atom[]{ Atom.newAtom("Recording!")});
    }
    
    public void stop_recording() {
	outlet(0,"set", new Atom[] { Atom.newAtom("")});
    }
    
    public void save_recording() {
	/* do nothing */
    }

}
