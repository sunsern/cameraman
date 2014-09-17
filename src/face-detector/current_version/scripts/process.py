#! /usr/bin/python

import os
import sys
import mov2avi
import detect_face
import generate_jboost

framerate = 1
xml = '../../xml/full_opencv.xml' 

def process():
    
    global framerate
    
    l = os.listdir('.')
    for f in l:
        if f.endswith('.mov'):
            print 'Processing...%s'%f
            #mov2avi.convert2avi(f,framerate)
            detect_face.detect(f.replace('.mov','.avi'))
            generate_jboost.make_jboost_files(xml,f.replace('.mov',''))
            
    print 'Done!'
    

if __name__ == "__main__":
    process()
    