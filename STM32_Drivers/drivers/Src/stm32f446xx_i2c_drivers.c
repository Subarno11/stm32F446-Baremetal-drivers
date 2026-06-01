/*
 * stm32f446xx_i2c_drivers.c
 *
 *  Created on: 17-May-2026
 *      Author: Subarno Pal
 */

#include "stm32f446xx.h"


#define SLAVE_READ			1
#define SLAVE_WRITE			0
//Helper Function
static void I2C_GenerateStartCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->I2C_CR1 |=  (1<< I2C_CR1_START);
}

static void I2C_ExecuteAddressPhase(I2C_RegDef_t *pI2Cx , uint8_t SlaveAddr , uint8_t rw){
	SlaveAddr = SlaveAddr << 1;
	if(rw == SLAVE_WRITE){
		SlaveAddr &= ~(1<<0); //Slave Address = 7 bit Slave Address + R/W bit
	}

	else if(rw == SLAVE_READ){
		SlaveAddr |= 1 << 0;
	}
	pI2Cx->I2C_DR = SlaveAddr;
}



static void I2C_ClearADDRFlag(I2C_Handle_t *pI2CHandle){
	uint32_t dummy_read;
	//Check for device mdoe
	if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)){
		//Device is master mode
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){

			if(pI2CHandle->RxSize == 1){
				//First disable the Acking
				I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
				//Now clear the ADDR flag
				dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
				dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
				(void)dummy_read;
			}
		}
		else{
			//Straight away clear the ADDR flag
			dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
			dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
			(void)dummy_read;
		}
	}

	else{
		//Device is in Slave Mode
		//Straight away clear the ADDR flag
		dummy_read = pI2CHandle->pI2Cx->I2C_SR1;
		dummy_read = pI2CHandle->pI2Cx->I2C_SR2;
		(void)dummy_read;
	}
}

void I2C_GenerateStopCondition(I2C_RegDef_t *pI2Cx){
	pI2Cx->I2C_CR1 |=  (1<< I2C_CR1_STOP);
}

//Some private helper functions
static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle);
static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle);



/*********************************************************************
 * @fn      		  - I2C_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given I2C peripheral
 *
 * @param[in]         - base address of the I2C peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @return            -  none
 *
 * @Note              -  none

 */
void I2C_PeriClockControl(I2C_RegDef_t *pI2Cx , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pI2Cx == I2C1)
			I2C1_PCLK_EN();
		else if(pI2Cx == I2C2)
			I2C2_PCLK_EN();
		else if(pI2Cx == I2C3)
			I2C3_PCLK_EN();

	}

	else{
		if(pI2Cx == I2C1)
			I2C1_PCLK_DI();
		else if(pI2Cx == I2C2)
			I2C2_PCLK_DI();
		else if(pI2Cx == I2C3)
			I2C3_PCLK_DI();
	}
}



