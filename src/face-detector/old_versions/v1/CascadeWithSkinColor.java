import java.io.File;
import java.util.StringTokenizer;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class CascadeWithSkinColor {

	private Feature[] features;
	private int feature_num;
	private Stage[] stages;
	private int num_stages;
	private static int window_size = 20;

	public void addStage(Stage s) {
		stages[num_stages] = s;
		num_stages++;
	}

	public double[] getFeatureArray(Candidate c, IntegralImage ii, int n) {
		if (n > feature_num)
			n = feature_num;
		double[] feat_array = new double[n];
		for (int i = 0; i < n; i++) {
			Feature f = features[i];
			if (f instanceof RectFeature) {
				RectFeature r = (RectFeature) f;
				feat_array[i] = r.getFeatureValue(c, ii, window_size);
			}
		}
		return feat_array;
	}

	public boolean processCandidate(Candidate c, IntegralImage ii, IntegralImage skin) {
		double acc;
		
		/* fraction of skin in the candidate box */
		double fskin = (double)skin.getSum(c.x, c.y, c.size, c.size)/(c.size * c.size); 
		
		if (fskin < 0.4) return false;
		
		for (int i = 0; i < num_stages; i++) {
			Stage s = stages[i];
			acc = 0.0;
			for (int j = 0; j < s.getNumFeatures(); j++) {
				Feature f = s.getFeature(j);
				if (f instanceof RectFeature) {
					RectFeature r = (RectFeature) f;
					acc += r.testCandidate(c, ii, window_size);
				}
			}
			if (acc < s.getThreshold()) {
				return false;
			}
		}
		
		return true;
	}

	public CascadeWithSkinColor(String xml_filename) {

		feature_num = 0;

		try {
			File file = new File(xml_filename);
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(file);
			doc.getDocumentElement().normalize();

			// System.out.println("Root element " +
			// doc.getDocumentElement().getNodeName());
			NodeList hLst = doc
					.getElementsByTagName("haarcascade_frontalface_alt");

			Node hNode = hLst.item(0);
			NodeList cn = hNode.getChildNodes();

			// last one is the stages node
			NodeList stageLst = cn.item(3).getChildNodes();

			stages = new Stage[stageLst.getLength()];

			// get each stage
			// System.out.println(stageLst.getLength());
			for (int s = 0; s < stageLst.getLength(); s++) {
				Node snode = stageLst.item(s);
				if (snode.getNodeType() == Node.ELEMENT_NODE) {

					// at each stage, get all trees
					Element e = (Element) snode;
					NodeList treeLst = e.getElementsByTagName("trees");
					NodeList tlst = treeLst.item(0).getChildNodes();

					// create an object for each stage
					Stage curr_stage = new Stage(tlst.getLength());

					// extract info about each tree, then add to current cascade
					// stage
					for (int t = 0; t < tlst.getLength(); t++) {
						Node tnode = tlst.item(t);
						if (tnode.getNodeType() == Node.ELEMENT_NODE) {

							Element fe = (Element) tnode;
							// System.out.println(tnode.getNodeName());

							// get the feature elements, ie-rect info
							NodeList fLst = fe.getElementsByTagName("feature")
									.item(0).getChildNodes();

							RectFeature rf = new RectFeature(fLst.getLength());

							for (int jj = 0; jj < fLst.getLength(); jj++) {
								Node nn = fLst.item(jj);
								if (nn.getNodeType() == Node.ELEMENT_NODE) {

									// System.out.println(nn.getNodeName());

									if (nn.getNodeName() == "rects") {
										NodeList rcoords = nn.getChildNodes();

										for (int rc = 0; rc < rcoords
												.getLength(); rc++) {
											// extract rect info from xml file
											if (rcoords.item(rc).getNodeType() == Node.ELEMENT_NODE) {
												NodeList nvalList = rcoords
														.item(rc)
														.getChildNodes();

												// tokenize string for rect info
												String coord_str = nvalList
														.item(0).getNodeValue();
												StringTokenizer st = new StringTokenizer(
														coord_str);

												int x = Integer.valueOf(st
														.nextToken());
												int y = Integer.valueOf(st
														.nextToken());
												int w = Integer.valueOf(st
														.nextToken());
												int h = Integer.valueOf(st
														.nextToken());
												String str = st.nextToken();

												// remove the '.' at the end of
												// the string
												int v = Integer.valueOf(str
														.substring(0, str
																.length() - 1));

												// add coordinate to feature
												// object
												rf.addCoordinate(x, y, w, h, v);
											}
										}
									}
								}
							}

							// get the threshold
							NodeList threshLst = fe
									.getElementsByTagName("threshold");
							Node nn = threshLst.item(0);
							NodeList nv = nn.getChildNodes();
							double thresh = Double.valueOf(
									nv.item(0).getNodeValue()).doubleValue();

							// get the lvalue
							NodeList lLst = fe.getElementsByTagName("left_val");
							nn = lLst.item(0);
							nv = nn.getChildNodes();
							double lval = Double.valueOf(
									nv.item(0).getNodeValue()).doubleValue();

							// get the rvalue
							NodeList rLst = fe
									.getElementsByTagName("right_val");
							nn = rLst.item(0);
							nv = nn.getChildNodes();
							double rval = Double.valueOf(
									nv.item(0).getNodeValue()).doubleValue();

							// create and add feature to current stage
							rf.setThreshold(thresh);
							rf.setLeftValue(lval);
							rf.setRightValue(rval);
							// rf.printFeature();

							// add feature to current stage
							curr_stage.addFeature(rf);
							feature_num++;
						}
					}

					// extract and add stage threshold ot stage object
					NodeList stLst = e.getElementsByTagName("stage_threshold");
					NodeList stlst = stLst.item(0).getChildNodes();
					double stage_thresh = Double.valueOf(
							stlst.item(0).getNodeValue()).doubleValue();
					curr_stage.setThreshold(stage_thresh);

					// add stage to Cascade
					// myCascade.addStage(curr_stage);
					this.addStage(curr_stage);
				}
			}

			features = new Feature[feature_num];
			int k = 0;
			for (int i = 0; i < num_stages; i++) {
				Stage s = stages[i];
				for (int j = 0; j < s.getNumFeatures(); j++) {
					features[k] = s.getFeature(j);
					k++;
				}
			}

		} catch (Exception e) {
			e.printStackTrace();
		}
	}
}
