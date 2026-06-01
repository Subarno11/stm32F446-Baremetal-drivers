/*
 * 007spi_txonly_arduino.c
 *
 *  Created on: 13-May-2026
 *      Author: Subarno Pal
 */

#include "stm32f446xx_gpio_drivers.h"
#include "stm32f446xx_spi_drivers.h"
#include <string.h>


void delay(void){
	for(uint32_t i = 0; i<50000/2 ;i++);
}

void SPI2_GPIOInits(void){
	GPIO_Handle_t SPIPins;

	SPIPins.pGPIOx = GPIOB;
	SPIPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	SPIPins.GPIO_PinConfig.GPIO_PinAltFunMode = 5;
	SPIPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_PP;
	SPIPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	SPIPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCLK
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_13;
	GPIO_Init(&SPIPins);

	//MOSI
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_15;
	GPIO_Init(&SPIPins);

	//MISO
	//SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	//GPIO_Init(&SPIPins);

	//NSS
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_12;
	GPIO_Init(&SPIPins);

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
void SPI2_Inits(void){
	SPI_Handle_t SPI2handle;

	SPI2handle.pSPIx = SPI2;
	SPI2handle.SPIConfig.SPI_BusConfig = SPI_BUS_CONFIG_FD;
	SPI2handle.SPIConfig.SPI_DeviceMode = SPI_DEVICE_MODE_MASTER;
	SPI2handle.SPIConfig.SPI_SclkSpeed = SPI_SCLK_SPEED_DIV8;//Generates serial clock of 2 MHz
	SPI2handle.SPIConfig.SPI_DFF = SPI_DFF_8BITS;
	SPI2handle.SPIConfig.SPI_CPOL = SPI_CPOL_LOW;
	SPI2handle.SPIConfig.SPI_CPHA = SPI_CPHA_LOW;
	SPI2handle.SPIConfig.SPI_SSM = SPI_SSM_DI; //Hardware Slave Management

	SPI_Init(&SPI2handle);

}
int main(void){

	char user_data[] = "Hello World";
	//This function is used to initialize GPIO pins to behave as SPI2
	SPI2_GPIOInits();

	//This function is used to initialize SPI2 peripherals
	SPI2_Inits();
	GPIO_ButtonInit();
	/*
	 * Making SSOE 1 does NSS output enable
	 * The NSS pin is automatically managed by hardware
	 * i.e. when SPE = 1 , NSS will be pulled to low
	 * and NSS pin will be high when SPE = 0
	 */
	SPI_SSOEConfig(SPI2 , ENABLE);
	while(1){
	    // 1. Wait until button is pressed (goes to 0)
		while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));

		delay(); // debounce

		while(!GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
	    // Now send the data once
	    SPI_PeripheralControl(SPI2 , ENABLE);
	    delay();
	    uint8_t dataLen = strlen(user_data);
	    SPI_SendData(SPI2, &dataLen , 1);
	    SPI_SendData(SPI2, (uint8_t *)user_data, dataLen);

	    while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
	    SPI_PeripheralControl(SPI2 , DISABLE);
	}

	return 0;
}
