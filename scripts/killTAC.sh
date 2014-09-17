#!/bin/bash

usernotice=$(ps -x | grep [U]serNotification | awk '{ print $1 }')
maxpid=$(ps -x | grep [M]axMSP | awk '{ print $1 }')
qtpid=$(ps -x | grep [Q]uickTime | awk '{ print $1 }')
firefox=$(ps -x | grep [F]irefox | awk '{ print $1 }')

if [ $usernotice ]
then
    echo "Killing UserNotice..."
    kill -9 $usernotice
fi

if [ $maxpid ]
then
    echo "Killing MaxMSP..."
    kill -9 $maxpid
fi

if [ $qtpid ]
then
    echo "Killing QuickTime..."
    kill -9 $qtpid
fi

if [ $firefox ]
then
    echo "Killing Firefox..."
    kill -9 $firefox
fi
