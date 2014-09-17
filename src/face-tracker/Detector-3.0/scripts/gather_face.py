#! /usr/bin/python

import os
import sys


def gather():
    os.system('rm -rf face')
    os.system('mkdir face')
    os.system('rm -rf nonface')
    os.system('mkdir nonface')
    l = os.listdir('.')
    for f in l:
        if f.endswith('.mov'):
            print 'Processing...%s'%f
            dir = f.replace('.mov','')
            os.system('cp %s/face* face/'%(dir))
            os.system('cp %s/nonface* nonface/'%(dir))
            
            
    print 'Done!'
    
if __name__ == "__main__":
    gather()
    