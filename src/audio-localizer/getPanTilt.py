#!/usr/bin/pythonw

import math, urllib, threading, time, traceback, sys, os, array, logging

current_PT = [0.0, 0.0]  

# Update current pan/tilt variable -- current_PT
cmd = "http://192.168.1.11/command/visca-inquiry.cgi?VISCA=81090612FF"
x = urllib.urlopen(cmd).read()

pan_string  = x[5]  + x[7]  + x[9]  + x[11]
tilt_string = x[13] + x[15] + x[17] + x[19]

def hex2int(s):
    hex_map = { 'f':'7', 'e':'6', 'd':'5', 'c':'4', 
                    'b':'3', 'a':'2', '9':'1', '8':'0'}

    num = 0
    if s[0] in hex_map.keys():
        num += -1*pow(2,15)
        s   = hex_map[s[0]] + s[1:]

    num += int(s,16)
    return float(num)


# pan range = [f670,0990] = [-2448,2448] = [-170 degrees, 170 degrees]
# using the info above solve for pan in degrees
current_PT[0] = hex2int(pan_string) / 2448.0 * 170
    
# tilt range = [fcc4,03cc] = [-828, 972] = [+25 deg, -90 degrees]
# using the info above solve for the tilt in degrees
current_PT[1] = (hex2int(tilt_string) + 828.0) / (972.0+828.0) * (90.0+25.0) * -1.0 + 25

print "(Pan,Tilt) = (" + str(current_PT[0]) + "," + str(current_PT[1]) + ")"
