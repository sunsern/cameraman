The Automatic Cameraman
=======================
Our implementation of The Automatic Cameraman (TAC) has 3 main components: 
 1. audio-localizer (C++) 
 2. face-localizer (Java) 
 3. MaxMSP interfaces (Max patches) 

Requirements
============
Hardware:
* HTTP-based PTZ camera [https://pro.sony.com/bbsc/ssr/cat-camerasptz/]
* Video interface [https://www.blackmagicdesign.com/products/intensity/]
* Audio interface [http://www.motu.com/] 

Software:
* Max (w/ MSP and Jitter) v.>=5 [http://cycling74.com/products/max/]
* Quicktime [http://www.apple.com/quicktime/download/]
* Soundflower [http://cycling74.com/soundflower/]
* curl
* Xcode
* Java (w/ Ant)

Setting up
==========
1. Clone the repo on to TAC machine.
2. Compile audio localizer + controller by running ```src/controller/make.sh```
3. Compile face localizer by running ```ant``` on ```src/face-tracker/build.xml```
4. Fix any path errors if needed.


Managing TAC
============
+ Start TAC by running ```scripts/startTAC.sh```
+ Stop TAC by running ```scripts/killTAC.sh```

Issues
======
+ If the camera pan speed is slow, run ```scripts/fix_camera_speed.sh```
