//============================================================================
// Name        : bbg_json.cpp
// Author      : Abhinav Saxena
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================
#define _CRT_SECURE_NO_WARNINGS			// stop complaining about deprecated functions

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "jsmn.h"
#include "jWrite.h"
#include "json.h"
#include "Application.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

char cmd_type[100] =			"NULL" ;
char cmd_id[100] = 				"NULL" ;
char user_id[100] = 			"NULL" ;
char gw_id[100] =  				"NULL" ;
char command[100] =  			"NULL" ;
char timestamp[100] =  			"NULL" ;
char response[100] =  			"NULL" ;
char scan_device[100] =  		"NULL" ;
char device_name[100] =  		"NULL" ;
char mac[100] =  				"NULL" ;
char location[100] =  			"NULL" ;
char favorites[100] =  			"NULL" ;
char manufacture_name[100] =  	"NULL" ;
char manufacture_id[100] =  	"NULL" ;
char dev_type[100] =  			"NULL" ;
char req_type[100] =  			"NULL" ;
char req_id[100] =  			"NULL" ;
char err_id[100] =  			"NULL" ;
char message[100] = 			"NULL" ;
char role[100] =  				"NULL" ;
char kelvin[100] = 				"NULL" ;
char humidity[100] =			"NULL" ;

char trans_id[100] =			"NULL" ;
char cloud_trans_id[100] =		"NULL" ;
char lan[100] =  				"NULL" ;
char wlan[100] =  				"NULL" ;
char proto_id[100] =	  		"NULL" ;
char hm_id[100] =  				"NULL" ;
char memory[100] = 				"NULL" ;
char com_id[100] = 				"NULL" ;
char cmd_log[100] = 			"NULL" ;
char CameraIP[20]="NULL";
char UserIP[20]="NULL";
//char city_name[100]="NULL";

int JSON_PARSER(char *JSON_STRING);
char* JSON_GENERATOR(char in_json[]);
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start
			&& strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

