 /*
 * Copyright 2017
 *
 * Authors:            	Shibin P
 * Contributers:        Suraj R, Uma Maheshwari
 * Name:                Application.c
 * Description:         Middleware between main program and end protocols.
 *                      Processes the data.
 * Version:             Pre Alpha
 *
 * Add copyright notice here
 *
 */


#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Application.h"
#include "json.h"
#include "lednotify.h"
//#include "useroperation.h"
#include "ble.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

int gln=0;
char nocean_id[50]; 
//char buffer_for_response[100];
void weather_report_generator();
int xstrsearch ( char * s1, char * s2 );
rx_pack response_data[200];
Json_reg_topic json_hdr_count[50];
int cmmd;
unsigned char ip_address[16];
int net_status = 256;
extern char mqt_buff_head, mqt_buff_tail;
extern char  gatewayid[10];
enum head_count_t
{
	gw_reg_req,
	login_req,
	gw_hb_req,
	ser_hb_req,
	gw_detls_req,
	zwave_hb_intern,
	dev_reg_req,
	dev_config_req,
	ble_proto,
	bluetooth_proto,
	zwave_proto,
	zigbee_proto,
	inclusion,
	exclusion,
	audio,
	video,
	peripherals,
	config,
	on12,
	off12,
	on13,
	off13,
	onall,
	offall,
	cam_proto,
	search,
	stream,
	int_motion,
	up,
	down,
	left,
	right,
	stop,
	weather_report,
	enocean,
};

void jsn_hdr_topic(void)
{
	json_hdr_count[gw_reg_req].json_req_type = "GW_REG_REQ";
	json_hdr_count[login_req].json_req_type = "GW_LOGIN_REQ";
	json_hdr_count[gw_hb_req].json_req_type = "GW_HB_REQ";
	json_hdr_count[ser_hb_req].json_req_type = "SER_HB_REQ";
	json_hdr_count[gw_detls_req].json_req_type = "GW_DETLS_REQ";
	json_hdr_count[zwave_hb_intern].json_req_type = "ZWAVE_HB_INTERN";
	json_hdr_count[dev_reg_req].json_req_type = "DEV_REG_REQ";
	json_hdr_count[dev_config_req].json_req_type = "DEV_CONFIG_REQ";
	json_hdr_count[ble_proto].json_req_type = "BLE";
	json_hdr_count[bluetooth_proto].json_req_type = "BLUETOOTH";
	json_hdr_count[zwave_proto].json_req_type = "Zwave";
	json_hdr_count[zigbee_proto].json_req_type = "ZIGB";
	json_hdr_count[inclusion].json_req_type = "INCLUSION";
	json_hdr_count[exclusion].json_req_type = "EXCLUSION";
	json_hdr_count[audio].json_req_type = "AUDIO";
	json_hdr_count[video].json_req_type = "VIDEO";
	json_hdr_count[peripherals].json_req_type = "PERIPHERALS";
	json_hdr_count[config].json_req_type = "CONFIG";

	json_hdr_count[on12].json_req_type = "ON12";
	json_hdr_count[off12].json_req_type = "OFF12";
	json_hdr_count[on13].json_req_type = "ON13";
	json_hdr_count[off13].json_req_type = "OFF13";
	json_hdr_count[onall].json_req_type = "ONALL";
	json_hdr_count[offall].json_req_type = "OFFALL";
	json_hdr_count[cam_proto].json_req_type = "CAMERA";
    json_hdr_count[search].json_req_type = "SEARCH";
	json_hdr_count[stream].json_req_type = "STREAM";
	json_hdr_count[int_motion].json_req_type = "int_motion";
	json_hdr_count[up].json_req_type = "up";
	json_hdr_count[down].json_req_type = "down";
	json_hdr_count[left].json_req_type = "left";
	json_hdr_count[right].json_req_type = "right";
	json_hdr_count[stop].json_req_type = "stop";
	json_hdr_count[weather_report].json_req_type = "weather_report";
	json_hdr_count[enocean].json_req_type = "enocean";
 }

