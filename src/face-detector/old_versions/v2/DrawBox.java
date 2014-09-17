import com.cycling74.max.*;
import com.cycling74.jitter.*;

public class DrawBox extends MaxObject {
    
    private JitterMatrix jm = new JitterMatrix();
    private int[] border_color = new int[] {255,255,255,0};
    private int[] offset = new int[] {0,0};
    private int input_w, input_h, line_width;
    
    
    private void drawFace(int tx,int ty,int tsize) {
    	int[] dim = jm.getDim();
    	int planecount = jm.getPlanecount();
    	int[] row = new int[dim[0] * planecount];

    	int x = tx*dim[0]/input_w;
    	int y = ty*dim[1]/input_h;
    	int size = tsize*dim[1]/input_h;

	
       	for (int k=0;k<line_width;k++) {
       		offset[0] = 0;
       		offset[1] = y+k;
       		jm.copyVectorToArray(0, offset, row, dim[0]*planecount, 0);
       		
       		for (int j=0;j<size;j++) {
       			for (int p=0;p<planecount;p++) {
       				row[(x+j)*planecount + p] = border_color[p];
       			}
       		}
       		
       		jm.copyArrayToVector(0, offset, row, dim[0]*planecount, 0);
       	}
	
       	for (int k=0;k<line_width;k++) {
       		offset[0] = 0;
       		offset[1] = y+size-1-k;
       		jm.copyVectorToArray(0, offset, row, dim[0]*planecount, 0);
       		
       		for (int j=0;j<size;j++) {
       			for (int p=0;p<planecount;p++) {
       				row[(x+j)*planecount + p] = border_color[p];
       			}
       		}
       		
       		jm.copyArrayToVector(0, offset, row, dim[0]*planecount, 0);
       	}
       	
	
	for (int i=0;i<size;i++) {
	    offset[0] = 0;
	    offset[1] = y+i;
	    jm.copyVectorToArray(0, offset, row, dim[0]*planecount, 0);
	    
	    for (int k=0;k<line_width;k++) {
		for (int p=0;p<planecount;p++) {
		    row[(x+k)*planecount + p] = border_color[p];
		    row[(x+size-1-k)*planecount + p] = border_color[p];
		    
		}
	    }
       		
	    jm.copyArrayToVector(0, offset, row, dim[0]*planecount, 0);
	}    
    }
    
    public DrawBox(int d_x,int d_y,int l_w ) {
    	line_width = l_w;
    	input_w = d_x;
    	input_h = d_y;
    	declareIO(2,1);
    }
    
    public void jit_matrix(String s) {
    	jm.frommatrix(s);
    }
    
    public void face(Atom[] l) {
    	if (l != null) {
	    int k=0;
	    for (int i=0;i<l.length/4;i++) {
		drawFace(l[k].getInt(),l[k+1].getInt(),l[k+2].getInt());
		k+=4;
	    }
    	}
    	outlet(0,"jit_matrix",jm.getName());
    }
    
    
}
