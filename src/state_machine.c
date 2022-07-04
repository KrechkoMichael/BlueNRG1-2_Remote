
#include "state_machine.h"


void state_machine_init(void)
{

}

void state_machine_tick(void)
{
	uint8_t status;

	status = power_tick();
	if(status != BLE_STATUS_SUCCESS) return;

	/* check buttons */
	buttons_tick();

	bluetooth_tick();
}


