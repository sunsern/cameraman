#!/bin/bash

usernotice=$(ps -x | grep [U]serNotification | awk '{ print $1 }')
maxpid=$(ps -x | grep [M]axMSP | awk '{ print $1 }')
qtpid=$(ps -x | grep [Q]uickTime | awk '{ print $1 }')

if [ $maxpid ] 
then
    echo "TAC is running properly"
else 
    echo "TAC is not running. Trying to revive TAC..."
    . /Users/cameraman/cameraman/killTAC.sh
    . /Users/cameraman/cameraman/startTAC.sh
fi