/*****************************************************************
*
*
*
****************************************************************/
int system_handler(void)
{
  int err_val = true, proto_dump = 0;

  //ledOn(2);
  switch(validate_req_type(gw_reg_req, req_type))
     {
     	case gw_reg_req :
     		 printf("Success : Request to ADD New User or Delete existing user\n");
     		 err_val = user_management(cmd_id, user_id);
     		 break;

     	case login_req :
     		 if (userid_auth() != 0){
     		 	Gw_Auto_val_response("Existing USER");}
     		 else{
     			Gw_Auto_val_response("Invalid USER");}
     		 err_val = true;
     		 break;
     	case gw_hb_req :
     		 //api call to send the heartbeat message
     		 printf("Gateway HeartBeat Request\n");
     		 err_val = Gw_HB_response(gw_hb_req);
     		 break;
     	case ser_hb_req :
     		 //api call to send the heartbeat message
     		 printf("Gateway HeartBeat Request from server\n");
     		 err_val = Gw_HB_response(ser_hb_req);
     		 break;
     	case gw_detls_req :
     		printf("Gateway Details Request\n"); 
		err_val = Gw_Details_response();
     		break;
     	case zwave_hb_intern :
     		 z_wave_status = 1;
     		 err_val = false;
     		 break;
     	case dev_reg_req :
     		 err_val = peripheral_iface_mgt(proto_id);
     		 break;
     	case dev_config_req :
     		 proto_dump = validate_req_type(ble_proto, proto_id);
     		 err_val = Device_configurations(proto_dump);
     		 break;
     	case weather_report :
     		 printf("*********weather_report**********\n");
     		 weather_report_generator();
     		 err_val = false;
     		break;
	case enocean :
		 printf("XXXXXXX Now you can play with bluetooth switch XXXXXXXX\n");
//                 bluetooth_switch();
		 //printf("*****returned from bluetooth switch ***********\n");
		 //proto_dump = validate_req_type(ble_proto, proto_id);
		 //err_val = Device_configurations(proto_dump);
                 //err_val = false;

//int bluetooth_switch()
//{
	    int test=0;
        sleep(2);
        system("./bluetooth_voice_alert.sh you can operate  nano switch now");
        sleep(2);
        while(test<1)
        {
        char* nano = ble_data_read();
        printf("Checking Nano Switch input: %s\n",nano);
#if 0
       // Returns first token
       char *token = strtok(nano, ",");
       printf("%s\n", token);
       //int gln=0;
       // Keep printing tokens while one of the
       // delimiters present in nano[].
  
     while (token != NULL)
        {
       // if(gln==0){
        printf("gw_id[]: %s\n", token);
        strcpy(gw_id,token);
        token = strtok(NULL, ",");
        //gln++;
       // }
        //else if(gln==1){
        printf("proto_id[]: %s\n", token);
        //strcpy(proto_id,token);
        token = strtok(NULL, ",");
        //gln++;
        //}
        //else if(gln==2){
        printf("dev_type[]: %s\n", token);
        //strcpy(dev_type,token);
        token = strtok(NULL, ",");
        //gln++;
        //}
        //else if(gln==3){
        printf("cmd_id[]: %s\n", token);
        strcpy(nocean_id,"ON12");
        token = strtok(NULL, ",");
        //gln++;
        //}
          //       proto_dump = validate_req_type(ble_proto, proto_id);
            //     err_val = Device_configurations(proto_dump);
        }
#endif
        sleep(1);
	if(xstrsearch(nano,"ON12") != -1)
	{
	strcpy(nocean_id,"ON12");
	}
	if(xstrsearch(nano,"ON13") != -1)
        {
        strcpy(cmd_id,"ON13");
        }
	if(xstrsearch(nano,"OFF12") != -1)
        {
        strcpy(cmd_id,"OFF12");
        }
	if(xstrsearch(nano,"OFF13") != -1)
        {
        strcpy(cmd_id,"OFF13");
        }


       proto_dump = validate_req_type(ble_proto, proto_id);
       err_val = Device_configurations(proto_dump);

        test=(test+1);
        gln=0;
        }
        test=0;

//        return 1;
//}

		 break;
     	default :
     		 printf("Success : Invalid request_type %s\n", req_type);
     		 err_val = false;
     		 break;

     }
 return err_val;

}
/*******************************************************************
* peripheral_iface_mgt : peripheral Packet related data handling
*
*
*******************************************************************/
int peripheral_iface_mgt(char proto[])
{
	int err_val = true;
	switch (validate_req_type(ble_proto, proto))
	{
	case ble_proto://BLE
		 err_val = ble_iface_mgt();
		 break;

	case bluetooth_proto: //Bluetooth
	     err_val = bluetooth_iface_mgt();
		 break;
	case zwave_proto:  //Zwave
		 printf("Success : Packages are not under list.. It is For Z-wave\n");
		 err_val = false;
		 ///mqt_buff_tail++;
	     break;
    case zigbee_proto:
    	 zigbee_iface_mgt();
		 err_val = false;
		 //mqt_buff_tail++;
	     break;
    default :
    	err_val = false;
    	break;
	}
	//err_val = device_management(net_status, status);

	return err_val;
}

