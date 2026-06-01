/*
 * stm32f446xx_spi_drivers.h
 *
 *  Created on: 11-May-2026
 *      Author: Subarno Pal
 */

#ifndef INC_STM32F446XX_SPI_DRIVERS_H_
#define INC_STM32F446XX_SPI_DRIVERS_H_

#include  "stm32f446xx.h"

/************Configuration Structure of a SPI Peripherals***************/
typedef struct{
	uint8_t SPI_DeviceMode;
	uint8_t SPI_BusConfig;
	uint8_t SPI_DFF;
	uint8_t SPI_CPHA;
	uint8_t SPI_CPOL;
	uint8_t SPI_SSM;
	uint8_t SPI_SclkSpeed;

}SPI_Config_t;

/************Handle Structure of a SPI Peripherals***************/
typedef struct{
	SPI_RegDef_t *pSPIx;
	SPI_Config_t SPIConfig;
	uint8_t *pTxBuffer;  //To store the application's Tx buffer address
	uint8_t *pRxBuffer; //To store the application's Tx buffer address
	uint32_t TxLen;    //To store the Tx Len
	uint32_t RxLen;	  //TO store the Rx Len
	uint8_t	TxState; //To store the Tx state
	uint8_t RxState; //To store the Rx state

}SPI_Handle_t;

/******Macros for Configurable Peripherals of SPI*********/

/*
 * @SPI_DeviceMode
 */
#define SPI_DEVICE_MODE_MASTER 					1
#define SPI_DEVICE_MODE_SLAVE 					0

/*
 * @SPI_BusConfig
 */
#define SPI_BUS_CONFIG_FD						1
#define SPI_BUS_CONFIG_HD						2
#define SPI_BUS_CONFIG_SIMPLEX_RXONLY			3

/*
 * @SPI_SclkSpeed
 */
#define SPI_SCLK_SPEED_DIV2						0
#define SPI_SCLK_SPEED_DIV4						1
#define SPI_SCLK_SPEED_DIV8						2
#define SPI_SCLK_SPEED_DIV16					3
#define SPI_SCLK_SPEED_DIV32					4
#define SPI_SCLK_SPEED_DIV64					5
#define SPI_SCLK_SPEED_DIV128					6
#define SPI_SCLK_SPEED_DIV256					7

/*
 * @SPI_DFF
 */
#define SPI_DFF_8BITS							0
#define SPI_DFF_16BITS							1

/*
 * @SPI_CPOL
 */
#define SPI_CPOL_HIGH							1
#define SPI_CPOL_LOW							0

/*
 * @SPI_CPHA
 */
#define SPI_CPHA_HIGH							1
#define SPI_CPHA_LOW							0

/*
 *@SPI_SSM
 */
#define SPI_SSM_EN								1
#define SPI_SSM_DI								0


/*
 * SPI related Status flag definitions
 */
#define SPI_TXE_FLAG							(1<<SPI_SR_TXE)
#define SPI_RXNE_FLAG							(1<<SPI_SR_RXNE)
#define SPI_BUSY_FLAG							(1<<SPI_SR_BSY)


/*
 * Possible SPI Application States:
 */
#define SPI_READY								0
#define SPI_BUSY_IN_RX							1
#define SPI_BUSY_IN_TX							2

/*
 * Possible SPI Application events:
 */
#define SPI_EVENT_TX_CMPLT						1
#define SPI_EVENT_RX_CMPLT						2
#define SPI_EVENT_OVR_ERR						3
#define SPI_EVENT_CRC_ERR						4
/*************************************************************
 * 					APIs supported by this Driver
 * 		For more info about the APIs check the function description
 * ***********************************************************/

//Peripheral Clock Setup:
void SPI_PeriClockControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi);

//Init and De-Init:
void SPI_Init(SPI_Handle_t *pSPIHandle);
void SPI_DeInit(SPI_RegDef_t *pSPIx);

//Data Send and Recieve:
void SPI_SendData(SPI_RegDef_t *pSPIx , uint8_t *pTxBuffer , uint32_t len);
void SPI_RecieveData(SPI_RegDef_t *pSPIx , uint8_t *pRxBuffer , uint32_t len);

uint8_t SPI_SendDataIT(SPI_Handle_t *pSPIHandle , uint8_t *pTxBuffer , uint32_t len);
uint8_t SPI_RecieveDataIT(SPI_Handle_t *pSPIx , uint8_t *pRxBuffer , uint32_t len);

//IRQ Configuration and ISR handling:
void SPI_IRQInterruptConfig(uint8_t IRQNumber , uint8_t EnorDi);
void SPI_IRQPriorityConfig(uint8_t IRQNumber , uint32_t IRQPriority);
void SPI_IRQHandling(SPI_Handle_t *pSPIHandle);

/*
 * Other Peripheral Control APIs
 */
uint8_t SPI_GetFlagStatus(SPI_RegDef_t *pSPIx , uint32_t FlagName);
void SPI_PeripheralControl(SPI_RegDef_t *pSPIx , uint8_t EnorDi);
void SPI_SSIConfig(SPI_RegDef_t *pSPIx , uint8_t EnorDi);
void SPI_SSOEConfig(SPI_RegDef_t *pSPIx , uint8_t EnorDi);
void SPI_ClearOVRFlag(SPI_RegDef_t *pSPIx );
void SPI_CloseTransmission(SPI_Handle_t *pSPIHandle);
void SPI_CloseReception(SPI_Handle_t *pSPIHandle);


/*
 * Application Callback
 */
void SPI_ApplicationEventCallBack(SPI_Handle_t *pSPIHandle , uint8_t AppEv);
#endif /* INC_STM32F446XX_SPI_DRIVERS_H_ */
