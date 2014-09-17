import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class MaxFloorDetector extends MaxObject {

	private double[] input, output;
	private boolean[] positive;
	private int _p, _w, _h;
	private JitterMatrix jm;
	private int[] dim;
	private int[] offset = new int[] {0,0};

	
	public MaxFloorDetector(int w, int h) {
		_w = w;
		_h = h;
		_p = 4;
		
		input = new double[_p*_w*_h];
		output = new double[_p*_w*_h];

		positive = new boolean[_w*_h];
	
		jm = new JitterMatrix();

		declareIO(1,1);
	}
	
	
	
	private void compute() {
		
		double[] temp = new double[3];
		int idx;
		
		for (int i = 0; i < _h; i++) {
			for (int j = 0; j < _w;j++) {		
				for (int k = 1;k < _p;k++) {					
					idx = (i*_w+j)*_p+k;
					temp[k-1] = input[idx];
				}
				positive[i*_w+j] = (FloorScorer.getScore(temp) < -1.0);
			}
		}
	}
	
	private void createOutput() {
		int idx;
		for (int i = 0; i < _h; i++) {
			for (int j = 0; j < _w;j++) {
				idx = (i*_w+j)*_p;
				if (positive[i*_w+j]) { 
					output[idx]   = 1.0;
					output[idx+1] = 0.33;
					output[idx+2] = 1.0;
					output[idx+3] = 0.5;
				}
				else {
					output[idx]   = input[idx];
					output[idx+1] = input[idx+1];
					output[idx+2] = input[idx+2];
					output[idx+3] = input[idx+3];
				}		
			}
		}
	}


	public void jit_matrix(String s) {
		jm.frommatrix(s);
		dim = jm.getDim();

		for (int i = 0; i < dim[1]; i++) {
			offset[1] = i;
			jm.copyVectorToArray(0, offset, input, dim[0]*_p, dim[0]*i*_p);	
		}
		
		compute();
		createOutput();
		
		for (int i = 0; i < dim[1]; i++) {
			offset[1] = i;
			jm.copyArrayToVector(0, offset, output, dim[0]*_p, dim[0]*i*_p);
		}

		outlet(0,"jit_matrix",jm.getName());
	}
}