/*********************************************************************
 * @fn      		  - I2C_PeripheralControl
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void I2C_PeripheralControl(I2C_RegDef_t *pI2Cx , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pI2Cx->I2C_CR1 |= 1<<I2C_CR1_PE;
	}
	else{
		pI2Cx->I2C_CR1 &= ~(1<<I2C_CR1_PE);
	}
}





/*********************************************************************
 * @fn      		  - I2C_Init
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void I2C_Init(I2C_Handle_t *pI2CHandle){

	//Enable the peripheral clock of the I2C
	I2C_PeriClockControl(pI2CHandle->pI2Cx, ENABLE);

	uint32_t tempreg = 0;

	//Ack controll bit:
	tempreg |= pI2CHandle->I2C_Config.I2C_ACKControl << I2C_CR1_ACK;
	pI2CHandle->pI2Cx->I2C_CR1 = tempreg;

	//FREQ field of CR2:
	tempreg = 0;
	tempreg |= RCC_GetPCLK1Value()/1000000U;

	pI2CHandle->pI2Cx->I2C_CR2 = (tempreg & 0x3F);

	//Program the device own address
	tempreg = 0;
	tempreg |= pI2CHandle->I2C_Config.I2C_DeviceAddress << 1;
	tempreg |= 1<<14; //14th bit of OAR1 always kept to 1 by software
	pI2CHandle->pI2Cx->I2C_OAR1 = tempreg;

	//CCR calculations:
	uint16_t ccr_value = 0;
	tempreg =0;
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
		//Mode is standard mode
		//Mode configured as SM by default
		//Taking 50% duty cycle
		ccr_value = RCC_GetPCLK1Value()/(2 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		tempreg |= ccr_value & (0xFFF); //Masking the first 12 bits

	}
	else{
		//Mode is Fast Mode
		//Set the mode to FM Mode
		tempreg |= (1<<15);
		//Store the DUTY cycle:
		tempreg |= (pI2CHandle->I2C_Config.I2C_FMDutyCycle << 14);
		if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_2){
			ccr_value = RCC_GetPCLK1Value()/(3 * pI2CHandle->I2C_Config.I2C_SCLSpeed);
		}

		else if(pI2CHandle->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_16_9){
			ccr_value = RCC_GetPCLK1Value()/(25 * pI2CHandle->I2C_Config.I2C_SCLSpeed);

		}
		tempreg |= ccr_value & (0xFFF);
	}

	pI2CHandle->pI2Cx->I2C_CCR = tempreg;

	//TRISE Configuration:
	if(pI2CHandle->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
		//Mode is standard mode
		tempreg = (RCC_GetPCLK1Value() / 1000000U) + 1;

	}
	else{
		//Mode is fast mode

		tempreg = ((RCC_GetPCLK1Value() * 300) / 1000000000U) + 1;
	}
	pI2CHandle->pI2Cx->I2C_TRISE = tempreg & (0x3F);
}

/*********************************************************************
 * @fn      		  - I2C_DeInit
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void I2C_DeInit(I2C_RegDef_t *pI2Cx){
	if(pI2Cx == I2C1){
		I2C1_REG_RESET();
	}
	else if(pI2Cx == I2C2){
		I2C2_REG_RESET();
	}
	else if(pI2Cx == I2C3){
		I2C3_REG_RESET();
	}

}

/*********************************************************************
 * @fn      		  - I2C_MasterSendData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void I2C_MasterSendData(I2C_Handle_t *pI2CHandle , uint8_t *pTxBuffer , uint32_t len , uint8_t SlaveAddr , uint8_t Sr){
	//1.Generate the start condition:
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag in the SR1
	//Note :- Untill SB is cleared SCL will be stretched (pulled to low)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the Slave with r/nw bit set to w(0) (Total 8 bits)
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx , SlaveAddr , SLAVE_WRITE);

	//4. Confirm that Address phase is completed by checking the ADDR flag :
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	//5. Clear the ADDR flag according to its software sequence
	//Note: Untill ADDR is cleared SCL will be stretched
	I2C_ClearADDRFlag(pI2CHandle);

	//6. Send the Data untill length becomes = 0
	while(len >0 ){
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE)); //Wait untill TXE flag is set
		pI2CHandle->pI2Cx->I2C_DR = *pTxBuffer;
		pTxBuffer++;
		len--;
	}

	//7. When len becomes 0 wait for TXE = 1 and BTF = 1 before generating the STOP condition
	//Note: TXE = 1 and BTF = 1, means that both SR and DR are empty and next transmission should begin
	//when BTF =1 SCL will  be stretched (pulled to LOW)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_TXE));
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_BTF));

	//8. Generate STOP condition and Master need not to wait for the completion of stop condition
	//Note: Generating STOP automatically clears the BTF
	if(Sr == I2C_DISABLE_SR){
		I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	}
}


/*********************************************************************
 * @fn      		  - I2C_MasterRecieveData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void I2C_MasterRecieveData(I2C_Handle_t *pI2CHandle , uint8_t *pRxBuffer , uint32_t len , uint8_t SlaveAddr , uint8_t Sr){
	//1.Generate the start condition:
	I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

	//2. Confirm that start generation is completed by checking the SB flag in the SR1
	//Note :- Untill SB is cleared SCL will be stretched (pulled to low)
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_SB));

	//3. Send the address of the Slave with r/nw bit set to w(1) (Total 8 bits)
	I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx , SlaveAddr , SLAVE_READ);

	//4. Confirm that Address phase is completed by checking the ADDR flag :
	while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_ADDR));

	/***For 1 Byte reception only****/
	if(len == 1){
		//Disable ACK
		I2C_ManageAcking(pI2CHandle->pI2Cx , I2C_ACK_DISABLE);

		//clear ADDR flag
		I2C_ClearADDRFlag(pI2CHandle);

		//Wait untill RXNE becomes 1
		while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE));

		//Generate STOP  condition
		if(Sr == I2C_DISABLE_SR){
			I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
		}


		//read Data
		*pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;

	}

	/*****Multi Byte reception*****/

	if(len > 1){
		//Clear the ADDR flag according to its software sequence
		I2C_ClearADDRFlag(pI2CHandle);

		//Recieve Data untill len = 0
		while(len >0 ){

			while(! I2C_GetFlagStatus(pI2CHandle->pI2Cx, I2C_FLAG_RXNE)); //Wait untill RXNE flag is set

			//When 2 bytes remain
			if(len == 2){
				//Disable ACKing
				I2C_ManageAcking(pI2CHandle->pI2Cx , I2C_ACK_DISABLE);

				//Generate STOP  condition
				if(Sr == I2C_DISABLE_SR){
					I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
				}
			}

			*pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
			pRxBuffer++;
			len--;
		}
	}
	//Re- Enable ACK
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
		I2C_ManageAcking(pI2CHandle->pI2Cx , I2C_ACK_ENABLE);
	}
}

