#!/bin/bash

#SERVER_STATUS=1

if sshpass -p terralogic ssh -qn uma@111.93.133.142 pidof node-red &>/dev/null ; then
     echo "Server is up";
     #INTERNET_STATUS=1
     #echo $SERVER_STATUS
     exit 1;
else
     echo "Server is down";
     #INTERNET_STATUS=0
     #echo $SERVER_STATUS
     exit 0;
fi
