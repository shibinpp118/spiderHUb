/*
 * Copyright 2017
 *
 * Author:              Suraj
 * Description:         Program to subscribe to userID's from MQTT server and storing it into a file
 * Version:             Pre Alpha
 *
 */


#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"

#define ADDRESS     "localhost:1883"
#define CLIENTID    "ExampleClientSub"
#define TOPIC       "USER_ID"
#define PAYLOAD     "Hello World!"
#define QOS         1
#define TIMEOUT     10000L

volatile MQTTClient_deliveryToken deliveredtoken;

void delivered(void *context, MQTTClient_deliveryToken dt)
{
	 printf("Message with token value %d delivery confirmed\n", dt);
	 deliveredtoken = dt;
}

int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	 int i;
	 char* payloadptr;
         char* userid;
	 userid = (char *)malloc((5*sizeof(char)));
         int j=0;
	 FILE *fp;
	 FILE *userid_update_status;// new userid updated in file?
	 
	 /*Since the control came in msgarrvd, that info should be updated as status true in flag_user_ids.txt*/
	 userid_update_status = fopen("flag_user_ids.txt", "w+");
         fprintf(userid_update_status,"%s", "1");
         fclose(userid_update_status);
         printf("Status 1 updated to  flag_user_ids.txt file!!!\r\n");

	 printf("SpiderHub received a new Authenticated User ID\n");
	 printf("     topic: %s\n", topicName);
	 printf("   message: ");

	 payloadptr = message->payload;
	 for(i=0; i<message->payloadlen; i++)
	 {
		userid[j]=*payloadptr++;
		j++;
	 }
	 userid[j]='\0';
	 printf("message is %s\n",userid);
	 putchar('\n');
	 fp = fopen("registered_user_ids.txt", "a");
	 fprintf(fp,"%s", userid);
	 fprintf(fp,"%s", ",");
	 fclose(fp);
	 MQTTClient_freeMessage(&message);
	 MQTTClient_free(topicName);
	 return 1;
}

void connlost(void *context, char *cause)
{
	 printf("\nConnection lost\n");
	 printf("     cause: %s\n", cause);
}

int main(int argc, char* argv[])
{
	 MQTTClient client;
	 MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
	 int rc;
	 int ch;

	 MQTTClient_create(&client, ADDRESS, CLIENTID,
		  MQTTCLIENT_PERSISTENCE_NONE, NULL);
	 conn_opts.keepAliveInterval = 20;
	 conn_opts.cleansession = 1;

	 MQTTClient_setCallbacks(client, NULL, connlost, msgarrvd, delivered);

	 if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
	 {
		  printf("Failed to connect, return code %d\n", rc);
		  exit(-1);
	 }
	 printf("Subscribing to topic %s\nfor client %s using QoS%d\n\n"
			  "Press Q<Enter> to quit\n\n", TOPIC, CLIENTID, QOS);
	 MQTTClient_subscribe(client, TOPIC, QOS);

	 do
	 {
		  ch = getchar();
	 } while(ch!='Q' && ch != 'q');

	 MQTTClient_disconnect(client, 10000);
	 MQTTClient_destroy(&client);
	 return rc;
}
