#!/bin/bash
#echo "ip is : " 
ip=$(ifconfig wlan0 | grep "inet addr" | cut -d ':' -f 2 | cut -d ' ' -f 1)
echo $ip
