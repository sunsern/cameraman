#! /usr/bin/python

import os
import sys


def clean():
    
    xml = '/home/scheaman/workspace/NewFaceDetector/xml/Jan12_12.xml' 
    java_cp = '/home/scheaman/workspace/NewFaceDetector/bin'
    
    l = os.listdir('.')
    for f in l:
        if (os.access('./'+f,os.F_OK) and (f.startswith('1')) and (not f.endswith('.mov'))):
            print 'Processing...%s'%f
            os.system('rm -rf %s/output'%f)
            os.system('rm -rf %s/detection'%f)
            os.system('rm -rf %s/face'%f)
            
    print 'Done!'
    
if __name__ == "__main__":
    clean()
    