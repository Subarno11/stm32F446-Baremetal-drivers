/*
 *  stm32f446xx_spi_drivers.c
 *
 *  Created on: 11-May-2026
 *      Author: Subarno Pal
 */

#include "stm32f446xx.h"


static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle);
static void spi_ovr_interrupt_handle(SPI_Handle_t *pSPIHandle);

/*********************************************************************
 * @fn      		  - SPI_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given SPI peripheral
 *
 * @param[in]         - base address of the spi peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @return            -  none
 *
 * @Note              -  none

 */

void SPI_PeriClockControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pSPIx == SPI1)
			SPI1_PCLK_EN();
		else if(pSPIx == SPI2)
			SPI2_PCLK_EN();
		else if(pSPIx == SPI3)
			SPI3_PCLK_EN();
		else if(pSPIx == SPI4)
			SPI4_PCLK_EN();

	}

	else{
		if(pSPIx == SPI1)
			SPI1_PCLK_DI();
		else if(pSPIx == SPI2)
			SPI2_PCLK_DI();
		else if(pSPIx == SPI3)
			SPI3_PCLK_DI();
		else if(pSPIx == SPI4)
			SPI4_PCLK_DI();
	}
}

/*********************************************************************
 * @fn      		  - SPI_Init
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void SPI_Init(SPI_Handle_t *pSPIHandle){

	//Peripheral Clock Enable :
	SPI_PeriClockControl(pSPIHandle->pSPIx, ENABLE);

	//First lets configure the SPI_CR1 register
	uint32_t tempreg = 0;
	//1. Configure the Device Mode:
	tempreg |= pSPIHandle->SPIConfig.SPI_DeviceMode << SPI_CR1_MSTR;

	//2.Configure the Bus Configurations:
	if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_FD){
		//BIDI Mode Should be cleared
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);
	}

	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_HD){
		//BIDI Mode should be set
		tempreg |= (1<<SPI_CR1_BIDIMODE);
	}
	else if(pSPIHandle->SPIConfig.SPI_BusConfig == SPI_BUS_CONFIG_SIMPLEX_RXONLY){
		//BIDI MOde should be cleared and RXONLY bit set
		tempreg &= ~(1<<SPI_CR1_BIDIMODE);
		tempreg |= 1<<SPI_CR1_RXONLY;
	}

	//3.Configure the SCLK_Speed :
	tempreg |= pSPIHandle->SPIConfig.SPI_SclkSpeed << 3;

	//4. Configure the DFF(Data Frame Format ) :
	tempreg |= pSPIHandle->SPIConfig.SPI_DFF << 11;

	//5. Configure the CPOL :
	tempreg |= pSPIHandle->SPIConfig.SPI_CPOL << 1;

	//5. Configure the CPHA :
	tempreg |= pSPIHandle->SPIConfig.SPI_CPHA << 0;

	pSPIHandle->pSPIx->SPI_CR1 = tempreg;
}

/*********************************************************************
 * @fn      		  - SPI_DeInit
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void SPI_DeInit(SPI_RegDef_t *pSPIx){
	if(pSPIx == SPI1){
		SPI1_REG_RESET();
	}
	else if(pSPIx == SPI2){
		SPI2_REG_RESET();
	}
	else if(pSPIx == SPI3){
		SPI3_REG_RESET();
	}
	else if(pSPIx == SPI4){
		SPI4_REG_RESET();
	}
}

uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName){
	if(pSPIx->SPI_SR & FlagName){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

/*********************************************************************
 * @fn      		  - SPI_SendData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              - This is a blocking call

 */

void SPI_SendData(SPI_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t len){
	while(len > 0){
		//1.Wait untill TXE is set
		while(SPI_GetFlagStatus(pSPIx , SPI_TXE_FLAG) == FLAG_RESET){

		}

		//2. Check the DFF Bit
		if(pSPIx->SPI_CR1 & (1<<SPI_CR1_DFF)){
			//16 Bit DFF
			//1.Load the data in the DR(2 bytes each time)
			pSPIx->SPI_DR = *((uint16_t *)pTxBuffer); //Send 2 bytes data
			if(len>1) len-=2;
			else len = 0;
			pTxBuffer +=2 ; //Increased the buffer address by two bytes

		}
		else{
			//8 bit DFF :
			pSPIx->SPI_DR = *(pTxBuffer); // Send 1 byte data
			len--;
			pTxBuffer++;
		}
	}
}


/*********************************************************************
 * @fn      		  - SPI_RecieveData
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              - This is a blocking call

 */

