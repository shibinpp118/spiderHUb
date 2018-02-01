/************************************************************************************
 * Copyright 2017
 *
 * Authors:             Shibin, Suraj R, Uma Maheswari
 * Name:		Main.c
 * Description:         Main program for Spider Hub. Subcribes to topics publised from smartphone.
 *			Smartphone parses the subscribed json data and populates global variables
 *			Validates SpiderHub harware ID
 * Version:             Pre Alpha
 *gcc Application.c filesystem.c jsmn.c json.c jWrite.c lednotify.c User_management.c useroperation.c main.c -o spider -lpaho-mqtt3c
 *************************************************************************************/

#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <time.h>

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <net/if.h>
#include <arpa/inet.h>

#include "MQTTClient.h"
#include "json.h"
#include "jsmn.h"
#include "filesystem.h"
#include "Application.h"
#include "jWrite.h"
#include "ble.h"

/****Here goes the headers for log libraries and protocol handler libraries****/
/***public ip 111.93.133.142 ***/
char topic_name[50];
char gatewayid[10] = "12345";
unsigned char ip_addr[50];

#define ONLINE_ADDRESS   "tcp://111.93.133.142:1883"
#define OFFLINE_ADDRESS  "tcp://localhost:1883"
#define CLIENTID    "SpiderHub"
//#define TOPIC	    topic_name
#define PAYLOAD     "Json_data"
#define QOS         2
#define TIMEOUT     10000L

MQTTClient client;
volatile MQTTClient_deliveryToken deliveredtoken;
MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
MQTTClient_message pubmsg = MQTTClient_message_initializer;
MQTTClient_deliveryToken token;

char Mqtt_connect(int inter_stat);
int Online_status_publish();
char *content;
char* log_content;
//char gatewayid[10] = "12345";
int rc;
char temp[250];
int inter_stat = 0;
char mqt_buff_head = 0, mqt_buff_tail = 0, mqt_buff_head_follow = 0;
mqtt_packs mtt_msg_pack[50];
char Delivery_flag = 'T', mqtt_connection = 'F';
char off_line_flag = true;



/***********************************************************************************
 * function delivered
 * return type void
 * parameter void pointer
 *
 ***********************************************************************************/
void delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Success : Message with token value %d delivery confirmed\n", dt);
	deliveredtoken = dt;
	mqt_buff_tail++;
	Delivery_flag = 'T';
}

/***********************************************************************************
 *
 *
 *
 *
 ***********************************************************************************/
int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
 {
	int i, j;
	char* payloadptr;

	/*JSON PARSING*/
	//global_release();

	if (mqt_buff_head < MQTT_PACK_LMT)
	{
		printf("Success : message->payloadlen: %d received\n",strlen(message->payload));
		printf("MSG Buff : %s\n", message->payload);
		strcpy(mtt_msg_pack[mqt_buff_head].msg_load,message->payload);

	for(j=strlen(message->payload);j >= message->payloadlen;j--)//flushing extra buffer data
	{
		mtt_msg_pack[mqt_buff_head].msg_load[j] = 0;
	}

		mqt_buff_head++;
	}
	if ((mqt_buff_head_follow < (MQTT_PACK_LMT - 1)) && (mqt_buff_head >= MQTT_PACK_LMT) &&
		(mqt_buff_head_follow < mqt_buff_tail))
	{
		printf("Success : message->payloadlen: %d received\n",strlen(message->payload));
		strcpy(mtt_msg_pack[mqt_buff_head_follow].msg_load,message->payload);

	for(j=strlen(message->payload);j >= message->payloadlen;j--)//flushing extra buffer data
	{
		mtt_msg_pack[mqt_buff_head_follow].msg_load[j] = 0;

	}
		mqt_buff_head_follow++;
	}

	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);


 return 1;
 }

/*************************************************************************************
 *
 *
 ************************************************************************************/
void connlost(void *context, char *cause)
{
 printf("\n Error : Connection lost : %s\n", cause);
 //system("./main_kill.sh");    //it will run the script inside the c code.
 inter_stat = false;
 mqtt_connection = 'F';
}


/*************************************************************************************
 *
 *
 ************************************************************************************/
