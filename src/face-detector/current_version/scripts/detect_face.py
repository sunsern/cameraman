#! /usr/bin/python

import os
import sys


def detect(avi):
    
    java_cp = '/home/scheaman/workspace/FaceDetector/bin'
    
    print 'Creating temporary images ...'
    dir = avi.replace('.avi','')
    os.system('rm -fr ' + dir)
    os.system('mkdir %s'%(dir))
    os.system('ffmpeg -y -i %s -vcodec png %s/%%7d.png'%(avi,dir))
    
    # Use FaceCapture instead of FaceDetector    
    print 'Starting face detector'
    os.system('java -cp %s FaceCapture ../../xml/full_opencv.xml %s'%(java_cp,dir))

    print 'Done!'


if __name__ == "__main__":
    if len(sys.argv) == 2: 
        detect(sys.argv[1])
    else:
        print 'Usage: %s <AVI file>' % sys.argv[0]
