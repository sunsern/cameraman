#! /usr/bin/python

import os
import sys

java_cp = '../../bin'


def create_xml(tree_file,spec_file,xml_file,old_xml):

    global java_cp
    
    print 'Generating XML file'
    os.system('java -cp %s Face_CreateXMLFromTree %s %s %s %s'%(java_cp,tree_file,spec_file,xml_file,old_xml))
    
    print 'Done!'


if __name__ == "__main__":
    if len(sys.argv) == 5: 
        create_xml(sys.argv[1],sys.argv[2],sys.argv[3],sys.argv[4])
    else:
        print 'Usage: %s <tree-file> <spec-file> <new xml> <old xml>' % sys.argv[0]
        
