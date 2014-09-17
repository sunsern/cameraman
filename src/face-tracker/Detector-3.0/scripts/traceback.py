#! /usr/bin/python

import os
import sys

def copy_files(files, target_dir):
    for file in files:
        os.system('cp %s %s'%(file,target_dir))

def parse_filename(idx,filename):
    output = []
    file = open(filename)
    for line in file:
        s = line.split()
        t = s[0].split(',')
        if t[0] in idx:
            output.append(s[-2])
    return output
    
def parse_index(filename):
    output = []
    file = open(filename)
    for line in file:
        s = line.split()
        if len(s) == 3:
            output.append(s[2])
    return output
 
def main(idx_filename, data_filename, target_dir):
    idx = parse_index(idx_filename)
    fn = parse_filename(idx,data_filename)
    copy_files(fn, target_dir)
 
if __name__ == "__main__":
    if len(sys.argv) != 4:
        print 'usage: %s idx_filename data_filename target_dir'%sys.argv[0]
    else:
        main(sys.argv[1],sys.argv[2],sys.argv[3])


    