void SPI_RecieveData(SPI_RegDef_t *pSPIx , uint8_t *pRxBuffer , uint32_t len){
	while(len > 0){
		//1.Wait untill TXE is set
		while(SPI_GetFlagStatus(pSPIx , SPI_RXNE_FLAG) == FLAG_RESET){

		}

		//2. Check the DFF Bit
		if(pSPIx->SPI_CR1 & (1<<SPI_CR1_DFF)){
			//16 Bit DFF
			//1.Read the data from the DR ot RxBuffer Address
			*((uint16_t *)pRxBuffer) = pSPIx->SPI_DR ; //Read 2 bytes data
			if(len>1) len-=2;
			else len = 0;
			pRxBuffer +=2 ; //Increased the buffer address by two bytes

		}
		else{
			//8 bit DFF :
			*(pRxBuffer) = pSPIx->SPI_DR; // Send 1 byte data
			len--;
			pRxBuffer++;
		}
	}
}

/*********************************************************************
 * @fn      		  - SPI_PeripheralControl
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void SPI_PeripheralControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->SPI_CR1 |= (1 << SPI_CR1_SPE);
	}
	else{
		pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SPE);
	}
}

/*********************************************************************
 * @fn      		  - SPI_SSIConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void SPI_SSIConfig(SPI_RegDef_t *pSPIx , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->SPI_CR1 |= (1 << SPI_CR1_SSI);
	}
	else{
		pSPIx->SPI_CR1 &= ~(1 << SPI_CR1_SSI);
	}
}

/*********************************************************************
 * @fn      		  - SPI_SSOEConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		pSPIx->SPI_CR2 |= (1 << SPI_CR2_SSOE);
	}
	else{
		pSPIx->SPI_CR2 &= ~(1 << SPI_CR2_SSOE);
	}
}

/*********************************************************************
 * @fn      		  - SPI_IRQInterruptConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void SPI_IRQInterruptConfig(uint8_t IRQNumber , uint8_t EnorDi){
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
 * @fn      		  - SPI_IRQPriorityConfig
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void SPI_IRQPriorityConfig(uint8_t IRQNumber , uint32_t IRQPriority){
	//1.first lets find out the ipr register
	//Each iprx is 32 bits and is divided into 4 sections each having an IRQ number and each of 8 bit
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = IRQNumber%4;
	uint8_t shift_amount = (8 * iprx_section) + (8- NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xF <<shift_amount);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);

}

/*********************************************************************
 * @fn      		  - SPI_SendDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              - This is a non blocking type

 */
uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle , uint8_t *pTxBuffer , uint32_t len){
	uint8_t state = pSPIHandle->TxState;
	if(state != SPI_BUSY_IN_TX){
		//1.Save the Tx buffer address and Len info in some global variables
		pSPIHandle->pTxBuffer = pTxBuffer;
		pSPIHandle->TxLen = len;

		//2.Mark the SPI state as busy in transmission so that
		//no other code can take over same SPI peripheral untill transmission is over
		pSPIHandle->TxState = SPI_BUSY_IN_TX;

		//3. Enable the TXIE control bit to get interrupt whenever TXE flag is set in SR
		pSPIHandle->pSPIx->SPI_CR2 |= (1<<SPI_CR2_TXEIE);

		//4. Data Transmission will be handled by the ISr code
	}
	return state;
}

/*********************************************************************
 * @fn      		  - SPI_RecieveDataIT
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              - This is a non blocking type

 */
uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIHandle , uint8_t *pRxBuffer , uint32_t len){
	uint8_t state = pSPIHandle->RxState;
	if(state != SPI_BUSY_IN_RX){
		//1.Save the Rx buffer address and Len info in some global variables
		pSPIHandle->pRxBuffer = pRxBuffer;
		pSPIHandle->RxLen = len;

		//2.Mark the SPI state as busy in reciever so that
		//no other code can take over same SPI peripheral untill transmission is over
		pSPIHandle->RxState = SPI_BUSY_IN_RX;

		//3. Enable the RXNEIE control bit to get interrupt whenever RXNE flag is set in SR
		pSPIHandle->pSPIx->SPI_CR2 |= (1<<SPI_CR2_RXNEIE);


	}
	return state;
}

/*********************************************************************
 * @fn      		  - SPI_IRQHandling
 *
 * @brief             -
 *
 * @param[in]         -
 * @param[in]         -
 * @return            -
 *
 * @Note              -

 */

