
#include "bluetooth.h"

/* --------------------------------------
 * important to include only in one c-file
 * where BlueNRG_Stack_Initialization using
 * to prevention multiply definition NO_INIT_SECTION's
 * */
#include "flexi_config.h"
/*---------------------------------------*/

this_device_t device = {
	.state = DEVICE_IDLE,
	.role = ROLE_PERIPHERAL,
	.name = "005-REMOTE",
	.appearance = 0,
	.address_type = ADDR_PUBLIC
};

ble_slot_t peer = {
		.state = SLOT_FREE
};

static void data_init(void);
static ErrorStatus add_serial_service(void);
static uint8_t set_device_discoverable(void);
static void Attribute_Modified_CB(uint16_t handle, uint16_t data_length, uint8_t *att_data);

uint16_t chatServHandle, TXCharHandle, RXCharHandle;
static Service_UUID_t service_uuid;
static Char_UUID_t char_uuid;


ErrorStatus bluetooth_init(void)
{
	uint16_t gap_service_handle;
	uint16_t dev_name_char_handle;
	uint16_t appearance_char_handle;
	uint8_t appearance[2] = { device.appearance & 0xff, device.appearance >> 8 };
	uint8_t status;

	/* data structures reset */
	data_init();

	/* BlueNRG-1 stack init */
	status = BlueNRG_Stack_Initialization(&BlueNRG_Stack_Init_params);
	if (status != BLE_STATUS_SUCCESS) {	return ERROR; }

	/* copy unique serial number */
	memcpy(device.address, (void *)USN_ADDRESS, sizeof(device.address));

	status = aci_hal_write_config_data(CONFIG_DATA_PUBADDR_OFFSET, CONFIG_DATA_PUBADDR_LEN, device.address);
	if (status != BLE_STATUS_SUCCESS) { return ERROR; }

	status = aci_hal_set_tx_power_level(0x01,0x07);
	if (status != BLE_STATUS_SUCCESS) { return ERROR; }

	status = aci_gatt_init();
	if (status != BLE_STATUS_SUCCESS) { return ERROR; }

	status = aci_gap_init(GAP_PERIPHERAL_ROLE, 0x00, sizeof(device.name), &gap_service_handle, &dev_name_char_handle, &appearance_char_handle);
	if (status != BLE_STATUS_SUCCESS) { return ERROR; }

	/* Set the device name */
	status = aci_gatt_update_char_value_ext(0,gap_service_handle,dev_name_char_handle,0,sizeof(device.name),0x00,sizeof(device.name),(uint8_t*)device.name);
	if (status != BLE_STATUS_SUCCESS) { return ERROR; }

	/* Set the device appearance */
	status = aci_gatt_update_char_value_ext(0,gap_service_handle,appearance_char_handle,0,sizeof(appearance),0x00,sizeof(appearance),appearance);
	if (status != BLE_STATUS_SUCCESS) { return ERROR; }

	status = hci_le_set_scan_response_data(0x00, NULL);
	if (status != BLE_STATUS_SUCCESS) { return ERROR; }

	/* add service */
	status = add_serial_service();
	if (status != BLE_STATUS_SUCCESS) { return ERROR; }

	return SUCCESS;
}

void data_init(void)
{
	/* peer slot clearing */
	memset(&peer, 0, sizeof(ble_slot_t));
}

ErrorStatus add_serial_service(void)
{
  uint8_t ret;

  const uint8_t uuid[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe0,0xf2,0x73,0xd9};
  const uint8_t charUuidTX[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe1,0xf2,0x73,0xd9};
  const uint8_t charUuidRX[16] = {0x66,0x9a,0x0c,0x20,0x00,0x08,0x96,0x9e,0xe2,0x11,0x9e,0xb1,0xe2,0xf2,0x73,0xd9};

  memcpy(&service_uuid.Service_UUID_128, uuid, 16);
  ret = aci_gatt_add_service(UUID_TYPE_128, &service_uuid, PRIMARY_SERVICE, 6, &chatServHandle);
  if (ret != BLE_STATUS_SUCCESS){ return ERROR; }

  memcpy(&char_uuid.Char_UUID_128, charUuidTX, 16);
  ret =  aci_gatt_add_char(chatServHandle, UUID_TYPE_128, &char_uuid, 20, CHAR_PROP_NOTIFY, ATTR_PERMISSION_NONE, GATT_DONT_NOTIFY_EVENTS,
                16, 1, &TXCharHandle);
  if (ret != BLE_STATUS_SUCCESS){ return ERROR; }

  memcpy(&char_uuid.Char_UUID_128, charUuidRX, 16);
  ret =  aci_gatt_add_char(chatServHandle, UUID_TYPE_128, &char_uuid, 20, CHAR_PROP_WRITE|CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                16, 1, &RXCharHandle);
  if (ret != BLE_STATUS_SUCCESS){ return ERROR; }

  return SUCCESS;
}

