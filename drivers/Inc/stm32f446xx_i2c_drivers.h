/*
 * stm32f446xx_i2c_drivers.h
 *
 *  Created on: 17-May-2026
 *      Author: Subarno Pal
 */

#ifndef INC_STM32F446XX_I2C_DRIVERS_H_
#define INC_STM32F446XX_I2C_DRIVERS_H_

#include "stm32f446xx.h"
/*
 * Configuration Structures for I2Cx peripheral
 */
typedef struct{
	uint32_t I2C_SCLSpeed;
	uint8_t I2C_DeviceAddress;
	uint8_t I2C_ACKControl;
	uint16_t I2C_FMDutyCycle;
}I2C_Config_t;

/*
 * Handle Structure for I2Cx for I2Cx peripheral
 */
typedef struct{
	I2C_RegDef_t *pI2Cx;
	I2C_Config_t I2C_Config;
	uint8_t *pTxBuffer;  //To store the application's Tx buffer address
	uint8_t *pRxBuffer; //To store the application's Tx buffer address
	uint32_t TxLen;    //To store the Tx Len
	uint32_t RxLen;	  //TO store the Rx Len
	uint8_t TxRxState; // To store Communication state
	uint8_t DevAddr; //TO store Slave/device address
	uint32_t RxSize; //To store the Rx size
	uint8_t Sr; //To store repeated start value

}I2C_Handle_t;

/*
 * @I2C_SCLSpeed
 */
#define I2C_SCL_SPEED_SM			100000
#define I2C_SCL_SPEED_FM4K			400000
#define I2C_SCL_SPEED_FM2K			200000

/*
 * @I2C_ACKControl
 */
#define I2C_ACK_ENABLE				1
#define I2C_ACK_DISABLE				0

/*
 * @I2C_FMDutyCycle
 */
#define I2C_FM_DUTY_2				0
#define I2C_FM_DUTY_16_9			1

/*
 * I2C related Status flag definitions
 */
#define I2C_FLAG_TXE							(1<< I2C_SR1_TXE)
#define I2C_FLAG_RXNE							(1<< I2C_SR1_RXNE)
#define I2C_FLAG_SB								(1<< I2C_SR1_SB)
#define I2C_FLAG_ADDR							(1<< I2C_SR1_ADDR)
#define I2C_FLAG_STOPF							(1<< I2C_SR1_STOPF)
#define I2C_FLAG_BERR							(1<< I2C_SR1_BERR)
#define I2C_FLAG_AF								(1<< I2C_SR1_AF)
#define I2C_FLAG_OVR							(1<< I2C_SR1_OVR)
#define I2C_FLAG_TIMEOUT						(1 << I2C_SR1_TIMEOUT)
#define I2C_FLAG_BTF							(1 << I2C_SR1_BTF)

#define I2C_ENABLE_SR 									ENABLE
#define I2C_DISABLE_SR									DISABLE

/*
 * Possible I2C Application States:
 */
#define I2C_READY								0
#define I2C_BUSY_IN_RX							1
#define I2C_BUSY_IN_TX							2

/*
 * I2C Application Events Macros
 */
#define I2C_EV_TX_CMPLT							0
#define I2C_EV_RX_CMPLT							1
#define I2C_EV_STOP								2
#define I2C_ERROR_BERR  						3
#define I2C_ERROR_ARLO  						4
#define I2C_ERROR_AF    						5
#define I2C_ERROR_OVR   						6
#define I2C_ERROR_TIMEOUT 						7
#define I2C_EV_DATA_REQ							8
#define I2C_EV_DATA_RCV							9

/*************************************************************
 * 					APIs supported by this Driver
 * 		For more info about the APIs check the function description
 * ***********************************************************/

//Peripheral Clock Setup:
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx , uint8_t EnorDi);

//Init and De-Init:
void I2C_Init(I2C_Handle_t *pI2CHandle);
void I2C_DeInit(I2C_RegDef_t *pI2Cx);

//Data Send and Recieve:
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t Len , uint8_t SlaveAddr , uint8_t Sr);
void I2C_MasterRecieveData(I2C_Handle_t *pI2CHandle , uint8_t *pRxBuffer , uint32_t len , uint8_t SlaveAddr, uint8_t Sr);

uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t Len , uint8_t SlaveAddr , uint8_t Sr);
uint8_t  I2C_MasterRecieveDataIT(I2C_Handle_t *pI2CHandle , uint8_t *pRxBuffer , uint32_t len , uint8_t SlaveAddr, uint8_t Sr);

void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx , uint8_t data);
uint8_t I2C_SlaveRecieveData(I2C_RegDef_t *pI2Cx);


//IRQ Configuration and ISR handling:
void I2C_IRQInterruptConfig(uint8_t IRQNumber , uint8_t EnorDi);
void I2C_IRQPriorityConfig(uint8_t IRQNumber , uint32_t IRQPriority);

//For ISR handling in I2C there will be two ISRs ISR1-> For event handling and ISR2->For Error Handling
void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle);

/*
 * Other Peripheral Control APIs
 */
uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName);
void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx , uint8_t EnorDi);
void I2C_ManageAcking(I2C_RegDef_t *pI2Cx , uint8_t EnorDi);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);
void I2C_CloseRecieveData(I2C_Handle_t *pI2CHandle);
void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx);

void I2C_SlaveEnableDisableCallBackEvents(I2C_RegDef_t *pI2Cx , uint8_t EnorDi);
/*
 * Application Callback
 */
void I2C_ApplicationEventCallBack(I2C_Handle_t *pI2CHandle , uint8_t AppEv);


#endif /* INC_STM32F446XX_I2C_DRIVERS_H_ */
