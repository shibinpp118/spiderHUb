/*
 * SpiderHub Project
 *
 * ble.c
 *
 *  This code contain all API related to BLE communication at UART4 and /dev/ttyO4
 *
 *  ble_data_read();
 *  - This is to read data from BLE which is connected to RX of UART4 of BBG, Timout is 5 sec.
 *
 *  ble_data_write();
 *  - This is to write data to BLE which is connected to TX of UART4 of BBG.
 *
 *  ble_enable();
 *  - This is to switch ON BLE device on BBG.
 *
 *  ble_disable();
 *  - This is to switch OFF BLE device on BBG.
 */
#include <stdio.h>
#include "core.h"
#include "gpio.h"
#include "ble.h"
#include "uart.h"

#define LEDGPIO 60;


char* ble_data_read()
{
	static unsigned char receive[100];

	init_bbc_lib();
	uart_properties *uart = malloc(sizeof(uart_properties));
	uart->uart_id = uart4;
	uart->baudrate = B115200;

	uint8_t isOpen = uart_open(uart);


		if (isOpen == 0)
		{
	// Read data from uart1
	if (uart_read(uart, receive, 100) < 0)
	{
		printf("Could not read data from uart port\n");
		return (char*)-1;
	}

	printf("Read: %s\n",receive);
	usleep(500000);

uart_close(uart);

		}
		 else
		 {
			printf("isOpen failed!!!\n");
		 }
		return (char*)receive;
}

int ble_data_write(char* c)
{

	init_bbc_lib();

	uart_properties *uart = malloc(sizeof(uart_properties));

	uart->uart_id = uart4;
	uart->baudrate = B115200;
	//printf("Test1\n");
	uint8_t isOpen = uart_open(uart);

	if (isOpen == 0)
	{

	char buf[30]={0};
	char buf1[30]={0};
	char buf2[30]={0};


	sprintf(buf, "u");
	sprintf(buf1, "\r");
	sprintf(buf2, "\n");
	int x = uart_send(uart, c, 1);
	uart_send(uart, buf1, 1);
	uart_send(uart, buf2, 1);


	// Send data to uart4
	if (x < 0)
	{
		printf("Could not send data to uart port\n");
		return -1;
	}
	;
	//usleep(100000);
	uart_close(uart);
}
	else {
	printf("isOpen failed!!!\n");
}

free(uart);
	return 0;
}

int zigb_data_write(char* c)
{
  int i = 0;
	init_bbc_lib();

	uart_properties *uart = malloc(sizeof(uart_properties));

	uart->uart_id = uart1;
	uart->baudrate = B9600;
	//printf("Test1\n");
	uint8_t isOpen = uart_open(uart);

	if (isOpen == 0)
	{

	int x = uart_send(uart, c, 20);
	sleep(0.5);

	// Send data to uart2
	if (x < 0)			{
		printf("Could not send data to uart port\n");
		return -1;		}
	//usleep(100000);
	uart_close(uart);
	}
	else 								{
	printf("isOpen failed!!!\n");		}

  free(uart);
  return 0;
}

int ble_enable()
{
	init_bbc_lib();
	gpio_properties *gpio = malloc(sizeof(gpio_properties));
	gpio->nr = LEDGPIO;
	gpio->direction = OUTPUT_PIN;
	int isOpen = gpio_open(gpio);

		if (isOpen == 0)
		{
	gpio_set_value(gpio, 1);
		}
	//gpio_close(gpio);
	free(gpio);

	return 0;
}

int ble_disable()
{
	init_bbc_lib();
	gpio_properties *gpio = malloc(sizeof(gpio_properties));
	gpio->nr = LEDGPIO;
	gpio->direction = OUTPUT_PIN;
	int isOpen = gpio_open(gpio);
	if (isOpen == 0)
			{
		gpio_set_value(gpio, 0);
			}
		//gpio_close(gpio);

	free(gpio);

	return 0;
}




/*

#include <stdio.h>
#include "serialib.h"
#include "ble.h"


#ifdef __linux__
#define         DEVICE_PORT             "/dev/ttyO4"                         // ttyO4 for linux
#endif





char* ble_data_read()
{
	serialib LS;                                                            // Object of the serialib class
	int Ret;                                                                // Used for return values
	     static char Buffer[128]="";

	    // Open serial port

	    Ret=LS.Open(DEVICE_PORT,115200);                                        // Open serial link at 115200 bauds
	    if (Ret!=1) {                                                           // If an error occurred...
	        printf ("Error while opening port. Permission problem ?\n");        // ... display a message ...
	  //      return r;                                                         // ... quit the application
	    }
	    //printf ("Serial port opened successfully !\n");

   	    Ret=LS.ReadString(Buffer,'\n',128,10000);                      // Read a maximum of 128 characters with a timeout of 10 seconds
	                                                                            // The final character of the string must be a line feed ('\n')
	    if (Ret>0){                                                              // If a string has been read from, print the string
	        printf ("String read from serial port : %s",Buffer);
	    }
	    else
	        printf ("TimeOut reached. No data received !\n");                   // If not, print a message.

	    // Close the connection with the device


	    LS.Close();

	    return Buffer;


}

int ble_data_write(char * String)
{
	serialib LS;                                                            // Object of the serialib class
	int Ret;                                                                // Used for return values
	     static char Buffer[128]="";

	    // Open serial port

	    Ret=LS.Open(DEVICE_PORT,115200);                                        // Open serial link at 115200 bauds
	    if (Ret!=1) {                                                           // If an error occurred...
	        printf ("Error while opening port. Permission problem ?\n");        // ... display a message ...
	  //      return r;                                                         // ... quit the application
	    }
	    //printf ("Serial port opened successfully !\n");



  	    Ret=LS.WriteString(String);// Send the command on the serial port
  	    LS.WriteString("\r\n");
  	    if (Ret!=1)
   	    {                                                           // If the writing operation failed ...
   	        printf ("Error while writing data\n");                              // ... display a message ...
   	    //    return r;                                                         // ... quit the application.
   	    }
   	   // printf ("Write operation is successful \n");



   	    Ret=LS.ReadString(Buffer,'\n',128,10000);                      // Read a maximum of 128 characters with a timeout of 10 seconds
	                                                                            // The final character of the string must be a line feed ('\n')
	    if (Ret>0){                                                              // If a string has been read from, print the string
	        printf ("String read from serial port : %s",Buffer);
	    }
	    else
	        printf ("TimeOut reached. No data received !\n");                   // If not, print a message.

	    // Close the connection with the device

	    LS.Close();

	    return 0;
}
*/

