/*************************************************************************
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
 ***********************************************************************************/
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Application.h"
#include "json.h"
//#include "useroperation.h"

extern rx_pack response_data[200];

char flag_userid_update = 1;
char content[200];
int flag_boot_userid_update = 1;
/*******************************************************************
* user_management : Packet related data handling
*
*
*******************************************************************/
int user_management(char *cmd, char *usr_)
{
 int err_val = true;

 if (strcmp(cmd,"ADD") == 0)
  {
   if (add_User_id() != 0)	{
	  printf("Success : New user added \n");
	  User_Id_res(1, 1); } //response as user added
   else	 {
	  printf("Error : Unable to Add New user\n");
	  User_Id_res(1, 0);  }//response as user  not added
  }
 else if (strcmp(cmd,"DEL")==0)
  {
   printf("DEBUG :Delete user .......\n");
   if (file_delete_userid(user_id) == 0) {// checks if user_id is deleted
	  printf("Success : user deleted from Spider\n");
	  User_Id_res(0, 1); } //response as user added
   else	{
	  printf("Error : Unable to Delete user from Spider\n");
	  User_Id_res(0, 0);  } //response as user  not added
  }
 else{
	  err_val = false;}

 return err_val;
}


/*=================== Add user id to DataBase =========================
*add_User_id : is used to log new registered user in gw DB.
*return type is int for verification status
*param is null
======================================================================*/
int add_User_id()
{
	//printf("..........testing in adduser()..........\r\n");
	FILE *fptr = fopen("registered_user_ids.txt","a+");
	if (fptr == NULL)	{
		printf("Error : Cannot open registered_user_ids file\n");
		return 0;  		}
	else				{
		fputs(user_id,fptr);
		fputs(",",fptr);
		fclose(fptr);
		flag_userid_update = 1;
		return 1; 		}
}

/*==========================================================
* User addition response implementation function.
* void return type
* One parameter to pass :For add user or Delete User.
===========================================================*/
void User_Id_res(int resp, int param)
 {
 	unsigned int i = 0;
 	//printf("creating response for user_ID_Reg..........\r\n");

	response_data[i].key= "req_type";
	response_data[i].value = "GW_REG_RES";
	response_data[++i].key= "gw_id";
	response_data[i].value = gw_id;
	response_data[++i].key= "trans_id"; //collected from Req message
	response_data[i].value = trans_id;
	response_data[++i].key= "user_id";
	response_data[i].value = user_id;
	response_data[++i].key= "err_id";
	response_data[i].value = VALID;
	response_data[++i].key= "message";

	switch(resp)
	{
	 case 0:
		if (param == 1)	{
		   response_data[i].value = "USER DELETED";	}
		else	{
		   response_data[i].value = "USER Not DELETED";	}
        break;
	 case 1:
		if (param == 1)	{
		   response_data[i].value = "USER ADDED";	}
		else	{
		   response_data[i].value = "USER Not ADDED";	}
		break;
	 default :
		break;
	}
	response_data[++i].key= "timestamp";
	response_data[i].value = timestamp; //time stamp should be updated
	//printf("Response buffer:done\r\n");
    strcpy(topic_publish,"GW_REG_RES/12345"); // give the topic to publish
	create_string_4json(i); // create string for sending to json generator
}


/**************************************************************
 * userid_auth : this function checks whether valid user_id
 * return type is integer
 * No parameter as if now
 *************************************************************/
