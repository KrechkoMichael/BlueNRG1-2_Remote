
#ifndef INC_TIMERS_H_
#define INC_TIMERS_H_

#include <string.h>
#include "bluenrg1_stack.h"
#include "BlueNRG1_conf.h"
#include "ble_const.h"
#include "sleep.h"
#include "sys_time.h"


typedef void (* TIMER_CALLBACK_TYPE)(void);

typedef struct v_timer_s {
  FlagStatus active;
  uint8_t state;
  uint64_t start;
  uint64_t interval;
  TIMER_CALLBACK_TYPE function;
  uint64_t interval_1;
  uint64_t interval_2;
  TIMER_CALLBACK_TYPE function_1;
  uint32_t arg_1;
  TIMER_CALLBACK_TYPE function_2;
  uint32_t arg_2;
} v_timer_t;

typedef enum {
	V_TIMER_0 = 0,
	V_TIMER_1,
	V_TIMER_2,
	V_TIMER_END
} timersVirtual;

typedef enum {
	TIMER_POWER_TIMEOUT = 0,
	TIMER_POWER_OFF = 1,
	TIMER_SLEEP = 2,
	TIMER_3 = 3
} timersState;


void timers_init(void);
void timers_tick(void);
void sleep_ms(uint32_t time_ms);
void timer_once_set(timersVirtual timer, uint32_t interval_ms);
void timer_stop(timersVirtual timer);
void timer_restart(timersVirtual timer);
void timer_periodic_set(timersVirtual timer, uint32_t interval_ms, TIMER_CALLBACK_TYPE function);
void timer_periodic_2states_set(timersVirtual timer, uint32_t interval_1_ms, uint32_t interval_2_ms, TIMER_CALLBACK_TYPE function_1, TIMER_CALLBACK_TYPE function_2);




#endif /* INC_TIMERS_H_ */
