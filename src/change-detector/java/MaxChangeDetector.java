import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class MaxChangeDetector extends MaxObject {

	private double[] ap, am, input, output, mask_output;
	private boolean[] inRange;
	private int _p, _w, _h;
	private JitterMatrix jm, mask_jm;
	private int[] dim;
	private int[] offset = new int[] {0,0};
	private double activity;
	
	private double K = 4;
	private double ALPHA = 0.001;
	private double C = 16;
	private double RESET_AMOUNT = 0.4;
	
	
	public MaxChangeDetector(int w, int h) {
		_w = w;
		_h = h;
		_p = 4;
		
		ap = new double[_p*_w*_h];
		am = new double[_p*_w*_h];
		input = new double[_p*_w*_h];
		output = new double[_p*_w*_h];
		mask_output = new double[_w*_h];

		inRange = new boolean[_w*_h];
	
		mask_jm = new JitterMatrix(1,"char",w,h);
		jm = new JitterMatrix();

		this.reset();

		declareAttribute("K");
		declareAttribute("C");
		declareAttribute("ALPHA");
		declareAttribute("RESET_AMOUNT");
		declareIO(2,2);
	}
	
	
	private void update() {
		double au,al;
		int idx;
		for (int i = 0; i < _h; i++) {
			for (int j = 0; j < _w;j++) {
				for (int k = 1;k < _p;k++) {
					idx = (i*_w+j)*_p+k;					
					if (am[idx] <= input[idx] && input[idx] <= ap[idx]) {						
						au = ((K+1)*ap[idx] + (K-1)*am[idx])/(2*K);
						al = ((K-1)*ap[idx] + (K+1)*am[idx])/(2*K);					
						if (input[idx] > au) 
							ap[idx] = ap[idx] + C*ALPHA*(input[idx]-au);
						else
							ap[idx] = ap[idx] + ALPHA*(input[idx]-au);
					
						if (input[idx] > al) 
							am[idx] = am[idx] + ALPHA*(input[idx]-al);
						else
							am[idx] = am[idx] + C*ALPHA*(input[idx]-al);
					}
					
				}
			}
		}
	}
	
	private void reset() {
		int idx;
		for (int i = 0; i < _h; i++) {
			for (int j = 0; j < _w;j++) {
				for (int k = 1;k < _p;k++) {	
					idx = (i*_w+j)*_p+k;
					am[idx] = input[idx] - RESET_AMOUNT;
					ap[idx] = input[idx] + RESET_AMOUNT;
				}
			}
		}
		
	}
	
	private double computeInRange() {
		int idx;
		int count = 0;
		for (int i = 0; i < _h; i++) {
			for (int j = 0; j < _w;j++) {
				inRange[i*_w+j] = true;
				for (int k = 1;k < _p;k++) {	
					idx = (i*_w+j)*_p+k;
					if (input[idx] < am[idx] || ap[idx] < input[idx]) {
						inRange[i*_w+j] = false;
					}
				}
				if (!inRange[i*_w+j]) count++;
			}
		}
		return (double)count/(_w*_h);
	}
	
	private void createOutput() {
		int idx;
		for (int i = 0; i < _h; i++) {
			for (int j = 0; j < _w;j++) {
				idx = (i*_w+j)*_p;
				if (inRange[i*_w+j]) { 
					output[idx]   = 1.0;
					output[idx+1] = 0.0;
					output[idx+2] = 0.0;
					output[idx+3] = 0.0;
				}
				else {
					/*
					output[idx]   = input[idx];
					output[idx+1] = input[idx+1];
					output[idx+2] = input[idx+2];
					output[idx+3] = input[idx+3];
					*/
					output[idx]   = 1.0;
					output[idx+1] = 1.0;
					output[idx+2] = 1.0;
					output[idx+3] = 1.0;
				}		
			}
		}
	}


	public void bang() {
		int inlet_num = getInlet();
		/* reset */
		if (inlet_num == 1) {
			this.reset();
		}
	}
	
	public void jit_matrix(String s) {
		jm.frommatrix(s);
		dim = jm.getDim();

		for (int i = 0; i < dim[1]; i++) {
			offset[1] = i;
			jm.copyVectorToArray(0, offset, input, dim[0]*_p, dim[0]*i*_p);	
		}
		
		update();
		activity = computeInRange();
		createOutput();
		
		for (int i = 0; i < dim[1]; i++) {
			offset[1] = i;
			jm.copyArrayToVector(0, offset, output, dim[0]*_p, dim[0]*i*_p);
		}

		outlet(0,"jit_matrix",jm.getName());
		outlet(1,"float",activity);
	}
}
