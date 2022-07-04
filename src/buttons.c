
#include "buttons.h"

#include "leds.h"
#include "step_up.h"
#include "bluetooth.h"
#include <stdio.h>
#include <inttypes.h>

static const char *byte_to_binary(int x);

buttons_state_t buttons = {
	.all_buttons = 0
};

void buttons_init(void)
{
	/* Enable the GPIO Clock */
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

	GPIO_InitType GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = (BUTTON_UP | BUTTON_DOWN | BUTTON_LEFT | BUTTON_RIGHT | BUTTON_CENTER);
	GPIO_InitStructure.GPIO_Mode = GPIO_Input;
	GPIO_InitStructure.GPIO_Pull = DISABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);
}


void buttons_tick(void)
{
	/*GPIO state*/
	static uint32_t prev_state = 0;
	/* buttons state */
	static buttons_state_t prev_buttons = {
			.all_buttons = 0
	};

	uint32_t state = READ_BUTTONS;

	if(state != prev_state){

		buttons.all_buttons = RESET;
		buttons.up = ((state & BUTTON_UP) != RESET);
		buttons.down = ((state & BUTTON_DOWN) != RESET);
		buttons.left = ((state & BUTTON_LEFT) != RESET);
		buttons.right = ((state & BUTTON_RIGHT) != RESET);
		buttons.center = ((state & BUTTON_CENTER) != RESET);

		/* Center button is also a power button */
//		if(buttons.center != prev_buttons.center){
//			if(buttons.center == SET){
//				leds_all_on();
//				HAL_VTimerStart_ms(TIMER_POWER_OFF, 1000);
//			} else {
//				leds_all_off();
//				HAL_VTimer_Stop(TIMER_POWER_OFF);
//			}
//			goto send;
//		}

		if(buttons.all_buttons == 0){ LED_GREEN_1(DISABLE); }
		else { LED_GREEN_1(ENABLE); }

//send:
		send_buttons_state();

		/* save state as previous */
		prev_state = state;
		memcpy(&prev_buttons, &buttons, sizeof(buttons_state_t));
	}


}

void send_buttons_state(void)
{
	char string[20];

	sprintf(string, byte_to_binary(buttons.all_buttons));
	strcat(string, "\r");
	send_to_ble((uint8_t*)string, strlen(string));

}

const char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}
