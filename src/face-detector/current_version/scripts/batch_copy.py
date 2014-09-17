#! /usr/bin/python

import os
import sys


def copy(dir):
    os.system('mkdir %s'%dir)
    l = os.listdir('.')
    for f in l:
        if (os.access('./'+f,os.F_OK) and (f.startswith('1')) and (not f.endswith('.mov'))):
            print 'Processing...%s'%f
            os.system('cp %s/%s/* ./%s'%(f,dir,dir))
            
    print 'Done!'
    
if __name__ == "__main__":
    copy(sys.argv[1])
    
