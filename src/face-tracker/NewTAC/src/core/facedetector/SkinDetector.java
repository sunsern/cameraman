package core.facedetector;

import java.io.IOException;
import javax.xml.parsers.ParserConfigurationException;
import org.xml.sax.SAXException;

/**
 * Detect skin color using a HSV_Scorer
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class SkinDetector {

	private SkinXMLReader scorer;

	public SkinDetector(String xml) throws SAXException, IOException, ParserConfigurationException {
		scorer = new SkinXMLReader(xml);
	}
	
	public double[] getScoreVector(double[] hsv, int w, int h) {
		
		double[] score = new double[w * h];
		double[] temp = new double[3];
		
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				temp[0] = hsv[(i * w + j) * 3];
				temp[1] = hsv[(i * w + j) * 3 + 1];
				temp[2] = hsv[(i * w + j) * 3 + 2];
				score[i*w+j] = scorer.getScore(temp);
			}
		}
		
		return score;	
	}
	
	public double[] getScoreVector(double[] hsv, int w, int h, int downSamplingFactor) {
		
		double[] score = new double[w * h];
		double[] temp = new double[3];
		
		for (int i = 0; i < h; i++) {
			for (int j = 0; j < w; j++) {
				if (i % downSamplingFactor == 0 && 
						j % downSamplingFactor == 0) {
					temp[0] = hsv[(i * w + j) * 3];
					temp[1] = hsv[(i * w + j) * 3 + 1];
					temp[2] = hsv[(i * w + j) * 3 + 2];
					score[i*w+j] = scorer.getScore(temp);
				}
				else if (i % downSamplingFactor == 0) {
					score[i*w+j] = score[i*w+j-1];
				}
				else  {
					score[i*w+j] = score[(i-1)*w+j];
				}
			}
		}
		
		return score;	
	}

}
