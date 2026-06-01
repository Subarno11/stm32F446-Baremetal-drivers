/*
 * stm32f446xx.h (MCU SPecific Header File)
 *
 *  Created on: 05-Mar-2026
 *      Author: Subarno Pal
 */

#ifndef INC_STM32F446XX_H_
#define INC_STM32F446XX_H_

#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#define __vo volatile
#define __weak __attribute__((weak))


/***************ARM Cortex M4 Processor Specific Details*************************/
//NVIC ISERx register addresses :
#define NVIC_ISER0				((__vo uint32_t *)0xE000E100)
#define NVIC_ISER1				((__vo uint32_t *)0xE000E104)
#define NVIC_ISER2				((__vo uint32_t *)0xE000E108)
#define NVIC_ISER3				((__vo uint32_t *)0xE000E10c)

//ARM Cortex Mx Processor NVIC ICERx register Address
#define NVIC_ICER0				((__vo uint32_t *)0XE000E180)
#define NVIC_ICER1				((__vo uint32_t *)0xE000E184)
#define NVIC_ICER2				((__vo uint32_t *)0xE000E188)
#define NVIC_ICER3				((__vo uint32_t *)0xE000E18c)

//IRQ Priorities:
#define NVIC_IRQ_PRI0			0
#define NVIC_IRQ_PRI1			1
#define NVIC_IRQ_PRI2			2
#define NVIC_IRQ_PRI3			3
#define NVIC_IRQ_PRI4			4
#define NVIC_IRQ_PRI5			5
#define NVIC_IRQ_PRI6			6
#define NVIC_IRQ_PRI7			7
#define NVIC_IRQ_PRI8			8
#define NVIC_IRQ_PRI9			9
#define NVIC_IRQ_PRI10			10
#define NVIC_IRQ_PRI11			11
#define NVIC_IRQ_PRI12			12
#define NVIC_IRQ_PRI13			13
#define NVIC_IRQ_PRI14			14
#define NVIC_IRQ_PRI15			15

//ARM Cortex Mx Processor Priority register configuration:
#define NVIC_PR_BASE_ADDR		((__vo uint32_t *)0xE000E400)

#define NO_PR_BITS_IMPLEMENTED			4
/*******BASE ADDRESS OF FLASH AND SRAM**************/
#define FLASH_BASE_ADDR			0x08000000U
#define SRAM1_BASE_ADDR         0x20000000U //112 kB
#define SRAM2_BASE_ADDR			0x2001C000U
#define SRAM 					SRAM1_Base_Addr
#define ROM_BASE_ADDR			0x1FFF0000U

/*******AHBx and APBx bus Peripheral Base address**************/
#define PERIPH_ADDR				0x40000000U
#define APB1_PERIPH_BASE		PERIPH_ADDR
#define APB2_PERIPH_BASE		0x40010000U
#define AHB1_PERIPH_BASE		0x40020000U
#define AHB2_PERIPH_BASE	 	0x50000000U
#define AHB3_PERIPH_BASE		0xA0001000U

/*******Base address of peripherals which are hanging on the AHB1 Bus**************/
#define GPIOA_BASE_ADDR 		(AHB1_PERIPH_BASE + 0X0000U)
#define GPIOB_BASE_ADDR 		(AHB1_PERIPH_BASE + 0X0400U)
#define GPIOC_BASE_ADDR 		(AHB1_PERIPH_BASE + 0X0800U)
#define GPIOD_BASE_ADDR 		(AHB1_PERIPH_BASE + 0X0C00U)
#define GPIOE_BASE_ADDR 		(AHB1_PERIPH_BASE + 0X1000U)
#define GPIOF_BASE_ADDR 		(AHB1_PERIPH_BASE + 0X1400U)
#define GPIOG_BASE_ADDR 		(AHB1_PERIPH_BASE + 0X1800U)
#define GPIOH_BASE_ADDR 		(AHB1_PERIPH_BASE + 0X1C00U)
#define RCC_BASE_ADDR			(AHB1_PERIPH_BASE + 0x3800U)

