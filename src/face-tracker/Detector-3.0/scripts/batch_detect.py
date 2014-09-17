#! /usr/bin/python

import os
import sys


def detect():
    
    xml = '/home/scheaman/workspace/NewFaceDetector/xml/Jan22-cascade-2.xml' 
    java_cp = '/home/scheaman/workspace/NewFaceDetector/bin'
    
    l = os.listdir('.')
    for f in l:
        if (os.access('./'+f,os.F_OK) and (f.startswith('1')) and (not f.endswith('.mov'))):
            print 'Processing...%s'%f
            os.system('java -cp %s Face_Detect %s %s -1'%(java_cp,xml,f))
            
    print 'Done!'
    
if __name__ == "__main__":
    detect()
    
