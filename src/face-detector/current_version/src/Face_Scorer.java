import gray.Gray_Cache;
import gray.Gray_Feature;

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

import shared.Candidate;
import shared.Constants;
import shared.Feature;
import shared.RootFeature;
import shared.Stage;
import skin.Skin_Cache;
import skin.Skin_Feature;

import HOG.HOG_Cache;
import HOG.HOG_Feature;

/**
 * Parse a trained classifier of XML format
 *  
 * @author scheaman
 *
 */
public class Face_Scorer {

	/** Parent's condition: {-1,0,1}	 */
	protected int[] cond;
	/** Contain features of each stage */
	protected Stage[] stages;
	/** number of stages */
	protected int num_stages;
	/** number of features */
	protected int num_features;
	

	public Face_Scorer(String xml_filename) throws ParserConfigurationException, SAXException, IOException {		
			loadFromXML(xml_filename);
	}
	
	private int getIntFromElement(Element elem, String tag) {
  	Node node = elem.getElementsByTagName(tag).item(0);
		NodeList nodeLst = node.getChildNodes();
		return Integer.parseInt(nodeLst.item(0).getNodeValue());
	}
	
	
  private double getDoubleFromElement(Element elem, String tag) {
  	Node node = elem.getElementsByTagName(tag).item(0);
		NodeList nodeLst = node.getChildNodes();
		return Double.parseDouble(nodeLst.item(0).getNodeValue());
	}
	