/*******Base address of peripherals which are hanging on the APB1 Bus**************/
#define I2C1_BASE_ADDR			(APB1_PERIPH_BASE + 0X5400U)
#define I2C2_BASE_ADDR			(APB1_PERIPH_BASE + 0X5800U)
#define I2C3_BASE_ADDR			(APB1_PERIPH_BASE + 0X5C00U)

#define SPI2_BASE_ADDR			(APB1_PERIPH_BASE + 0X3800U)
#define SPI3_BASE_ADDR			(APB1_PERIPH_BASE + 0X3C00U)

#define USART2_BASE_ADDR		(APB1_PERIPH_BASE + 0X4400U)
#define USART3_BASE_ADDR		(APB1_PERIPH_BASE + 0X4800U)
#define UART4_BASE_ADDR			(APB1_PERIPH_BASE + 0X4C00U)
#define UART5_BASE_ADDR			(APB1_PERIPH_BASE + 0X5000U)

/*******Base address of peripherals which are hanging on the APB2 Bus**************/
#define SPI1_BASE_ADDR			(APB2_PERIPH_BASE + 0X3000U)
#define SPI4_BASE_ADDR			(APB2_PERIPH_BASE + 0X3400U)
#define USART1_BASE_ADDR		(APB2_PERIPH_BASE + 0X1000U)
#define USART6_BASE_ADDR		(APB2_PERIPH_BASE + 0X1400U)
#define EXTI_BASE_ADDR			(APB2_PERIPH_BASE + 0X3C00U)
#define SYSCFG_BASE_ADDR		(APB2_PERIPH_BASE + 0X3800U)

/*******Peripheral Register Definition Structures**************/

//GPIO registers Definitions
typedef struct{
	__vo uint32_t MODER;	//GPIO port mode register					Address Offset: 0x00
	__vo uint32_t OTYPER;	//GPIO port output type register 			Address Offset: 0x04
	__vo uint32_t OSPEEDER;	//GPIO port output speed register			Address Offset: 0x08
	__vo uint32_t PUPDR;	//GPIO port pull-up/pull-down register    	Address Offset: 0x0C
	__vo uint32_t IDR;		//GPIO port input data register				Address Offset: 0x10
	__vo uint32_t ODR;		//GPIO port output data register 			Address Offset: 0x14
	__vo uint32_t BSRR;		//GPIO port bit set/reset register			Address Offset: 0x18
	__vo uint32_t LCKR;		//GPIO port configuration lock register		Address Offset: 0x1c
	__vo uint32_t AFR[2];	//It has two subsets AFRL = AFR[0], Offset : 0x20 and AFRL = AFR[1], offset: 0x24
}GPIO_RegDef_t;


//RCC Register Definitions :
typedef struct{
	__vo uint32_t CR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CFGR;
	__vo uint32_t CIR;

	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;

	uint32_t RESERVED0;

	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;

	uint32_t RESERVED1[2];

	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;

	uint32_t RESERVED2;

	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;

	uint32_t RESERVED3[2];

	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;

	uint32_t RESERVED4;

	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;

	uint32_t RESERVED5[2];

	__vo uint32_t BDCR;
	__vo uint32_t CSR;

	uint32_t RESERVED6[2];

	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;
	__vo uint32_t PLLSAICFGR;
	__vo uint32_t DCKCFGR;
	__vo uint32_t CKGATENR;
	__vo uint32_t DCKCFGR2;


}RCC_RegDef_t;


//EXTI register definition:
typedef struct{
	__vo uint32_t IMR;
	__vo uint32_t EMR;
	__vo uint32_t RTSR;
	__vo uint32_t FTSR;
	__vo uint32_t SWIER;
	__vo uint32_t PR;
}EXTI_RegDef_t;

//SYSCFG register maps:
typedef struct{
	__vo uint32_t MEMRMP;   		//Offset: 0x00
	__vo uint32_t PMC;				//Offset: 0x04
	__vo uint32_t EXTICR[4];		//Offset: 0x08 - 0x14
	uint32_t RESERVED1[2];			//Offset: 0x18 - 0x1c
	__vo uint32_t CMPCR;			//Offset: 0x20
	uint32_t RESERVED2[2];			//Offset: 0x24 - 0x28
	__vo uint32_t CFGR;				//Offset: 0x2c

}SYSCFG_RegDef_t;


