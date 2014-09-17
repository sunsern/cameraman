import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class RGB2HSV extends MaxObject {
    
    private int[] input;
    private float[] output;

    private int _p, _w, _h;
    private JitterMatrix jm;

    private int[] dim;
    private float[] hsv  = new float[3];
    private int[] hist = new int[256];

    private static void rgb2hsv(int r, int g, int b, float hsv[]) {
	
	int min; // Min. value of RGB
	int max; // Max. value of RGB
	int delMax; // Delta RGB value
	
	if (r > g) {
	    min = g;
	    max = r;
	}
	else {
	    min = r;
	    max = g;
	}
	if (b > max) max = b;
	if (b < min) min = b;
	
	delMax = max - min;
	
	float H = 0, S;
	float V = max / 255f;
	
	if (delMax == 0) {
	    H = 0;
	    S = 0;
	}
	else {
	    S = (float)delMax / max;
	    if (r == max) H = ((float)(g - b) / (float) delMax) * 60;
	    else if (g == max) H = (2 + (float)(b - r) / (float) delMax) * 60;
	    else if (b == max) H = (4 + (float)(r - g) / (float) delMax) * 60;
	}
	if (H < 0) H += 360;
	if (H > 360) H -= 360;
	
	hsv[0] = H / 360f;
	hsv[1] = S;
	hsv[2] = V;
	
    }
    
    public RGB2HSV() {

	_p = 4;
	_w = 0;
	_h = 0;

	input = null;
	output = null;

	jm = new JitterMatrix();
	
	declareIO(1,1);

    }
	
    public void jit_matrix(String s) {

	int r,g,b;

	jm.frommatrix(s);
	dim = jm.getDim();

	if (_w != dim[0] || _h != dim[1]) {
	    _w = dim[0];
	    _h = dim[1];
	    input = new int[_w*_h*4];
	    output = new float[_w*_h*4];
	}
	jm.setType("char");
	jm.copyMatrixToArray(input);
		
	for (int i=0; i < 256; i++) hist[i]=0;
	
	for (int i = 0; i < _w*_h; i++) {
	    /* read rgb */
	    r = input[i*4+1];
	    g = input[i*4+2];
	    b = input[i*4+3];
		
	    /* convert to hsv */
	    rgb2hsv(r,g,b,hsv);
		
	    /* create output */
	    output[i*4]   = 1f;
	    output[i*4+1] = hsv[0];
	    output[i*4+2] = hsv[1];
	    output[i*4+3] = hsv[2];

	    //hist[(int)(hsv[2]*255f) & 0xff]++;
	}
	
	//for (int i=1; i < 256; i++) hist[i]=hist[i]+hist[i-1];
	//for (int i=0; i < _w*_h; i++) {
	//    output[i*4+3] = (float)hist[(int)(output[i*4+3]*255) & 0xff] / hist[255];
	//}
	
	jm.setType("float32");
	jm.copyArrayToMatrix(output);

	outlet(0,"jit_matrix",jm.getName());
    }
}
