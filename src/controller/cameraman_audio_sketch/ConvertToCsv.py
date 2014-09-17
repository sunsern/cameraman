#! /usr/bin/env python

f = file('/Users/cameraman/Documents/pdtree_log.txt')
lines = f.readlines()
f.close()

f = file('/Users/cameraman/cameraman/controller/cameraman_audio_sketch/pd_trees.csv','w')
write_me = []
for line in lines:
    splitter = [x.strip().replace(',','') for x in line.split(' ')]    
    splitter.pop(1)
    write_me.append(','.join(splitter) + '\n')
    if(len(write_me) > 100000):
        f.writelines(write_me)
        write_me = []
f.writelines(write_me)
f.close()
        
    