void set_ip()
{
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
	strcpy(ip_addr,inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr));
	//printf("System IP Address is: %s\n",ip_address);

}
/**************************************************************************************
*
*
**
**************************************************************************************/
char Mqtt_connect(int internet_stat)
  {
	char mqtt_status_flag = 'F';
	MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	char willTopic[50];
	//int rc;
	FILE *fileptr = fopen("internet_status.txt","w");
	int ch;
	if(internet_stat == 0)	{
	  MQTTClient_create(&client, OFFLINE_ADDRESS, CLIENTID,
	  MQTTCLIENT_PERSISTENCE_NONE, NULL);
	  printf("Success : Gateway_OFFLINE\n");
	  system("./bluetooth_voice_alert.sh I lost Internet: use me off line");
	  off_line_flag = true;
	  fprintf(fileptr,"%s","OFFLINE");	}
	else	{
	  MQTTClient_create(&client, ONLINE_ADDRESS, CLIENTID,
	  MQTTCLIENT_PERSISTENCE_NONE, NULL);
	  printf("Success : Gateway_ONLINE\n");
	  system("./bluetooth_voice_alert.sh I am going online");
	  inter_stat = 0;
	  off_line_flag = false;
	  fprintf(fileptr,"%s","ONLINE"); 	}
   	fclose(fileptr);
	conn_opts.keepAliveInterval = 30;
	conn_opts.cleansession = 1;
	conn_opts.will = &will_opts;
	set_ip();
	//printf("System IP Address is: %s\n",ip_addr);
	//will_opts.topicName = "Spider_Hub_will_topic";
	strcpy(willTopic,"Spider_Hub_will_topic/");
	strcat(willTopic,gatewayid);
	will_opts.topicName = willTopic ;
	//printf("will topic name: %s\n",will_opts.topicName);
	will_opts.message = ip_addr/*"Spider is disconnected from mqtt broker"*/;
 	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);
	sprintf(topic_name, "Spider/%s/#",gatewayid);
	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)	{
	  printf("Error : Failed to connect, return code %d\n", rc);
	  system("./bluetooth_voice_alert.sh Something wrong I am unable to contact server");
	  mqtt_status_flag = 'F';
	  return mqtt_status_flag;	}
//#if 0
		char shell_file_name[50];
		sprintf(shell_file_name,"./zwaveCall.sh %d",internet_stat);
		system(shell_file_name);
