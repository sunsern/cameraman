#! /usr/bin/python

import os
import sys


def makemov(prefix):
    
    l = os.listdir('.')
    for f in l:
        if (os.access('./'+f,os.F_OK) and (not f.startswith('Jan'))):
            print 'Processing...%s'%f
            os.chdir('./%s/output'%f)
            os.system('mencoder "mf://*.png" -mf fps=3 -o mencoder_temp.avi -ovc lavc')
            os.system('ffmpeg -y -i mencoder_temp.avi -r 3 %s_result_%s.mov'%(prefix,f))
            os.system('rm mencoder_temp.avi');
            os.chdir('../..')
        
    print 'Done!'
    
if __name__ == "__main__":
    if len(sys.argv) == 2:
        makemov(sys.argv[1])
    else:
        print 'Usage: %s <prefix>' % sys.argv[0]

    
    