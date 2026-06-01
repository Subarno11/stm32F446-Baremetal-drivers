/*
 * stm32f446xx_gpio_drivers.c
 *
 *  Created on: 06-Mar-2026
 *      Author: Subarno Pal
 */
#include "stm32f446xx.h"

/*********************************************************************
 * @fn      		  - GPIO_PeriClockControl
 *
 * @brief             - This function enables or disables peripheral clock for the given GPIO port
 *
 * @param[in]         - base address of the gpio peripheral
 * @param[in]         - ENABLE or DISABLE macros
 * @return            -  none
 *
 * @Note              -  none

 */
void GPIO_PeriClockControl(GPIO_RegDef_t *pGPIOx , uint8_t EnorDi){
	if(EnorDi == ENABLE){
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_EN();
		else if(pGPIOx == GPIOB)
			GPIOB_PCLK_EN();
		else if(pGPIOx == GPIOC)
			GPIOC_PCLK_EN();
		else if(pGPIOx == GPIOD)
			GPIOD_PCLK_EN();
		else if(pGPIOx == GPIOE)
			GPIOE_PCLK_EN();
		else if(pGPIOx == GPIOF)
			GPIOF_PCLK_EN();
		else if(pGPIOx == GPIOG)
			GPIOG_PCLK_EN();
		else if(pGPIOx == GPIOH)
			GPIOH_PCLK_EN();
	}

	else{
		if(pGPIOx == GPIOA)
			GPIOA_PCLK_DI();
		else if(pGPIOx == GPIOB)
			GPIOB_PCLK_DI();
		else if(pGPIOx == GPIOC)
			GPIOC_PCLK_DI();
		else if(pGPIOx == GPIOD)
			GPIOD_PCLK_DI();
		else if(pGPIOx == GPIOE)
			GPIOE_PCLK_DI();
		else if(pGPIOx == GPIOF)
			GPIOF_PCLK_DI();
		else if(pGPIOx == GPIOG)
			GPIOG_PCLK_DI();
		else if(pGPIOx == GPIOH)
			GPIOH_PCLK_DI();
	}
}

/*********************************************************************
 * @fn      		  - GPIO_Init
 *
 * @brief             - Initializes a GPIO pin according to the configuration
 *                      specified in the GPIO handle structure. This function
 *                      configures the mode, speed, pull-up/pull-down settings,
 *                      and output type of the selected GPIO pin.
 *
 * @param[in]         - pGPIOHandle : Pointer to the GPIO handle structure
 *                                    which contains the GPIO port base address
 *                                    and GPIO pin configuration settings.
 *
 * @return            - None
 *
 * @Note              - Before calling this function, the clock for the
 *                      corresponding GPIO peripheral should be enabled.
 *                      This function only configures one GPIO pin at a time
 *                      based on the parameters stored in the GPIO_Handle_t
 *                      structure.
 *
 */