void bluetooth_tick(void)
{
	uint8_t status;
	static uint8_t first_state_flag = TRUE;

	switch(peer.state)
	{
		case SLOT_FREE:
		case DISCONNECTED:

			first_state_flag = TRUE;
			HAL_VTimerStart_ms(TIMER_POWER_TIMEOUT, 5000);

			if (device.state == DEVICE_IDLE)
			{
				status = set_device_discoverable();

				if(status == BLE_STATUS_SUCCESS){
					device.state = DEVICE_ADV;
				}

				timer_stop(V_TIMER_0);
			}
			break;

		case CONNECTED:
			timer_periodic_2states_set(V_TIMER_0, 100, 2000, led_orange_on, led_orange_off);
			HAL_VTimer_Stop(TIMER_POWER_TIMEOUT);
			break;

		case NOTIFY_ON:

			if(first_state_flag){
				send_buttons_state();
				first_state_flag = FALSE;
			}


			break;

		default:
			break;
	}
}

uint8_t set_device_discoverable(void)
{
  uint8_t status;
  uint8_t Local_Name[1 + strlen(device.name)];

  Local_Name[0] = AD_TYPE_COMPLETE_LOCAL_NAME;
  memcpy(&Local_Name[1], device.name, strlen(device.name));

  status = aci_gap_set_discoverable(ADV_IND,
		  	  	  	  	  	  	  			0x0050,
																		0x00A0,
																		PUBLIC_ADDR,
                                    NO_WHITE_LIST_USE,
                                    sizeof(Local_Name),
                                    Local_Name,
                                    0x00,
                                    NULL,
                                    0x0000, //
                                    0x0000); // this values not influence on tablet behavior(?)

  if (status != BLE_STATUS_SUCCESS) {
	  led_orange_on();
  }

  return(status);
}

void send_to_ble(uint8_t *data, uint16_t size)
{
	uint8_t buf[20];
	static BOOL busy = FALSE;
	ble_slot_t *slot = &peer;
	uint8_t status;

	if(busy){ return; }
	busy = TRUE;

	if(slot->state != NOTIFY_ON) goto end;

	/* check data size */
	if(size <= sizeof(buf)){ memcpy(buf, data, size); }
	else { goto end; }

	/* slave to master, notify should be ENABLED on master */
	status = aci_gatt_update_char_value_ext(slot->handle,
											chatServHandle,
											TXCharHandle,
											NOTIFICATION,
											size,
											0,
											size,
											data);

	switch(status)
	{
		case BLE_STATUS_SUCCESS: break; // everything okay
		case BLE_STATUS_INSUFFICIENT_RESOURCES: // stack TX buffer full, retry later
			 goto end; break;
		case BLE_STATUS_NOT_ALLOWED: // stack busy, retry later
			 goto end; break;
		default: ;
		goto end; break;
	}

end:
	busy = FALSE;
	return;
}


/* *************** BlueNRG-1 Stack Callbacks****************/

void aci_gap_proc_complete_event(uint8_t procedure_code, uint8_t status, uint8_t data_length, uint8_t data[])
{
	if(procedure_code == GENERAL_DISCOVERY_PROC){

	}
}

void hci_le_connection_complete_event(uint8_t status,
		                              uint16_t connection_handle,
									  uint8_t role,
									  uint8_t peer_address_type,
									  uint8_t peer_address[6],
									  uint16_t conn_interval,
									  uint16_t conn_latency,
									  uint16_t supervision_timeout,
									  uint8_t master_clock_accuracy)
{
	if(role == CENTRAL_ROLE){ // event from tablet
		// copy parameters to Tablet structure
		peer.address_type = peer_address_type;
		memcpy(peer.address, peer_address, 6);
		peer.handle = connection_handle;
		// update flags
		peer.state = CONNECTED;
		device.state = CONNECTED;
	}
}

void aci_gatt_proc_complete_event(uint16_t connection_handle, uint8_t error_code)
{
	if(error_code != BLE_STATUS_SUCCESS){
		return;
	}

	if(connection_handle == peer.handle){}
}

void aci_l2cap_connection_update_resp_event(uint16_t connection_handle,uint16_t result){}

void aci_gatt_attribute_modified_event(uint16_t connection_handle,uint16_t attr_handle,uint16_t offset,uint16_t attr_data_length,uint8_t attr_data[])
{
	if (connection_handle == peer.handle){
		Attribute_Modified_CB(attr_handle, attr_data_length, attr_data);
	}
}

void hci_disconnection_complete_event(uint8_t status, uint16_t connection_handle, uint8_t reason)
{
	if (connection_handle == peer.handle){
		peer.state = DISCONNECTED;
		device.state = DEVICE_IDLE;
	}
}

// data from peer
void Attribute_Modified_CB(uint16_t handle, uint16_t data_length, uint8_t *att_data)
{
	if(handle == RXCharHandle + 1)
	{
//		add_data_to_tx_uart_fifo(att_data, data_length); // tablet ask to receive data
	}
	else if(handle == TXCharHandle + 2) // tablet ask to enable notifications
	{
		if(att_data[0] == 0x01){
			peer.state = NOTIFY_ON;
		} else if(att_data[0] == 0x00){
			/* notify off */
			peer.state = CONNECTED;
		}
	}
}

void aci_gatt_tx_pool_available_event(uint16_t Connection_Handle,
                                      uint16_t Available_Buffers)
{
  /* It allows to notify when at least 2 GATT TX buffers are available */
}

void aci_l2cap_proc_timeout_event(uint16_t Connection_Handle,
                                  uint8_t Data_Length,
                                  uint8_t Data[])
{
	if (Connection_Handle == peer.handle){}
}

