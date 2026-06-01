/*
 * stm32f446xx_gpio_drivers.h (GPIO specific header file)
 *
 *  Created on: 06-Mar-2026
 *      Author: Subarno Pal
 */

#ifndef INC_STM32F446XX_GPIO_DRIVERS_H_
#define INC_STM32F446XX_GPIO_DRIVERS_H_

#include  "stm32f446xx.h" //MCU specific header file

/************Configuration Structure of a GPIO pin***************/
typedef struct{
	uint8_t GPIO_PinNumber;				/*possible values from @GPIO_PIN_NUMBERS*/
	uint8_t GPIO_PinMode;				/*possible values from @GPIO_PIN_MODES*/
	uint8_t GPIO_PinSpeed;				/*possible values from @GPIO_PIN_SPEED*/
	uint8_t GPIO_PinPuPdControl;		/*possible values from @GPIO_PIN_PuPdControl*/
	uint8_t GPIO_PinOPType;				/*possible values from @GPIO_PIN_OpType*/
	uint8_t GPIO_PinAltFunMode;			/*possible values from @GPIO_PIN_ALTFN*/

}GPIO_PinConfig_t;

/************Handle Structure of a GPIO pin***************/
typedef struct{
	GPIO_RegDef_t *pGPIOx;  //This holds the base address of the GPIO port which the pin belongs
	GPIO_PinConfig_t GPIO_PinConfig;  //This holds pin configuration settings

}GPIO_Handle_t;
/*
 * @GPIO_PIN_MODES
 */
#define GPIO_MODE_IN		0
#define GPIO_MODE_OUT		1
#define GPIO_MODE_ALTFN		2
#define GPIO_MODE_ANALOG	3
#define GPIO_MODE_IT_RT		4
#define GPIO_MODE_IT_FT		5
#define GPIO_MODE_IT_RFT	6

/*
 * @GPIO_PIN_OpType
 */
#define GPIO_OP_TYPE_PP		0
#define GPIO_OP_TYPE_OD		1

/*
 * @GPIO_PIN_SPEED
 */
#define GPIO_SPEED_LOW		0
#define GPIO_SPEED_MEDIUM	1
#define GPIO_SPEED_FAST		2
#define GPIO_SPEED_HIGH		3

/*
 * @GPIO_PIN_PuPdControl
 */
#define GPIO_NO_PUPD		0
#define GPIO_PU				1
#define GPIO_PD				2

/*
 * @GPIO_PIN_NUMBERS
 */
#define GPIO_PIN_NO_0		0
#define GPIO_PIN_NO_1		1
#define GPIO_PIN_NO_2		2
#define GPIO_PIN_NO_3		3
#define GPIO_PIN_NO_4		4
#define GPIO_PIN_NO_5		5
#define GPIO_PIN_NO_6		6
#define GPIO_PIN_NO_7		7
#define GPIO_PIN_NO_8		8
#define GPIO_PIN_NO_9		9
#define GPIO_PIN_NO_10		10
#define GPIO_PIN_NO_11		11
#define GPIO_PIN_NO_12		12
#define GPIO_PIN_NO_13		13
#define GPIO_PIN_NO_14		14
#define GPIO_PIN_NO_15		15

/*
 * @GPIO_PIN_ALTFN
 */
#define GPIO_PIN_ALTFN_0		0
#define GPIO_PIN_ALTFN_1		1
#define GPIO_PIN_ALTFN_2		2
#define GPIO_PIN_ALTFN_3		3
#define GPIO_PIN_ALTFN_4		4
#define GPIO_PIN_ALTFN_5		5
#define GPIO_PIN_ALTFN_6		6
#define GPIO_PIN_ALTFN_7		7
#define GPIO_PIN_ALTFN_8		8
#define GPIO_PIN_ALTFN_9		9
#define GPIO_PIN_ALTFN_10		10
#define GPIO_PIN_ALTFN_11		11
#define GPIO_PIN_ALTFN_12		12
#define GPIO_PIN_ALTFN_13		13
#define GPIO_PIN_ALTFN_14		14
#define GPIO_PIN_ALTFN_15		15
/*************************************************************
 * 					APIs supported by this Driver
 * 		For more info about the APIs check the function description
 * ***********************************************************/

//Peripheral Clock Setup:
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx , uint8_t EnorDi);

//Init and De-Init:
void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_RegDef_t *pGPIOx);

//Data Read and Write:
uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx , uint16_t Value);
void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber);

//IRQ Configuration and ISR handling:
void GPIO_IRQInterruptConfig(uint8_t IRQNumber , uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber , uint32_t IRQPriority);
void GPIO_IRQHandling( uint8_t PinNumber);




#endif /* INC_STM32F446XX_GPIO_DRIVERS_H_ */
