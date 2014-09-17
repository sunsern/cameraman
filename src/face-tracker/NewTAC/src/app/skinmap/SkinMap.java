package app.skinmap;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

import javax.imageio.ImageIO;

import core.huemap.HueMap;

public class SkinMap {

	private HueMap hm;
	
	public SkinMap(BufferedImage bimg, int skinbox_x, int skinbox_y, int skinbox_w, int skinbox_h) {
		BufferedImage skin = bimg.getSubimage(skinbox_x, skinbox_y, skinbox_w, skinbox_h);
		hm = new HueMap(skin);
	}
	
	public BufferedImage getSkinMap(BufferedImage bimg) {
		BufferedImage skinmap = hm.createSmoothMapImage(bimg);
		return skinmap;
	}
	
	public static void main(String[] args) {
		String filename = args[0];
		int x = Integer.parseInt(args[1]);
		int y = Integer.parseInt(args[2]);
		int w = Integer.parseInt(args[3]);
		int h = Integer.parseInt(args[4]);
		try {
			BufferedImage bimg = ImageIO.read(new File(filename));
			SkinMap skmap = new SkinMap(bimg,x,y,w,h);
			BufferedImage output = skmap.getSkinMap(bimg);
			ImageIO.write(output, "jpg", new File(filename + ".out.jpg"));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}	
}
