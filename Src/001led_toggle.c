/*
 * 001led_toggle.c
 *
 *  Created on: 08-Mar-2026
 *      Author: Subarno Pal
 */

#include "stm32f446xx_gpio_drivers.h"

void delay(void){
	for(uint32_t i = 0; i<50000 ;i++);
}

int main(){

	GPIO_Handle_t GpioLed;

	GpioLed.pGPIOx = GPIOA;

	GpioLed.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;



	GPIO_Init(&GpioLed);

	while(1){
		GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);
		delay();
	}

	return 0;
}
