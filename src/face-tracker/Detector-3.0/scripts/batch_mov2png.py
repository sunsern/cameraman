#! /usr/bin/python

import os
import sys
    
def batch_convert():
    
    framerate = 1
    vframes = 100
    
    l = os.listdir('.')
    for f in l:
        if f.endswith('.mov'):
            dir = f.replace('.mov','')
            os.system('rm -rf %s'%(dir))
            os.system('mkdir %s'%(dir))
            os.system('ffmpeg -y -i %s -an -s qvga -vframes %s -sameq -r %s -vcodec png %s/%%7d.png'%(f,vframes,framerate,dir))
        
    print 'Done!'
    

if __name__ == "__main__":
    batch_convert()
    