void I2C_ManageAcking(I2C_RegDef_t *pI2Cx , uint8_t EnorDi){
	if(EnorDi == I2C_ACK_ENABLE){
		pI2Cx->I2C_CR1 |= (1 << I2C_CR1_ACK);
	}
	else{
		pI2Cx->I2C_CR1 &= ~(1 << I2C_CR1_ACK);
	}
}

/*********************************************************************
 * @fn      		  - I2C_GetFlagStatus
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

uint8_t I2C_GetFlagStatus(I2C_RegDef_t *pI2Cx , uint32_t FlagName){

		if(pI2Cx->I2C_SR1 & FlagName){
			return FLAG_SET;
		}
		return FLAG_RESET;

}

/*********************************************************************
 * @fn      		  - I2C_MasterSendDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -  Complete the below code . Also include the function prototype in header file

 */
uint8_t  I2C_MasterSendDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pTxBuffer, uint32_t Len,uint8_t SlaveAddr,uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pTxBuffer = pTxBuffer;
		pI2CHandle->TxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_TX;
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Enable ITERREN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);

	}

	return busystate;

}

/*********************************************************************
 * @fn      		  - I2C_MasterReceiveDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              - Complete the below code . Also include the fn prototype in header file

 */
uint8_t I2C_MasterRecieveDataIT(I2C_Handle_t *pI2CHandle,uint8_t *pRxBuffer, uint32_t Len, uint8_t SlaveAddr,uint8_t Sr)
{

	uint8_t busystate = pI2CHandle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2CHandle->pRxBuffer = pRxBuffer;
		pI2CHandle->RxLen = Len;
		pI2CHandle->TxRxState = I2C_BUSY_IN_RX;
		pI2CHandle->RxSize = Len; //Rxsize is used in the ISR code to manage the data reception
		pI2CHandle->DevAddr = SlaveAddr;
		pI2CHandle->Sr = Sr;

		//Generate START Condition
		I2C_GenerateStartCondition(pI2CHandle->pI2Cx);

		//Enable ITBUFEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Enable ITEVTEN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Enable ITERREN Control Bit
		pI2CHandle->pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);

	}

	return busystate;
}