// SPI register definitions:
typedef struct{
	__vo uint32_t SPI_CR1;
	__vo uint32_t SPI_CR2;
	__vo uint32_t SPI_SR;
	__vo uint32_t SPI_DR;
	__vo uint32_t SPI_CRCPR;
	__vo uint32_t SPI_RXCRCR;
	__vo uint32_t SPI_TXCRCR;
	__vo uint32_t SPI_I2SCFGR;
	__vo uint32_t SPI_I2SPR;
}SPI_RegDef_t;

//I2C register definitions:
typedef struct{
	__vo uint32_t I2C_CR1;
	__vo uint32_t I2C_CR2;
	__vo uint32_t I2C_OAR1;
	__vo uint32_t I2C_OAR2;
	__vo uint32_t I2C_DR;
	__vo uint32_t I2C_SR1;
	__vo uint32_t I2C_SR2;
	__vo uint32_t I2C_CCR;
	__vo uint32_t I2C_TRISE;
	__vo uint32_t I2C_FLTR;

}I2C_RegDef_t;

//USART register definitions:
typedef struct{
	__vo uint32_t USART_SR;
	__vo uint32_t USART_DR;
	__vo uint32_t USART_BRR;
	__vo uint32_t USART_CR1;
	__vo uint32_t USART_CR2;
	__vo uint32_t USART_CR3;
	__vo uint32_t USART_GTPR;

}USART_RegDef_t;

/*******Peripheral Definitions (Peripheral base address typecasted to xxx_RegDef_t)**************/

#define GPIOA		((GPIO_RegDef_t *)GPIOA_BASE_ADDR)
#define GPIOB		((GPIO_RegDef_t *)GPIOB_BASE_ADDR)
#define GPIOC		((GPIO_RegDef_t *)GPIOC_BASE_ADDR)
#define GPIOD		((GPIO_RegDef_t *)GPIOD_BASE_ADDR)
#define GPIOE		((GPIO_RegDef_t *)GPIOE_BASE_ADDR)
#define GPIOF		((GPIO_RegDef_t *)GPIOF_BASE_ADDR)
#define GPIOG		((GPIO_RegDef_t *)GPIOG_BASE_ADDR)
#define GPIOH		((GPIO_RegDef_t *)GPIOH_BASE_ADDR)


#define RCC 		((RCC_RegDef_t *)RCC_BASE_ADDR)

#define EXTI		((EXTI_RegDef_t *)EXTI_BASE_ADDR)

#define SYSCFG		((SYSCFG_RegDef_t *)SYSCFG_BASE_ADDR)

#define SPI1		((SPI_RegDef_t *)SPI1_BASE_ADDR)
#define SPI2		((SPI_RegDef_t *)SPI2_BASE_ADDR)
#define SPI3		((SPI_RegDef_t *)SPI3_BASE_ADDR)
#define SPI4		((SPI_RegDef_t *)SPI4_BASE_ADDR)


#define I2C1		((I2C_RegDef_t *)I2C1_BASE_ADDR)
#define I2C2		((I2C_RegDef_t *)I2C2_BASE_ADDR)
#define I2C3		((I2C_RegDef_t *)I2C3_BASE_ADDR)

#define USART1		((USART_RegDef_t *)USART1_BASE_ADDR)
#define USART2		((USART_RegDef_t *)USART2_BASE_ADDR)
#define USART3		((USART_RegDef_t *)USART3_BASE_ADDR)
#define UART4		((USART_RegDef_t *)UART4_BASE_ADDR)
#define UART5		((USART_RegDef_t *)UART5_BASE_ADDR)
#define USART6		((USART_RegDef_t *)USART6_BASE_ADDR)

