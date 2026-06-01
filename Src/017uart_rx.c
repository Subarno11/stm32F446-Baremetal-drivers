/*
 * 017uart_rx.c
 *
 *  Created on: 28-May-2026
 *      Author: Subarno Pal
 */

#include "stm32f446xx.h"
#include <string.h>
#include <stdio.h>

USART_Handle_t usart1_handle;

uint8_t data;

extern void initialise_monitor_handles();

void delay(void)
{
	for(uint32_t i = 0 ; i < 50000 ; i++);
}

void USART1_GPIOInit(void)
{
	GPIO_Handle_t usart_gpio;

	usart_gpio.pGPIOx = GPIOA;

	usart_gpio.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	usart_gpio.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_PIN_ALTFN_7;
	usart_gpio.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	usart_gpio.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_PU;
	usart_gpio.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	// PA9 -> USART1_TX
	usart_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_9;
	GPIO_Init(&usart_gpio);

	// PA10 -> USART1_RX
	usart_gpio.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_10;
	GPIO_Init(&usart_gpio);
}

void USART1_Init(void)
{
	usart1_handle.pUSARTx = USART1;

	usart1_handle.USART_Config.USART_Baud = USART_STD_BAUD_9600;
	usart1_handle.USART_Config.USART_HWFlowControl = USART_HW_FLOW_CTRL_NONE;
	usart1_handle.USART_Config.USART_Mode = USART_MODE_TXRX;
	usart1_handle.USART_Config.USART_NoOfStopBits = USART_STOPBITS_1;
	usart1_handle.USART_Config.USART_ParityControl = USART_PARITY_DISABLE;
	usart1_handle.USART_Config.USART_WordLength = USART_WORDLEN_8BITS;

	USART_Init(&usart1_handle);
}

int main(void)
{
	initialise_monitor_handles();

	    printf("UART Receive API Test\n");

	    USART1_GPIOInit();
	    USART1_Init();

	    USART_PeripheralControl(USART1, ENABLE);

	    while(1)
	    {
	        // Wait for first byte
	        USART_ReceiveData(&usart1_handle, &data, 1);

	        // Print received byte
	        printf("%c", data);
	    }
}