/*********************************************************************
 * @fn      		  - I2C_SlaveEnableDisableCallBackEvents
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void I2C_SlaveEnableDisableCallBackEvents(I2C_RegDef_t *pI2Cx , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		//Enable ITBUFEN Control Bit
		pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Enable ITEVTEN Control Bit
		pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITEVTEN);

		//Enable ITERREN Control Bit
		pI2Cx->I2C_CR2 |= (1 << I2C_CR2_ITERREN);
	}
	else{
		//Disable ITBUFEN Control Bit
		pI2Cx->I2C_CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

		//Disable ITEVTEN Control Bit
		pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

		//Disable ITERREN Control Bit
		pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITERREN);
	}
}

/*********************************************************************
 * @fn      		  - I2C_IRQInterruptConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void I2C_IRQInterruptConfig(uint8_t IRQNumber , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(IRQNumber <= 31){
			//Program ISER0 register
			*NVIC_ISER0 |= (1<<IRQNumber); //1 = interrupt enabled , 0 = no effect
		}

		else if(IRQNumber > 31 && IRQNumber < 64){
			//Program ISER1 register
			*NVIC_ISER1 |= (1<<IRQNumber % 32); //1 = interrupt enabled , 0 = no effect

		}
		else if(IRQNumber >= 64 && IRQNumber < 96){
			//Program ISER2 register
			*NVIC_ISER2 |= (1<<IRQNumber % 64); //1 = interrupt enabled , 0 = no effect
		}
	}
	else{
		if(IRQNumber <= 31){
			//Program ISER0 register
			*NVIC_ICER0 |= (1<<IRQNumber); //1 = interrupt disabled , 0 = no effect
		}

		else if(IRQNumber > 31 && IRQNumber < 64){
			//Program ISER1 register
			*NVIC_ICER1 |= (1<<IRQNumber % 32); //1 = interrupt disabled , 0 = no effect

		}
		else if(IRQNumber >= 64 && IRQNumber < 96){
			//Program ISER2 register
			*NVIC_ICER2 |= (1<<IRQNumber % 64); //1 = interrupt disabled , 0 = no effect
		}
	}
}

/*********************************************************************
 * @fn      		  - I2C_IRQPriorityConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void I2C_IRQPriorityConfig(uint8_t IRQNumber , uint32_t IRQPriority){
	//1.first lets find out the ipr register
	//Each iprx is 32 bits and is divided into 4 sections each having an IRQ number and each of 8 bit
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = IRQNumber%4;
	uint8_t shift_amount = (8 * iprx_section) + (8- NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xF <<shift_amount);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);

}


static void I2C_MasterHandleTXEInterrupt(I2C_Handle_t *pI2CHandle){
	if(pI2CHandle->TxLen > 0){
		//1. Load the data in the DR
		pI2CHandle->pI2Cx->I2C_DR = *(pI2CHandle->pTxBuffer);

		//2. Decreament the TxLen
		(pI2CHandle->TxLen) --;

		//3. Increase the Buffer address
		(pI2CHandle->pTxBuffer)++;
	}
}



static void I2C_MasterHandleRXNEInterrupt(I2C_Handle_t *pI2CHandle){
	//we have to do reception:

	/***1 byte reception***/
	if(pI2CHandle->RxSize == 1){

	    *pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;

	    pI2CHandle->RxLen--;

	    //Generate STOP condition
	    if(pI2CHandle->Sr == I2C_DISABLE_SR){
	        I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
	    }

	    //Close RX
	    I2C_CloseRecieveData(pI2CHandle);

	    //Notify application
	    I2C_ApplicationEventCallBack(pI2CHandle , I2C_EV_RX_CMPLT);
	}

	/*****Multiple Byte reception*****/
	if(pI2CHandle->RxSize > 1 ){

		if(pI2CHandle->RxLen == 2){
			//Disable ACKING
			I2C_ManageAcking(pI2CHandle->pI2Cx, DISABLE);
		}

		//Read DR
		*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
		pI2CHandle->RxLen--;
		pI2CHandle->pRxBuffer++;

		if(pI2CHandle->RxLen == 0){
			//Close the I2C reception and notify the application

			//1. Generate the stop condition
			if(pI2CHandle->Sr == I2C_DISABLE_SR){
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}

			//2. Close the I2C_RX
			I2C_CloseRecieveData(pI2CHandle);

			//3. Notify the application
			I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_RX_CMPLT);


		}
	}
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle){
	//Disable all the interrupt controll bits
	//Disable the ITBUFFEN
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);
	//Disable the ITEVEV
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	//reset all the I2CHandle parameters
	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
	pI2CHandle->TxLen = 0;

	//Re enable acking
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
		I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
	}
}

void I2C_CloseRecieveData(I2C_Handle_t *pI2CHandle){
	//Disable all the interrupt controll bits
	//Disable the ITBUFFEN
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);
	//Disable the ITEVEV
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);


	//reset all the I2CHandle parameters
	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxSize = 0;
	pI2CHandle->RxLen = 0;

	//Re enable acking
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
		I2C_ManageAcking(pI2CHandle->pI2Cx, ENABLE);
	}

}