int userid_auth()
{
  int err_val = 0;
	//Do UserID verification here
  if((flag_boot_userid_update == 1) || (flag_userid_update == 1))	{
	 printf("Success : Refreshing UserID Buffer\n");
	 flag_boot_userid_update = 0;
	 //	FILE *f = fopen("registered_user_ids.txt", "rb");
	 FILE *idvar = fopen("registered_user_ids.txt", "r");
	 printf("Success : seg check1");
	 //	fseek(f, 0, SEEK_END);
	 //	long fsize = ftell(f);
	 //	fseek(f, 0, SEEK_SET);  //same as rewind(f);
	 fscanf(idvar,"%s",content);
	 printf("Success : seg check : %s\r\n",content);
	 //content = malloc(fsize);
	 //fread(content, fsize, 1, f);
	 fclose(idvar);
	 idvar = fopen("flag_user_ids.txt", "w+");

	if (idvar == NULL)	{
	 printf("Error : Cannot open file \n");
	 err_val = 0;			}
	else	{
	 printf("Success : Flag overwritten to 0\r\n");
	 fprintf(idvar, "%s", "0");
	 fclose(idvar);		}
   }
	if(!strstr(content, user_id))	{
	 printf("Error : User Authentication Failed!!!\r\n");
	//printf("content : %s =  %s : UserID \n", content, user_id);
	 err_val =  0;	}
	else		{
	 printf("Success : User Authenticated Successfully\r\n");
	 //printf("content : %s =  %s : UserID \n", content, user_id);
	 err_val =  1;	}
 return err_val;
}
/*********************************************************************
 *
 *
 *
 ********************************************************************/
int file_delete_userid(char *ur_id) {

	char buff[100];
	char buff1[100];
	char buff2[100];
	char final_buff[200];
	int y = 0;
	//printf("DEBUG : delete.....\n");
	//FILE * fp3;

////////////////// Opening File ////////////////////////
    printf("DEBUG : delete.....1\n");
	FILE *fp3 = fopen("registered_user_ids.txt", "r+b");
	if (fp3 == NULL ) {
		printf("Could not open file for user id deletion\n");
		return 1;
	}

///////////////// Store all file data in buffer ///////////////////
	printf("DEBUG : delete.....2\n");
	y = 0;
	while (fscanf(fp3, "%s", buff) != EOF) {
		int x = 0;
		while (x < (strlen(buff))) {
			final_buff[y] = buff[x];
			y++;
			x++;
			
		}
		
	}
	printf("final_buff before : %s\n", final_buff);

////////////////// To find user id in buffer //////////////////////

	int i = 1, j, k = 0, pos = 0;

	while (final_buff[i] != '\0') {
		//if first character matches
		if (final_buff[i] == ur_id[0]) {
			j = 1;
			//if next character of string and pattern same
			while (ur_id[j] != '\0' && final_buff[j + i] != '\0'
					&& ur_id[j] == final_buff[j + i]) {
				j++;
				k = 1;
			}
			if (ur_id[j] == '\0') {
				//printf("pattern string found at %d position \n", i);
				pos = i;
			}
		}
		i++;
		if (k == 0) {
			if (final_buff[j + i] == '\0')
				printf("pattern not found \n");
		}
	}

	int o=0;

	if (pos == 0)
		return 1;

//////////////////////// Copying file content before user id////////////////////

	fseek(fp3, 0, SEEK_SET); //fseek to position
	y = 0;
	char c;
	for (o = 0; o < pos; o++) {
		c = fgetc(fp3);
		buff2[o] = c;
	}

	//printf("buff2: %s\n", buff2);

///////////////////////// Copying file content after user id////////////////////

	fseek(fp3, (pos + 5), SEEK_SET);
	y = 0;
	while (fscanf(fp3, "%s", buff) != EOF) {
		int x = 0;

		while (x <= (strlen(buff))) {
			buff1[y] = buff[x];
			y++;
			x++;

		}
	}
	//printf("buff1: %s\n", buff1);

//////////////// Removing old File////////////////////////////

	if (remove("registered_user_ids.txt") == 0){}
		//printf("Deleted successfully\r\n");
	else
		printf("Unable to delete the file\r\n");

	if (fp3)
		fclose(fp3);

////////// create new file ///////////////////////////

	fp3 = fopen("registered_user_ids.txt", "w");
	if (fp3 == NULL ) {
		printf("Could not open file for user id deletion\n");
		return 1;
	} else
	//	printf("file created \r\n");

/////////////// writing into file //////////////////////////

	fputs(buff2, fp3); //fwrite the buffer

	fputs(buff1, fp3); //fwrite the buffer

	if (fp3)
		fclose(fp3);

	return 0;

}
