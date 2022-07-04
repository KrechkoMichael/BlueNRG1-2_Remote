
#ifndef INC_BUTTONS_H_
#define INC_BUTTONS_H_

#include <string.h>
#include "BlueNRG1_conf.h"
#include "pins.h"

typedef union
{
	uint8_t all_buttons;
 struct
  {
	FlagStatus right : 1;
	FlagStatus left : 1;
	FlagStatus bit2 : 1;
	FlagStatus bit3 : 1;
	FlagStatus center : 1;
	FlagStatus bit5 : 1;
	FlagStatus up : 1;
	FlagStatus down : 1;
  };
} buttons_state_t;

#define BUTTONS_MASK 			(BUTTON_UP | BUTTON_DOWN | BUTTON_LEFT | BUTTON_RIGHT | BUTTON_CENTER)
#define READ_BUTTONS			((GPIO->DATA ^ BUTTONS_MASK) & BUTTONS_MASK)
#define IS_UNPRESSED(button)	(GPIO->DATA & button)
#define IS_PRESSED(button)		!(IS_UNPRESSED(button))


void buttons_init(void);
void buttons_tick(void);
void send_buttons_state(void);

extern buttons_state_t buttons;

#endif /* INC_BUTTONS_H_ */
