/*
 * stm32f446xx_tim2_drivers.h
 *
 *  Created on: 06-Jun-2026
 *      Author: Subarno Pal
 */

#ifndef INC_STM32F446XX_TIM2_DRIVERS_H_
#define INC_STM32F446XX_TIM2_DRIVERS_H_

#include "stm32f446xx.h"

typedef struct
{
    uint32_t TIM_Prescaler;
    uint32_t TIM_AutoReload;

} TIM_Config_t;

typedef struct
{
    TIM_RegDef_t *pTIMx;
    TIM_Config_t TIM_Config;

}TIM_Handle_t;


/*
 * Application Event Macros
 */
#define TIM_EVENT_UPDATE     1
#define TIM_EVENT_CC1        2
#define TIM_EVENT_CC2        3
#define TIM_EVENT_CC3        4
#define TIM_EVENT_CC4        5

/****************Driver API Prototypes********************/
void TIM_PeriClockControl(TIM_RegDef_t *pTIMx, uint8_t EnorDi);

void TIM_Init(TIM_Handle_t *pTIMHandle);

void TIM_DeInit(TIM_RegDef_t *pTIMx);

void TIM_PeripheralControl(TIM_RegDef_t *pTIMx, uint8_t EnorDi);

void TIM_IRQInterruptConfig(uint8_t IRQNumber, uint8_t EnorDi);

void TIM_IRQPriorityConfig(uint8_t IRQNumber, uint32_t IRQPriority);

void TIM_UpdateInterruptControl(TIM_RegDef_t *pTIMx, uint8_t EnorDi);

void TIM_IRQHandling(TIM_Handle_t *pTIMHandle);

void TIM_ApplicationEventCallBack(TIM_Handle_t *pTIMHandle , uint8_t AppEv);
#endif /* INC_STM32F446XX_TIM2_DRIVERS_H_ */
