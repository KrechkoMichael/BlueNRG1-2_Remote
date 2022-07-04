
#ifndef INC_STEP_UP_H_
#define INC_STEP_UP_H_

#include "BlueNRG1_conf.h"
#include "flipflop.h"
#include "pins.h"

void step_up_init(void);

#define STEP_UP(state)\
({ \
	CP_STATE(Bit_RESET);	\
	GPIO_WriteBit(STEP_UP_PIN, state); \
	CP_STATE(Bit_SET); \
})

#endif /* INC_STEP_UP_H_ */
