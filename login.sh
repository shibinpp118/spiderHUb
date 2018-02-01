#!/bin/bash
pkill node
sleep 10
cd /root/Zwave/open-zwave/open-zwave-master/node_modules/openzwave-shared/
sleep 1
node openzwave_mqtt.js
