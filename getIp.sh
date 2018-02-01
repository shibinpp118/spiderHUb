#!/bin/bash
ip=$(hostname -I|cut -f2 -d ' ') 
echo $ip
