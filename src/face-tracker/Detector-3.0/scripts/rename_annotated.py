#! /usr/bin/python

import os
import sys


def process(dir):
    
    f = file(dir + '/labels.txt','r')
    data = f.readlines()
    f.close()
    for x in data:
        y = x.split(' ')
        fn = y[0]
        label = int(y[1])
        #print fn + ':' + label
        if label == -1:
            os.system('cp %s/%s %s/nonface_%s'%(dir,fn,dir,fn))
        elif label == 1:
            os.system('cp %s/%s %s/face_%s'%(dir,fn,dir,fn))
            
    print 'Done!'
    
if __name__ == "__main__":
    if len(sys.argv) == 2: 
        process(sys.argv[1])
    else:
        print 'Usage: %s <directory>' % sys.argv[0]
    
    