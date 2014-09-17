import java.io.File;
import java.util.StringTokenizer;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;

public class Cascade {

	private int[] cond;
	private Stage[] stages;
	private int num_stages;
	private int num_features;

	/* TODO: variable window size */
	private static int window_size = 20;

	/**
	 * Add a Stage into this cascade.
	 * 
	 * @param s
	 *          Stage
	 */
	private void addStage(Stage s) {
		stages[num_stages] = s;
		num_stages++;
	}

	public int getNumFeatures() {
		return num_features;
	}

	public double[] getFeatureArray(Candidate c, IntegralImage ii,
																	IntegralImage skin, int max_num_features) {

		if (max_num_features > num_features) max_num_features = num_features;

		double[] feat_array = new double[max_num_features];
		double stdDev = ii.getStdDev(c.x, c.y, c.size);

		int k = 0;
		for (int i = 0; i < num_stages; i++) {
			Stage s = stages[i];
			for (int j = 0; j < s.getNumFeatures(); j++) {
				Feature f = s.getFeature(j);
				if (f.getType().compareTo("root") == 0) {
					/* root has no feature value */
					feat_array[k++] = 0.0;
				}
				else if (f.getType().compareTo("rect") == 0) {
					RectFeature r = (RectFeature) f;
					feat_array[k++] = r.getFeatureValue(c, ii, window_size, stdDev);
				}
				else if (f.getType().compareTo("skincolor") == 0) {
					SkinColorFeature sc = (SkinColorFeature) f;
					feat_array[k++] = sc.getFeatureValue(c, skin);
				}
				if (k >= max_num_features) break;
			}
			if (k >= max_num_features) break;
		}
		return feat_array;
	}

	public boolean processCandidate(Candidate c, IntegralImage ii,
																	IntegralImage skin) {

		double acc;
		double stdDev = ii.getStdDev(c.x, c.y, c.size);

		for (int i = 0; i < num_stages; i++) {
			Stage s = stages[i];
			/* reset score in each stage? */
			acc = 0.0;
			for (int j = 0; j < s.getNumFeatures(); j++) {
				Feature f = s.getFeature(j);
				int fid = f.getFeatureID();
				if (f.getType().compareTo("root") == 0) {
					RootFeature root = (RootFeature) f;
					acc += root.getLeftValue();
					cond[fid] = 1;
				}
				else if (f.getType().compareTo("rect") == 0) {
					RectFeature r = (RectFeature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (r.testCandidate(c, ii, window_size, stdDev)) {
							acc += r.getLeftValue();
							cond[fid] = 1;
						}
						else {
							acc += r.getRightValue();
							cond[fid] = -1;
						}
					}
					else {
						cond[fid] = 0;
					}
				}
				else if (f.getType().compareTo("skincolor") == 0) {
					SkinColorFeature sc = (SkinColorFeature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (sc.testCandidate(c, skin)) {
							acc += sc.getLeftValue();
							cond[fid] = 1;
						}
						else {
							acc += sc.getRightValue();
							cond[fid] = -1;
						}
					}
					else {
						cond[fid] = 0;
					}
				}
			}
			if (acc < s.getThreshold()) {
				return false;
			}
		}
		return true;
	}


