#! /usr/bin/python

import os
import sys
import random
import time
import getopt
import shutil


def shuffle(datafile):
	
    f= file(datafile, 'r')
    data= f.readlines()
    f.close()
    random.seed()
    random.shuffle(data)
    
    outputname = datafile.replace(".data","_shuffled.data")     
    outputfile= file(outputname, 'w')
    outputfile.writelines(data)
    outputfile.close()

    
if __name__ == "__main__":
	if len(sys.argv) == 2:
		shuffle(sys.argv[1])
	else:
		print 'Usage: %s <data file>' % sys.argv[0]
    