/*******************************************************************
*validate_req_type : Packet related data handling
*
*
*******************************************************************/
int validate_req_type(int count, char *dt_type)
{
 int i = 0;

  for(i = count; i <= enocean; i++)
	{
	 if(strcmp(dt_type, json_hdr_count[i].json_req_type) == 0)	  {
	   printf("Hai %d %s %s\n", count, dt_type, json_hdr_count[i].json_req_type);
	   break;	}
	}
 return i;
}

 /*=================== ======================= =========================
 *ble_iface_mgt : .
 *return type is int for verification status
 *param is null
 ======================================================================*/
int ble_iface_mgt()
{
 int err_val = false, online_access = true;

 switch(validate_req_type(inclusion, dev_type))   		 {
  case inclusion :
	 if ((strcmp(cmd_id,"ADD") == 0) && (net_status))	 {
	     printf("Success : BLE Device to ADD\n");
	     system("./bluetooth_voice_alert.sh Spider, is ready to add device");
	     online_access = false;	 						 }
	  break;
  case exclusion :
 	 if ((strcmp(cmd_id,"DEL") == 0) && (net_status)) 	 {
 	     printf("Success : BLE Device to DELETE\n");
	     system("./bluetooth_voice_alert.sh Spider, is ready to delete device");
 	     online_access = false; 	 					 }
 	  break;
    }
  if(!net_status){
   err_val = Gw_Auto_val_response("INVALID : Do not Support in Offline");
   system("./bluetooth_voice_alert.sh Add or delete feature, not supported Offline");	}

 return err_val;
}

/*=================== ======================= =========================
*bluetooth_iface_mgt : .
*return type is int for verification status
*param is null
======================================================================*/
int bluetooth_iface_mgt()
{
 int err_val = false;
 switch(validate_req_type(inclusion, dev_type))
 {
  case inclusion :
	   if ((strcmp(cmd_id,"ADD") == 0) && (net_status))	       {
	   printf("Success : Bluetooth Device to ADD\n");
	   system("./bluetooth_voice_alert.sh Spider, is ready to add device"); }
	   break;
  case exclusion :
	   if ((strcmp(cmd_id,"DEL") == 0) && (net_status))	          {
	   printf("Success : Bluetooth Device to DELETE\n");
	   system("./bluetooth_voice_alert.sh Spider is ready to delete device"); }
	   break;
  default :
	   err_val = false;
	   break;
  }
 if (!net_status){
	 err_val = Gw_Auto_val_response("INVALID : Do not Support in Offline");
	 system("./bluetooth_voice_alert.sh Add or delete feature, not supported Offline");	}
return err_val;
}


/*=================== ======================= =========================
*zigbee_iface_mgt : .
*return type is int for verification status
*param is null
======================================================================*/
int zigbee_iface_mgt()
{
	int err_val = true;
	 switch(validate_req_type(inclusion, dev_type))
	 {
	  case inclusion :
		   if ((strcmp(cmd_id,"ADD") == 0) && (net_status))	   {
		   printf("Success :  Device to ADD\n");
		   system("./bluetooth_voice_alert.sh Spider, is ready to add device");	   }
		   break;
	  case exclusion :
		   if ((strcmp(cmd_id,"DEL") == 0) && (net_status))	   {
		   printf("Success : Zigbee Device to DELETE\n");
		   system("./bluetooth_voice_alert.sh Spider, is ready to delete device");	}
		   break;
	  default :
		   err_val = false;
		   break;
	  }
	 if (!net_status){
		 err_val = Gw_Auto_val_response("INVALID : Do not Support in Offline");
		 system("./bluetooth_voice_alert.sh Add or delete feature, not supported Offline");	}
	return err_val;
	}
