import processing.core.*; 
import processing.xml.*; 

import maxlink.*; 

import java.applet.*; 
import java.awt.*; 
import java.awt.image.*; 
import java.awt.event.*; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class cameraman_audio_sketch extends PApplet {

// Evan Ettinger - Feb 2010
// Draws audio points onto top 3 PCs in real time.
// Ball colors correspond to dominant depth in PD-tree for particles in NH-tracker
//    Red   = depth 0
//    Green = depth 1
//    Blue  = depth 2
// Balls slowly fade out by using alpha channel.



int MAX_ARRAY_SIZE = 20;
int BALL_SIZE = 3;

PImage bg;
MaxLink link = new MaxLink(this, "cameraman_audio_sketch");
ArrayList audio_pts;
int ballWidth = 10;
String input;
float lastImageLoadDay = 0;

public void setup() {
  size(500, 500); 
  frameRate(10);
  audio_pts = new ArrayList();
  link.declareInlet("input", "AddBall");
  bg = loadImage("../audio_visualization_background.png");
  background(bg);
  lastImageLoadDay = day();
}

public void AddBall(String s) {
  print("Received string: " + s + "\n");
  String[] nums = split(s, ' ');
  if(nums.length == 5) {
    while(audio_pts.size() > MAX_ARRAY_SIZE) {
      audio_pts.remove(0);
    }
    audio_pts.add(new Ball(PApplet.parseFloat(nums[1]), PApplet.parseFloat(nums[2]), PApplet.parseFloat(nums[3]), PApplet.parseFloat(nums[4])));
  }
}

public void draw() {
  if(lastImageLoadDay != day()) {
    lastImageLoadDay = day();
    bg = loadImage("../audio_visualization_background.png");
  }
  background(bg);
  translate(250,250);
  noStroke();
  for (int i = audio_pts.size()-1; i >= 0; i--) { 
      // An ArrayList doesn't know what it is storing so we have to cast the object coming out
      try{
        Ball ball = (Ball) audio_pts.get(i);
        if(ball != null) {
          ball.display();
          if (ball.finished()) {
            // Items can be deleted with remove()
            audio_pts.remove(i);
          }    
        }
      }
      catch(Exception e) {
      }
  }  
}

class Ball {
  
  float x;
  float y;
  float z;
  float radius = BALL_SIZE;
  float depth;
  int life = 255;
  int r = 255;
  int g = 255;
  int b = 255;
  Ball(float tempX, float tempY, float tempZ, float tempD) {
    x = tempX/2;
    y = tempY/2;
    z = tempZ;
    depth = tempD; 
    if(depth == 0.0f) {
      r = 255;
    } else if(depth == 1.0f) {
      g = 255;
    }
    else {
      b = 255;
    }   
  }
   
  public boolean finished() {
    // Balls fade out
    life-= 12;
    if (life < 0) {
      return true;
    } else {
      return false;
    }
  }
  
  public void display() {   
    // Display the circle
    fill(r,g,b);
    pushMatrix();
    translate(x,y);
    ellipse(0,0,radius,radius);
    popMatrix();
  }
}  

static public void main(String args[]) {
 PApplet.main(new String[] {"cameraman_audio_sketch"});
}

}
