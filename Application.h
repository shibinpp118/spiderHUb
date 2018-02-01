#ifndef APPLICATION_H_
#define APPLICATION_H_

/*=======================================================*/
//Cmd_Id : // ready
#define inclusion_fail  0
#define in_inclusion    1
#define inclusion_complete  2
#define deletion_success    3
#define deletion_failure    4

#define VALID   "0"
#define INVALID "1"
#define MQTT_PACK_LMT 30
#define MSG_MAX_LOAD 200
#define JSON_HDR_LMT 30
/*========================================================*/

int system_handler(void);
int add_User_id();
int userid_auth();
int device_management(int inter_status, int protcol);
int Gw_HB_response();
int Gw_Details_response();
int Gw_Auto_val_response(char* val);
int device_config_response(char* val, char* proto);

void User_Id_res(int resp, int param);
void Gw_Notific_Req();
void create_string_4json(int length); 
void flush_buffer();
void global_release();
void jsn_hdr_topic(void);
int validate_req_type(int count, char *dt_type);
int user_management(char *cmd,char *usr_);
int file_delete_userid(char *ur_id);
int peripheral_iface_mgt(char proto[]);
int bluetooth_iface_mgt(void);
int ble_iface_mgt(void);
int zigbee_iface_mgt(void);
int Zigbee_iface_mgt(void);
int zwave_iface_mgt(void);
int Device_configurations(int status);
void zigb_pkt_buffer(char id, char val, char type);
int bluetooth_switch(void);
//data packets for response keys and values
typedef struct 
{
    char *key;
    char *value;
}rx_pack;

typedef struct
{
        char msg_load[MSG_MAX_LOAD];
}mqtt_packs;

typedef struct 
{
	char *json_req_type;
}Json_reg_topic;


char Buff_Fill;
char buffer_for_response[500];
char z_wave_status, z_wave_reset_count;
char enable_ble;

/*NANO Switch globals*/
char* nano;

/********************* Zigbee *****************************/
//#define SOP 0x2D
//char zb_d2_off[20]={0x2D,0x13,0x01,0x3C,0xC1,0xF6,0x12,0x00,0x00,0x00,0x12,0xFF,0xFF,0x08,0x08,0x00,0x06,0x11,0x02,0x00};
/*unsigned char zb_d2_off[20];
unsigned char zb_d2_on[20];
unsigned char zb_d3_off[20];
unsigned char zb_d3_on[20];
unsigned char zb_dAll_off[20];
unsigned char zb_dAll_on[20];*/
unsigned char zb_pkt_buff[25];
//char zb_d2_on[20]={0x2D,0x13,0x01,0x3C,0xC1,0xF6,0x12,0x00,0x00,0x00,0x12,0xFF,0xFF,0x08,0x08,0x00,0x06,0x11,0x02,0x01};
//char zb_d3_off[20]={0x2D,0x13,0x01,0x3C,0xC1,0xF6,0x12,0x00,0x00,0x00,0x13,0xFF,0xFF,0x08,0x08,0x00,0x06,0x11,0x02,0x00};
//char zb_d3_on[20]={0x2D,0x13,0x01,0x3C,0xC1,0xF6,0x12,0x00,0x00,0x00,0x13,0xFF,0xFF,0x08,0x08,0x00,0x06,0x11,0x02,0x01};
//char zb_dAll_off[20]={0x2D,0x13,0x04,0x3C,0xC1,0xF6,0x12,0x00,0x00,0x00,0x12,0xFF,0xFF,0x08,0x08,0x00,0x06,0x11,0x02,0x00};
//char zb_dAll_on[20]={0x2D,0x13,0x04,0x3C,0xC1,0xF6,0x12,0x00,0x00,0x00,0x12,0xFF,0xFF,0x08,0x08,0x00,0x06,0x11,0x02,0x01};

/*************************************************************/
#endif