  public void loadFromXML(String xml_filename) throws ParserConfigurationException, SAXException, IOException {

  	// reset number of features and number of stages
  	num_features = 0;
  	num_stages   = 0;

  	File file = new File(xml_filename);
  	DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
  	DocumentBuilder db = dbf.newDocumentBuilder();
  	Document doc = db.parse(file);
  	doc.getDocumentElement().normalize();

  	NodeList hLst = doc.getElementsByTagName(Constants.HEADER);
  	Element hElem = (Element) hLst.item(0);

  	/* stages is item 1 */
  	Element stagesElem =
  		(Element) hElem.getElementsByTagName(Constants.STAGES).item(0);
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
  					int feature_id = Integer.valueOf(fElem.getAttribute(Constants.ID));

  					if (fType.compareTo(RootFeature.NAME) == 0) {

  						double ct = getDoubleFromElement(fElem,Constants.COND_TRUE_VAL);

  						// create root feature
  						RootFeature rootf = new RootFeature();
  						rootf.setType(RootFeature.NAME);
  						rootf.setFeatureID(feature_id);
  						rootf.setLeftValue(ct);

  						// add feature to current stage
  						curr_stage.addFeature(rootf);
  						num_features++;

  					}
  					else if (fType.compareTo(Skin_Feature.NAME) == 0) {

  						int skinindex = getIntFromElement(fElem,Constants.SEPARATOR); 
  						double thresh = getDoubleFromElement(fElem,Constants.THRESHOLD);
  						double ct = getDoubleFromElement(fElem,Constants.COND_TRUE_VAL);
  						double cf = getDoubleFromElement(fElem,Constants.COND_FALSE_VAL); 
  						int parent = getIntFromElement(fElem,Constants.PARENT);
  						int parent_cond = getIntFromElement(fElem,Constants.PARENT_COND); 

  						// create skin feature
  						Skin_Feature scf = new Skin_Feature();
  						scf.setType(Skin_Feature.NAME);
  						scf.setFeatureID(feature_id);
  						scf.setThreshold(thresh);
  						scf.setLeftValue(ct);
  						scf.setRightValue(cf);
  						scf.setParent(parent);
  						scf.setParentCondition(parent_cond);
  						scf.skinindex = skinindex;

  						// add feature to current stage
  						curr_stage.addFeature(scf);
  						num_features++;

  					}
  					else if (fType.compareTo(Gray_Feature.NAME) == 0) {

  						int grayindex = getIntFromElement(fElem,Constants.SEPARATOR); 
  						double thresh = getDoubleFromElement(fElem,Constants.THRESHOLD);
  						double ct = getDoubleFromElement(fElem,Constants.COND_TRUE_VAL);
  						double cf = getDoubleFromElement(fElem,Constants.COND_FALSE_VAL); 
  						int parent = getIntFromElement(fElem,Constants.PARENT);
  						int parent_cond = getIntFromElement(fElem,Constants.PARENT_COND); 

  						// create grey feature
  						Gray_Feature gf = new Gray_Feature();
  						gf.setType(Gray_Feature.NAME);
  						gf.setFeatureID(feature_id);
  						gf.setThreshold(thresh);
  						gf.setLeftValue(ct);
  						gf.setRightValue(cf);
  						gf.setParent(parent);
  						gf.setParentCondition(parent_cond);
  						gf.grayindex = grayindex;

  						// add feature to current stage
  						curr_stage.addFeature(gf);
  						num_features++;

  					}
  					else if (fType.compareTo(HOG_Feature.NAME) == 0) {

  						int hogindex = getIntFromElement(fElem,Constants.SEPARATOR); 
  						double thresh = getDoubleFromElement(fElem,Constants.THRESHOLD);
  						double ct = getDoubleFromElement(fElem,Constants.COND_TRUE_VAL);
  						double cf = getDoubleFromElement(fElem,Constants.COND_FALSE_VAL); 
  						int parent = getIntFromElement(fElem,Constants.PARENT);
  						int parent_cond = getIntFromElement(fElem,Constants.PARENT_COND); 

  						// create hog feature
  						HOG_Feature HOGf = new HOG_Feature();
  						HOGf.setType(HOG_Feature.NAME);
  						HOGf.setFeatureID(feature_id);
  						HOGf.setThreshold(thresh);
  						HOGf.setLeftValue(ct);
  						HOGf.setRightValue(cf);
  						HOGf.setParent(parent);
  						HOGf.setParentCondition(parent_cond);
  						HOGf.hogindex = hogindex;

  						// add feature to current stage
  						curr_stage.addFeature(HOGf);
  						num_features++;
  					}
  				}
  			}

  			// extract and add stage threshold to stage object
  			double stage_thresh = getDoubleFromElement(e, Constants.STAGE_THRESHOLD);

  			curr_stage.setThreshold(stage_thresh);

  			// add stage to Cascade
  			this.addStage(curr_stage);

  		}
  	}

  	// initialize cond array
  	cond = new int[num_features];

  }

	/**
	 * Add a Stage into this cascade.
	 * 
	 * @param stage
	 */
	private void addStage(Stage s) {
		stages[num_stages] = s;
		num_stages++;
	}

	public int getNumFeatures() {
		return num_features;
	}

	/**
	 * Get scores after evaluation of each feature. 
	 * This is useful for building cascade.
	 * 
	 * @param c
	 * @param skincache
	 * @param hogcache
	 * @param graycache
	 * @return
	 */
	public double[] getCandidateScoreVector(Candidate c, 
	                                        Skin_Cache skincache,
	                                        HOG_Cache hogcache,
	                                        Gray_Cache graycache) {

		double acc = 0.0;
		int total_feature = 0;
		
		for (int i = 0; i < num_stages; i++) {
			Stage s = stages[i];
			total_feature += s.getNumFeatures();
		}
		
		double[] scoreVector = new double[total_feature];
		int scoreVectorIdx = 0;                            
		
		for (int i = 0; i < num_stages; i++) {
			Stage s = stages[i];
			for (int j = 0; j < s.getNumFeatures(); j++) {
				Feature f = s.getFeature(j);
				int fid = f.getFeatureID();
				if (f.getType().compareTo(RootFeature.NAME) == 0) {
					RootFeature root = (RootFeature) f;
					acc += root.getLeftValue();
					cond[fid] = 1;
				}
				else if (f.getType().compareTo(Skin_Feature.NAME) == 0) {
					Skin_Feature sc = (Skin_Feature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (sc.testCandidate(skincache)) {
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
				else if (f.getType().compareTo(Gray_Feature.NAME) == 0) {
					Gray_Feature g = (Gray_Feature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (g.testCandidate(graycache)) {
							acc += g.getLeftValue();
							cond[fid] = 1;
						}
						else {
							acc += g.getRightValue();
							cond[fid] = -1;
						}
					}
					else {
						cond[fid] = 0;
					}
				}
				else if (f.getType().compareTo(HOG_Feature.NAME) == 0) {
					HOG_Feature hog = (HOG_Feature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (hog.testCandidate(hogcache)) {
							acc += hog.getLeftValue();
							cond[fid] = 1;
						}
						else {
							acc += hog.getRightValue();
							cond[fid] = -1;
						}
					}
					else {
						cond[fid] = 0;
					}
				}
				scoreVector[scoreVectorIdx++] = acc;
			}
		}
		return scoreVector;
	}

	

	public double getCandidateScore(Candidate c, 
	                                Skin_Cache skincache,
																	HOG_Cache hogcache,
																	Gray_Cache graycache) {
		double acc = 0.0;

		for (int i = 0; i < num_stages; i++) {
			Stage s = stages[i];
			for (int j = 0; j < s.getNumFeatures(); j++) {
				Feature f = s.getFeature(j);
				int fid = f.getFeatureID();
				if (f.getType().compareTo(RootFeature.NAME) == 0) {
					RootFeature root = (RootFeature) f;
					acc += root.getLeftValue();
					cond[fid] = 1;
				}
				else if (f.getType().compareTo(Skin_Feature.NAME) == 0) {
					Skin_Feature sc = (Skin_Feature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (sc.testCandidate(skincache)) {
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
				else if (f.getType().compareTo(Gray_Feature.NAME) == 0) {
					Gray_Feature g = (Gray_Feature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (g.testCandidate(graycache)) {
							acc += g.getLeftValue();
							cond[fid] = 1;
						}
						else {
							acc += g.getRightValue();
							cond[fid] = -1;
						}
					}
					else {
						cond[fid] = 0;
					}
				}
				else if (f.getType().compareTo(HOG_Feature.NAME) == 0) {
					HOG_Feature hog = (HOG_Feature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (hog.testCandidate(hogcache)) {
							acc += hog.getLeftValue();
							cond[fid] = 1;
						}
						else {
							acc += hog.getRightValue();
							cond[fid] = -1;
						}
					}
					else {
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
