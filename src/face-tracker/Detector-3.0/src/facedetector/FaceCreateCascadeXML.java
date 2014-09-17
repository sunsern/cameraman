package facedetector;
import features.AbstractFeature;
import features.GrayFeature;
import features.HogFeature;
import features.HsvSkinFeature;
import features.SkinFeature;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Date;

import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import shared.*;

/**
 * This class generate a cascade version of 
 * input XML file according to a cascade file.
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class FaceCreateCascadeXML {

	private static final int MAX_STAGES = 100;

	private Stage[] stages;
	
	private AbstractFeature[] features;
	private int num_features;
	
	
	private double[] new_stage_thres;
	private int[] new_stage_id;
	private int new_num_stages;
	
	public FaceCreateCascadeXML(String xml_in, String cascade) throws NumberFormatException, IOException, ParserConfigurationException, SAXException {
		
		// load XML
		FaceScorer fs = new FaceScorer(xml_in);
		stages = fs.stages;
		num_features = fs.num_features;
		
		// load cascade
		new_stage_thres = new double[MAX_STAGES];
		new_stage_id = new int[MAX_STAGES];
		new_num_stages = 0;
		BufferedReader br = new BufferedReader(new FileReader(cascade));
		String str;
		while ((str = br.readLine()) != null) {
			/* if not an empty line */
			if (str.length() > 1) {
				String[] items = str.split("\\s");
				int stage_id = Integer.valueOf(items[0]);
				double stage_thres = Double.valueOf(items[1]);
				new_stage_thres[new_num_stages] = stage_thres;
				new_stage_id[new_num_stages++] = stage_id;
			}
		}
		br.close();
		
	}

	public void printXML(BufferedWriter bw) throws IOException {

		int curr_feature = 0;
		int curr_stage = 0;
		
		// create a vector of features
		features = new AbstractFeature[num_features];
		for (int i = 0; i < stages.length; i++) {
			Stage s = stages[i];
			for (int j = 0; j < s.getNumFeatures(); j++) {
				features[curr_feature++] = s.getFeature(j);
			}
		}

		// Header
		bw.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		bw.write("<face_detector>\n");
		Date now = new Date();
		bw.write("<generate_time>" + now + "</generate_time>\n");
	
		// Rules
		bw.write("<stages>\n");
		bw.write("\t<stage id=\"0\">\n");
		bw.write("\t\t<features>\n");
		bw.write("\t\t\t<feature id=\"0\" type=\"root\">\n");
		bw.write("\t\t\t\t<cond_true_val>" + features[0].getLeftValue()
				+ "</cond_true_val>\n");
		bw.write("\t\t\t</feature>\n");

		curr_feature = 1;

		while (curr_feature < num_features) {
			if (curr_feature == new_stage_id[curr_stage]) {
				bw.write("\t\t</features>\n");
				bw.write("\t\t<stage_threshold>" + new_stage_thres[curr_stage]
						+ "</stage_threshold>\n");
				bw.write("\t</stage>\n");
				bw.write("\t<stage id=\"" + (curr_stage + 1) + "\">\n");
				bw.write("\t\t<features>\n");
				curr_stage++;
			}
			AbstractFeature f = features[curr_feature];
			if (f instanceof HogFeature) {
				HogFeature hf = (HogFeature)f;
				bw.write("\t\t\t<feature id=\"" + curr_feature + "\" type=\"" + HogFeature.NAME + "\">\n");
				bw.write("\t\t\t\t<_>" + hf.hogindex + "</_>\n");	
			}
			else if (f instanceof SkinFeature) {
				SkinFeature sf = (SkinFeature)f;
				bw.write("\t\t\t<feature id=\"" + curr_feature + "\" type=\"" + SkinFeature.NAME + "\">\n");
				bw.write("\t\t\t\t<_>" + sf.skinindex + "</_>\n");
			}
			else if (f instanceof HsvSkinFeature) {
				HsvSkinFeature hsvskinf = (HsvSkinFeature)f;
				bw.write("\t\t\t<feature id=\"" + curr_feature + "\" type=\"" + HsvSkinFeature.NAME + "\">\n");
				bw.write("\t\t\t\t<_>" + hsvskinf.hsvskinindex + "</_>\n");
			}
			else if (f instanceof GrayFeature) {
				GrayFeature gf = (GrayFeature)f;
				bw.write("\t\t\t<feature id=\"" + curr_feature + "\" type=\"" + GrayFeature.NAME + "\">\n");
				bw.write("\t\t\t\t<_>" + gf.grayindex + "</_>\n");
			}
			bw.write("\t\t\t\t<threshold>" + f.getThreshold() + "</threshold>\n");
			bw.write("\t\t\t\t<cond_true_val>" + f.getLeftValue()
								+ "</cond_true_val>\n");
			bw.write("\t\t\t\t<cond_false_val>" + f.getRightValue()
								+ "</cond_false_val>\n");
			bw.write("\t\t\t\t<parent>" + f.getParent() + "</parent>\n");
			bw.write("\t\t\t\t<parent_cond>" + f.getParentCondition()
								+ "</parent_cond>\n");
			bw.write("\t\t\t</feature>\n");

			curr_feature++;

		}
		bw.write("\t\t</features>\n");
		bw.write("\t\t<stage_threshold>0.0</stage_threshold>\n");
		bw.write("\t</stage>\n");
		bw.write("</stages>\n");
		
		// Footer
		bw.write("</face_detector>\n");
	}

	public static void main(String[] args) {

		if (args.length != 3) {
			System.out.println("Usage: Face_CreateCascadeXML <input-xml> <cascade-file> <output-xml>");
			return;
		}

		String xml_in  = args[0];
		String cascade = args[1];
		String xml_out = args[2];

		try {
			FaceCreateCascadeXML fcc = new FaceCreateCascadeXML(xml_in,cascade);
			BufferedWriter bw = new BufferedWriter(new FileWriter(xml_out));
			fcc.printXML(bw);
			bw.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}