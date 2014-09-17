#! /bin/sh

jython -Dpython.path=.:$JBOOST_DIR/bin:$JBOOST_DIR/lib/concurrent.jar:$JBOOST_DIR/lib/jcommon-1.0.8.jar:$JBOOST_DIR/lib/jfreechart-1.0.10.jar:$JBOOST_DIR/lib/swing-layout-1.0.jar $JBOOST_DIR/scripts/VisualizeScores.py "$1*"