//#endif
	mqtt_status_flag = 'S';
	printf("\n\nSubscribing to topic %s for client %s using QoS%d***\n\n"
	"Press Q<Enter> to quit\n\n", topic_name, CLIENTID, QOS);

	MQTTClient_subscribe(client, topic_name, QOS);
	sleep(5);
	if(internet_stat != 0){
		flush_buffer();
		Gw_WeatherRequest();
		Online_status_publish();
	}

	return mqtt_status_flag;
}
/************************************************************************************
*	/*Main starts here
*	int main(int argc, char* argv[])
**
*************************************************************************************/
void main()
{
	int system_return = false;
	unsigned long time_now;
	int feedback;

	time_t t;
	z_wave_reset_count = 0;
	t = time(NULL); //Get latest Epoch Time

	printf("\n\n:::::SpiderHub Started:::::\n\n");
	if( access("registered_user_ids.txt", F_OK ) != -1 )  {
    	printf("Registered_user_ids DB file found!!!\n"); }
	else {
	 FILE *fptr = fopen("registered_user_ids.txt","w");
	 if (fptr == NULL)	{
	    printf("Error : Cannot open registered_user_ids file\n");	}
	 else	{
	    fputs("$",fptr);
	    fclose(fptr);
	    printf("Success : UserID DB created with $ as start symbol!!!\n");
	    }
	} // file doesn't exist

	jsn_hdr_topic();
/*	int test=0;
	sleep(2);
	system("./bluetooth_voice_alert.sh can you click Nano Switch?");
	sleep(2);
	while(test<=4)
	{
	char* nano = ble_data_read();
	printf("Checking Nano Switch input: %s\n",nano);

       // Returns first token
       char *token = strtok(nano, ",");
       //int gln=0;
       // Keep printing tokens while one of the
       // delimiters present in nano[].
        while (token != NULL)
        {
	if(gln==0){
        printf("gw_id[]: %s\n", token);
	//gw_id=token[];
	token = strtok(NULL, ",");
	gln++;
	}
	else if(gln==1){
        printf("proto_id[]: %s\n", token);
        //proto_id=token[];
        token = strtok(NULL, ",");
	gln=gln++;
        }
        else if(gln==2){
        printf("dev_type[]: %s\n", token);
        //dev_type=token[];
        token = strtok(NULL, ",");
	gln=gln++;
        }
        else if(gln==3){
        printf("cmd_id[]: %s\n", token);
        //cmd_id=token[];
        token = strtok(NULL, ",");
	gln=gln++;
	}
	}
	sleep(1);
	test=(test+1);
	gln=0;
	}
	test=0;
*/
#if 0
	MQTTClient_willOptions will_opts = MQTTClient_willOptions_initializer;
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	//int rc;
	int ch;

	MQTTClient_create(&client, ADDRESS, CLIENTID,
	MQTTCLIENT_PERSISTENCE_NONE, NULL);
	conn_opts.keepAliveInterval = 60;
	conn_opts.cleansession = 1;
	conn_opts.will = &will_opts;
	will_opts.topicName = "Spdier_Hub_will_topic";
	will_opts.message = "Spider is disconnected from mqtt broker";
	MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
		{
			printf("Error : Failed to connect, return code %d\n", rc);
			exit(EXIT_FAILURE);
		}

	printf("\n\nSubscribing to topic %s for client %s using QoS%d***\n\n"
	"Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);

	MQTTClient_subscribe(client, TOPIC, QOS);
#endif
	while(1)
	{
	t = time(NULL);
	if ((off_line_flag == true) && (t >= time_now + 30))	{
	    time_now = t;
		inter_stat = system("./Internet_connection.sh");
		printf("Success : Internet_status : %d\n", inter_stat);
	  if(inter_stat == 256){
		Gw_Auto_val_response("GATEWAY ONLINE");
		system("./bluetooth_voice_alert.sh I got internet");
		Online_status_publish();	}	}
	if ((mqtt_connection == 'F') || (inter_stat))	{
		 feedback = MQTTClient_disconnect(client, 10000);
		 sleep(10);
		 printf("mqtt status: %d\n",feedback);
		 mqt_buff_tail = mqt_buff_head = 0;
		 mqtt_connection = Mqtt_connect(inter_stat);	}

	  //inter_stat = system("./in");
	if ((mqt_buff_head > mqt_buff_tail) && (Delivery_flag == 'T'))	 {
		//printf("mqt_buff_head : %d mqt_buff_tail : %d \n",mqt_buff_head, mqt_buff_tail);
		flush_buffer();
		Delivery_flag = 'F';
		printf("mqt_buff_head : %d mqt_buff_tail : %d mqt_buff_head_follow : %d\n",mqt_buff_head, mqt_buff_tail, mqt_buff_head_follow);
		JSON_PARSER(mtt_msg_pack[mqt_buff_tail].msg_load);
		printf("Incoming json: %s\r\n",mtt_msg_pack[mqt_buff_tail].msg_load);
		file_write("Message arrived from server.","wifiauth");
		//printf("Command parsed-userId: %s\r\n",gw_id);
		file_write("Gateway Authenticated1","wifiauth");

	if((gw_id != NULL) && (strcmp(gw_id,gatewayid) == 0))	 {
		printf("Success : Gateway Authenticated!!!\n");
		file_write("Gateway Authenticated","wifiauth");
		system_return = system_handler(); /*Call Application program*/
	if (system_return == false)	  {
		Delivery_flag = 'T';
		if (mqt_buff_tail < mqt_buff_head)
		   mqt_buff_tail++;	  }	}/*Gateway Authentication*/

	else if((gw_id != NULL) && (strcmp(gw_id,gatewayid)!= 0))	{
		printf("Error : Gateway Authentication Failed : %s Mismatch\n", gw_id);
		file_write("Gateway Authentication Failed!!!","wifiauth");
		if(mqt_buff_tail < mqt_buff_head){
			mqt_buff_tail++;}
		Gw_Auto_val_response("INVALID : Invalid Gateway Id");	}
	else	{
		printf("Error : Not a Gateway auth Json!!!\r\n");
		if((mqt_buff_tail < mqt_buff_head)){
		  mqt_buff_tail++;	}
		Gw_Auto_val_response("INVALID : Invalid Msg format");	}

	if(Buff_Fill == true)	 {
		//printf("Response: %s\r\n",buffer_for_response);
		char* publish = NULL;
		publish = JSON_GENERATOR(buffer_for_response);
		pubmsg.payload = publish;
		pubmsg.payloadlen = strlen(publish);
		pubmsg.qos = QOS;
		pubmsg.retained = 0;
		MQTTClient_publishMessage(client,topic_publish, &pubmsg, &token);
		printf("Success : Waiting for up to %d seconds for publication of %s\n"
		"on topic %s for client with ClientID: %s\n",
		(int)(TIMEOUT/1000),publish , topic_publish, CLIENTID);
		printf("\nSuccess : Published\n");
		printf("Success : %ld \n", time(NULL));		 }
	else{
		 sleep(0.1);	}

	}
	  if (mqt_buff_tail >= MQTT_PACK_LMT){
	  	    	mqt_buff_head = mqt_buff_head_follow;
	  	    	mqt_buff_head_follow = mqt_buff_tail = 0;	  }
	}
  exit (0);
	//return rc;
}
/**********************************************************************************
 *Online_status_publish
 *
 *********************************************************************************/
int Online_status_publish()
{
	int status= 0;
	char* publish = NULL;
	publish = JSON_GENERATOR(buffer_for_response);
	pubmsg.payload = publish;
	pubmsg.payloadlen = strlen(publish);
	pubmsg.qos = QOS;
	pubmsg.retained = 0;
	status = MQTTClient_publishMessage(client,topic_publish, &pubmsg, &token);
	printf("Success : Waiting for up to %d seconds for publication of %s\n"
	"on topic %s for client with ClientID: %s\n",
	(int)(TIMEOUT/1000),publish , topic_publish, CLIENTID);
	printf("\nSuccess : Published\n");

	return status;
}

