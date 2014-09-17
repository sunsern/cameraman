import java.util.ArrayList;
import java.util.Iterator;

import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class MaxFaceDetectorThreaded extends MaxObject {

	private static final int MAX_NUM_FACES = 10;
	private static final int MAX_NUM_TIERS = 2;
	private static final int MAX_NUM_CANDIDATES = 200000;
	
	private static final int[] sizes = new int[] {29, 35, 41, 50, 60, 72, 86, 103, 124, 149, 178};
	
	private JitterMatrix jm = new JitterMatrix();
	private int[] dim;
	private int[] offset = new int[] {0,0};
	private int[] raw,buf;
	
	private Candidate[][] candidatesArray;
	private Candidate[] facesArray = new Candidate[MAX_NUM_FACES * MAX_NUM_TIERS];
	
	private Cascade cc;
	private YUVImage currentFrame;
	private int currentTier;
	private int img_width, img_height, output_width, output_height;
	
	private boolean enable;
	
	
	public MaxFaceDetector(int ow, int oh, int w, int h) {
		
		img_width = w;
		img_height = h;
		output_width = ow;
		output_height = oh;
	
		cc = new Cascade("haarcascade_frontalface_alt.xml");
		
		int ct = 0;
		
		candidatesArray = new Candidate[MAX_NUM_TIERS][]; 
					
		for (int i=0; i < sizes.length; i++) {
			candidatesArray[i] = new Candidate[MAX_NUM_CANDIDATES];
			int k = 0;
			for (int y=0; y+sizes[i] < h;y+= 1+(sizes[i]/20) ) {
				for (int x=0; x+sizes[i] < w;x+= 1+(sizes[i]/20) ) {
					candidatesArray[i][k] = new Candidate(x,y,sizes[i]);
					k++;
				}
			}
		}
		
		raw = new int[w*h];	
		buf = new int[w*4];

		enable = false;
		
		currentTier = 0;
		
	}
	
	public void start() {
		enable = true;
		post("FaceDetector enabled");
	}

	public void stop() {
		enable = false;
		post("FaceDetector disabled");
	}

	
	private void detect() {

		int num_faces = 0;

		IntegralImage ii = new IntegralImage(currentFrame);

		for (int i = 0; i < candidatesArray[currentTier].length; i++) {
		
			Candidate c = candidatesArray[currentTier][i];				
			if (cc.processCandidate(c, ii)) {
				
				int updated = 0;          		
				for (int j = 0; j < num_faces; j++) {
					
					Candidate f = facesArray[j];
					if (f.equals(c)) {
						if (c.size > f.size) { 
							f.x = (f.x+c.x)/2;
							f.y = (f.y+c.y)/2;
							f.size = (f.size+c.size)/2;
							f.weight = f.weight + 1;
						}
						else {
							f.x = (f.weight*f.x+c.x)/(f.weight+1);
							f.y = (f.weight*f.y+c.y)/(f.weight+1);
							f.size = (f.weight*f.size+c.size)/(f.weight+1);
							f.weight = f.weight + 1;	
						}
						updated = 1;
						break;
					}
					
				}
				
				if (updated == 0) {
					facesArray[num_faces] = c;
					num_faces++;
				}
			} 
		}

		Atom[] face_array = new Atom[num_faces*4+1];
		
		int k = 0;
		
		face_array[k++] = Atom.newAtom("face");

		for (int i=0;i < num_faces;i++) {
			Candidate c = facesArray[i];
			face_array[k] = Atom.newAtom(c.x * output_width / img_width);
			face_array[k+1] = Atom.newAtom(c.y * output_height / img_height);
			face_array[k+2] = Atom.newAtom(c.size * output_height / img_height);
			face_array[k+3] = Atom.newAtom(0);
			k += 4;
		}

		outlet(0,face_array);

	}
	

	public void jit_matrix(String s) {
		
		jm.frommatrix(s);

		if (enable) {
			dim = jm.getDim();
			for (int i = 0; i < dim[1]; i++) {
				offset[1] = i;
				jm.copyVectorToArray(0, offset, buf, dim[0]*4, 0);

				/* convert rgb to grayscale */
				for (int j = 0; j < dim[0]; j++) {
					raw[i*img_width+j] = (int)(0.3*buf[j*4+1]+0.59*buf[j*4+2]+0.11*buf[j*4+3]);
				}
			}

			currentFrame = new YUVImage(img_width,img_height,raw);
			
			detect();
			
		}
		
	}
	
}
