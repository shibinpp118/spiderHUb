#!/bin/bash

# Check if "main" is running
# -x flag only match processes whose name (or command line if -f is
# specified) exactly match the pattern. 

while true;
do
sleep 1
if pgrep "spider" > /dev/null
	then
		echo "Main is Running"
else
	
	 cd /WiFi_For_SpiderHub/
	 sleep 2
	 echo "Main not Running"
    ./spider > /dev/null 2>&1
fi
done