package facedetector;
import java.awt.*;
import java.awt.event.KeyEvent;
import java.awt.event.KeyListener;
import java.awt.image.BufferedImage;

import javax.imageio.ImageIO;
import javax.swing.*;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.Arrays;

public class FaceAnnotate extends JComponent implements KeyListener {

  /**
	 * 
	 */
	private static final long serialVersionUID = 5064999726422136154L;
	
	
	protected BufferedImage im;
  protected int width = 100, height=100;
  protected Container cp;

  
  protected String path;
  protected String[] filenames;
  protected int[] labels;
  protected int current_idx;
  
  protected Label lb;
  
  private static int W = 120;
  private static int H = 120;
  
  
  /** Construct an ImageView viewer, given a filename. */
  public FaceAnnotate(String input_dir) {
  
  	FilenameFilter filter = new FilenameFilter() {
			public boolean accept(File dir, String name) {
				return (name.toLowerCase().endsWith(".png"));
			}
  	};
  	
  	this.path = input_dir;

		File dir = new File(input_dir);
		if (!dir.exists()) {
			System.out.println("Error: cannot find directory " + input_dir);
			throw new RuntimeException();
		}

		// create a file list
		filenames = dir.list(filter);
		
		if (filenames == null) {
			System.out.println("Error: make sure " + input_dir + " contains png files");
			throw new RuntimeException();
		}
		
		// sort by file name
		Arrays.sort(filenames);

		labels = new int[filenames.length];
		current_idx = 0;
		
		// load labels
		
		try {
			BufferedReader br = new BufferedReader(new FileReader(path + File.separator + "labels.txt"));
			String line = br.readLine();  
			while (line != null) {
				String[] temp = line.split(" ");
				for (int i=0;i < filenames.length;i++) { 
					if (temp[0].compareTo(filenames[i]) == 0) {
						labels[i] = Integer.valueOf(temp[1]);
					}
				}
				line = br.readLine();
			}
			br.close();
		}
		catch (Exception e) {
			System.out.println("no label file");
		}
		
		

		cp = this;
    
  }

  public void paint(Graphics g) {

  	Graphics2D g2 = (Graphics2D)g;
  	int newW = Math.min(this.getWidth(),this.getHeight());
  	int newH = newW;
  	g2.setRenderingHint(RenderingHints.KEY_INTERPOLATION,
  	                    RenderingHints.VALUE_INTERPOLATION_BILINEAR);  
    g2.drawImage(im, 0, 0, newW, newH, this);
    
  }

  public static void main(String[] arg) {
  	if (arg.length == 0) {
  		System.err.println("Usage: Face_Annotate <dir>");
  	} else {

  		FaceAnnotate fa = new FaceAnnotate(arg[0]);
  		fa.lb = new Label("");
    	
  		JFrame jf = new JFrame("Face Annotator: ");
  		  		
  		jf.addKeyListener(fa);
  		jf.setMinimumSize(new Dimension(W,H));
  		jf.setLocation(500, 200);
  	
  		//jf.setLayout(new FlowLayout(FlowLayout.CENTER));
  		
  		
  		jf.getContentPane().add(fa);
  		
  		//jf.getContentPane().add(fa.lb);
			jf.setSize(fa.getSize());
			
			fa.loadImage(0);
			jf.setVisible(true);

  	}
  }

