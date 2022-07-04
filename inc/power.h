
#ifndef INC_POWER_H_
#define INC_POWER_H_

#include "BlueNRG1_conf.h"
#include "step_up.h"
#include "pins.h"
#include "buttons.h"
#include "leds.h"
#include "timers.h"

typedef enum {
	POWER_RESET = RESET,
	POWER_TURNING_ON,
	POWER_ACTIVE,
	POWER_BATTERY_WARNING,
	POWER_BATTERY_EMPTY,
	POWER_TURNING_OFF
} powerState;


void power_init(void);
uint8_t power_tick(void);

extern volatile RESET_REASON_Type xResetReason;
extern volatile powerState power_state;


#endif /* INC_POWER_H_ */
