#!/bin/bash

cd /WiFi_For_SpiderHub
echo "Startup script entered..."
sleep 5;
./spider 2> /dev/null 2>&1 &
#./spider &
echo "Startup script running..."
sleep 3;
cd /root/Zwave/open-zwave/open-zwave-master/node_modules/openzwave-shared/
echo "Zwave Initialising...";
sleep 5;
node openzwave_mqtt.js;