/*=================== ======================= =========================
*bluetooth_iface_mgt : .
*return type is int for verification status
*param is null
======================================================================*/
int Device_configurations(int status)
{
 printf("entered device configurations\n");
 int err_val = true;
 char streaming_cmd[500] = {0};
 if (status == bluetooth_proto)						    {
  switch(validate_req_type(audio, dev_type))			{
  case audio :
	   if((strcmp(cmd_id,"ENABLE") == 0))	 	        {
		   system("./start_BLE.sh");
		   sleep(2);
		   printf("Success : BLUETOOTH Device Switched ON\n");
		   device_config_response("AUDIO ENABLE", proto_id);
		   system("./bluetooth_voice_alert.sh");
		   enable_ble = 'Y';				 	        }
	   else if ((strcmp(cmd_id,"DISABLE") == 0))	   	{
		   system("./stop_BLE.sh");
		   sleep(2);
		   device_config_response("AUDIO DISABLE", proto_id);
		   printf("Success : BLUETOOTH Device Switched OFF\n");
		   enable_ble = 'N';						   	}
	   else{
		   err_val = false;								}
	   break;
  case config :
	   if((strcmp(cmd_id,"PLAY") == 0))	  	            {
		   printf("Success : PLAY Music\n");
		   system("./start_music.sh");
		   device_config_response("CONFIG PLAY", proto_id);	  	}
	   else if((strcmp(cmd_id,"STOP") == 0))		    {
		   printf("Success : PAUSE Music\n");
		   system("./stop_music.sh");
		   device_config_response("CONFIG STOP", proto_id);		}
	   else if((strcmp(cmd_id,"PAUSE") == 0))		    {
		   printf("Success : PAUSE Music\n");
		   system("./pause_music.sh");
		   device_config_response("CONFIG PAUSE", proto_id);		}
	   else {
		   err_val = false;	}
	   break;
   default :
 	   	   err_val = false;
 	   break;
  }	}
 else if (status == ble_proto)						    {
  switch(validate_req_type(audio, dev_type))			{
   case peripherals :
	 if((strcmp(cmd_id,"ENABLE") == 0))	 	        	{
		 //call ble_gpio_enable
		ble_enable();
		printf("BLE Enable\n");
		sleep(2);
		device_config_response("BLE ENABLE", proto_id);			}
	 else if ((strcmp(cmd_id,"DISABLE") == 0))	   		{
		 //Disable ble_gpio
		ble_disable();
		printf("BLE Disable\n");
		sleep(2);
		device_config_response("BLE DISABLE", proto_id);			}
   break;
   case config :
	   if((strcmp(cmd_id,"UP") == 0))	  	            {
		   //send "up" command to ble for device drive
		ble_data_write("up");
		printf("BLE data write done\n");
		sleep(2);
		device_config_response("UP", proto_id);					}
	   else if((strcmp(cmd_id,"DOWN") == 0))	  	    {
		   //send "down" command to ble for device drive
		ble_data_write("down");
		printf("BLE data write done\n");
		sleep(2);
		device_config_response("DOWN", proto_id);					}
	   break;
   default :
	    device_config_response("NOT SUPPORT", proto_id);
	    printf("Success : Audio not support\n");
	    err_val = false;
	    break;
	 }
 }

 else if (status == cam_proto){
	 printf("Entered for camera operation..................\n");
    switch(validate_req_type(search, dev_type))
    {
    case search :
     printf("Searching for Cameras..................\n");
    system("./get-onvif-cameras.sh");
    char cam_name[1000];
    FILE *cam = fopen("onvif-camera-list.txt","r");
         if (cam == NULL)      {
            printf("Camera list file missing!\n");   }
         else   {
            fscanf(cam,"%[^\n]", cam_name);
            fclose(cam);
            printf("Camera list: %s\r\n",cam_name);        }

    device_config_response(cam_name, proto_id);
    break;
    //Restream to another IP, not activated currently
    case stream :
	printf("userip::::::::: %s\r\n",UserIP);
	strcpy(streaming_cmd,"ffmpeg -rtsp_transport tcp -i rtsp://");
	strcat(streaming_cmd,CameraIP);
	strcat(streaming_cmd,":10554/tcp/av0_0 -preset slow -c:v copy -f mpegts udp://");
	strcat(streaming_cmd,UserIP);
	strcat(streaming_cmd,":1111 -hide_banner");
	printf("Streaming Command >>>>>>>>>>>>>>>>>>>>>>>: %s\r\n",streaming_cmd);
	system(streaming_cmd);// "> /dev/null 2>&1 & disown"
 	err_val = false;
    break;

    case int_motion :
        printf("");
	char cam_motion[200]="";
	printf("Enabling camera controls, Please wait.........\n");
	strcpy(cam_motion,"./Camera_motion.sh ");
	strcat(cam_motion, CameraIP);
	strcat(cam_motion, "> /dev/null 2>&1 & disown");
	//char cam_motion[200]= "./Camera_motion.sh 192.168.100.158 > /dev/null 2>&1 & disown";
	printf("Camera_call ------------->: %s\r\n",cam_motion);
        system(cam_motion);
        device_config_response("Enabling camera controls this will take 1 minute Please wait", proto_id);
	system("./bluetooth_voice_alert.sh Enabling camera controls. This will take 1 minute. Please wait");
    break;

    case up :
	printf("Camera is moving up^^^^^^^^^^^\n");
	printf("camera direction::::::::: %s\r\n",dev_type);
        system("echo up > /WiFi_For_SpiderHub/onvif/python-onvif/examples/Direction_file.txt");
        err_val = false;
    break;
    case down :
	printf("Camera is moving down.........\n");
        system("echo dn > /WiFi_For_SpiderHub/onvif/python-onvif/examples/Direction_file.txt");
        err_val = false;
    break;
    case left :
	printf("Camera is moving left<<<<<<<<<\n");
        system("echo lt > /WiFi_For_SpiderHub/onvif/python-onvif/examples/Direction_file.txt");
        err_val = false;
    break;
    case right :
	printf("Camera is moving right>>>>>>>>>\n");
        system("echo rt > /WiFi_For_SpiderHub/onvif/python-onvif/examples/Direction_file.txt");
        err_val = false;
    break;
    case stop :
	printf("Killed Streaming x-x-x-x-x-x-x-x-x\n");
        system("pkill ffmpeg");
        err_val = false;
    break;
    default :
//enter field here
	printf("Invalid Camera Command!!!\n");
    	err_val = false;
    break;
     }
}

 else if (status == zigbee_proto)					    {
	printf("Entered to zigbee ..................dev:type : %s\n", dev_type);

    switch(validate_req_type(audio, dev_type))			{
    case peripherals :
 	 if((strcmp(cmd_id,"ENABLE") == 0))	 	        	{
 		printf("ZIGB Enable\n");
 		sleep(0.5);
 		device_config_response("ZIGB ENABLE", proto_id);			}
 	 else if ((strcmp(cmd_id,"DISABLE") == 0))	   		{
 		printf("ZIGB Disable\n");
 		sleep(1);
 		device_config_response("ZIGB DISABLE", proto_id);			}
 	 else										        {
 	 		  err_val = false;					        }
    break;
    case config :
    	printf("testing enocean %s\n", nocean_id); //shibin /should be deleted
    	if((strcmp(cmd_id,"ON12") == 0) ||(strcmp(nocean_id,"ON12") == 0))
        	{
 		printf("ZIGBEE data write done\n");
			//zigb_data_write(zb_d2_on);
			zigb_pkt_buffer(18,01,01);
			zigb_data_write(zb_pkt_buff);
			sleep(1);
			memset(nocean_id, 0, 49);
			device_config_response("ON12", proto_id);
		}
 	   else if(strcmp(cmd_id,"OFF12") == 0)	  	   		{
			printf("ZIGBEE data write done\n");
			zigb_pkt_buffer(18,00,01);
			zigb_data_write(zb_pkt_buff);
			sleep(1);
			device_config_response("OFF12", proto_id);			}
 	   else if(strcmp(cmd_id,"ON13") == 0)	  	        {
 	   		printf("ZIGBEE data write done\n");
 	   		//zigb_data_write(zb_d2_on);
 	   		zigb_pkt_buffer(19,01,01);
 	   		zigb_data_write(zb_pkt_buff);
 	   		sleep(1);
 	   		device_config_response("ON13", proto_id);			    }
 	   else if(strcmp(cmd_id,"OFF13") == 0)	  	   		{
 	   		printf("ZIGBEE data write done\n");
 	   		zigb_pkt_buffer(19,00,01);
 	   		zigb_data_write(zb_pkt_buff);
 	   		sleep(1);
 	   		device_config_response("OFF13", proto_id);			}
 	   else if(strcmp(cmd_id,"ONALL") == 0)				{
			printf("ZIGBEE data write done\n");
			//zigb_data_write(zb_dAll_on);
			zigb_pkt_buffer(18,01,04);
			zigb_data_write(zb_pkt_buff);
			sleep(1);
			device_config_response("ONALL", proto_id);			}
 	  else if(strcmp(cmd_id,"OFFALL") == 0)				{
			 printf("ZIGBEE data write done\n");
			 //zigb_data_write(zb_dAll_off);
			 zigb_pkt_buffer(18,00,04);
			 zigb_data_write(zb_pkt_buff);
			 sleep(1);
			 device_config_response("OFFALL", proto_id); 	  	  	}
 	  else										 	    {
 			printf("Invalid cmd\n\n");//need to delete
 		  	  err_val = false;						 	}
 	  break;
    default :
 	    	device_config_response("NOT SUPPORT", proto_id);
 	    	printf("Success : Zigbee not support\n");
 	    	err_val = false;
 	  break;
 	 	 	 	 	 	 	 	 	 	 	 	 	 	 }
   }
 else					{
	 err_val = false;	}
 return err_val;
 }


