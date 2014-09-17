package facedetector;

import features.GrayFeature;
import features.GrayIntegralImage;
import features.HogFeature;
import features.HogIntegralImage;
import features.HsvSkinFeature;
import features.SkinFeature;
import features.HsvSkinIntegralImage;
import features.SkinIntegralImage;

import java.awt.image.BufferedImage;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.FilenameFilter;
import java.util.ArrayList;
import java.util.Arrays;

import javax.imageio.ImageIO;

import shared.Candidate;
import shared.Util;
import skindetector.SkinDetector;


public class FaceExtractFeature {

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
	
	// output setting
	private static boolean outputHsvSkin = true;
	//private static boolean outputSkin = true;
	private static boolean outputHOG  = true;
	private static boolean outputGrey = false;
	
	// skin color setting
	//private static String SKIN_XML = "/home/scheaman/workspace/NewFaceDetector/xml/" +
	//		"skin-Nov18-newHSV-cascade.xml";
	
	private static ArrayList<Example> examples;

	/**
	 * Obtain directory name from a full path
	 * @param s full path
	 * @return directory name or s
	 */
	public static String find_dirname(String s) {
		for (int i = s.length() - 1; i >= 0; i--) {
			if (s.charAt(i) == File.separatorChar) {
				return s.substring(i + 1);
			}
		}
		return s;
	}

	public static void main(String[] args) {

		try {
			
			int face = 0, nonface = 0;
			
			if (args.length != 1) {
				System.out.println("usage: java Face_ExtractFeature <input dir>");
				return;
			}

			String input_dir = args[0];
			String dirname   = find_dirname(input_dir);
			
			// check input directory
			File dir = new File(input_dir);
			if (!dir.exists()) {
				System.out.println(input_dir + " is not found");
				return;
			}

			// filter only extracted face images
			FilenameFilter png_filter = new FilenameFilter() {
				public boolean accept(File dir, String name) {
					return (name.toLowerCase().endsWith(".png") && 
							(name.toLowerCase().startsWith("face") ||
									name.toLowerCase().startsWith("nonface")));
				}
			};

			String[] input_files = dir.list(png_filter);

			if (input_files == null) {
				System.out.println("no PNG files in " + args[1]);
				return;
			}

			Arrays.sort(input_files);

			
			examples = new ArrayList<Example>();

			BufferedWriter fout_data =
				new BufferedWriter(new FileWriter(dirname + ".data"));
			BufferedWriter fout_spec =
				new BufferedWriter(new FileWriter(dirname + ".spec"));

			fout_spec.write("exampleTerminator=;\n" + 
			                "attributeTerminator=,\n"	+ 
			                "maxBadExa=0\n");

			fout_spec.write("labels (nonface,face)\n");

			if (outputHsvSkin) {
				for (int j = 0; j < HsvSkinFeature.nFeature; j++) {
					fout_spec.write(HsvSkinFeature.NAME + j + " \t number\n");
				}
			}
//			if (outputSkin) {
//				for (int j = 0; j < SkinFeature.nFeature; j++) {
//					fout_spec.write(SkinFeature.NAME + j + " \t number\n");
//				}
//			}
			if (outputHOG) {
				for (int j = 0; j < HogFeature.nFeature; j++) {
					fout_spec.write(HogFeature.NAME + j + " \t number\n");
				}
			}
			if (outputGrey) {
				for (int j = 0; j < GrayFeature.nFeature; j++) {
					fout_spec.write(GrayFeature.NAME + j + " \t number\n");
				}
			}
			
			// for each image
			for (int i = 0; i < input_files.length; i++) {

				String filename = input_dir + File.separator + input_files[i];
				System.out.println("Processing " + filename);

				examples.clear();

				BufferedImage bimg = ImageIO.read(new File(filename));

				// get HSV
				int[] packedRgbs = new int[bimg.getWidth() * bimg.getHeight()];
				bimg.getRGB(0, 0, bimg.getWidth(), bimg.getHeight(), packedRgbs, 0,
				            bimg.getWidth());

				int l = bimg.getWidth() * bimg.getHeight();
				int[] greyvalue = new int[l];
				double[] hsv = new double[l * 3];
				double[] temp_hsv = new double[3];
				int[] rgbs = new int[l * 3];
				
				for (int j = 0; j < l; j++) {
					int b = packedRgbs[j] & 0xff;
					int g = (packedRgbs[j] >> 8) & 0xff;
					int r = (packedRgbs[j] >> 16) & 0xff;

					Util.rgb2hsv(r, g, b, temp_hsv);

					rgbs[j*3]     = r;
					rgbs[j*3 + 1] = g;
					rgbs[j*3 + 2] = b;
					
					hsv[j*3]     = temp_hsv[0];
					hsv[j*3 + 1] = temp_hsv[1];
					hsv[j*3 + 2] = temp_hsv[2];

					greyvalue[j] = (int)(0.3d*r + 0.59d*g + 0.11d*b);
				}

				
				// detect skin
				//SkinDetector sd = new SkinDetector(SKIN_XML);
				//double[] skinScore = sd.getScoreVector(hsv, bimg.getWidth(), bimg.getHeight());
				
				// integral SkinHSV
				HsvSkinIntegralImage hsvskinii = 
					new HsvSkinIntegralImage(hsv,bimg.getWidth(), bimg.getHeight());
				// integral image for skin
				//SkinIntegralImage skinii =
				//	new SkinIntegralImage(skinScore, bimg.getWidth(), bimg.getHeight());
				// integral image for HOG
				HogIntegralImage hogii = HogFeature.getHOGIntegralImage(rgbs, bimg.getWidth(), bimg.getHeight());
				// integral image for Grey
				GrayIntegralImage greyii = new GrayIntegralImage(greyvalue, bimg.getWidth(), bimg.getHeight());

				// add 1 example which is the entire image
				if (input_files[i].toLowerCase().startsWith("face"))
					examples.add(new Example(true,0,0,Math.min(bimg.getWidth(), bimg.getHeight())));
				else
					examples.add(new Example(false,0,0,Math.min(bimg.getWidth(), bimg.getHeight())));

				// for each example
				for (int k = 0; k < examples.size(); k++) {

					Example ex = examples.get(k);
					Candidate c = new Candidate(ex.x, ex.y, ex.size);

					// write label
					if (ex.label) { 
						fout_data.write("face");
							face++;
					}
					else {
						fout_data.write("nonface");
						nonface++;
					}

					// output skin feature
//					if (outputSkin) {
//						double[] fv = SkinFeature.getSkinFeatureVector(c, skinii);
//						for (int j = 0; j < fv.length; j++) {
//							fout_data.write(", " + fv[j]);
//						}
//					}

					if (outputHsvSkin) {
						double[] fv = HsvSkinFeature.getSkinFeatureVector(c, hsvskinii);
						for (int j = 0; j < fv.length; j++) {
							fout_data.write(", " + fv[j]);
						}
					}
					
				  // output HOG features
					if (outputHOG) {
						double[] fv = HogFeature.getHOGFeatureVector(c, hogii);
						for (int j = 0; j < fv.length; j++) {
							fout_data.write(", " + fv[j]);
						}
					}

					// output Gray features
					if (outputGrey) {
						double[] fv = GrayFeature.getGreyFeatureVector(c, greyii);
						for (int j = 0; j < fv.length; j++) {
							fout_data.write(", " + fv[j]);
						}
					}

					fout_data.write("; /* "+ filename + " */ \n");
				}				
			}
			fout_data.close();
			fout_spec.close();
			
			System.out.println("face: " + face);
			System.out.println("nonface: " + nonface);
			
		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
}
