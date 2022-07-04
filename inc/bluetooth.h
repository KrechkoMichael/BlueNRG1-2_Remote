
#ifndef INC_BLUETOOTH_H_
#define INC_BLUETOOTH_H_

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "bluenrg1_stack.h"
#include "BlueNRG1_conf.h"
#include "hal_types.h"
#include "ble_const.h"
#include "bluenrg1_gap.h"

#include "leds.h"
#include "timers.h"
#include "buttons.h"

#define CENTRAL_ROLE				1
#define PERIPHERAL_ROLE  			0

#define MAX_DEVICE_NAME_LEN 	20
#define MAC_ADDRESS_LEN 		6

#define USN_ADDRESS				0x100007F4
#define MAX_DATA_PER_PACKET 	20

/* fixed parameters by STM */
#define GUARD_TIME_END 		2.5
#define GUARD_TIME 			1.6
#define ADV_LEN_MS 			14.6

/* */
#define CONN_INT_PERIPH_MS 			50
#define CONN_INT_CENTRAL_MS 		10
#define ADV_INT_MS 					(CONN_INT_PERIPH_MS - 5)
#define SCAN_INT_MS 				(CONN_INT_PERIPH_MS)
#define SCAN_WIN_MS 				20.0
#define PACKETS_PER_CI 				6
#define TIMEOUT_PERIPH_PACKETS_NUM 	8
#define TIMEOUT_CENTRAL_PACKETS_NUM 4
//TODO cei should be added
// Connection parameters preprocessing
#define CONN_INT_PERIPH			(uint16_t) MAX (6, CONN_INT_PERIPH_MS/1.25)
#define CONN_INT_CENTRAL		(uint16_t) MAX (6, CONN_INT_CENTRAL_MS/1.25)
#define ADV_INT_MIN				(uint16_t) 32
#define ADV_INT_MAX				(uint16_t) MAX (32, ADV_INT_MS/0.625)
#define SCAN_INT				(uint16_t) MAX (4, SCAN_INT_MS/0.625)
#define SCAN_WIN 				(uint16_t) MAX (4, SCAN_WIN_MS/0.625)
#define CE_LENGTH_MS			(PACKETS_PER_CI*1.25)
#define CE_LENGTH				(uint16_t) (CE_LENGTH_MS/0.625)
#define CONN_TIMEOUT_PERIPH		(uint16_t) MAX (10, (CONN_INT_PERIPH_MS*TIMEOUT_PERIPH_PACKETS_NUM)/10) 	// 10 = 100 ms = min
#define CONN_TIMEOUT_CENTRAL	(uint16_t) MAX (10, (CONN_INT_CENTRAL_MS*TIMEOUT_CENTRAL_PACKETS_NUM)/10)	// 10 = 100 ms = min

#define NOTIFICATION 	0x01

#define GENERAL_DISCOVERY_PROC	0x02

typedef enum {
	DEVICE_IDLE = 0,
	DEVICE_SCAN,
	DEVICE_ADV
} this_device_state;

typedef enum {
	SLOT_FREE = RESET,
	CONNECTED,
	NOTIFY_ON,
	DISCONNECTED
} connState;

typedef enum {
	ROLE_CENTRAL = 0,
	ROLE_PERIPHERAL
} role_type;

typedef enum {
	ADDR_PUBLIC = 0x00,
	ADDR_RANDOM = 0x01,
	ADDR_PUBLIC_IDN = 0x02,
	ADDR_RANDOM_IDN = 0x03
} address_type;

typedef struct this_device_s {
	this_device_state state;
	role_type role;
	char name[MAX_DEVICE_NAME_LEN];
	address_type address_type;
	uint8_t address[MAC_ADDRESS_LEN];
	uint16_t appearance;
} this_device_t;

typedef struct ble_slot_s {
	connState state;
	uint16_t handle;
	role_type role;
	char name[MAX_DEVICE_NAME_LEN];
	address_type address_type;
	uint8_t address[MAC_ADDRESS_LEN];
} ble_slot_t;

extern this_device_t device;

ErrorStatus bluetooth_init(void);
void bluetooth_tick(void);
void send_to_ble(uint8_t *data, uint16_t size);



#endif /* INC_BLUETOOTH_H_ */