void zigb_pkt_buffer(char id, char val, char type)
{
	int i = 0;
	/*0x2D,0x13,0x01,0x3C,0xC1,0xF6,0x12,0x00,0x00,0x00,0x12,0xFF,0xFF,0x08,0x08,0x00,0x06,0x11,0x02,0x00*/
	zb_pkt_buff[i++] = 0x2D;		//SOP : Start Of Packet
	zb_pkt_buff[i++] = 0x13;		//Packet Length excluding SOP
	zb_pkt_buff[i++] = type;		//01: IEEE Addr based, 04: N/W Addr based
	zb_pkt_buff[i++] = 0x3C;		//
	zb_pkt_buff[i++] = 0xC1;		//
	zb_pkt_buff[i++] = 0xF6;		//
	zb_pkt_buff[i++] = 0x12;		//
	zb_pkt_buff[i++] = 0x00;		//
	zb_pkt_buff[i++] = 0x00;		//
	zb_pkt_buff[i++] = 0x00;		//
	zb_pkt_buff[i++] = id;			//8 bytes of IEEE addr
	zb_pkt_buff[i++] = 0xFF;		//
	zb_pkt_buff[i++] = 0xFF;		//2 bytes of N/W addr
	zb_pkt_buff[i++] = 0x08;		//Source id
	zb_pkt_buff[i++] = 0x08;		//Dest Id
	zb_pkt_buff[i++] = 0x00;		//
	zb_pkt_buff[i++] = 0x06;		//
	zb_pkt_buff[i++] = 0x11;		//
	zb_pkt_buff[i++] = 0x02;		//4 bytes reserved
	zb_pkt_buff[i++] = val;			//00: OFF, 01: ON, 02: Toggle
 }