void GPIO_Init(GPIO_Handle_t *pGPIOHandle){
	uint32_t temp =0; //temp register

	//First Enable the Peripheral clock:
	GPIO_PeriClockControl(pGPIOHandle->pGPIOx, ENABLE);
	//1. Configure the mode of gpio pin
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode <= GPIO_MODE_ANALOG){
		//Non-interrupt mode:
		temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER &= ~(0X3 << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		pGPIOHandle->pGPIOx->MODER |= temp;

	}

	else{
		pGPIOHandle->pGPIOx->MODER &= ~(0X3 << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
		/*****************Interrupt Mode*******************/
		if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_FT){
			//1. Configure the FTSR
			EXTI->FTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Enables falling edge detection line on the EXTI line correspong to pin number
			//Clear the corresponding RTSR bit:
			EXTI->RTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RT){
			//1. Configure the  RTSR
			EXTI->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber); // Enables rising edge detection line on the EXTI line correspong to pin number
			//Clear the corresponding FTSR bit:
			EXTI->FTSR &= ~(1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}
		else if (pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_IT_RFT){
			//1. COnfigure both FTSR and RTSR
			EXTI->RTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
			//Clear the corresponding FTSR bit:
			EXTI->FTSR |= (1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
		}

		//2. Configure the GPIO port selection in the SYSCFG_EXTICR
		uint8_t temp1 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber / 4;
		uint8_t temp2 = pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber % 4;
		uint8_t portcode = GPIO_BASEADDR_TO_CODE(pGPIOHandle->pGPIOx);
		SYSCFG_PCLK_EN();
		SYSCFG->EXTICR[temp1] &= ~(0xF << (temp2 * 4));
		SYSCFG->EXTICR[temp1] |= portcode << (temp2 * 4);
		//3. Enable the EXTI interrupt delivery using the IMR
		EXTI->IMR |= 1<<pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber;
	}

	temp = 0;

	//2. Configure  the speed
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinSpeed << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDER &= ~(0X3 << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OSPEEDER |= temp;
	temp = 0;

	//3. Configure the PuPd settings
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinPuPdControl << (2 * pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR &= ~(0X3 << (2*pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->PUPDR |= temp;
	temp = 0;

	//4. Configure the Optype
	temp = (pGPIOHandle->GPIO_PinConfig.GPIO_PinOPType << (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber));
	pGPIOHandle->pGPIOx->OTYPER &= ~(1 << pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber);
	pGPIOHandle->pGPIOx->OTYPER |= temp;
	temp = 0;

	//5. Configure the alt functionality
	if(pGPIOHandle->GPIO_PinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
		uint8_t temp1 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)/8;
		uint8_t temp2 = (pGPIOHandle->GPIO_PinConfig.GPIO_PinNumber)%8;
		pGPIOHandle->pGPIOx->AFR[temp1] &= ~(0XF << (4*temp2));
		pGPIOHandle->pGPIOx->AFR[temp1] |= (pGPIOHandle->GPIO_PinConfig.GPIO_PinAltFunMode << (4*temp2));
	}

}


/*********************************************************************
 * @fn      		  - GPIO_DeInit
 *
 * @brief             - De-initializes the given GPIO peripheral by
 *                      resetting it to its default reset state.
 *
 * @param[in]         - pGPIOx : Base address of the GPIO peripheral
 *                               (GPIOA, GPIOB, GPIOC, etc.)
 *
 * @return            - None
 *
 * @Note              - This function resets all the registers of the
 *                      selected GPIO port. After reset, all GPIO pins
 *                      return to their default configuration as
 *                      defined in the reference manual.
 *
 */

void GPIO_DeInit(GPIO_RegDef_t *pGPIOx){
	if(pGPIOx == GPIOA){
		GPIOA_REG_RESET();
	}
	else if(pGPIOx == GPIOB){
		GPIOB_REG_RESET();
	}
	else if(pGPIOx == GPIOC){
		GPIOC_REG_RESET();
	}
	else if(pGPIOx == GPIOD){
		GPIOD_REG_RESET();
	}
	else if(pGPIOx == GPIOE){
		GPIOE_REG_RESET();
	}
	else if(pGPIOx == GPIOF){
		GPIOF_REG_RESET();
	}
	else if(pGPIOx == GPIOG){
		GPIOG_REG_RESET();
	}
	else if(pGPIOx == GPIOH){
		GPIOH_REG_RESET();
	}
}

//Data Read and Write:

/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPin
 *
 * @brief             - Reads the value from a specified GPIO input pin.
 *
 * @param[in]         - pGPIOx : Base address of the GPIO peripheral
 *                               (GPIOA, GPIOB, GPIOC, etc.)
 * @param[in]         - PinNumber : GPIO pin number from which the
 *                                  input value is to be read.
 *
 * @return            - uint8_t : Returns the logical state of the pin.
 *                                1 → Pin is HIGH
 *                                0 → Pin is LOW
 *
 * @Note              - This function reads the status of the pin from
 *                      the Input Data Register (IDR) of the GPIO port.
 *
 */

uint8_t GPIO_ReadFromInputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber){
	uint8_t value = (uint8_t )(pGPIOx->IDR >> PinNumber) & 0x0000001; //First moved the bit at the position = PinNumber to the 0th bit and then masked it by 0x0001 to return the value
	return value;
}

/*********************************************************************
 * @fn      		  - GPIO_ReadFromInputPort
 *
 * @brief             - Reads the value of the entire GPIO input port.
 *
 * @param[in]         - pGPIOx : Base address of the GPIO peripheral
 *                               (GPIOA, GPIOB, GPIOC, etc.)
 *
 * @return            - uint16_t : Returns the 16-bit value of the
 *                                 Input Data Register (IDR) which
 *                                 represents the logic levels of
 *                                 all 16 GPIO pins of the port.
 *
 * @Note              - Each bit of the returned value corresponds
 *                      to the state of a GPIO pin:
 *                      Bit 0 → Pin 0
 *                      Bit 1 → Pin 1
 *                      ...
 *                      Bit 15 → Pin 15
 *
 */

uint16_t GPIO_ReadFromInputPort(GPIO_RegDef_t *pGPIOx){
	uint16_t value = (uint16_t )(pGPIOx->IDR); //First moved the bit at the position = PinNumber to the 0th bit and then masked it by 0x0001 to return the value
	return value;
}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPin
 *
 * @brief             - Writes a logic value to a specified GPIO
 *                      output pin.
 *
 * @param[in]         - pGPIOx : Base address of the GPIO peripheral
 *                               (GPIOA, GPIOB, GPIOC, etc.)
 * @param[in]         - PinNumber : GPIO pin number to which the value
 *                                  is to be written.
 * @param[in]         - Value : Logic value to be written to the pin
 *                              (SET or RESET).
 *
 * @return            - None
 *
 * @Note              - This function writes the specified value to the
 *                      corresponding bit in the Output Data Register (ODR)
 *                      of the GPIO port.
 *
 */

void GPIO_WriteToOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber , uint8_t value){
	if(value == GPIO_PIN_SET){
		pGPIOx->ODR |= (1<<PinNumber);
	}
	else{
		pGPIOx->ODR &= ~(1<<PinNumber);
	}
}

/*********************************************************************
 * @fn      		  - GPIO_WriteToOutputPort
 *
 * @brief             - Writes a value to the entire GPIO output port.
 *
 * @param[in]         - pGPIOx : Base address of the GPIO peripheral
 *                               (GPIOA, GPIOB, GPIOC, etc.)
 * @param[in]         - Value : 16-bit value to be written to the
 *                              Output Data Register (ODR) of the port.
 *
 * @return            - None
 *
 * @Note              - Each bit of the value corresponds to a GPIO pin:
 *                      Bit 0 → Pin 0
 *                      Bit 1 → Pin 1
 *                      ...
 *                      Bit 15 → Pin 15
 *
 */

void GPIO_WriteToOutputPort(GPIO_RegDef_t *pGPIOx , uint16_t value){
	pGPIOx->ODR = value;
}

/*********************************************************************
 * @fn      		  - GPIO_ToggleOutputPin
 *
 * @brief             - Toggles the current state of a specified GPIO
 *                      output pin.
 *
 * @param[in]         - pGPIOx : Base address of the GPIO peripheral
 *                               (GPIOA, GPIOB, GPIOC, etc.)
 * @param[in]         - PinNumber : GPIO pin number whose output state
 *                                  is to be toggled.
 *
 * @return            - None
 *
 * @Note              - If the current state of the pin is HIGH, it will
 *                      be changed to LOW and vice versa. The toggling
 *                      operation is performed by modifying the
 *                      corresponding bit in the Output Data Register
 *                      (ODR) of the GPIO port.
 *
 */

void GPIO_ToggleOutputPin(GPIO_RegDef_t *pGPIOx , uint8_t PinNumber){
	pGPIOx->ODR ^= (1<<PinNumber);
}

//IRQ Configuration and ISR handling:
void GPIO_IRQInterruptConfig(uint8_t IRQNumber , uint8_t EnorDi){
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

void GPIO_IRQPriorityConfig(uint8_t IRQNumber , uint32_t IRQPriority){
	//1.first lets find out the ipr register
	//Each iprx is 32 bits and is divided into 4 sections each having an IRQ number and each of 8 bit
	uint8_t iprx = IRQNumber/4;
	uint8_t iprx_section = IRQNumber%4;
	uint8_t shift_amount = (8 * iprx_section) + (8- NO_PR_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR + iprx) &= ~(0xF <<shift_amount);
	*(NVIC_PR_BASE_ADDR + iprx) |= (IRQPriority << shift_amount);

}

void GPIO_IRQHandling( uint8_t PinNumber){
	//Clear the EXTI PR register corresponding to the pin number:
	if(EXTI->PR & (1 << PinNumber)){
		//Clear
		EXTI->PR |= (1<<PinNumber);
	}
}
