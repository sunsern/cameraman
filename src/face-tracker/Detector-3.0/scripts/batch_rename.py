#! /usr/bin/python

import os
import sys
    
def batch_rename(prefix):
    
    l = os.listdir('.')
    for f in l:
        if f.endswith('.png'):
            os.system('mv %s %s_%s'%(f,prefix,f))
            
    print 'Done!'
    

if __name__ == "__main__":
    if len(sys.argv) == 2:
        batch_rename(sys.argv[1])
    else:
        print 'Usage: %s <prefix>' % sys.argv[0]

    
    