/*==========================================================
* Auto validtion response implementation function.
* void return type
* No more parameters as if now
===========================================================*/
int Gw_Auto_val_response(char* val)
{
	int loop = 0;

	response_data[loop].key= "req_type";
	response_data[loop].value = "GW_AUTO_RES";
	response_data[++loop].key= "Gw_Id";
	response_data[loop].value = gw_id;
	response_data[++loop].key= "trans_id"; //collected from Req message
	response_data[loop].value = trans_id;
	response_data[++loop].key= "user_Id";
	response_data[loop].value = (user_id ) ? user_id : "0000";
	response_data[++loop].key= "message";
	response_data[loop].value = val;
	response_data[++loop].key= "timeStamp";
	response_data[loop].value = timestamp; //time stamp should be updated
	//printf("Invalid User check response buffer:done\r\n");

	strcpy(topic_publish,"GW_AUTO_RES/12345"); // give the topic to publish

	create_string_4json(loop);

  return 1;

}
#if 1

/*==========================================================
* device_config_response response implementation function.
* void return type
* No more parameters as if now
===========================================================*/
int device_config_response(char* val, char* protocol)
{
 int loop = 0;

	response_data[loop].key= "req_type";
	response_data[loop].value = "DEV_CONFIG_RES";
	response_data[++loop].key= "gw_id";
	response_data[loop].value = gw_id;
	response_data[++loop].key= "trans_id"; //collected from Req message
	response_data[loop].value = trans_id;
	response_data[++loop].key= "user_id";
	response_data[loop].value = user_id;
	response_data[++loop].key= "proto_id";
	response_data[loop].value = protocol;
	response_data[++loop].key= "message";
	response_data[loop].value = val;
	response_data[++loop].key= "timestamp";
	response_data[loop].value = timestamp; //time stamp should be updated
	//printf("Invalid User check response buffer:done\r\n");

	strcpy(topic_publish,"DEV_CONFIG_RES/12345"); // give the topic to publish

	create_string_4json(loop);

return 1;
}
/*==========================================================
* Heart beat response implementation function.
* void return type
* No more parameters as if now
===========================================================*/
int Gw_HB_response(int topic_name)
{
	char Buf_1[50], Buf_2[50];
	int loop = 0;
 	char ip1[16]; 

	response_data[loop].key= "req_type";
	response_data[loop].value = "GW_HB_RES";
	response_data[++loop].key= "gw_id";
	response_data[loop].value = gw_id;
	response_data[++loop].key= "transId"; //collected from Req message
	response_data[loop].value = trans_id;
	response_data[++loop].key= "message";

   // unsigned char ip_address[16];
    int fd;
    struct ifreq ifr;

    /*AF_INET - to define network interface IPv4*/
    /*Creating soket for it.*/
    fd = socket(AF_INET, SOCK_DGRAM, 0);

    /*AF_INET - to define IPv4 Address type.*/
    ifr.ifr_addr.sa_family = AF_INET;

    /*eth0 - define the ifr_name - port name
    where network attached.*/
    memcpy(ifr.ifr_name, "wlan0", IFNAMSIZ-1);

    /*Accessing network interface information by
    passing address using ioctl.*/
    ioctl(fd, SIOCGIFADDR, &ifr);
    /*closing fd*/
    close(fd);

    /*Extract IP Address*/
    strcpy(ip_address,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
    //printf("System IP Address is: %s\n",ip_address);
    
    response_data[loop].value = "pong";//arr;
	response_data[++loop].key= "timeStamp";
	response_data[loop].value = timestamp; //time stamp should be updated
	//printf("Success : Response buffer:done\r\n");
	if(topic_name == ser_hb_req)
		strcpy(topic_publish,"SER_HB_RES/12345"); // give the topic to publish
	else
		strcpy(topic_publish,"GW_HB_RES/12345"); // give the topic to publish


	create_string_4json(loop);

  return 1;
}

int Gw_WeatherRequest()
{
  char loop = 0;

	response_data[loop].key= "city_name";
	response_data[loop].value = "Bangalore";
	response_data[++loop].key= "gw_id";
	response_data[loop].value = gatewayid;
	strcpy(topic_publish,"get_weather_report/12345"); //give the topic to publish
	create_string_4json(loop);
  return 1;
}


/*==========================================================
* Gw Details response implementation function.
* void return type
* No more parameters as if now
===========================================================*/
int Gw_Details_response()
{
  char loop = 0;

	response_data[loop].key= "req_type";
	response_data[loop].value = "GW_DETLS_RES";
	response_data[++loop].key= "gw_id";
	response_data[loop].value =  gatewayid;
	response_data[++loop].key= "trans_id"; //collected from Req message
	response_data[loop].value = trans_id;
	response_data[++loop].key= "gw_detls";
	response_data[loop].value = "Listed";
	response_data[++loop].key = "lan";
	response_data[loop].value =  "0";
	response_data[++loop].key = "wlan";
	response_data[loop].value = ip_address ;
	response_data[++loop].key = "interfaces";
	response_data[loop].value =  "0";
	response_data[++loop].key = "hm_id";
	response_data[loop].value =  "0";
	response_data[++loop].key = "memory";
	response_data[loop].value = "0" ;
	/*gateway details has to be added inside*/
	response_data[++loop].key= "timestamp";
	response_data[loop].value = timestamp; //time stamp should be updated

	strcpy(topic_publish,"GW_DETLS_RES/12345"); // give the topic to publish

	create_string_4json(loop);
  return 1;
}

#endif

/*******************************************************************************
 * create_string_4json to create single buffer for json creation.
 *
 *******************************************************************************/
void create_string_4json(int length)
{	
	int loop;
	//flush_buffer();
	for( loop = 0; loop <= length; loop++)
	{
		sprintf(buffer_for_response+strlen(buffer_for_response),"%s,",response_data[loop].key);
		sprintf(buffer_for_response+strlen(buffer_for_response),"%s,",response_data[loop].value);
	}
	printf("Success : created string_4_json\r\n");
	Buff_Fill = true;
}
/****************************************************************************************
 * flush_buffer : is implemented for cearing response buffer after publishing messages. 
 * return type integer
 *
 ****************************************************************************************/
void flush_buffer()
{
 unsigned int loop = 0;
 loop = strlen(buffer_for_response);// flush the buffer with earlier data
	while ( loop--  != 0 )	{
	 buffer_for_response[loop] = 0;		}
  global_release();
  Buff_Fill = false; // make Buff_Fill flag false to for new set of commands
  printf("\nSuccess : Input buffer flushed\n");
}

/************************************************************************************
*Release all the globals
*
***********************************************************************************/

void global_release()
{
    unsigned int val = 100;
    char num = 0x00;

    while(val-- != 0)
    {
    	cmd_type[val -1] = 0;
    	cmd_id[val-1] = 0;
    	trans_id[val-1] = 0;
    	user_id[val-1] = 0;
    	gw_id[val-1] = 0;
    	command[val-1] = 0;
    	timestamp[val-1] = 0;
    	response[val-1] = 0;
    	scan_device[val-1] = 0;
    	device_name[val-1] = 0;
    	mac[val-1] = 0;
    	location[val-1] = 0;
    	favorites[val-1] = 0;
    	manufacture_name[val-1] = 0;
    	manufacture_id[val-1] = 0;
    	dev_type[val-1] = 0;
    	req_type[val-1] = 0;
    	req_id[val-1] = 0;
    	err_id[val-1] = 0;
    	message[val-1] = 0;
    	role[val-1] = 0;
    	proto_id[val-1] = 0;
    	cmd_log[val-1] = 0;
    	//topic_publish[val-1] = 0;
    	memory[val-1] = 0;

    }

}

/***********************************************************
 * *
 *
 **********************************************************/
void weather_report_generator()
{
char data[200] = {0};

//printf("hai\n");
sprintf(data, "./bluetooth_voice_alert.sh ");
sprintf(data + 27,"Current city temperature %s",kelvin);
sprintf(data + 54," and humidity is %s",humidity);
sprintf(data + 73,"");
//printf("%s\n", data);
system(data);
}

/*
int bluetooth_switch()
{	int test=0;
        sleep(2);
        system("./bluetooth_voice_alert.sh you can operate  nano switch now");
        sleep(2);
	while(test<1)
        {
        char* nano = ble_data_read();
        printf("Checking Nano Switch input: %s\n",nano);

       // Returns first token
       char *token = strtok(nano, ",");
       printf("%s\n", token);
       //int gln=0;
       // Keep printing tokens while one of the
       // delimiters present in nano[].
        while (token != NULL)
        {
        if(gln==0){
        printf("gw_id[]: %s\n", token);
        strcpy(gw_id,token);
        token = strtok(NULL, ",");
        gln++;
        }
        else if(gln==1){
        printf("proto_id[]: %s\n", token);
        //strcpy(proto_id,token);
        token = strtok(NULL, ",");
        gln++;
        }
        else if(gln==2){
        printf("dev_type[]: %s\n", token);
        //strcpy(dev_type,token);
        token = strtok(NULL, ",");
        gln++;
        }
        else if(gln==3){
        printf("cmd_id[]: %s\n", token);
        strcpy(cmd_id,token);
        token = strtok(NULL, ",");
        gln++;
        }
                 proto_dump = validate_req_type(ble_proto, proto_id);
                 err_val = Device_configurations(proto_dump);
        }
        sleep(1);
        test=(test+1);
        gln=0;
        }
        test=0;

	return 1;
}
*/


/* searches for the given pattern s2 into the string s1 */
int xstrsearch ( char * s1, char * s2 )
{
    int i, j, k ;
    int l1 = strlen ( s1 ) ;
    int l2 = strlen ( s2 ) ;

    for ( i = 0 ; i <= l1 - l2 ; i++ )
    {
        j = 0 ;
        k = i ;
        while ( ( s1[k] == s2[j] ) && ( j < l2 ) )
        {
            k++ ;
            j++ ;
        }
        if ( j == l2 )
            return i ;
    }
    return -1 ;
}