  public void loadImage(int i) {
  	try {
  		System.out.println("Filename: " + filenames[i]);
  		
			im = ImageIO.read(new File(path + File.separator +
			                           filenames[i]));
			if (labels[i]==1) {
				System.out.println("label: FACE");
				lb.setText("FACE");
			}
			else if (labels[i]==-1) {
				System.out.println("label: NONFACE");
				lb.setText("NONFACE");
			} 
			else {
				System.out.println("label: ");
				lb.setText("");
			}
			
			
			int rgbs[] = new int[im.getWidth() * im.getHeight()];
			im.getRGB(0, 0, im.getWidth(), im.getHeight(), rgbs, 0,
									im.getWidth());
			
			
			int size = im.getWidth();
			int x = size / 4;
			int y = size / 4;

   		// add to it a red box
			for (int yy=y;yy < y+size/2; yy++) {
				for (int xx=x;xx < x+size/2; xx++) {
					if (yy==y || xx==x || yy==y+size/2-1 || xx==x+size/2-1){
					int org = rgbs[yy*im.getWidth()+xx];
					int b = org & 0xff;
					int g = (org >> 8) & 0xff;
					int r = (org >> 16) & 0xff;

					r = 255 & 0xff; 

					rgbs[yy*im.getWidth()+xx] = ((r<<16) | (g<<8) | b);
					}
				}
			}


			int box1_x = 2 * size / 8;
			int box1_y = 3 * size / 8;
			int box1_w = size / 2;
			int box1_h = size / 4;
			
			int box2_x = box1_x;
			int box2_y = box1_y + size / 16;
			int box2_w = 3 * size /16;
			int box2_h = size / 8;
			
			int box3_x = box1_x + size / 4 + size / 16;
			int box3_y = box2_y;
			int box3_w = 3 * size /16;
			int box3_h = size / 8;
			
  		// draw eye
			for (int yy=box1_y;yy < box1_y+box1_h; yy++) {
				for (int xx=box1_x;xx < box1_x+box1_w; xx++) {
					int org = rgbs[yy*im.getWidth()+xx];
					int b = org & 0xff;
					int g = (org >> 8) & 0xff;
					int r = (org >> 16) & 0xff;

					b = 255 & 0xff;

					rgbs[yy*im.getWidth()+xx] = ((r<<16) | (g<<8) | b);
				}
			}

			
			for (int yy=box2_y;yy < box2_y+box2_h; yy++) {
				for (int xx=box2_x;xx < box2_x+box2_w; xx++) {
					int org = rgbs[yy*im.getWidth()+xx];
					int b = org & 0xff;
					int g = (org >> 8) & 0xff;
					int r = (org >> 16) & 0xff;

					r = 255 & 0xff; 

					rgbs[yy*im.getWidth()+xx] = ((r<<16) | (g<<8) | b);
				}
			}

			for (int yy=box3_y;yy < box3_y+box3_h; yy++) {
				for (int xx=box3_x;xx < box3_x+box3_w; xx++) {
					int org = rgbs[yy*im.getWidth()+xx];
					int b = org & 0xff;
					int g = (org >> 8) & 0xff;
					int r = (org >> 16) & 0xff;

					r = 255 & 0xff; 
					
					rgbs[yy*im.getWidth()+xx] = ((r<<16) | (g<<8) | b);
				}
			}

			
			im.setRGB(0, 0, im.getWidth(), im.getHeight(), rgbs, 0, im.getWidth());
			
			this.repaint();
		}
		catch (IOException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		}
  }


  public void nextImage() {

		if (current_idx+1 < filenames.length) { 
			current_idx++; 
			loadImage(current_idx);
		}
  }
  
	public void previousImage() {
		
		
		if (current_idx > 0) { 
			current_idx--; 
			loadImage(current_idx);
		}
		
	}

	public void mark(boolean label) {
		if (label) {
			labels[current_idx] = 1;
		}
		else {
			labels[current_idx] = -1;
		}
		nextImage();
	}
	
	public void quit() {
		// save labels
		try {
			BufferedWriter bw = new BufferedWriter(new FileWriter(path + File.separator +
			                                                      "labels.txt"));
			for (int i=0;i<labels.length;i++) {
				bw.write(filenames[i] + " " + labels[i]+ "\n");
			}
			bw.close();
		}
		catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		
		
		System.exit(0);
	}
	
    //@Override
	public void keyPressed(KeyEvent e) {

		switch (e.getKeyCode()) {
			case KeyEvent.VK_LEFT:
				previousImage();
				break;
			case KeyEvent.VK_RIGHT:
				nextImage();
				break;
			case KeyEvent.VK_Y:
				mark(true);
				break;
			case KeyEvent.VK_N:
				mark(false);
				break;
			case KeyEvent.VK_Q:
				quit();
				break;
				
		}

	}

    //@Override
	public void keyReleased(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}

    //@Override
	public void keyTyped(KeyEvent e) {
		// TODO Auto-generated method stub
		
	}
}
