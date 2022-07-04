
#ifndef INC_STATE_MACHINE_H_
#define INC_STATE_MACHINE_H_

#include "buttons.h"
#include "leds.h"
#include "bluetooth.h"
#include "power.h"


void state_machine_init(void);
void state_machine_tick(void);


#endif /* INC_STATE_MACHINE_H_ */
