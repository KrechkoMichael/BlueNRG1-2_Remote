
#ifndef INC_LEDS_H_
#define INC_LEDS_H_

#include "BlueNRG1_conf.h"
#include "pins.h"
#include "flipflop.h"

void leds_init(void);
void leds_tick(void);

void led_orange_on(void);
void led_orange_off(void);
void led_green_1_on(void);
void led_green_1_off(void);
void led_green_2_on(void);
void led_green_2_off(void);
void leds_all_on(void);
void leds_all_off(void);


#define IS_LED_ON(led_pin)	READ_BIT(GPIO->DATA, led_pin)

#define LED_GREEN_1(state)\
({ \
	CP_STATE(Bit_RESET); \
	GPIO_WriteBit(LED_GREEN_1_PIN, state); \
	CP_STATE(Bit_SET); \
})

#define LED_GREEN_2(state)\
({ \
	CP_STATE(Bit_RESET); \
	GPIO_WriteBit(LED_GREEN_2_PIN, state); \
	CP_STATE(Bit_SET); \
})

#define LED_ORANGE(state)\
({ \
	CP_STATE(Bit_RESET); \
	GPIO_WriteBit(LED_ORANGE_PIN, state); \
	CP_STATE(Bit_SET); \
})

#define LEDS_RESET() \
({ \
	CP_STATE(Bit_RESET); \
	GPIO_ResetBits(LED_GREEN_1_PIN | LED_GREEN_2_PIN | LED_ORANGE_PIN); \
	CP_STATE(Bit_SET); \
})

#define LEDS_SET() \
({ \
	CP_STATE(Bit_RESET); \
	GPIO_SetBits(LED_GREEN_1_PIN | LED_GREEN_2_PIN | LED_ORANGE_PIN); \
	CP_STATE(Bit_SET); \
})

#endif /* INC_LEDS_H_ */
