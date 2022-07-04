
/* Includes-----------------------------------------------------------------*/
#include "bluenrg1_stack.h"
#include "sleep.h"
#include "init.h"

void APP_Tick(void);
void sleep_manager(void);

int main(void)
{
	/* Initialization HW and BLE Stack */
	init();

	/* Main loop */
	while(1)
	{
		/* BlueNRG-1 stack tick */
		BTLE_StackTick();

		/* Application Tick */
		APP_Tick();

		/* Power Save management */
		sleep_manager();
	}

}

void APP_Tick(void)
{
	/* update global tick counter */
	sys_time_tick();

	/* check timers */
	timers_tick();

	state_machine_tick();
}

void sleep_manager(void)
{
	if(power_state == POWER_ACTIVE)
	{
		BlueNRG_Sleep(SLEEPMODE_NOTIMER, 0, WAKEUP_IOx_LOW);  // WAKEUP_IO9 - SWO
	}
	else
	{
		STEP_UP(DISABLE);
		BlueNRG_Sleep(SLEEPMODE_NOTIMER, WAKEUP_IO12, WAKEUP_IOx_LOW);  // WAKEUP_IO9 - SWO
	}

}

/****************** BlueNRG-1 Sleep Management Callback ********************************/

SleepModes App_SleepMode_Check(SleepModes sleepMode)
{
	return  SLEEPMODE_NOTIMER;
}

/***************************************************************************************/

#ifdef USE_FULL_ASSERT
	void assert_failed(uint8_t* file, uint32_t line){
		ALERT("Wrong param: %s, line %d", file, (int)line);
	}
#endif