/*********************************************************************
 * @fn      		  - I2C_SlaveSendData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void I2C_SlaveSendData(I2C_RegDef_t *pI2Cx , uint8_t data){
	pI2Cx->I2C_DR = data;
}

/*********************************************************************
 * @fn      		  - I2C_SlaveRecieveData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
uint8_t I2C_SlaveRecieveData(I2C_RegDef_t *pI2Cx){
	return (uint8_t )pI2Cx->I2C_DR;
}
/*********************************************************************
 * @fn      		  - I2C_EV_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle){
	//Interrupt handling for Master and slave mode of a device

	uint32_t temp1 , temp2, temp3;

	temp1 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITEVTEN);
	temp2 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITBUFEN);

	temp3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_SB);
	//1. Handle the interrupt generated by a SB event
	//Note : SB Flag is only applicable for Master Mode
	if(temp1 && temp3){
		//The interrupt is generated because of SB event
		//This block will not be executed in slave mode because for slave SB is always 0
		//In this block lets execute the address phase
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
			I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, SLAVE_WRITE);
		}

		else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
			I2C_ExecuteAddressPhase(pI2CHandle->pI2Cx, pI2CHandle->DevAddr, SLAVE_READ);
		}
	}

	//2. Handle the interrupt generated by a ADDR event
	//Note : When Master mode : Address is sent
	//		 When Slave mode : Address matched with its own adderss
	temp3 =  pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_ADDR);
	if(temp1 && temp3){
			//Interrupt is generated because of ADDR flag:
			I2C_ClearADDRFlag(pI2CHandle);
		}

	//3. Handle the interrupt generated by a BTF(Byte transferred Finished) event
	temp3 =  pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_BTF);
	if(temp1 && temp3){
			//BTF Flag is Set
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
				//Make sure TXE is also set
				if(pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE)){
					//BTF and TXE both are set
					if(pI2CHandle->TxLen == 0){
						//1. Generate the STOP condition
						if(pI2CHandle->Sr == I2C_DISABLE_SR){
							I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
						}

						//2. Reset all the member elements of the handle structure
						I2C_CloseSendData(pI2CHandle);

						//3.Notify the application about transmission complete:
						I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_TX_CMPLT);
				}
				}
			}

	}

	//4. Handle the interrupt generated by a STOPF event
	//Note: Stop detection flag is applicable only slave mode for master . For master this flag will never be set
	//The below code block will not be executed by the master since STOPf will not set in master mode
	temp3 =  pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_STOPF);
	if(temp1 && temp3){

		//STOPF Flag is Set
		//Clear the STOPF (1. read the SR1 , 2.Write to CR1

		pI2CHandle->pI2Cx->I2C_CR1 |= 0x0000; //written a data which will not affect the CR1

		//Notify the application that STOP is detected:
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_STOP);
	}

	//5. Handle for interrupt generated for TXE event:
	temp3 =  pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE);
	if(temp1 && temp2 && temp3){
		//Check for Device mode:
		if( pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)){
			//TXE Flag is Set
			//Here we have to do the Data transmission:
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){

				I2C_MasterHandleTXEInterrupt(pI2CHandle);
			}
		}

		else{
			//Slave Mode
			//First check if Slave is in transmitter mode:
			if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_TRA)){
				I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_DATA_REQ);
			}
		}

	}

	//6. Handle for interrupt generated for RXNE event:
	temp3 =  pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_RXNE);
	if(temp1 && temp2 && temp3){
			//RXNE Flag is Set
		//Check for Device mode:
		if( pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)){
			if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){

				I2C_MasterHandleRXNEInterrupt(pI2CHandle);
			}
		}

		else{
			//Slave Mode
			//First check if Slave is in transmitter mode:
			if(! (pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_TRA))){
				I2C_ApplicationEventCallBack(pI2CHandle, I2C_EV_DATA_RCV);
			}
		}

	}
}

/*********************************************************************
 * @fn      		  - I2C_ER_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @param[in]         -
 *
 * @return            -
 *
 * @Note              -


 */

void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->I2C_CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallBack(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_ARLO);
		//Implement the code to notify the application about the error
	   I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_ARLO);
	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_AF);
		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_AF);
	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_OVR);
		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_OVR);

	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~(1 << I2C_SR1_TIMEOUT);
		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallBack(pI2CHandle, I2C_ERROR_TIMEOUT);
	}

}


__weak void I2C_ApplicationEventCallBack(I2C_Handle_t *pI2CHandle , uint8_t AppEv){
	//This is a weak implementation
}
