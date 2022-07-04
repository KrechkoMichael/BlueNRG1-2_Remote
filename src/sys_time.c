
#include "sys_time.h"


static time_global_ticks_t time_global = {
	.count = 0,
	.prev = 0
};

void sys_time_init(void)
{
	time_global.prev = HAL_VTimerGetCurrentTime_sysT32();
}

void sys_time_tick(void)
{
	__disable_irq();

	volatile uint32_t ticks = HAL_VTimerGetCurrentTime_sysT32();
	volatile uint32_t diff = ticks - time_global.prev;

   if(diff >= 0){
	   time_global.count += diff;
   } else {
	   time_global.count += ((0xFFFFFFFF - time_global.prev) + ticks);
   }

   time_global.prev = ticks;
   __enable_irq();
}

double get_time_sec(void){

	sys_time_tick();
	return ((double)time_global.count)/TIME_DIV;
}

uint64_t get_time_ticks(void){

	sys_time_tick();
	return time_global.count;
}
