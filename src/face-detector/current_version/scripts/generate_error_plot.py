#! /usr/bin/python

import os
import sys

tree = 'ADD_ALL'

def make_error_plot(dir):
    global tree
    l = os.listdir(dir + '/' + tree)
    for f in l:
        c = len(f.split('.'))
        if (f.endswith('.info')) and (c == 2):
            os.system('../../scripts/error.py --info=%s --logaxis'%(dir+'/'+tree+'/'+f))
            os.system('mv error.png %s.error.png'%(dir+'/'+f.replace('.info','')))
            os.system('rm -f error.dat')
            os.system('rm -f error.gnuplot')
            
            
if __name__ == "__main__":
    if len(sys.argv) == 2:
        make_error_plot(sys.argv[1])
    else:
        print 'Usage: %s <directory of JBoost output>' % sys.argv[0]