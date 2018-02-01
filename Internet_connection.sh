#!/bin/bash

# Initialize internet status
#int_status=0

#wget -q --tries=10 --timeout=20 --spider http://google.com
#wget -q --tries=3 --timeout=1 --spider http://google.com
#sleep 1
#if [[ $? -eq 0 ]]; then
#        echo "Online"
#	int_status=1
        #echo The value of variable int_status = $int_status
#else
#        echo "Offline"
#        int_status=0
	#echo The value of variable int_status = $int_status
#fi

# Return the captured return code as our script's exit status.
#exit $int_status

INTERNET_STATUS=0

    #ping -c 1 -W 0.7 8.8.4.4 > /dev/null 2>&1
    #timeout 5 'echo -e "GET google.com HTTP/1.0\n\n" | nc google.com 80 > /dev/null 2>&1'
    timeout 5 ping -c 1 -W 0.7 8.8.4.4 > /dev/null 2>&1
    if [ $? -eq 0 ] ; then
	#echo "Online"
	#INTERNET_STATUS=1
	echo $INTERNET_STATUS
	exit 1
    else
	#echo "Offline"
	#INTERNET_STATUS=0
	#echo $INTERNET_STATUS
	exit 0
    fi
    sleep 1
