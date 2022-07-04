
#ifndef INC_SYS_TIME_H_
#define INC_SYS_TIME_H_

#include <inttypes.h>
#include "BlueNRG1_conf.h"
#include "bluenrg1_stack.h"


typedef struct time_global_ticks_s {
  uint64_t count; // number of ticks
  uint32_t prev; // previous tick value
} time_global_ticks_t;

#define TIME_DIV	409605 // CLK32 divider

void sys_time_init(void);
void sys_time_tick(void);
double get_time_sec(void);
uint64_t get_time_ticks(void);

#endif /* INC_SYS_TIME_H_ */
