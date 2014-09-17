package skin;

import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

import org.xml.sax.SAXException;

import HSV.*;

/**
 * Detect skin color using a HSV_Scorer
 * 
 * @author Sunsern Cheamanunkul
 *
 */
public class Skin_Detector {

	private HSV_Scorer scorer;

	public Skin_Detector(String xml) throws SAXException, IOException, ParserConfigurationException {
		scorer = new HSV_Scorer(xml);
	}
	
	public double[] getScoreVector(double[] hsv, int w, int h) {
		
		//double[] smooth = new double[w * h];
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

		// smooth score
		/*
		for (int i = 0; i < h; i++) {
			smooth[i*w] = score[i*w];
			smooth[i*w + (w-1)] = score[i*w + (w-1)];
		}
		for (int i = 0; i < w; i++) {
			smooth[i] = score[i];
			smooth[(h-1)*w + i] = score[(h-1)*w + i];
		}
		
		for (int i = 1; i < h-1; i++) {
			for (int j = 1; j < w-1; j++) {
				double s =
					score[(i-1)*w+j-1] +	
					score[(i-1)*w+j] +		
					score[(i-1)*w+j+1] +
					score[i*w+j-1] + 	
					2 * score[i*w+j] +		
					score[i*w+j+1] +
					score[(i+1)*w+j-1] +	
					score[(i+1)*w+j] +		
					score[(i+1)*w+j+1];
				
					smooth[i*w+j] = s / 10d;
			}
		}
		return smooth;
		*/
		return score;
		
	}

}
