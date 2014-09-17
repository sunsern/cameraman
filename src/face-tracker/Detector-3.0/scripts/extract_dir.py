#! /usr/bin/python

import os
import sys


def extract(dir):
    
    xml = '../../xml/full_opencv.xml'
    java_cp = '../../bin' 

    os.system('java -cp %s Face_Extractor_PNG %s %s'%(java_cp,xml,dir))
            
    print 'Done!'
    
if __name__ == "__main__":
    if len(sys.argv) == 2: 
        extract(sys.argv[1])
    else:
        print 'Usage: %s <directory>' % sys.argv[0]
    
    