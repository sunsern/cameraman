package core.facedetector;

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

import core.facedetector.features.AbstractFeature;
import core.facedetector.features.EyeFeature;
import core.facedetector.features.GrayCache;
import core.facedetector.features.GrayFeature;
import core.facedetector.features.HogCache;
import core.facedetector.features.HogFeature;
import core.facedetector.features.HsvSkinCache;
import core.facedetector.features.HsvSkinFeature;
import core.facedetector.features.RootFeature;
import core.facedetector.features.SkinCache;
import core.facedetector.features.SkinFeature;


/**
 * Parse a trained classifier of XML format
 *  
 * @author scheaman
 *
 */
public class FaceXMLReader {

	/** Parent's condition: {-1,0,1}	 */
	protected int[] cond;
	/** Contain features of each stage */
	protected CascadeStage[] stages;
	/** number of stages */
	protected int num_stages;
	/** number of features */
	protected int num_features;
	

	public FaceXMLReader(String xml_filename) throws ParserConfigurationException, SAXException, IOException {		
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

  	NodeList hLst = doc.getElementsByTagName(XMLConstants.FACE_HEADER);
  	Element hElem = (Element) hLst.item(0);

  	/* stages is item 1 */
  	Element stagesElem =
  		(Element) hElem.getElementsByTagName(XMLConstants.STAGES).item(0);
  	NodeList stageLst = stagesElem.getElementsByTagName(XMLConstants.STAGE);

  	stages = new CascadeStage[stageLst.getLength()];

  	// get each stage
  	for (int s = 0; s < stageLst.getLength(); s++) {

  		Node snode = stageLst.item(s);

  		if (snode.getNodeType() == Node.ELEMENT_NODE) {

  			// at each stage, get all features
  			Element e = (Element) snode;
  			NodeList fcLst = e.getElementsByTagName(XMLConstants.FEATURES);

  			Element fcElem = (Element) fcLst.item(0);
  			NodeList fLst = fcElem.getElementsByTagName(XMLConstants.FEATURE);

  			// create an object for each stage
  			CascadeStage curr_stage = new CascadeStage(fLst.getLength());

  			// add each feature into the stage
  			for (int t = 0; t < fLst.getLength(); t++) {

  				Node tnode = fLst.item(t);
  				if (tnode.getNodeType() == Node.ELEMENT_NODE) {

  					// get feature type
  					Element fElem = (Element) tnode;
  					String fType = fElem.getAttribute(XMLConstants.TYPE);
  					int feature_id = Integer.valueOf(fElem.getAttribute(XMLConstants.ID));

  					if (fType.compareTo(RootFeature.NAME) == 0) {

  						double ct = getDoubleFromElement(fElem,XMLConstants.COND_TRUE_VAL);

  						// create root feature
  						RootFeature rootf = new RootFeature();
  						rootf.setType(RootFeature.NAME);
  						rootf.setFeatureID(feature_id);
  						rootf.setLeftValue(ct);

  						// add feature to current stage
  						curr_stage.addFeature(rootf);
  						num_features++;

  					}
  					else if (fType.compareTo(SkinFeature.NAME) == 0) {

  						int skinindex = getIntFromElement(fElem,XMLConstants.SEPARATOR); 
  						double thresh = getDoubleFromElement(fElem,XMLConstants.THRESHOLD);
  						double ct = getDoubleFromElement(fElem,XMLConstants.COND_TRUE_VAL);
  						double cf = getDoubleFromElement(fElem,XMLConstants.COND_FALSE_VAL); 
  						int parent = getIntFromElement(fElem,XMLConstants.PARENT);
  						int parent_cond = getIntFromElement(fElem,XMLConstants.PARENT_COND); 

  						// create skin feature
  						SkinFeature scf = new SkinFeature();
  						scf.setType(SkinFeature.NAME);
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
   					else if (fType.compareTo(EyeFeature.NAME) == 0) {

  						int eyeindex = getIntFromElement(fElem,XMLConstants.SEPARATOR); 
  						double thresh = getDoubleFromElement(fElem,XMLConstants.THRESHOLD);
  						double ct = getDoubleFromElement(fElem,XMLConstants.COND_TRUE_VAL);
  						double cf = getDoubleFromElement(fElem,XMLConstants.COND_FALSE_VAL); 
  						int parent = getIntFromElement(fElem,XMLConstants.PARENT);
  						int parent_cond = getIntFromElement(fElem,XMLConstants.PARENT_COND); 

  						// create eye feature
  						EyeFeature gf = new EyeFeature();
  						gf.setType(EyeFeature.NAME);
  						gf.setFeatureID(feature_id);
  						gf.setThreshold(thresh);
  						gf.setLeftValue(ct);
  						gf.setRightValue(cf);
  						gf.setParent(parent);
  						gf.setParentCondition(parent_cond);
  						gf.eyeindex = eyeindex;

  						// add feature to current stage
  						curr_stage.addFeature(gf);
  						num_features++;

  					}
  					else if (fType.compareTo(GrayFeature.NAME) == 0) {

  						int grayindex = getIntFromElement(fElem,XMLConstants.SEPARATOR); 
  						double thresh = getDoubleFromElement(fElem,XMLConstants.THRESHOLD);
  						double ct = getDoubleFromElement(fElem,XMLConstants.COND_TRUE_VAL);
  						double cf = getDoubleFromElement(fElem,XMLConstants.COND_FALSE_VAL); 
  						int parent = getIntFromElement(fElem,XMLConstants.PARENT);
  						int parent_cond = getIntFromElement(fElem,XMLConstants.PARENT_COND); 

  						// create grey feature
  						GrayFeature gf = new GrayFeature();
  						gf.setType(GrayFeature.NAME);
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
  					else if (fType.compareTo(HogFeature.NAME) == 0) {

  						int hogindex = getIntFromElement(fElem,XMLConstants.SEPARATOR); 
  						double thresh = getDoubleFromElement(fElem,XMLConstants.THRESHOLD);
  						double ct = getDoubleFromElement(fElem,XMLConstants.COND_TRUE_VAL);
  						double cf = getDoubleFromElement(fElem,XMLConstants.COND_FALSE_VAL); 
  						int parent = getIntFromElement(fElem,XMLConstants.PARENT);
  						int parent_cond = getIntFromElement(fElem,XMLConstants.PARENT_COND); 

  						// create hog feature
  						HogFeature HOGf = new HogFeature();
  						HOGf.setType(HogFeature.NAME);
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
  					else if (fType.compareTo(HsvSkinFeature.NAME) == 0) {

  						int hsvskinindex = getIntFromElement(fElem,XMLConstants.SEPARATOR); 
  						double thresh = getDoubleFromElement(fElem,XMLConstants.THRESHOLD);
  						double ct = getDoubleFromElement(fElem,XMLConstants.COND_TRUE_VAL);
  						double cf = getDoubleFromElement(fElem,XMLConstants.COND_FALSE_VAL); 
  						int parent = getIntFromElement(fElem,XMLConstants.PARENT);
  						int parent_cond = getIntFromElement(fElem,XMLConstants.PARENT_COND); 

  						// create HsvSkin feature
  						HsvSkinFeature hsvskinf = new HsvSkinFeature();
  						hsvskinf.setType(HsvSkinFeature.NAME);
  						hsvskinf.setFeatureID(feature_id);
  						hsvskinf.setThreshold(thresh);
  						hsvskinf.setLeftValue(ct);
  						hsvskinf.setRightValue(cf);
  						hsvskinf.setParent(parent);
  						hsvskinf.setParentCondition(parent_cond);
  						hsvskinf.hsvskinindex = hsvskinindex;

  						// add feature to current stage
  						curr_stage.addFeature(hsvskinf);
  						num_features++;
  					}
  				}
  			}

  			// extract and add stage threshold to stage object
  			double stage_thresh = getDoubleFromElement(e, XMLConstants.STAGE_THRESHOLD);

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
	private void addStage(CascadeStage s) {
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
	public double[] getCandidateScoreVector(FaceCandidate c, 
	                                        SkinCache skincache,
	                                        HogCache hogcache,
	                                        GrayCache graycache,
	                                        HsvSkinCache hsvskincache) {

		double acc = 0.0;
		int total_feature = 0;
		
		for (int i = 0; i < num_stages; i++) {
			CascadeStage s = stages[i];
			total_feature += s.getNumFeatures();
		}
		
		double[] scoreVector = new double[total_feature];
		int scoreVectorIdx = 0;                            
		
		for (int i = 0; i < num_stages; i++) {
			CascadeStage s = stages[i];
			for (int j = 0; j < s.getNumFeatures(); j++) {
				AbstractFeature f = s.getFeature(j);
				int fid = f.getFeatureID();
				if (f.getType().compareTo(RootFeature.NAME) == 0) {
					RootFeature root = (RootFeature) f;
					acc += root.getLeftValue();
					cond[fid] = 1;
				}
				else if (f.getType().compareTo(SkinFeature.NAME) == 0) {
					SkinFeature sc = (SkinFeature) f;
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
				else if (f.getType().compareTo(EyeFeature.NAME) == 0) {
					EyeFeature g = (EyeFeature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (g.testCandidate(c,graycache.ii)) {
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
				else if (f.getType().compareTo(GrayFeature.NAME) == 0) {
					GrayFeature g = (GrayFeature) f;
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
				else if (f.getType().compareTo(HogFeature.NAME) == 0) {
					HogFeature hog = (HogFeature) f;
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
				else if (f.getType().compareTo(HsvSkinFeature.NAME) == 0) {
					HsvSkinFeature hsvskin = (HsvSkinFeature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (hsvskin.testCandidate(hsvskincache)) {
							acc += hsvskin.getLeftValue();
							cond[fid] = 1;
						}
						else {
							acc += hsvskin.getRightValue();
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

	

	public double getCandidateScore(FaceCandidate c, 
	                                SkinCache skincache,
																	HogCache hogcache,
																	GrayCache graycache,
																	HsvSkinCache hsvskincache) {
		double acc = 0.0;

		for (int i = 0; i < num_stages; i++) {
			CascadeStage s = stages[i];
			for (int j = 0; j < s.getNumFeatures(); j++) {
				AbstractFeature f = s.getFeature(j);
				int fid = f.getFeatureID();
				if (f.getType().compareTo(RootFeature.NAME) == 0) {
					RootFeature root = (RootFeature) f;
					acc += root.getLeftValue();
					cond[fid] = 1;
				}
				else if (f.getType().compareTo(SkinFeature.NAME) == 0) {
					SkinFeature sc = (SkinFeature) f;
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
				else if (f.getType().compareTo(EyeFeature.NAME) == 0) {
					EyeFeature g = (EyeFeature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (g.testCandidate(c,graycache.ii)) {
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
				else if (f.getType().compareTo(GrayFeature.NAME) == 0) {
					GrayFeature g = (GrayFeature) f;
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
				else if (f.getType().compareTo(HogFeature.NAME) == 0) {
					HogFeature hog = (HogFeature) f;
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
				else if (f.getType().compareTo(HsvSkinFeature.NAME) == 0) {
					HsvSkinFeature hsvskin = (HsvSkinFeature) f;
					if (cond[f.getParent()] == f.getParentCondition()) {
						if (hsvskin.testCandidate(hsvskincache)) {
							acc += hsvskin.getLeftValue();
							cond[fid] = 1;
						}
						else {
							acc += hsvskin.getRightValue();
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
