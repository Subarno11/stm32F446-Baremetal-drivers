/*
 * 016uart_tx.c
 *
 *  Created on: 28-May-2026
 *      Author: Subarno Pal
 */

#include "stm32f446xx.h"
#include <string.h>
char msg[] = "USART Tx testing...\r\n";

USART_Handle_t usart1_handle;

void delay(void){
	for(uint32_t i = 0; i<50000/2 ;i++);
}

void USART1_GPIOInit(void){
	GPIO_Handle_t usart_gpios;

	usart_gpios.pGPIOx = GPIOA;

	usart_gpios.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpios.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpios.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
	usart_gpios.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	usart_gpios.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_PIN_ALTFN_7;

	//Initializing PA9 -> USART1 Tx
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&usart_gpios);

	//Initializing PA10 -> USART2 Rx
	usart_gpios.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(&usart_gpios);
}

void USART1_Init(void){
	usart1_handle.pUSARTx = USART1;
	usart1_handle.USART_Config.USART_Baud = USART_STD_BAUD_115200;
	usart1_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart1_handle.USART_Config.USART_Mode = USART_MODE_ONLY_TX;
	usart1_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart1_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	usart1_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

	USART_Init(&usart1_handle);

}

void GPIO_ButtonInit(void){
	GPIO_Handle_t GpioBtn;

	//Button Initialization:
	GpioBtn.pGPIOx = GPIOC;

	GpioBtn.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GpioBtn.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_IN;
	GpioBtn.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;
	GpioBtn.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;

	GPIO_Init(&GpioBtn);
}

void ButtonPress(void){
	while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
	delay(); // debounce
	while(!GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
}

int main(void){
	USART1_GPIOInit();

	USART1_Init();

	GPIO_ButtonInit();

	USART_PeripheralControl(USART1, ENABLE);

	while(1){
		ButtonPress();
		USART_SendData(&usart1_handle,(uint8_t *) msg, strlen(msg));
	}
	return 0;
}
