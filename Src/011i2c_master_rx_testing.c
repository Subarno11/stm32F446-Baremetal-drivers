/*
 * 011i2c_master_rx_testing.c
 *
 *  Created on: 20-May-2026
 *      Author: Subarno Pal
 */


#include "stm32f446xx.h"
#include <stdio.h>
#include <string.h>
extern void initialise_monitor_handles();

I2C_Handle_t I2C1Handle;
#define MY_ADDR			0x61
#define SLAVE_ADDR		0x68


uint32_t len;
uint8_t rcv_Buffer[32];
void delay(void){
	for(uint32_t i = 0; i<50000/2 ;i++);
}

/*
 * PB6 -> SCL
 * PB9 -> SDA
 */


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

void I2C1_GPIOInits(void){
	GPIO_Handle_t I2CPins;

	I2CPins.pGPIOx = GPIOB;

	I2CPins.GPIO_PinConfig.GPIO_PinMode = GPIO_MODE_ALTFN;
	I2CPins.GPIO_PinConfig.GPIO_PinAltFunMode = GPIO_PIN_ALTFN_4;
	I2CPins.GPIO_PinConfig.GPIO_PinOPType = GPIO_OP_TYPE_OD;
	I2CPins.GPIO_PinConfig.GPIO_PinPuPdControl = GPIO_NO_PUPD;
	I2CPins.GPIO_PinConfig.GPIO_PinSpeed = GPIO_SPEED_FAST;

	//SCL
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = 6;
	GPIO_Init(&I2CPins);

	//SDA
	I2CPins.GPIO_PinConfig.GPIO_PinNumber = 9;
	GPIO_Init(&I2CPins);

}


void I2C1_Inits(void){
	I2C1Handle.pI2Cx = I2C1;

	I2C1Handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C1Handle.I2C_Config.I2C_DeviceAddress = MY_ADDR;
	I2C1Handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_2;
	I2C1Handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;

	I2C_Init(&I2C1Handle);
}


void ButtonPress(void){
	while(GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
	delay(); // debounce
	while(!GPIO_ReadFromInputPin(GPIOC, GPIO_PIN_NO_13));
}

int main(){

	uint8_t commandCode;

	uint8_t len;

	initialise_monitor_handles();

	printf("Apllication is running\n");
	//I2C pin inits
	I2C1_GPIOInits();

	//I2C peripheral config;
	I2C1_Inits();

	//Enable the I2C peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//Ack is made 1 after PE = 1
	I2C_ManageAcking(I2C1, ENABLE);

	//Button Initialisation
	GPIO_ButtonInit();

	while(1){
		//Wait till Button is pressed:
		ButtonPress();

		commandCode = 0x51;
		//So we have to send this command code to the slave
		I2C_MasterSendData(&I2C1Handle, &commandCode, 1 , SLAVE_ADDR , I2C_ENABLE_SR);
		//Recieve the Length data from the Slave:
		I2C_MasterRecieveData(&I2C1Handle, &len, 1, SLAVE_ADDR , I2C_ENABLE_SR);
		printf("Rcvd Length of data : %d\n",len);

		commandCode = 0x52;
		//So we have to send this command code to the slave
		I2C_MasterSendData(&I2C1Handle, &commandCode, 1 , SLAVE_ADDR , I2C_ENABLE_SR);
		//Recieve the Data from the Slave:
		I2C_MasterRecieveData(&I2C1Handle, rcv_Buffer, len, SLAVE_ADDR , I2C_DISABLE_SR);
		rcv_Buffer[len] = '\0';
		printf("Recieved Message : %s\n",rcv_Buffer);
	}
	return 0;
}