/*******Clock Enable Macros for GPIOx peripherals**********/
#define GPIOA_PCLK_EN()		(RCC->AHB1ENR |=(1<<0))
#define GPIOB_PCLK_EN()		(RCC->AHB1ENR |=(1<<1))
#define GPIOC_PCLK_EN()		(RCC->AHB1ENR |=(1<<2))
#define GPIOD_PCLK_EN()		(RCC->AHB1ENR |=(1<<3))
#define GPIOE_PCLK_EN()		(RCC->AHB1ENR |=(1<<4))
#define GPIOF_PCLK_EN()		(RCC->AHB1ENR |=(1<<5))
#define GPIOG_PCLK_EN()		(RCC->AHB1ENR |=(1<<6))
#define GPIOH_PCLK_EN()		(RCC->AHB1ENR |=(1<<7))

/*******Clock Enable Macros for I2Cx peripherals**********/
#define I2C1_PCLK_EN()		(RCC->APB1ENR |= (1<<21))
#define I2C2_PCLK_EN()		(RCC->APB1ENR |= (1<<22))
#define I2C3_PCLK_EN()		(RCC->APB1ENR |= (1<<23))

/*******Clock Enable Macros for SPIx peripherals**********/
#define SPI1_PCLK_EN()		(RCC->APB2ENR |= (1<<12))
#define SPI2_PCLK_EN()		(RCC->APB1ENR |= (1<<14))
#define SPI3_PCLK_EN()		(RCC->APB1ENR |= (1<<15))
#define SPI4_PCLK_EN()		(RCC->APB2ENR |= (1<<13))

/*******Clock Enable Macros for USARTx peripherals**********/
#define USART1_PCLK_EN()		(RCC->APB2ENR |= (1<<4))
#define USART2_PCLK_EN()		(RCC->APB1ENR |= (1<<17))
#define USART3_PCLK_EN()		(RCC->APB1ENR |= (1<<18))
#define UART4_PCLK_EN()			(RCC->APB1ENR |= (1<<19))
#define UART5_PCLK_EN()			(RCC->APB1ENR |= (1<<20))
#define USART6_PCLK_EN()		(RCC->APB2ENR |= (1<<5))

/*******Clock Enable Macros for SYSCFG peripherals**********/
#define SYSCFG_PCLK_EN()		(RCC->APB2ENR |= (1<<14))


/*******Clock Disable Macros for GPIOx peripherals**********/
#define GPIOA_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<0))
#define GPIOB_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<1))
#define GPIOC_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<2))
#define GPIOD_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<3))
#define GPIOE_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<4))
#define GPIOF_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<5))
#define GPIOG_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<6))
#define GPIOH_PCLK_DI()		(RCC->AHB1ENR &= ~(1<<7))

/*******Clock Disable Macros for I2Cx peripherals**********/
#define I2C1_PCLK_DI()		(RCC->APB1ENR &= ~(1<<21))
#define I2C2_PCLK_DI()		(RCC->APB1ENR &= ~(1<<22))
#define I2C3_PCLK_DI()		(RCC->APB1ENR &= ~(1<<23))

/*******Clock Disable Macros for SPIx peripherals**********/
#define SPI1_PCLK_DI()		(RCC->APB2ENR &= ~(1<<12))
#define SPI2_PCLK_DI()		(RCC->APB1ENR &= ~(1<<14))
#define SPI3_PCLK_DI()		(RCC->APB1ENR &= ~(1<<15))
#define SPI4_PCLK_DI()		(RCC->APB2ENR &= ~(1<<13))

/*******Clock Disable Macros for USARTx peripherals**********/
#define USART1_PCLK_DI()		(RCC->APB2ENR &= ~(1<<4))
#define USART2_PCLK_DI()		(RCC->APB1ENR &= ~(1<<17))
#define USART3_PCLK_DI()		(RCC->APB1ENR &= ~(1<<18))
#define UART4_PCLK_DI()			(RCC->APB1ENR &= ~(1<<19))
#define UART5_PCLK_DI()			(RCC->APB1ENR &= ~(1<<20))
#define USART6_PCLK_DI()		(RCC->APB2ENR &= ~(1<<5))

