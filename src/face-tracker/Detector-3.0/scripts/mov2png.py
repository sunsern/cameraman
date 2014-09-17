#! /usr/bin/python

import os
import sys
    
def convert(mov, framerate):
    if mov.endswith('.mov'):
        dir = mov.replace('.mov','')
        os.system('rm -rf %s'%(dir))
        os.system('mkdir %s'%(dir))
        os.system('ffmpeg -y -i %s -an -s qvga -sameq -r %s -vcodec png %s/%%7d.png'%(mov,framerate,dir))
        print 'Done!'
    else:
        print 'Error!'
    

if __name__ == "__main__":
    if len(sys.argv) == 3:
        convert(sys.argv[1],sys.argv[2])
    else:
        print 'Usage: %s <mov file> <framerate>' % sys.argv[0]

    
    