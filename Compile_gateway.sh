#!/bin/bash

gcc Application.c bbg_gpio.c bbg_system_handler.c bbg_uart.c ble_handler.c filesystem.c jsmn.c json.c jWrite.c lednotify.c User_management.c  main.c -o spider -lpaho-mqtt3c

echo "Done"
exit 0
