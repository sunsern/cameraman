package facealign;


import java.awt.image.BufferedImage;
import java.io.File;
import java.io.FilenameFilter;

import javax.imageio.ImageIO;
import javax.swing.filechooser.FileFilter;

import facetracker.FaceScorer;
import facetracker.FaceState;

import particlefilter.Scorer;
import particlefilter.State;
import shared.Candidate;

public class FaceScorerCombine implements Scorer {

	FaceScorer fs;
	FaceScorerMask fsm;
	
	public FaceScorerCombine(FaceScorer fs, FaceScorerMask fsm)  {
		this.fs = fs;
		this.fsm = fsm;
	}

	public void loadImage(BufferedImage bimg) {
		
		fs.loadImage(bimg,2);
		fsm.loadImage(bimg);
		
	}

	public double getScore(State state) {
		return fs.getScore(state) + fsm.getScore(state);
	}

	public double getScore(Candidate c) {
		return fs.getScore(c) + fsm.getScore(c);
	}

	
	public double getScore(int x, int y, int size) {
		return fs.getScore(x, y, size) + fsm.getScore(x, y, size);
	}
	
		
	
}