int JSON_PARSER(char *JSON_STRING) {
	int i;
	int r;
        printf("Entering global release");
        global_release();
	jsmn_parser p;
	jsmntok_t t[128]; /* We expect no more than 128 tokens */

	jsmn_init(&p);

	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t,
			sizeof(t) / sizeof(t[0]));

	if (r < 0) {
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "cmd_type") == 0) {

			//memset(cmd_type, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					cmd_type[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_type: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "cloud_trans_id") == 0) {
			//memset(cloud_trans_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					cloud_trans_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "lan") == 0) {
			//memset(lan, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					lan[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "wlan") == 0) {
			//memset(wlan, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					wlan[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "proto_id") == 0) {
			//memset(proto_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					proto_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "hm_id") == 0) {
			//memset(hm_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					hm_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "memory") == 0) {
			//memset(memory, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					memory[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "com_id") == 0) {
			//memset(com_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					com_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "cmd_log") == 0) {
			//memset(cmd_log, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					cmd_log[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}


		else if (jsoneq(JSON_STRING, &t[i], "trans_id") == 0) {
			//memset(trans_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					trans_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}


		else if (jsoneq(JSON_STRING, &t[i], "cmd_id") == 0) {
			//memset(cmd_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					cmd_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "response") == 0) {
			//memset(response, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					response[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "scan_device") == 0) {
			//memset(scan_device, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					scan_device[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "device_name") == 0) {
			//memset(device_name, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					device_name[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "mac") == 0) {
			//memset(mac, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					mac[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "location") == 0) {
			//memset(location, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					location[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "favorites") == 0) {
			//memset(favorites, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					favorites[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "manufacture_name") == 0) {
			//memset(manufacture_name, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					manufacture_name[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "manufacture_id") == 0) {
			//memset(manufacture_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					manufacture_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "dev_type") == 0) {
			//memset(dev_type, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					dev_type[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "req_type") == 0) {
			//memset(req_type, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					req_type[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "req_id") == 0) {
			//memset(req_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					req_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Cmd_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "err_id") == 0) {
			//memset(err_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					err_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- err_id : %.*s\n", t[i + 1].end - t[i + 1].start,
//					JSON_STRING + t[i + 1].start);
//			printf("err_id value : %s\n", err_id);

			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "message") == 0) {
			//memset(message, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					message[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- message: %.*s\n", t[i + 1].end - t[i + 1].start,
//					JSON_STRING + t[i + 1].start);
//			printf("message value : %s\n", message);

			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "role") == 0) {
			//memset(role, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					role[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- role: %.*s\n", t[i + 1].end - t[i + 1].start,
//					JSON_STRING + t[i + 1].start);
//			printf("role value : %s\n", role);

			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "user_id") == 0) {
			//memset(user_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					user_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- User_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "gw_id") == 0) {
			//memset(gw_id, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					gw_id[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Gw_Id: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "command") == 0) {
			//memset(command, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl)
				{
					command[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}

//			printf("- Command: %.*s\n", t[i + 1].end - t[i + 1].start,
//					JSON_STRING + t[i + 1].start);
//			printf("command value : %s\n", command);
			i++;
		}

		else if (jsoneq(JSON_STRING, &t[i], "CameraIP") == 0) {

			//memset(timestamp, 0x00, 100);
			int dl = t[i + 1].end - t[i + 1].start;
			int x = 0;
			if (dl < 99)
				while (x < dl) {
					CameraIP[x] = (JSON_STRING + t[i + 1].start)[x];
					x++;
				}
//			printf("- Time stamp: %.*s\n", t[i+1].end-t[i+1].start,
//					JSON_STRING + t[i+1].start);
			i++;
		}

                else if (jsoneq(JSON_STRING, &t[i], "UserIP") == 0) {

                        //memset(timestamp, 0x00, 100);
                        int dl = t[i + 1].end - t[i + 1].start;
                        int x = 0;
                        if (dl < 99)
                                while (x < dl) {
                                        UserIP[x] = (JSON_STRING + t[i + 1].start)[x];
                                        x++;
                                }
//                      printf("- Time stamp: %.*s\n", t[i+1].end-t[i+1].start,
//                                      JSON_STRING + t[i+1].start);
                        i++;
                }
                else if (jsoneq(JSON_STRING, &t[i], "timestamp") == 0) {

                        //memset(timestamp, 0x00, 100);
                        int dl = t[i + 1].end - t[i + 1].start;
                        int x = 0;
                        if (dl < 99)
                                while (x < dl) {
                                        timestamp[x] = (JSON_STRING + t[i + 1].start)[x];
                                        x++;
                                }
//                      printf("- Time stamp: %.*s\n", t[i+1].end-t[i+1].start,
//                                      JSON_STRING + t[i+1].start);
                        i++;
                }
		      //SHIBIN Edited
                else if (jsoneq(JSON_STRING, &t[i], "kelvin") == 0) {

						//memset(timestamp, 0x00, 100);
						int dl = t[i + 1].end - t[i + 1].start;
						int x = 0;
						if (dl < 99)
								while (x < dl) {
									kelvin[x] = (JSON_STRING + t[i + 1].start)[x];
										x++;
								}
//                      printf("- Time stamp: %.*s\n", t[i+1].end-t[i+1].start,
//                                      JSON_STRING + t[i+1].start);
						i++;
                   }
                else if (jsoneq(JSON_STRING, &t[i], "humidity") == 0) {

                						//memset(timestamp, 0x00, 100);
                						int dl = t[i + 1].end - t[i + 1].start;
                						int x = 0;
                						if (dl < 99)
                								while (x < dl) {
                									humidity[x] = (JSON_STRING + t[i + 1].start)[x];
                										x++;
                								}
                //                      printf("- Time stamp: %.*s\n", t[i+1].end-t[i+1].start,
                //                                      JSON_STRING + t[i+1].start);
                						i++;
                                   }

		else {
			printf("Unexpected key: %.*s\n", t[i].end - t[i].start,
					JSON_STRING + t[i].start);
		}
	}
        printf("Exiting global release");
	return EXIT_SUCCESS;
}

char* JSON_GENERATOR(char in_json[]) {
	char* pt;
	char* pt1;
	char ch = ',';
	long int count = 0;
	int x;

	for (x = 0; in_json[x] != '\0'; x++) {
		if (in_json[x] == ch)
			count++;
	}

	count = count / 2;
	count = count + 1;

	static char buffer[1024];
	unsigned int buflen = 1024;
	int err;

	jwOpen(buffer, buflen, JW_OBJECT, JW_COMPACT); // start root object

	pt = strtok(in_json, ",");
	pt1 = strtok(NULL, ",");
	if (pt != NULL && pt1 != NULL )
		jwObj_string(pt, pt1); // add object key:value pairs
	pt = "";
	pt1 = "";

	for (x = 1; x < (count); x++) {

		pt = strtok(NULL, ",");
		pt1 = strtok(NULL, ",");
		if (pt != NULL && pt1 != NULL )
			jwObj_string(pt, pt1); // add object key:value pairs
		pt = "";
		pt1 = "";

	}

	err = jwClose(); // close and get error code

	//printf(buffer);
	if (err != JWRITE_OK)
		printf("Error: %s at function call %d\n", jwErrorToString(err),
				jwErrorPos());

	return buffer;
}

