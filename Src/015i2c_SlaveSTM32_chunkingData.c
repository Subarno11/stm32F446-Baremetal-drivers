/*
 * 015i2c_SlaveSTM32_chunkingData.c
 *
 *  Created on: 23-May-2026
 *      Author: Subarno Pal
 */


#include "stm32f446xx.h"
#include <stdio.h>
#include <string.h>



#define SLAVE_ADDR		0x68
#define MY_ADDR			0x68

uint8_t data_len = 0;
uint8_t Tx_Buffer[] = "Hi I am Subarno Pal , I am a pre-final year student in ECE department in NIT Durgapur";

uint8_t commandCode = 0;
uint32_t cnt = 0;
uint32_t w_ptr = 0;

I2C_Handle_t I2C1Handle;
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

	data_len = strlen((char *)Tx_Buffer);
	//I2C pin inits
	I2C1_GPIOInits();

	//I2C peripheral config;
	I2C1_Inits();


	//Enable the I2C peripheral
	I2C_PeripheralControl(I2C1, ENABLE);

	//Ack is made 1 after PE = 1
	I2C_ManageAcking(I2C1, ENABLE);

	//I2C_IRQ configurations
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQInterruptConfig(IRQ_NO_I2C1_ER, ENABLE);

	I2C_SlaveEnableDisableCallBackEvents(I2C1 , ENABLE);

	while(1);

}

void I2C1_EV_IRQHandler(void){
	I2C_EV_IRQHandling(&I2C1Handle);
}

void I2C1_ER_IRQHandler(void){
	I2C_ER_IRQHandling(&I2C1Handle);
}

void I2C_ApplicationEventCallBack(I2C_Handle_t *pI2CHandle , uint8_t AppEv){

	if(AppEv == I2C_ERROR_AF){
		//This happens only during slave Txing
		//Master has sent NACK , so slave should understand master does not need more data

		//If current active code is 0x52 then don't invalidate
		if(! (commandCode == 0x52)){
			commandCode = 0xFF;
		}

		//Reset the cnt variable as it is end of transmission
		cnt = 0;

		//Slave concludes it sent all the bytes when w_ptr reaches data_len
		if(w_ptr >= data_len){
			w_ptr = 0;
			commandCode = 0xFF;
		}
	}

	else if(AppEv == I2C_EV_STOP){
		//This happens only during reception
		//Master has ended the I2C communication with the Slave

		cnt =0;

	}

	else if(AppEv == I2C_EV_DATA_REQ){
		//Master wants some data and slave has to send it

		if(commandCode == 0x51){
			//Here we are sending 4 bytes of length info
			I2C_SlaveSendData(pI2CHandle->pI2Cx, ((data_len >> ((cnt%4) * 8)) & 0xFF));
			cnt++;
		}
		else if(commandCode == 0x52){
			//sending Tx_Buffer contents indexed by w_ptr variable
			I2C_SlaveSendData(pI2CHandle->pI2Cx, Tx_Buffer[w_ptr++]);
		}
	}

	else if(AppEv == I2C_EV_DATA_RCV){
		//Data is waiting for the slave to read , slave has to read
		commandCode = I2C_SlaveRecieveData(pI2CHandle->pI2Cx);
	}



}
