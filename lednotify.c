
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "lednotify.h"

#if 0
int ledOn(int x);
int ledOff(int x);
int ledFlash(int x);
#endif

//TO TURN ON THE LED: 

int ledOn(int x)
{
 const char LEDBrightness[]="/sys/class/leds/beaglebone:green:";
 const char LED0[]="usr0/brightness";
 const char LED1[]="usr1/brightness";
 const char LED2[]="usr2/brightness";
 const char LED3[]="usr3/brightness";
 char  *LEDBrightness2=malloc(strlen(LEDBrightness)+strlen(LED0)+1);
 FILE *LEDHandle = NULL;
 if(x==1)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED0);
   //printf("%s",LEDBrightness2);
 }
 
 else if (x==2)
 {
    strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED1);
   //printf("%s",LEDBrightness2);
 }
 else if (x==3)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED2);
  // printf("%s",LEDBrightness2);
   
 }
  else if (x==4)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED3);
   //printf("%s",LEDBrightness2);
 }
 else
 {
  printf("Enter Led 1 to 4");
 }
 
if((LEDHandle = fopen(LEDBrightness2, "r+")) != NULL)
 	{
 	 //printf("hi");
 		fwrite("1", sizeof(char), 1, LEDHandle);
 		fclose(LEDHandle);
 	}
 	return 1;
}


//TO TURN OFF THE LED

int ledOff(int x)
{
 const char LEDBrightness[]="/sys/class/leds/beaglebone:green:";
 const char LED0[]="usr0/brightness";
 const char LED1[]="usr1/brightness";
 const char LED2[]="usr2/brightness";
 const char LED3[]="usr3/brightness";
 char  *LEDBrightness2=malloc(strlen(LEDBrightness)+strlen(LED0)+1);
 FILE *LEDHandle = NULL;
 if(x==1)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED0);
   //printf("%s",LEDBrightness2);
 }
 
 else if (x==2)
 {
    strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED1);
   //printf("%s",LEDBrightness2);
   
 }
 else if (x==3)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED2);
   //printf("%s",LEDBrightness2);
 
 }
  else if (x==4)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED3);
   //printf("%s",LEDBrightness2);
 }
 else
 {
  printf("Enter Led 1 to 4");
 }
 
if((LEDHandle = fopen(LEDBrightness2, "r+")) != NULL)
 	{
 	 //printf("hi");
 		fwrite("0", sizeof(char), 1, LEDHandle);
 		fclose(LEDHandle);
 	}
 	
 
 	return 1;

}


//TO BLINK THE LED

int ledFlash(unsigned int x, unsigned long ontime)

{
 const char LEDBrightness[]="/sys/class/leds/beaglebone:green:";
 const char LED0[]="usr0/brightness";
 const char LED1[]="usr1/brightness";
 const char LED2[]="usr2/brightness";
 const char LED3[]="usr3/brightness";
 char  *LEDBrightness2=malloc(strlen(LEDBrightness)+strlen(LED0)+1);
 FILE *LEDHandle = NULL;
 int i;
 if(x==1)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED0);
    printf("%s",LEDBrightness2);
 }

 else if (x==2)
 {
    strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED1);
   printf("%s",LEDBrightness2);
 }
 else if (x==3)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED2);
   printf("%s",LEDBrightness2);
 }
  else if (x==4)
 {
   strcat(LEDBrightness2,LEDBrightness);
   strcat(LEDBrightness2,LED3);
   printf("%s",LEDBrightness2);
 }
 else
 {
  printf("Enter first Led 1 to 4");
 }

 //for(i=0; i<2; i++)
 
 { 
 	if((LEDHandle = fopen(LEDBrightness2, "r+")) != NULL)
 	{
 		fwrite("1", sizeof(char), 1, LEDHandle);
 		fclose(LEDHandle);
 	}
 	printf("LED Flash on\n");
 	usleep(2000000);
 	
 	{
 	if((LEDHandle = fopen(LEDBrightness2, "r+")) != NULL)
 	{
 		fwrite("0", sizeof(char), 1, LEDHandle);
 		fclose(LEDHandle);
 	}
 	printf("LED Flash off\n");
 	usleep(1000000);
 	
  }
 printf("LED Flash End\n");
 }
 return x;
}
