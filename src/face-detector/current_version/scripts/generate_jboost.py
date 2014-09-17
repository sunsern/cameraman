#! /usr/bin/python

import os
import sys

def make_jboost_files(xml,dir):
    
    java_cp = '../../bin' 

    print 'Removing *.data and *.spec ...'
    os.system('rm -f ' + dir + '/*.data')
    os.system('rm -f ' + dir + '/*.spec')
            
    print 'Creating jboost files ...'        
    os.system('java -cp %s Face_Extractor_DN %s %s'%(java_cp,xml,dir))

    print 'Combining *.data ...'
    l = os.listdir(dir)
    s = ''
    for f in l:
        if f.endswith('.data'):
            s = s + dir + '/' + f + ' ';
    os.system('cat '+s+'>%s.data'%dir)
    
    print 'Creating face.spec ...'    
    l = os.listdir(dir)
    s = ''
    for f in l:
        if f.endswith('.spec'):
            os.system('cp ' + dir + '/' + f + ' face.spec')
            break
    
    print 'Done!'


if __name__ == "__main__":
    if len(sys.argv) == 3: 
        make_jboost_files(sys.argv[1],sys.argv[2])
    else:
        print 'Usage: %s <xml> <directory>' % sys.argv[0]