/*******Clock Disable Macros for SYSCFG peripherals**********/
#define SYSCFG_PCLK_DI()		(RCC->APB2ENR &= ~(1<<14))

/*******MACROS to reset the GPIOx peripherals**********/
#define GPIOA_REG_RESET()		do{RCC->AHB1RSTR |= (1<<0); RCC->AHB1RSTR &= ~(1<<0);}while(0) //Executes only one time , this technique used to program multiple statemnt in a single macro
#define GPIOB_REG_RESET()		do{RCC->AHB1RSTR |= (1<<1); RCC->AHB1RSTR &= ~(1<<1);}while(0)
#define GPIOC_REG_RESET()		do{RCC->AHB1RSTR |= (1<<2); RCC->AHB1RSTR &= ~(1<<2);}while(0)
#define GPIOD_REG_RESET()		do{RCC->AHB1RSTR |= (1<<3); RCC->AHB1RSTR &= ~(1<<3);}while(0)
#define GPIOE_REG_RESET()		do{RCC->AHB1RSTR |= (1<<4); RCC->AHB1RSTR &= ~(1<<4);}while(0)
#define GPIOF_REG_RESET()		do{RCC->AHB1RSTR |= (1<<5); RCC->AHB1RSTR &= ~(1<<5);}while(0)
#define GPIOG_REG_RESET()		do{RCC->AHB1RSTR |= (1<<6); RCC->AHB1RSTR &= ~(1<<6);}while(0)
#define GPIOH_REG_RESET()		do{RCC->AHB1RSTR |= (1<<7); RCC->AHB1RSTR &= ~(1<<7);}while(0)

/*******MACROS to reset the SPIx peripherals**********/
#define SPI1_REG_RESET()		do{RCC->APB2RSTR |= (1<<12); RCC->AHB1RSTR &= ~(1<<12);}while(0)
#define SPI2_REG_RESET()		do{RCC->APB1RSTR |= (1<<14); RCC->AHB1RSTR &= ~(1<<14);}while(0)
#define SPI3_REG_RESET()		do{RCC->APB1RSTR |= (1<<15); RCC->AHB1RSTR &= ~(1<<15);}while(0)
#define SPI4_REG_RESET()		do{RCC->APB2RSTR |= (1<<13); RCC->AHB1RSTR &= ~(1<<13);}while(0)


/*******MACROS to reset the I2Cx peripherals**********/
#define I2C1_REG_RESET()		do{RCC->APB1RSTR |= (1<<21); RCC->AHB1RSTR &= ~(1<<21);}while(0)
#define I2C2_REG_RESET()		do{RCC->APB1RSTR |= (1<<22); RCC->AHB1RSTR &= ~(1<<22);}while(0)
#define I2C3_REG_RESET()		do{RCC->APB1RSTR |= (1<<23); RCC->AHB1RSTR &= ~(1<<23);}while(0)

/*******MACROS to reset the USARTx peripherals**********/
#define USART1_REG_RESET()		do{RCC->APB2RSTR |= (1<<4); RCC->AHB1RSTR &= ~(1<<4);}while(0)
#define USART2_REG_RESET()		do{RCC->APB1RSTR |= (1<<17); RCC->AHB1RSTR &= ~(1<<17);}while(0)
#define USART3_REG_RESET()		do{RCC->APB1RSTR |= (1<<18); RCC->AHB1RSTR &= ~(1<<18);}while(0)
#define UART4_REG_RESET()		do{RCC->APB1RSTR |= (1<<19); RCC->AHB1RSTR &= ~(1<<19);}while(0)
#define UART5_REG_RESET()		do{RCC->APB1RSTR |= (1<<20); RCC->AHB1RSTR &= ~(1<<20);}while(0)
#define USART6_REG_RESET()		do{RCC->APB2RSTR |= (1<<5); RCC->AHB1RSTR &= ~(1<<5);}while(0)

/***********Macro which returns port code for GPIOx base address ************/
#define GPIO_BASEADDR_TO_CODE(x)		 (	(x == GPIOA)?0:\
							        		(x == GPIOB)?1:\
							        		(x == GPIOC)?2:\
											(x == GPIOD)?3:\
											(x == GPIOE)?4:\
											(x == GPIOF)?5:\
											(x == GPIOG)?6:\
											(x == GPIOH)?7:0)
