#! /usr/bin/python

import os
import sys
    
def convert(mov, framerate):
    os.system('mencoder "mf://*.png" -mf fps=%s -o mencoder_temp.avi -ovc lavc'%(framerate))
    os.system('ffmpeg -y -i mencoder_temp.avi %s'%(mov))
    os.system('rm mencoder_temp.avi');
    print 'Done!'
    

if __name__ == "__main__":
    if len(sys.argv) == 3:
        convert(sys.argv[1],sys.argv[2])
    else:
        print 'Usage: %s <mov name> <framerate>' % sys.argv[0]

    
    