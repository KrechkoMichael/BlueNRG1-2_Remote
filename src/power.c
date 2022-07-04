
#include "power.h"

volatile RESET_REASON_Type xResetReason = 0;
volatile powerState power_state = POWER_RESET;


void power_init(void)
{
	xResetReason = SysCtrl_GetWakeupResetReason();
}

uint8_t power_tick(void)
{
	switch(power_state)
	{
	case POWER_RESET:

		switch(xResetReason)
		{
			case RESET_BLE_BOR: power_state = POWER_TURNING_ON; break;
			case RESET_BLE_POR: power_state = POWER_TURNING_ON; break;
			case RESET_WDG: power_state = POWER_ACTIVE; break;
			case RESET_SYSREQ: power_state = POWER_TURNING_ON; break;
			case RESET_LOCKUP: power_state = POWER_ACTIVE; break;
			default: power_state = POWER_ACTIVE; break;
		}
		break;

	case POWER_TURNING_ON:

		LED_ORANGE(ENABLE);

		while(IS_PRESSED(BUTTON_CENTER)){
			sleep_ms(100);
		}

		sleep_ms(100);

		if(IS_UNPRESSED(BUTTON_CENTER)){
			LED_ORANGE(DISABLE);
			STEP_UP(ENABLE);

			power_state = POWER_ACTIVE;
		}

		break;

	case POWER_ACTIVE:
		return BLE_STATUS_SUCCESS;
		break;

	case POWER_BATTERY_WARNING:
		break;

	case POWER_BATTERY_EMPTY:
		break;

	case POWER_TURNING_OFF:

		LED_ORANGE(DISABLE);
		STEP_UP(DISABLE);

		hci_reset();

		while(IS_PRESSED(BUTTON_CENTER)){
			sleep_ms(100);
		}

		sleep_ms(100);

		if(IS_UNPRESSED(BUTTON_CENTER)){
			NVIC_SystemReset();
		}
		break;

	default:
		break;
	}

	return BLE_STATUS_BUSY;
}

void HAL_VTimerTimeoutCallback(uint8_t timerNum){

	switch(timerNum)
	{
		case TIMER_POWER_TIMEOUT: power_state = POWER_TURNING_OFF; break;
		case TIMER_POWER_OFF: power_state = POWER_TURNING_OFF; break;
		case TIMER_SLEEP: break;
		case TIMER_3: break;
		default: break;
	}

}

