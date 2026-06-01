/*
 * 003ledToggleExtBtn.c
 *
 *  Created on: 09-Mar-2026
 *      Author: Subarno Pal
 */


#include "stm32f446xx_gpio_drivers.h"
#define BTN_Pressed		0

void delay(void){
	for(uint32_t i = 0; i<50000/2 ;i++);
}

int main(){
	GPIO_Handle_t GpioLed;
	GpioLed.pGPIOx = GPIOA;

	GpioLed.GPIO_PinConfig.GPIO_PinNumber= 5;
	GpioLed.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_OUT;
	GpioLed.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	GpioLed.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	GpioLed.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_Handle_t GpioBtn;
	GpioBtn.pGPIOx = GPIOB;

	GpioBtn.GPIO_PinConfig.GPIO_PinNumber= 12;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	GPIO_PeriClockControl(GPIOA, ENABLE);
	GPIO_PeriClockControl(GPIOB, ENABLE);

	GPIO_Init(&GpioLed);
	GPIO_Init(&GpioBtn);

	while(1){
		if(GPIO_ReadFromInputPin(GPIOB , GPIO_PIN_NO_12) == BTN_Pressed){
			delay();
			GPIO_ToggleOutputPin(GPIOA, GPIO_PIN_NO_5);

			while(GPIO_ReadFromInputPin(GPIOB , GPIO_PIN_NO_12) == BTN_Pressed);
		}

	}
	return 0;
}
