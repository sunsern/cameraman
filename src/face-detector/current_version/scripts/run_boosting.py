#! /usr/bin/python

import os
import sys



def run_boosting(class_name):

    booster_type = 'LogLossBoost'
    #booster_type = 'AdaBoost'
    folds = 2
    rounds = 200
    tree='ADD_ALL'

    cmd = 'java -Xmx1500M jboost.controller.Controller -b %s '%booster_type + \
          '-numRounds %s -p 4 -a -1 '%rounds + \
          '-ATreeType %s '%tree + \
          '-n %s.spec -t %s.data -T %s.data'%(class_name,class_name,class_name)

    #cmd = '../../scripts/nfold.py --booster=%s '%booster_type + \
    #      ' --folds=%d --data=./%s.data --spec=./%s.spec '%(folds,class_name,class_name) + \
    #      ' --rounds=%d --tree=%s --generate'%(rounds,tree)    
    os.system(cmd)
    os.system('mv data.log %s.log'%class_name)
    os.system('mv noname_out.info %s.info'%class_name)
    os.system('mv noname_out.train.boosting.info %s.train.boosting.info'%class_name)
    os.system('mv noname_out.test.boosting.info %s.test.boosting.info'%class_name)

              

if __name__ == "__main__":
    if len(sys.argv) == 2:
        run_boosting(sys.argv[1])
    else:
        print 'Usage: %s <stem>' % sys.argv[0]
