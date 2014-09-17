package facedetector;

import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;

import javax.imageio.ImageIO;

public class FaceExtractImage {

	// helper class
	public static class Example {
		public boolean label;
		public int x, y, size;

		public Example(boolean l, int tx, int ty, int tsize) {
			label = l;
			x = tx;
			y = ty;
			size = tsize;
		}
	}
		
	/* variables for XML parser */
	private static String DN_FILE_EXTENSION = ".bix";
	private static ArrayList<Example> examples;
	private static int parse_state, top_x, top_y;
	//private static boolean isFace;
	private static int nRandomExample = 0;
	private static int minRandomSize = 32;
	private static int maxRandomSize = 216;
	
	
	/**
	 * Parse DN XML format line by line 
	 * @param s  line from DN XML
	 * @return 0
	 */
	private static int parseData_DN(String s) {
		s = s.trim();
		// State 0 - begin parsing
		if (parse_state == 0) {
			if (s.contains("<gobject type=\"square\"")) {
				parse_state = 2;
			}
		}
		// State 1 - label
		/*
		else if (parse_state == 1) {
			if (s.contains("<tag value=\"correct\"")) {
				isFace = true;
				parse_state++;
			}
			else if (s.contains("<tag value=\"incorrect\"")) {
				isFace = false;
				parse_state++;
			}
		}
		*/
		// State 2 - top left coord
		else if (parse_state == 2) {
			if (s.contains("<vertex x=\"")) {
				String temp[] = s.split("\"");
				if (temp.length != 11) return -1;
				top_x = Math.round(Float.valueOf(temp[1]));
				top_y = Math.round(Float.valueOf(temp[3]));
				parse_state++;
			}
		}
		// State 3 - bottom right coord
		else if (parse_state == 3) {
			if (s.contains("<vertex x=\"")) {
				String temp[] = s.split("\"");
				if (temp.length != 11) return -1;
				int tmp_x = Math.round(Float.valueOf(temp[1]));
				int size = Math.abs(tmp_x - top_x);
				//examples.add(new Example(isFace, top_x, top_y, size));
				int new_size = size * 2;
				int new_top_x = top_x - size/2;
				int new_top_y = top_y - size/2;
				examples.add(new Example(true, new_top_x, new_top_y, new_size));
				parse_state = 0;
			}
		}
		return 0;
	}

	/**
	 * Parsing the annotation of a given file
	 * @param s Image filename
	 * @return 0 = success, -1 = error 
	 */
	private static int readInputFile(String s) {
		s = s + DN_FILE_EXTENSION;
		try {
			BufferedReader in = new BufferedReader(new FileReader(s));
			String str;
			parse_state = 0;
			while ((str = in.readLine()) != null) {
				parseData_DN(str);
			}
			in.close();
		}
		catch (IOException e) {
			e.printStackTrace();
			return -1;
		}
		return 0;
	}
	
	/**
	 * Obtain directory name from a full path
	 * @param s full path
	 * @return directory name or "" on error
	 */
	public static String find_dirname(String s) {
		for (int i = s.length() - 1; i >= 0; i--) {
			if (s.charAt(i) == File.separatorChar) {
				return s.substring(i + 1);
			}
		}
		return "";
	}

	public static void main(String[] args) {

		if (args.length != 1) {
			System.out.println("usage: java Face_ExtractImage <input dir>");
			return;
		}

		String input_dir = args[0];

		// check input directory
		File dir = new File(input_dir);
		if (!dir.exists()) {
			System.out.println(input_dir + " is not found");
			return;
		}

		// filter only png files
		FilenameFilter png_filter = new FilenameFilter() {
			public boolean accept(File dir, String name) {
				return (name.toLowerCase().endsWith(".png") && 
						    !(name.toLowerCase().startsWith("face") ||
						    	name.toLowerCase().startsWith("nonface")));
			}
		};

		// apply filter and sort by file name
		String[] input_files = dir.list(png_filter);
		Arrays.sort(input_files);

		// if no png files, exit 
		if (input_files == null) {
			System.out.println("no PNG files in " + args[1]);
			return;
		}

		examples = new ArrayList<Example>();

		// for each image
		for (int i = 0; i < input_files.length; i++) {

			String filename = input_dir + File.separator + input_files[i];
			System.out.println("Processing " + filename);

			examples.clear();

			if (readInputFile(filename) != 0) {
				System.out.println("error reading xml file: " + 
				                   filename + DN_FILE_EXTENSION);
				//return;
			}

			
			try {

				BufferedImage bimg = ImageIO.read(new File(filename));

				//nRandomExample = examples.size();

				// add random negative detections
				for (int j=0;j<nRandomExample;j++) {
					int size = minRandomSize + 
					(int)((Math.random()*(maxRandomSize-minRandomSize)));
					int x = (int)(Math.random()*(bimg.getWidth()-size));
					int y = (int)(Math.random()*(bimg.getHeight()-size));
					Example e = new Example(false,x,y,size);
					examples.add(e);
				}

				
				// for each example
				for (int k = 0; k < examples.size(); k++) {

					Example ex = examples.get(k);

					// extract images of examples
					BufferedImage img_out =
						bimg.getSubimage(ex.x, ex.y, ex.size, ex.size);
					if (ex.label) 
						ImageIO.write(img_out, "png",
						              new File(input_dir+File.separator+"face_"+input_dir+"_"+k+"_"+(int)(Math.random()*10000)+".png"));
					else 
						ImageIO.write(img_out, "png", 
						              new File(input_dir+File.separator+"nonface_"+input_dir+"_"+k+"_"+(int)(Math.random()*10000)+".png"));

				}

			}
			catch (Exception e) {
				e.printStackTrace();
			}

		}
	}
}
