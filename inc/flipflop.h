
#ifndef INC_FLIPFLOP_H_
#define INC_FLIPFLOP_H_

#include "BlueNRG1_conf.h"
#include "pins.h"

void flipflop_init(void);

#define CP_STATE(state)\
		({ \
	        GPIO_WriteBit(CP_PIN, state); \
		})

#define FLIP_FLOP_RESET() \
		({ \
			CP_STATE(Bit_RESET);	\
			GPIO_ResetBits(LED_GREEN_1_PIN | LED_GREEN_2_PIN | LED_ORANGE_PIN | STEP_UP_PIN); \
			CP_STATE(Bit_SET); \
		})


#endif /* INC_FLIPFLOP_H_ */
