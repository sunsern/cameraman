import gray.Gray_Feature;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Date;
import java.util.Stack;

import shared.RootFeature;
import skin.Skin_Feature;

import HOG.HOG_Feature;

public class Face_CreateXML {
	
	private Node[] nodes;
	private Stack<ParentInfo> parent;

	public Face_CreateXML(String tree_filename) throws IOException {
		 
		// make a stack
		parent = new Stack<ParentInfo>();
		
		// count number of rules in tree file
		int numRules = getNumRules(tree_filename);

		// allocate according to the number of rules
		nodes = new Node[numRules + 1];
		
		// create tree of rules
		createTree(tree_filename);
		
		
	}
	
	private static String getFeatureType(String s) {
		if (s.contains(HOG_Feature.NAME)) return HOG_Feature.NAME;
		else if (s.contains(Skin_Feature.NAME)) return Skin_Feature.NAME;
		else if (s.contains(Gray_Feature.NAME)) return Gray_Feature.NAME;
		else if (s.contains(RootFeature.NAME)) return RootFeature.NAME;
		else return "UNKNOWN_TYPE: " + s;
	}
	
	private static int getFeatureIndex(String s) {
		if (s.contains(RootFeature.NAME))
			return -1;
		else {
			// find 1st digit
			int k = -1;
			for (int i=0;i<s.length();i++) {
				if (Character.isDigit(s.charAt(i))) { 
					k = i;
					break;
				}
			}
			if (k != -1) return Integer.valueOf(s.substring(k));
			else return -1;
		}
	}

	private static int getNumRules(String tree_file_name) throws IOException {
		int count = 0;
		BufferedReader tree_in = new BufferedReader(new FileReader(tree_file_name));
		String aline = tree_in.readLine();
		while (aline != null && aline.length() > 0) {
			String[] input_line = aline.split("\\s");
			if (input_line[2].startsWith("Splitter")) {
				count++;
			}
			aline = tree_in.readLine();
		}
		tree_in.close();
		return count;
	}

	public void createTree(String tree_file_name) throws IOException {

		int findex;
		int current_d = -1, previous_d = 0;
		ParentInfo current_parent = null;
		String ftype;

		BufferedReader tree_in = new BufferedReader(new FileReader(tree_file_name));
		String aline = tree_in.readLine();

		// initialize the tree
		parent.push(new ParentInfo(0, 1));
		nodes[0] = new Node(0, -1, -1, 1, 0.0, RootFeature.NAME);

		while (aline != null && aline.length() > 0) {

			String[] input_line = aline.split("\\s");
			String[] tree_pos = input_line[1].split(":");

			int boosting_num = Integer.parseInt(input_line[0]);

			previous_d = current_d;

			if (tree_pos != null) current_d = tree_pos.length - 1;
			else current_d = 0;

			if (input_line[2].startsWith("Splitter")) {

				for (int j = previous_d; j > current_d; j--) {
					current_parent = parent.pop();
				}

				ftype = getFeatureType(input_line[5]);

				current_parent = parent.peek();

				if (ftype.compareTo(Skin_Feature.NAME) == 0) {
					findex = getFeatureIndex(input_line[5]);
					if (findex == -1) {
						throw new RuntimeException("Error resolving feature index");
					}
					nodes[boosting_num] =
						new Node(boosting_num, current_parent.id, current_parent.cond,
						         findex, Double.parseDouble(input_line[7]), Skin_Feature.NAME);
				}
				else if (ftype.compareTo(Gray_Feature.NAME) == 0) {
					findex = getFeatureIndex(input_line[5]);
					if (findex == -1) {
						throw new RuntimeException("Error resolving feature index");
					}
					nodes[boosting_num] =
						new Node(boosting_num, current_parent.id, current_parent.cond,
						         findex, Double.parseDouble(input_line[7]), Gray_Feature.NAME);
				}
				else if (ftype.compareTo(HOG_Feature.NAME) == 0) {
					findex = getFeatureIndex(input_line[5]);
					if (findex == -1) {
						throw new RuntimeException("Error resolving feature index");
					}
					nodes[boosting_num] =
						new Node(boosting_num, current_parent.id, current_parent.cond,
						         findex, Double.parseDouble(input_line[7]), HOG_Feature.NAME);
				}
				else {
					throw new RuntimeException("Error resolving feature type" + ftype);
				}

			}

			else if (input_line[2].startsWith("prediction")) {
				if (current_d > previous_d) {
					nodes[boosting_num].cond_true_value =
						Double.parseDouble(input_line[6]);
					parent.push(new ParentInfo(boosting_num, 1));
				}
				else {
					for (int j = previous_d; j >= current_d; j--) {
						current_parent = parent.pop();
					}
					nodes[boosting_num].cond_false_value =
						Double.parseDouble(input_line[6]);
					parent.push(new ParentInfo(boosting_num, -1));
				}
			}

			aline = tree_in.readLine();
		}

		for (int j = current_d; j > 0; j--) {
			current_parent = parent.pop();
		}

		tree_in.close();

	}

