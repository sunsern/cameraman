#! /usr/bin/python

import os
import sys


def copy(prefix):   
    l = os.listdir('.')
    a = 0
    print '<html>'
    print '<body>'
    for f in l:
        if (os.access('./'+f,os.F_OK) and (not f.startswith('Jan'))):
            print '<a href="%s_result_%s.mov"><img src="%s.jpg" width="100"></a>'%(prefix,f,f)
            os.system('cp -f %s/output/*.mov %s_result/'%(f,prefix))
            os.system('cp -f %s/output/0000001.png.out.png %s_result/%s.png'%(f,prefix,f))            
            if a % 8 == 7:
                print '<br>'
            a = a+1
    print '</body>'
    print '</html>'
    
    
if __name__ == "__main__":
    if len(sys.argv) == 2:
        copy(sys.argv[1])
    else:
        print 'Usage: %s <prefix>' % sys.argv[0]
    
    