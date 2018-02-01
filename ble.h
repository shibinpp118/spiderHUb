/*
 * SpiderHub Project
 *
 * ble.h
 *
 *  Declaration of all functions
 */

#ifndef BLE_H_
#define BLE_H_

extern int ble_data_write(char* c);
extern int zigb_data_write(char* c);
extern char * ble_data_read();

extern int ble_enable();
extern int ble_disable();

#endif /* BLE_H_ */