/*******Some Generic Macros************/
#define ENABLE 					1
#define DISABLE					0
#define SET						ENABLE
#define RESET					DISABLE
#define GPIO_PIN_SET			SET
#define GPIO_PIN_RESET			RESET
#define FLAG_RESET				RESET
#define FLAG_SET				SET
/*******Macros for IRQ numbers of the EXTI lines************/
#define IRQ_NO_EXTI0			6
#define IRQ_NO_EXTI1			7
#define IRQ_NO_EXTI2			8
#define IRQ_NO_EXTI3			9
#define IRQ_NO_EXTI4			10
#define IRQ_NO_EXTI9_5			23
#define IRQ_NO_EXTI15_10		40

/*******Macros for IRQ numbers of the SPI Peripherals************/
#define IRQ_NO_SPI1				35
#define IRQ_NO_SPI2				36
#define IRQ_NO_SPI3				51
#define IRQ_NO_SPI4				84

/*******Macros for IRQ numbers of the I2C Peripherals************/

#define IRQ_NO_I2C1_EV				31
#define IRQ_NO_I2C1_ER				32
#define IRQ_NO_I2C2_EV				33
#define IRQ_NO_I2C2_ER				34
#define IRQ_NO_I2C3_EV				72
#define IRQ_NO_I2C3_ER				73

/***********************Macros for IRQ Numbers of the USART Peripherals*********************************************/
#define IRQ_NO_USART1			37
#define IRQ_NO_USART2			38
#define IRQ_NO_USART3			39
#define IRQ_NO_UART4			52
#define IRQ_NO_UART5			53
#define IRQ_NO_USART6			71

/*************************************************************************
 * Bit Position Macros for SPI peripherals
 **************************************************************************/


/*
 * Bit position macros for SPI_CR1
 */
#define SPI_CR1_CPHA			0
#define SPI_CR1_CPOL			1
#define SPI_CR1_MSTR			2
#define SPI_CR1_BR 			    3
#define SPI_CR1_SPE				6
#define SPI_CR1_LSBFIRST		7
#define SPI_CR1_SSI				8
#define SPI_CR1_SSM				9
#define SPI_CR1_RXONLY			10
#define SPI_CR1_DFF				11
#define SPI_CR1_CRCNEXT			12
#define SPI_CR1_CRCEN			13
#define SPI_CR1_BIDIOE			14
#define SPI_CR1_BIDIMODE		15

/*
 * Bit position macros for SPI_CR2
 */
#define SPI_CR2_RXDMAEN			0
#define SPI_CR2_TXDMAEN			1
#define SPI_CR2_SSOE			2
#define SPI_CR2_FRF				4
#define SPI_CR2_ERRIE			5
#define SPI_CR2_RXNEIE			6
#define SPI_CR2_TXEIE			7

/*
 * Bit position macros for SPI_SR
 *
 */
#define SPI_SR_RXNE				0
#define SPI_SR_TXE				1
#define SPI_SR_CHSIDE			2
#define SPI_SR_UDR				3
#define SPI_SR_CRCERR			4
#define SPI_SR_MODF				5
#define SPI_SR_OVR				6
#define SPI_SR_BSY				7
#define SPI_SR_FRE				8

/*************************************************************************
 * Bit Position Macros for I2C peripherals
 **************************************************************************/

/*
 * Bit position macros for I2C_CR1
 */
#define I2C_CR1_PE				0
#define I2C_CR1_SMBUS			1
#define I2C_CR1_SMBTYPE			3
#define I2C_CR1_ENARP			4
#define I2C_CR1_ENPEC			5
#define I2C_CR1_ENGC			6
#define I2C_CR1_NOSTRETCH		7
#define I2C_CR1_START			8
#define I2C_CR1_STOP			9
#define I2C_CR1_ACK				10
#define I2C_CR1_POS				11
#define I2C_CR1_PEC				12
#define I2C_CR1_ALERT			13
#define I2C_CR1_SWRST			15

