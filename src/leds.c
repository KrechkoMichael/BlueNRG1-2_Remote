
#include "leds.h"

void leds_init(void)
{
	LEDS_RESET();
}

void leds_tick(void)
{

}

void led_orange_on(void){ LED_ORANGE(ENABLE); }
void led_orange_off(void){ LED_ORANGE(DISABLE); }

void led_green_1_on(void){ LED_GREEN_1(ENABLE); }
void led_green_1_off(void){ LED_GREEN_1(DISABLE); }

void led_green_2_on(void){ LED_GREEN_2(ENABLE); }
void led_green_2_off(void){ LED_GREEN_2(DISABLE); }

void leds_all_on(void){ LEDS_SET(); }
void leds_all_off(void){ LEDS_RESET(); }
