package skindetector;

import java.io.File;
import java.io.IOException;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;


import features.AbstractFeature;
import features.HSVFeature;
import features.RootFeature;

import shared.Constants;
import shared.Stage;


/**
 * This class output a score for each example
 * according to a given XML description.
 * 
 * @author scheaman
 *
 */
public class SkinScorer {
	
	private int[] cond;
	private Stage[] stages;
	
	private int num_features;
	private int num_stages;
	
	
	public double getLastStageThreshold() {
		return stages[num_stages-1].getThreshold();
	}
	
	private void addStage(Stage s) {
		stages[num_stages] = s;
		num_stages++;
	}
	
	public SkinScorer(String xml_filename) throws SAXException, IOException, ParserConfigurationException {
		num_features = 0;
		num_stages = 0;

		File file = new File(xml_filename);
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		DocumentBuilder db = dbf.newDocumentBuilder();
		Document doc = db.parse(file);
		doc.getDocumentElement().normalize();

		NodeList hLst = doc.getElementsByTagName(Constants.SKIN_HEADER);
		Element hElem = (Element)hLst.item(0);

		Element stagesElem = (Element)hElem.getElementsByTagName(Constants.STAGES).item(0);
		NodeList stageLst = stagesElem.getElementsByTagName(Constants.STAGE);

		stages = new Stage[stageLst.getLength()];

		// get each stage
		for (int s = 0; s < stageLst.getLength(); s++) {

			Node snode = stageLst.item(s);

			if (snode.getNodeType() == Node.ELEMENT_NODE) {

				// at each stage, get all features
				Element e = (Element) snode;
				NodeList fcLst = e.getElementsByTagName(Constants.FEATURES);

				Element fcElem = (Element) fcLst.item(0);
				NodeList fLst = fcElem.getElementsByTagName(Constants.FEATURE);

				// create an object for each stage
				Stage curr_stage = new Stage(fLst.getLength());

				// add each feature into the stage
				for (int t = 0; t < fLst.getLength(); t++) {

					Node tnode = fLst.item(t);
					if (tnode.getNodeType() == Node.ELEMENT_NODE) {

						// get feature type
						Element fElem = (Element) tnode;
						String fType = fElem.getAttribute(Constants.TYPE);
						int feature_id = Integer.valueOf(fElem
						                                 .getAttribute(Constants.ID));

						if (fType.compareTo(RootFeature.NAME) == 0) {

							// get the cond_true_val
							Node ctNode = fElem.getElementsByTagName(
							"cond_true_val").item(0);
							NodeList ctLst = ctNode.getChildNodes();
							double ct = Double.valueOf(ctLst.item(0)
							                           .getNodeValue());

							RootFeature rootf = new RootFeature();

							rootf.setType(RootFeature.NAME);
							rootf.setFeatureID(feature_id);
							rootf.setLeftValue(ct);

							// add feature to current stage
							curr_stage.addFeature(rootf);
							num_features++;

						} 
						else if (fType.compareTo(HSVFeature.NAME) == 0) {

							// get HSVindex
							Node HSVindexNode = fElem.getElementsByTagName(
							"_").item(0);
							NodeList HSVindexLst = HSVindexNode.getChildNodes();
							String HSV = HSVindexLst.item(0).getNodeValue();

							int HSVindex;

							if (HSV.compareTo("H")==0) HSVindex = 0;
							else if (HSV.compareTo("S")==0) HSVindex = 1;
							else if (HSV.compareTo("V")==0) HSVindex = 2;
							else HSVindex = -1;

							// get the threshold
							Node threshNode = fElem.getElementsByTagName(
							"threshold").item(0);
							NodeList threshLst = threshNode.getChildNodes();
							double thresh = Double.valueOf(threshLst
							                               .item(0).getNodeValue());

							// get the cond_true_val
							Node ctNode = fElem.getElementsByTagName(
							"cond_true_val").item(0);
							NodeList ctLst = ctNode.getChildNodes();
							double ct = Double.valueOf(ctLst.item(0)
							                           .getNodeValue());

							// get the cond_false_val
							Node cfNode = fElem.getElementsByTagName(
							"cond_false_val").item(0);
							NodeList cfLst = cfNode.getChildNodes();
							double cf = Double.valueOf(cfLst.item(0)
							                           .getNodeValue());

							// get the parent
							Node parNode = fElem.getElementsByTagName(
							"parent").item(0);
							NodeList parLst = parNode.getChildNodes();
							int parent = Integer.valueOf(parLst.item(0)
							                             .getNodeValue());

							// get the parent_cond
							Node parcNode = fElem.getElementsByTagName(
							"parent_cond").item(0);
							NodeList parcLst = parcNode.getChildNodes();
							int parent_cond = 
								Integer.valueOf(parcLst.item(0).getNodeValue());

							HSVFeature HSVf = new HSVFeature();

							HSVf.setType(HSVFeature.NAME);
							HSVf.setFeatureID(feature_id);
							HSVf.setHSVindex(HSVindex);
							HSVf.setThreshold(thresh);
							HSVf.setLeftValue(ct);
							HSVf.setRightValue(cf);
							HSVf.setParent(parent);
							HSVf.setParentCondition(parent_cond);

							// add feature to current stage
							curr_stage.addFeature(HSVf);
							num_features++;

						} 
					}
				}

				// extract and add stage threshold to stage object
				Node stNode = e.getElementsByTagName(Constants.STAGE_THRESHOLD)
				.item(0);
				NodeList stLst = stNode.getChildNodes();
				double stage_thresh = Double.valueOf(stLst.item(0)
				                                     .getNodeValue());

				curr_stage.setThreshold(stage_thresh);

				// add stage to Cascade
				this.addStage(curr_stage);

			}
		}

		// initialize cond array
		cond = new int[num_features];

	}
	
	/**
	 * Compute the score
	 *  
	 * @param fv Feature vector
	 * @return score
	 */
	public double getScore(double[] fv) {

		double acc = 0.0;
		for (int i = 0; i < num_stages; i++) {
			Stage s = stages[i];
			for (int j = 0; j < s.getNumFeatures(); j++) {
				AbstractFeature f = s.getFeature(j);
				int fid = f.getFeatureID();
				if (f.getType().compareTo(RootFeature.NAME) == 0) {
					RootFeature root = (RootFeature) f;
					acc += root.getLeftValue();
					cond[fid] = 1;
				}
				else if (f.getType().compareTo(HSVFeature.NAME) == 0) {
					HSVFeature HSV = (HSVFeature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (HSV.testCandidate(fv)) {
							acc += HSV.getLeftValue();
							cond[fid] = 1;
						} else {
							acc += HSV.getRightValue();
							cond[fid] = -1;
						}
					} else {
						cond[fid] = 0;
					}
				}
			}
			if (acc < s.getThreshold()) {
				return acc;
			}
		}
		return acc;
	}

}