	public Cascade(String xml_filename) {

		num_features = 0;
		num_stages = 0;

		try {
			File file = new File(xml_filename);
			DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
			DocumentBuilder db = dbf.newDocumentBuilder();
			Document doc = db.parse(file);
			doc.getDocumentElement().normalize();

			NodeList hLst = doc.getElementsByTagName("face_detector");
			Element hElem = (Element) hLst.item(0);

			/* stages is item 1 */
			Element stagesElem =
					(Element) hElem.getElementsByTagName("stages").item(0);
			NodeList stageLst = stagesElem.getElementsByTagName("stage");

			stages = new Stage[stageLst.getLength()];

			// get each stage
			for (int s = 0; s < stageLst.getLength(); s++) {

				Node snode = stageLst.item(s);

				if (snode.getNodeType() == Node.ELEMENT_NODE) {

					// at each stage, get all features
					Element e = (Element) snode;
					NodeList fcLst = e.getElementsByTagName("features");

					Element fcElem = (Element) fcLst.item(0);
					NodeList fLst = fcElem.getElementsByTagName("feature");

					// create an object for each stage
					Stage curr_stage = new Stage(fLst.getLength());

					// add each feature into the stage
					for (int t = 0; t < fLst.getLength(); t++) {

						Node tnode = fLst.item(t);
						if (tnode.getNodeType() == Node.ELEMENT_NODE) {

							// get feature type
							Element fElem = (Element) tnode;
							String fType = fElem.getAttribute("type");
							int feature_id = Integer.valueOf(fElem.getAttribute("id"));

							if (fType.compareTo("root") == 0) {

								// get the cond_true_val
								Node ctNode =
										fElem.getElementsByTagName("cond_true_val").item(0);
								NodeList ctLst = ctNode.getChildNodes();
								double ct = Double.valueOf(ctLst.item(0).getNodeValue());

								RootFeature rootf = new RootFeature();

								rootf.setType("root");
								rootf.setFeatureID(feature_id);
								rootf.setLeftValue(ct);

								// add feature to current stage
								curr_stage.addFeature(rootf);
								num_features++;

							}
							else if (fType.compareTo("skincolor") == 0) {

								// get the threshold
								Node threshNode =
										fElem.getElementsByTagName("threshold").item(0);
								NodeList threshLst = threshNode.getChildNodes();
								double thresh =
										Double.valueOf(threshLst.item(0).getNodeValue());

								// get the cond_true_val
								Node ctNode =
										fElem.getElementsByTagName("cond_true_val").item(0);
								NodeList ctLst = ctNode.getChildNodes();
								double ct = Double.valueOf(ctLst.item(0).getNodeValue());

								// get the cond_false_val
								Node cfNode =
										fElem.getElementsByTagName("cond_false_val").item(0);
								NodeList cfLst = cfNode.getChildNodes();
								double cf = Double.valueOf(cfLst.item(0).getNodeValue());

								// get the parent
								Node parNode = fElem.getElementsByTagName("parent").item(0);
								NodeList parLst = parNode.getChildNodes();
								int parent = Integer.valueOf(parLst.item(0).getNodeValue());

								// get the parent_cond
								Node parcNode =
										fElem.getElementsByTagName("parent_cond").item(0);
								NodeList parcLst = parcNode.getChildNodes();
								int parent_cond =
										Integer.valueOf(parcLst.item(0).getNodeValue());

								SkinColorFeature scf = new SkinColorFeature();

								scf.setType("skincolor");
								scf.setFeatureID(feature_id);
								scf.setThreshold(thresh);
								scf.setLeftValue(ct);
								scf.setRightValue(cf);
								scf.setParent(parent);
								scf.setParentCondition(parent_cond);

								// add feature to current stage
								curr_stage.addFeature(scf);
								num_features++;

							}
							else if (fType.compareTo("rect") == 0) {

								// get the rects
								Element rectElem =
										(Element) fElem.getElementsByTagName("rects").item(0);
								NodeList rectLst = rectElem.getElementsByTagName("_");

								RectFeature rf = new RectFeature(rectLst.getLength());

								for (int jj = 0; jj < rectLst.getLength(); jj++) {

									String coord_str =
											rectLst.item(jj).getChildNodes().item(0).getNodeValue();
									StringTokenizer st = new StringTokenizer(coord_str);

									int x = Integer.valueOf(st.nextToken());
									int y = Integer.valueOf(st.nextToken());
									int w = Integer.valueOf(st.nextToken());
									int h = Integer.valueOf(st.nextToken());
									double v = Double.valueOf(st.nextToken());

									rf.addCoordinate(x, y, w, h, v);
								}

								// get the threshold
								Node threshNode =
										fElem.getElementsByTagName("threshold").item(0);
								NodeList threshLst = threshNode.getChildNodes();
								double thresh =
										Double.valueOf(threshLst.item(0).getNodeValue());

								// get the cond_true_val
								Node ctNode =
										fElem.getElementsByTagName("cond_true_val").item(0);
								NodeList ctLst = ctNode.getChildNodes();
								double ct = Double.valueOf(ctLst.item(0).getNodeValue());

								// get the cond_false_val
								Node cfNode =
										fElem.getElementsByTagName("cond_false_val").item(0);
								NodeList cfLst = cfNode.getChildNodes();
								double cf = Double.valueOf(cfLst.item(0).getNodeValue());

								// get the parent
								Node parNode = fElem.getElementsByTagName("parent").item(0);
								NodeList parLst = parNode.getChildNodes();
								int parent = Integer.valueOf(parLst.item(0).getNodeValue());

								// get the parent_cond
								Node parcNode =
										fElem.getElementsByTagName("parent_cond").item(0);
								NodeList parcLst = parcNode.getChildNodes();
								int parent_cond =
										Integer.valueOf(parcLst.item(0).getNodeValue());

								// create and add feature to current stage
								rf.setType("rect");
								rf.setFeatureID(feature_id);
								rf.setThreshold(thresh);
								rf.setLeftValue(ct);
								rf.setRightValue(cf);
								rf.setParent(parent);
								rf.setParentCondition(parent_cond);

								// add feature to current stage
								curr_stage.addFeature(rf);
								num_features++;

							}
						}
					}

					// extract and add stage threshold to stage object
					Node stNode = e.getElementsByTagName("stage_threshold").item(0);
					NodeList stLst = stNode.getChildNodes();
					double stage_thresh = Double.valueOf(stLst.item(0).getNodeValue());

					curr_stage.setThreshold(stage_thresh);

					// add stage to Cascade
					this.addStage(curr_stage);

				}
			}

			// initialize cond array
			cond = new int[num_features];

		}
		catch (Exception e) {
			e.printStackTrace();
		}
	}
}
