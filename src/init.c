
#include "init.h"


/* Private functions */
static void setup(void);


void init(void)
{
	/* global settings */
	setup();

	SystemInit();

	/* D-Type pins set up */
	flipflop_init();

	/* battery --> 3.3V */
	step_up_init();

	/* reset LEDs */
	leds_init();

	buttons_init();

	/* global power mode set up */
	power_init();

	/* BLE stack set up */
	bluetooth_init();

	/* system time counter */
	sys_time_init();

	/* virtual timers */
	timers_init();

	/* main state machine set up */
	state_machine_init();
}

void setup(void)
{
	device.role = ROLE_PERIPHERAL;
	strcpy(device.name, "005-REMOTE");
}















