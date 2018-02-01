#!/bin/bash
cd /root/Zwave/open-zwave/node_modules/openzwave-shared/
if [ $1 == 0 ]
then 
	echo "run zwave code with localhost"
	pkill node
	node openzwave_mqtt_localhost.js > /dev/null 2>&1 &
else
	if [ $1 == 256 ]
	then
		echo "run zwave code with public ip"
		pkill node
		node openzwave_mqtt.js > /dev/null 2>&1 &
	else
		echo "invalid args/no args: $1"
	fi
fi