/*
 * 008spi_cmd_handling.c
 *
 *  Created on: 13-May-2026
 *      Author: Subarno Pal
 */


#include  "stm32f446xx.h"
#include <string.h>
#include <stdio.h>
extern void initialise_monitor_handles();

//Command Codes :
#define CMD_LED_CTRL				0x50
#define CMD_SENSOR_READ				0x51
#define CMD_LED_READ				0x52
#define CMD_PRINT					0x53
#define CMD_ID_READ					0x54

#define LED_OFF						0
#define LED_ON						1

//Arduino Analog pins:
#define ANALOG_PIN0   				0
#define ANALOG_PIN1   				1
#define ANALOG_PIN2   				2
#define ANALOG_PIN3   				3
#define ANALOG_PIN4   				4


//Arduino LED
#define LED_PIN						9

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
	SPIPins.GPIO_PinConfig.GPIO_PinNumber = GPIO_PIN_NO_14;
	GPIO_Init(&SPIPins);

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

uint8_t SPI_VerifyResponse(uint8_t ackbyte){
	if(ackbyte == 0xF5){
		return 1;
	}
	return 0;
}

void ButtonPress(void){
	while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
	delay(); // debounce
	while(!GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
}
int main(void){

	uint8_t dummy_write = 0xff;
	uint8_t dummy_read;

	initialise_monitor_handles();

	printf("Application is running\n");
	//This function is used to initialize GPIO pins to behave as SPI2
	SPI2_GPIOInits();

	//This function is used to initialize SPI2 peripherals
	SPI2_Inits();
	printf("SPI Initialised\n");
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
		ButtonPress();
	    // Enable the SPI2 peripheral
	    SPI_PeripheralControl(SPI2 , ENABLE);
	    delay();

	    //1.*******************CMD_LED_CTRL <pin no(1)> <value(1)>*************

	    uint8_t commandcode = CMD_LED_CTRL;
	    uint8_t ackbyte;
	    uint8_t args[2];
	    //Send command:
	    SPI_SendData(SPI2, &commandcode, 1);

	    //This transmission has resulted in 1 byte reception
	    //Do dummy read to clear off the RXNE
	    SPI_RecieveData(SPI2, &dummy_read, 1);

	    //Send dummy bits (1 Byte) to fetch the response from the slave
	    //Since SPI is synchronous, the slave cannot send data unless the master provides clock pulses.
	    SPI_SendData(SPI2, &dummy_write, 1);

	    //Read the acknowledgment byte recieved:
	    SPI_RecieveData(SPI2, &ackbyte, 1);

	    if(SPI_VerifyResponse(ackbyte)){
	    	args[0] = LED_PIN;
	    	args[1] = LED_ON;
	    	//Send Arguements
	    	SPI_SendData(SPI2, args, 2);
	    	printf("CMD_LED_CTRL Executed\n");
	    }

	    //End of CMD_LED_CTRL

	    //2. *****************CMD_SENSOR_READ  <analog Pin number(1)>*************
	    ButtonPress();

		commandcode = CMD_SENSOR_READ;
		//Send command:
		SPI_SendData(SPI2, &commandcode, 1);
	    //Do dummy read to clear off the RXNE
	    SPI_RecieveData(SPI2, &dummy_read, 1);

	    //Send dummy bits (1 Byte) to fetch the response from the slave
	    SPI_SendData(SPI2, &dummy_write, 1);

	    //Read the acknowledgment byte recieved:
	    SPI_RecieveData(SPI2, &ackbyte, 1);

	    if(SPI_VerifyResponse(ackbyte)){
	    	args[0] = ANALOG_PIN0;
	    	//Send Arguements
	    	SPI_SendData(SPI2, args, 1);
		    //Do dummy read to clear off the RXNE
		    SPI_RecieveData(SPI2, &dummy_read, 1);

		    //delay to give Time to the slave for ADC conversions
		    delay();
		    //Send dummy bits (1 Byte) to fetch the response from the slave
		    SPI_SendData(SPI2, &dummy_write, 1);

		    uint8_t analog_read;
		    SPI_RecieveData(SPI2, &analog_read, 1);
		    printf("CMD_SENSOR_READ %d\n" , analog_read);
	    }

	    //3. *****************CMD_LED_READ  <Pin number(1)>*************
	    ButtonPress();

	    commandcode = CMD_LED_READ;
	    //Send Command
	    SPI_SendData(SPI2, &commandcode, 1);
	    //Clear the recieved dummy byte
	    SPI_RecieveData(SPI2, &dummy_read, 1);

	    //Send a dummy byte to the slave for getting its response:
	    SPI_SendData(SPI2, &dummy_write, 1);
	    //Read the acknowledgement status of slave:
	    SPI_RecieveData(SPI2, &ackbyte, 1);

	    uint8_t LED_Status;
	    if(SPI_VerifyResponse(ackbyte)){
	    	args[0] = LED_PIN;

	    	//Send this arguement to the slave:
	    	SPI_SendData(SPI2, args, 1);
	    	//Do dummy read to clear off the RXNE
	    	SPI_RecieveData(SPI2, &dummy_read, 1);

	    	delay();

		    //Send a dummy byte to the slave for getting its response:
		    SPI_SendData(SPI2, &dummy_write, 1);
		    //Recieve the LED status data from the slave:
	    	SPI_RecieveData(SPI2, &LED_Status, 1);
	    	printf("CMD_LED_READ %d\n" , LED_Status);
	    }

	    //4. *****************CMD_PRINT  <Len(1)>    <message>*************
	    ButtonPress();

	    commandcode = CMD_PRINT;
	    char msg[] = "Hello Arduino";
	    //Send Command
	    SPI_SendData(SPI2, &commandcode, 1);
	    //Clear the recieved dummy byte
	    SPI_RecieveData(SPI2, &dummy_read, 1);

	    //Send a dummy byte to the slave for getting its response:
	    SPI_SendData(SPI2, &dummy_write, 1);
	    //Read the acknowledgement status of slave:
	    SPI_RecieveData(SPI2, &ackbyte, 1);

	    if(SPI_VerifyResponse(ackbyte)){
	    	uint8_t len  = strlen(msg);

	    	 //Send length
	    	 SPI_SendData(SPI2, &len, 1);

	    	 //Clear dummy byte
	    	 SPI_RecieveData(SPI2, &dummy_read, 1);

	    	 //Send message
	    	 SPI_SendData(SPI2, (uint8_t*)msg, len);
	    	 printf("CMD_PRINT Executed \n");

	    	 //Clear received dummy bytes
	    	 //for(uint8_t i = 0; i < len; i++){
	    	//	 SPI_RecieveData(SPI2, &dummy_read, 1);
	    	 //}


	    }

	    //5. *****************CMD_ID_READ  (No arguements)*************
	    ButtonPress();

	    commandcode = CMD_ID_READ;
	    //Send Command
	    SPI_SendData(SPI2, &commandcode, 1);
	    //Clear the recieved dummy byte
	    SPI_RecieveData(SPI2, &dummy_read, 1);

	    //Send a dummy byte to the slave for getting its response:
	    SPI_SendData(SPI2, &dummy_write, 1);
	    //Read Acknowledhment from the slave:
	    SPI_RecieveData(SPI2, &ackbyte, 1);

    	uint8_t ID[11];
	    if(SPI_VerifyResponse(ackbyte)){


	    	//Generate Clocks for 10 bytes
	    	for(uint32_t i = 0 ; i< 10 ; i++){
	    		delay();
	    		SPI_SendData(SPI2, &dummy_write, 1);
	    		SPI_RecieveData(SPI2, &ID[i], 1);
	    	}
	    	ID[10] = '\0';
	    	printf("CMD_ID_READ :  %s \n" ,ID );
	    }
	    while(SPI_GetFlagStatus(SPI2, SPI_BUSY_FLAG));
	    SPI_PeripheralControl(SPI2 , DISABLE);

	    printf("SPI Communication closed\n" );
	}

	return 0;
}
