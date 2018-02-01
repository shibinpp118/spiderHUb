/*
 * SpiderHub Project
 *
 * main.c
 *
============================================================================
 Description : Example code for usage of API for BLE.
============================================================================
*/

//============================================================================
// Name        : bbg_uart.cpp
// Author      : Abhinav Saxena
// Version     :
// Copyright   : Your copyright notice
// Description : Example code source for class serialib.
//               This example open the device on ttyO4.
//               (USB to RS232 converter under linux).
//               If the opening is successful, it sends the AT command
//               and waits for a string being received from the device.
//               After 10 seconds, if no valid data are received from the
//               device, reception is giving up.
//============================================================================
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include "ble.h"

int main()
{

	char* blesenddata = 'u';
	//char* blereceivedata;
	//int x=1;
	//char* c;
	int i;
	printf("BLE Test run\n");

	for(i=5;i>=0;i--)
	{


		ble_enable();
		printf("BLE Enable\n");
		sleep(5);

		//printf("Enter data to send over UART\n");
		//scanf("%s\n",&c);
		ble_data_write(&blesenddata);
		printf("BLE data write done\n");
		sleep(5);

//		ble_data_read();
//		printf("BLE data read done\n");
    	//sleep(5);

		ble_disable();
		printf("BLE Disable\n");

	    sleep(5);

}
	return 0;
}



