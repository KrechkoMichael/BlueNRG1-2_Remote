
#include "flipflop.h"

void flipflop_init(void)
{
	/* Enable the GPIO Clock */
	SysCtrl_PeripheralClockCmd(CLOCK_PERIPH_GPIO, ENABLE);

	GPIO_InitType GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = STEP_UP_PIN | LED_GREEN_2_PIN | LED_ORANGE_PIN | LED_GREEN_1_PIN | CP_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Output;
	GPIO_InitStructure.GPIO_Pull = DISABLE;
	GPIO_InitStructure.GPIO_HighPwr = DISABLE;
	GPIO_Init(&GPIO_InitStructure);

	FLIP_FLOP_RESET();
}


