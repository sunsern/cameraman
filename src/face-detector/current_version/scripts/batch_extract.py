#! /usr/bin/python

import os
import sys


def extract():
 
    java_cp = '../../bin'
    
    l = os.listdir('.')
    for f in l:
        if f.endswith('.mov'):
            print 'Processing...%s'%f
            dir = f.replace('.mov','')
            os.system('java -cp %s Face_Extractor_PNG %s'%(java_cp,dir))
            
    print 'Done!'
    
if __name__ == "__main__":
    extract() 
    
    