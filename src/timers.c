
#include "timers.h"


static v_timer_t timers_data[V_TIMER_END + 1];


void timers_init(void)
{
	/* clear */
	uint8_t size = sizeof(timers_data)/sizeof(v_timer_t);

	for(uint8_t i = 0; i < size; i++){
		memset(&timers_data[i], 0, sizeof(v_timer_t));
	}
}

void sleep_ms(uint32_t time_ms)
{
	HAL_VTimerStart_ms(TIMER_SLEEP, time_ms);
	BTLE_StackTick();
	BlueNRG_Sleep(SLEEPMODE_WAKETIMER, WAKEUP_IO9, WAKEUP_IOx_LOW);
}

void timer_once_set(timersVirtual timer, uint32_t interval_ms)
{
	timers_data[timer].start = get_time_ticks();
	timers_data[timer].interval = interval_ms*(TIME_DIV/1000);
}

void timer_periodic_set(timersVirtual timer,
					   uint32_t interval_ms,
					   TIMER_CALLBACK_TYPE function)
{
	/* is already set? */
	if(timers_data[timer].active == SET) return;

	timers_data[timer].start = get_time_ticks();
	timers_data[timer].interval = interval_ms*(TIME_DIV/1000);
	timers_data[timer].function = function;
	timers_data[timer].active = SET;

	function();
}

void timer_periodic_2states_set(timersVirtual timer,
							   uint32_t interval_1_ms,
							   uint32_t interval_2_ms,
							   TIMER_CALLBACK_TYPE function_1,
							   TIMER_CALLBACK_TYPE function_2)
{
	/* is already set? */
	if(timers_data[timer].active == SET) return;

	timers_data[timer].start = get_time_ticks();
	timers_data[timer].interval_1 = interval_1_ms*(TIME_DIV/1000);
	timers_data[timer].interval_2 = interval_2_ms*(TIME_DIV/1000);
	timers_data[timer].function_1 = function_1;
	timers_data[timer].function_2 = function_2;

	timers_data[timer].state = 1;
	timers_data[timer].active = SET;

	function_1();
}

void timers_tick(void)
{
	uint64_t tick = get_time_ticks();

	for(uint8_t i = 0; i <= V_TIMER_END; i++){

		if (timers_data[i].active == RESET) continue;

	   TIMER_CALLBACK_TYPE function_1 = timers_data[i].function_1;
	   TIMER_CALLBACK_TYPE function_2 = timers_data[i].function_2;
	   uint64_t start = timers_data[i].start;
	   uint64_t interval_1 = timers_data[i].interval_1;
	   uint64_t interval_2 = timers_data[i].interval_2;
	   uint64_t interval_full = interval_1 + interval_2;
	   uint8_t prev_state = timers_data[i].state;
	   uint8_t new_state;

	   uint64_t diff = (tick - start) + 1;
	   if(diff < interval_1){
		   new_state = 1;
	   } else if(diff < interval_full){
		   new_state = 2;
	   } else {
		   timers_data[i].start += interval_full;
		   new_state = 1;
	   }

	   switch(prev_state)
	   {
		   case 1: if(new_state != prev_state){
			   	   	   function_2();
			   	   	   timers_data[i].state = 2;
		   	   	   }
			   break;

		   case 2: if(new_state != prev_state){
			   	   	   function_1();
					   timers_data[i].state = 1;
				   }
				   break;
		   default: break;
	   }

	}

}

void timer_stop(timersVirtual timer)
{
	timers_data[timer].active = RESET;
}

void timer_restart(timersVirtual timer)
{
	timers_data[timer].start = get_time_ticks();
}




