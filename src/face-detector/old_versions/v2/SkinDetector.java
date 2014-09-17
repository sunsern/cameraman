
public class SkinDetector {

	private ColorScorer cs;
	
	public static void rgb2hsv(int _r, int _g, int _b, double hsv[]) {
		double var_R = (_r / 255f);
		double var_G = (_g / 255f);
		double var_B = (_b / 255f);
		double var_Min;
		double var_Max;
		double del_Max;
		if (var_R > var_G) {
			var_Min = var_G;
			var_Max = var_R;
		} else {
			var_Min = var_R;
			var_Max = var_G;
		}
		if (var_B > var_Max)
			var_Max = var_B;
		if (var_B < var_Min)
			var_Min = var_B;
		del_Max = var_Max - var_Min;
		double H = 0, S, V;
		V = (var_Max + var_Min) / 2f;
		if (del_Max == 0) {
			H = 0;
			S = 0;
		} else {
			if (V < 0.5)
				S = del_Max / (var_Max + var_Min);
			else
				S = del_Max / (2 - var_Max - var_Min);
			double del_R = (((var_Max - var_R) / 6f) + (del_Max / 2f))
					/ del_Max;
			double del_G = (((var_Max - var_G) / 6f) + (del_Max / 2f))
					/ del_Max;
			double del_B = (((var_Max - var_B) / 6f) + (del_Max / 2f))
					/ del_Max;
			if (var_R == var_Max)
				H = del_B - del_G;
			else if (var_G == var_Max)
				H = (1 / 3f) + del_R - del_B;
			else
				H = (2 / 3f) + del_G - del_R;
			if (H < 0)
				H += 1;
			if (H > 1)
				H -= 1;
		}
		hsv[0] = H;
		hsv[1] = S;
		hsv[2] = V;
	}

	public byte[] detect(double[] hsv, int w, int h, double margin) {
		byte[] positive = new byte[w * h];
		double[] temp = new double[3];

		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				temp[0] = hsv[(i * w + j) * 3];
				temp[1] = hsv[(i * w + j) * 3 + 1];
				temp[2] = hsv[(i * w + j) * 3 + 2];
				double s = cs.getScore(temp);
				
				// definitely correct
				if (s > margin) {
					positive[i * w + j] = 1;
				}
				// definitely incorrect
				else if (s < -margin) {
					positive[i * w + j] = 0;
				}
				// not sure
				else {
					if (i == 0 || j == 0) {
						positive[i * w + j] = 0;
					} else {
						int vote = 0;
						if (positive[(i - 1) * w + j] == 1)
							vote++;
						if (positive[(i - 1) * w + (j - 1)] == 1)
							vote++;
						if (positive[i * w + (j - 1)] == 1)
							vote++;
						if (vote >= 2) {
							positive[i * w + j] = 1;
						} else {
							positive[i * w + j] = 0;
						}
					}
				}
			}
		}
		return positive;
	}

	public SkinDetector(String xml) {
		cs = new ColorScorer(xml);
	}
	
}