	public void printXML(BufferedWriter bw) throws IOException {
		
		// Header
		bw.write("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
		bw.write("<face_detector>\n");
		Date now = new Date();
		bw.write("<generate_time>" + now + "</generate_time>\n");
		
		// Body
		bw.write("<stages>\n");
		bw.write("\t<stage id=\"0\">\n");
		bw.write("\t\t<features>\n");
		bw.write("\t\t\t<feature id=\"0\" type=\"root\">\n");
		bw.write("\t\t\t\t<cond_true_val>" + nodes[0].cond_true_value
							+ "</cond_true_val>\n");
		bw.write("\t\t\t</feature>\n");
		for (int i = 1; i < nodes.length; i++) {
			if (nodes[i].type.compareTo(HOG_Feature.NAME) == 0) {
				bw.write("\t\t\t<feature id=\"" + i + "\" type=\"" + HOG_Feature.NAME + "\">\n");
				bw.write("\t\t\t\t<_>" + nodes[i].findex + "</_>\n");	
			}
			else if (nodes[i].type.compareTo(Skin_Feature.NAME) == 0) {
				bw.write("\t\t\t<feature id=\"" + i + "\" type=\"" + Skin_Feature.NAME + "\">\n");
				bw.write("\t\t\t\t<_>" + nodes[i].findex + "</_>\n");
			}
			else if (nodes[i].type.compareTo(Gray_Feature.NAME) == 0) {
				bw.write("\t\t\t<feature id=\"" + i + "\" type=\"" + Gray_Feature.NAME + "\">\n");
				bw.write("\t\t\t\t<_>" + nodes[i].findex + "</_>\n");
			}
			
			bw.write("\t\t\t\t<threshold>" + nodes[i].threshold + "</threshold>\n");
			bw.write("\t\t\t\t<cond_true_val>" + nodes[i].cond_true_value
								+ "</cond_true_val>\n");
			bw.write("\t\t\t\t<cond_false_val>" + nodes[i].cond_false_value
								+ "</cond_false_val>\n");
			bw.write("\t\t\t\t<parent>" + nodes[i].parent_id + "</parent>\n");
			bw.write("\t\t\t\t<parent_cond>" + nodes[i].parent_cond
								+ "</parent_cond>\n");
			bw.write("\t\t\t</feature>\n");
		}
		bw.write("\t\t</features>\n");
		bw.write("\t\t<stage_threshold>0.0</stage_threshold>\n");
		bw.write("\t</stage>\n");
		bw.write("</stages>\n");
		
		// Footer
		bw.write("</face_detector>\n");
	}

	public static void main(String args[]) {

		BufferedWriter xml_out;

		if (args.length != 2) {
			System.out.
				println("usage: java Face_CreateXML <tree-filename> <new XML>");
			return;
		}

		String tree_filename = args[0];
		String xml_name      = args[1];
	
		try {
			Face_CreateXML fc = new Face_CreateXML(tree_filename);
			xml_out = new BufferedWriter(new FileWriter(xml_name));
			fc.printXML(xml_out);
			xml_out.close();
		}
		catch (IOException e) {
			e.printStackTrace();
		}
	}

	
	// helper classes 
	
	private class ParentInfo {
		public int id, cond;

		public ParentInfo(int id, int cond) {
			this.id = id;
			this.cond = cond;
		}
	}

	private class Node {
		public int id, parent_id, parent_cond, findex;
		public double threshold, cond_true_value, cond_false_value;
		public String type; 

		public Node(int n, int p_id, int p_cond, int fidx, double t, String ntype) {
			id = n;
			parent_id = p_id;
			parent_cond = p_cond;
			findex = fidx;
			threshold = t;
			type = ntype;
		}
	}

	
}
