


#ifndef INC_INIT_H_
#define INC_INIT_H_



#include "BlueNRG1_conf.h"
#include "pins.h"

#include "flipflop.h"
#include "step_up.h"
#include "leds.h"
#include "buttons.h"
#include "power.h"
#include "bluetooth.h"
#include "sys_time.h"
#include "state_machine.h"

extern volatile RESET_REASON_Type xResetReason;

void init(void);
float get_temp(void);
float get_batt(void);
void update_fifo_ptr(void);






#endif /* INC_INIT_H_ */
