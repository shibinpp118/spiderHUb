/*
 * json.h
 *
 *  Created on: 04-Aug-2017
 *      Author: abhinav
 */

#ifndef JSON_H_
#define JSON_H_

extern char cmd_type[];
extern char cmd_id[];
extern char user_id[100];
extern char gw_id[];
extern char command[];
extern char timestamp[];
extern char response[];
extern char scan_device[];
extern char device_name[];
extern char mac[];
extern char location[];
extern char favorites[];
extern char manufacture_name[];
extern char manufacture_id[];
extern char dev_type[];
extern char req_type[];
extern char req_id[];
extern char err_id[];
extern char message[];
extern char role[];
extern char kelvin[];
extern char humidity[];

extern char trans_id[];
extern char cloud_trans_id[];
extern char lan[];
extern char wlan[];
extern char proto_id[];
extern char hm_id[];
extern char memory[];
extern char com_id[];
extern char cmd_log[];

extern char CameraIP[];
extern char UserIP[];

char topic_publish[50];


int JSON_PARSER(char *JSON_STRING);

char * JSON_GENERATOR(char * in_json);

#endif /* JSON_H_ */