/*
 * Bit position macros for I2C_CR2
 */
#define I2C_CR2_FREQ			0
#define I2C_CR2_ITERREN			8
#define I2C_CR2_ITEVTEN			9
#define I2C_CR2_ITBUFEN			10
#define I2C_CR2_DMAEN			11
#define I2C_CR2_LAST			12

/*
 * Bit position macros for I2C_SR1
 */
#define I2C_SR1_SB				0
#define I2C_SR1_ADDR			1
#define I2C_SR1_BTF				2
#define I2C_SR1_ADD10			3
#define I2C_SR1_STOPF			4
#define I2C_SR1_RXNE			6
#define I2C_SR1_TXE				7
#define I2C_SR1_BERR			8
#define I2C_SR1_ARLO			9
#define I2C_SR1_AF				10
#define I2C_SR1_OVR				11
#define I2C_SR1_PECERR			12
#define I2C_SR1_TIMEOUT			14
#define I2C_SR1_SMBALERT		15

/*
 * Bit position macros for I2C_SR2
 */
#define I2C_SR2_MSL				0
#define I2C_SR2_BUSY			1
#define I2C_SR2_TRA				2
#define I2C_SR2_GENCALL			4
#define I2C_SR2_SMBDEFAULT		5
#define I2C_SR2_SMBHOST			6
#define I2C_SR2_DUALF			7
#define I2C_SR2_PEC				8

/*
 * Bit position macros for I2C_CCR
 */
#define I2C_CCR_CCR				0
#define I2C_CCR_DUTY			14
#define I2C_CCR_FS				15

/*************************************************************************
 * Bit Position Macros for USART peripherals
 **************************************************************************/

/*
 * Bit position macros for USART_SR
 */
#define USART_SR_PE				0
#define USART_SR_FE				1
#define USART_SR_NF				2
#define USART_SR_ORE			3
#define USART_SR_IDLE			4
#define USART_SR_RXNE			5
#define USART_SR_TC				6
#define USART_SR_TXE			7
#define USART_SR_LBD			8
#define USART_SR_CTS			9

/*
 * Bit position macros for USART_CR1
 */
#define USART_CR1_SBK			0
#define USART_CR1_RWU			1
#define USART_CR1_RE			2
#define USART_CR1_TE			3
#define USART_CR1_IDLEIE		4
#define USART_CR1_RXNEIE		5
#define USART_CR1_TCIE			6
#define USART_CR1_TXEIE			7
#define USART_CR1_PEIE			8
#define USART_CR1_PS			9
#define USART_CR1_PCE			10
#define USART_CR1_WAKE			11
#define USART_CR1_M				12
#define USART_CR1_UE			13
#define USART_CR1_OVER8			15
/*
 * Bit position macros for USART_CR2
 */
#define USART_CR2_ADD			0
#define USART_CR2_LBDL			5
#define USART_CR2_LBDIE			6
#define USART_CR2_LBCL			8
#define USART_CR2_CPHA			9
#define USART_CR2_CPOL			10
#define USART_CR2_CLKEN			11
#define USART_CR2_STOP			12
#define USART_CR2_LINEN			14

/*
 * Bit position macros for USART_CR3
 */
#define USART_CR3_EIE			0
#define USART_CR3_IREN			1
#define USART_CR3_IRLP			2
#define USART_CR3_HDSEL			3
#define USART_CR3_NACK			4
#define USART_CR3_SCEN			5
#define USART_CR3_DMAR			6
#define USART_CR3_DMAT			7
#define USART_CR3_RTSE			8
#define USART_CR3_CTSE			9
#define USART_CR3_CTSIE			10
#define USART_CR3_ONEBIT		11

#include "stm32f446xx_rcc_drivers.h"
#include "stm32f446xx_gpio_drivers.h"
#include "stm32f446xx_spi_drivers.h"
#include "stm32f446xx_i2c_drivers.h"
#include "stm32f446xx_usart_drivers.h"
#endif /* INC_STM32F446XX_H_ */