void SPI_IRQHandling(SPI_Handle_t *pSPIHandle){
	uint8_t temp1 , temp2;
	//1. Lets check for TXE flag
	temp1 = pSPIHandle->pSPIx->SPI_SR & (1<<SPI_SR_TXE);
	temp2 = pSPIHandle->pSPIx->SPI_CR2 & (1<<SPI_CR2_TXEIE);

	//When both of this is set then the interrupt is triggered because of setting of TXE flag
	if(temp1 && temp2){
		//handle TXE
		spi_txe_interrupt_handle(pSPIHandle);

	}

	//2. Lets check for RXNE flag
	temp1 = pSPIHandle->pSPIx->SPI_SR & (1<<SPI_SR_RXNE);
	temp2 = pSPIHandle->pSPIx->SPI_CR2 & (1<<SPI_CR2_RXNEIE);

	//When both of this is set then the interrupt is triggered because of setting of TXE flag
	if(temp1 && temp2){
		//handle TXE
		spi_rxne_interrupt_handle(pSPIHandle);

	}

	//3. Lets check for OVR flag
	temp1 = pSPIHandle->pSPIx->SPI_SR & (1<<SPI_SR_OVR);
	temp2 = pSPIHandle->pSPIx->SPI_CR2 & (1<<SPI_CR2_ERRIE);
	if(temp1 && temp2){
		//handle TXE
		spi_ovr_interrupt_handle(pSPIHandle);

	}
}

//Some Helper function implementations:

static void spi_txe_interrupt_handle(SPI_Handle_t *pSPIHandle){
	if(pSPIHandle->pSPIx->SPI_CR1 & (1<<SPI_CR1_DFF)){
		//16 Bit:
		pSPIHandle->pSPIx->SPI_DR = *((uint16_t*)pSPIHandle->pTxBuffer);
		(pSPIHandle->TxLen)-= 2;
		pSPIHandle->pTxBuffer+=2;

	}
	else{
		//8 Bit:
		pSPIHandle->pSPIx->SPI_DR = *(pSPIHandle->pTxBuffer);
		(pSPIHandle->TxLen)--;
		pSPIHandle->pTxBuffer++;
	}

	if(!pSPIHandle->TxLen){
		//Tx Len is zero , so close the SPI communicationand inform the
		//application that Tx is over

		//Close the SPI Transmission abruptly
		SPI_CloseTransmission(pSPIHandle);

		//Now inform the application:
		SPI_ApplicationEventCallBack(pSPIHandle , SPI_EVENT_TX_CMPLT);

	}
}

static void spi_rxne_interrupt_handle(SPI_Handle_t *pSPIHandle){
	if(pSPIHandle->pSPIx->SPI_CR1 & (1<<SPI_CR1_DFF)){
		//16 Bit:
		*((uint16_t*)pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->SPI_DR;
		(pSPIHandle->RxLen)-= 2;
		pSPIHandle->pRxBuffer+=2;

	}
	else{
		//8 Bit:
		*(pSPIHandle->pRxBuffer) = pSPIHandle->pSPIx->SPI_DR;
		(pSPIHandle->RxLen)--;
		pSPIHandle->pRxBuffer++;
	}

	if(!pSPIHandle->RxLen){
		//Rx Len is zero , so close the SPI communicationand inform the
		//application that Rx is over

		//Close the SPI Reception abruptly
		SPI_CloseReception(pSPIHandle);

		//Now inform the application:
		SPI_ApplicationEventCallBack(pSPIHandle , SPI_EVENT_RX_CMPLT);

	}
}

static void spi_ovr_interrupt_handle(SPI_Handle_t *pSPIHandle){
	//Clear the OVR flag:
	//Since OVR flag is hardware controlled we cant directly clear the bit
	//So , for clearing the bit we must give read acces to the SR and DR
	uint8_t temp;
	if(pSPIHandle->TxState != SPI_BUSY_IN_TX){
		temp = pSPIHandle->pSPIx->SPI_DR;
		temp = pSPIHandle->pSPIx->SPI_SR;
	}
	(void)temp;
	//Inform the Application:
	SPI_ApplicationEventCallBack(pSPIHandle , SPI_EVENT_OVR_ERR);
	//If SPI is busy in Tx then application has to call SPI_ClearOVRFlag explicitly
}

void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle){
	pSPIHandle->pSPIx->SPI_CR2 &= ~(1<< SPI_CR2_TXEIE);
	//Reset all parameters
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

void SPI_CloseReception(SPI_Handle_t *pSPIHandle){
	//For that first disable the RXNEIE
	pSPIHandle->pSPIx->SPI_CR2 &= ~(1<< SPI_CR2_RXNEIE);
	//Reset all parameters
	pSPIHandle->pRxBuffer = NULL;
	pSPIHandle->RxLen = 0;
	pSPIHandle->RxState = SPI_READY;
}

void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx ){
	uint8_t temp;
	temp = pSPIx->SPI_DR;
	temp = pSPIx->SPI_SR;
	(void)temp;
}

__weak void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle , uint8_t AppEv){
	//This is a weak implementation
}
