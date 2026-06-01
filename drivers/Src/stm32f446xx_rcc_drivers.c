/*
 * stm32f446xx_rcc_drivers.c
 *
 *  Created on: 28-May-2026
 *      Author: Subarno Pal
 */

#include "stm32f446xx.h"

uint32_t AHB_PreScalar[8]  = {2,4,8,16,64,128,256,512};
uint32_t APB1_PreScalar[4] = {2,4,8,16};
uint32_t APB2_PreScalar[4] = {2,4,8,16};


//API for getting peripheral clock speed for APB1
uint32_t RCC_GetPCLK1Value(void)
{
	uint32_t pclk1, SystemClk;
	uint32_t clksrc, temp, ahbp, apb1p;

	clksrc = ((RCC->CFGR >> 2) & 0x3);

	if(clksrc == 0)
	{
		//HSI selected as system clock
		SystemClk = 16000000;
	}
	else if(clksrc == 1)
	{
		//HSE selected as system clock
		SystemClk = 8000000;
	}
	else if(clksrc == 2)
	{
		//PLL selected as system clock
		//Modify this according to your PLL configuration
		SystemClk = 16000000;
	}
	else
	{
		SystemClk = 16000000;
	}

	//AHB prescaler
	temp = (RCC->CFGR >> 4) & 0xF;

	if(temp < 8)
	{
		ahbp = 1;
	}
	else
	{
		ahbp = AHB_PreScalar[temp - 8];
	}

	//APB1 prescaler
	temp = (RCC->CFGR >> 10) & 0x7;

	if(temp < 4)
	{
		apb1p = 1;
	}
	else
	{
		apb1p = APB1_PreScalar[temp - 4];
	}

	pclk1 = (SystemClk / ahbp) / apb1p;

	return pclk1;
}


//API for getting peripheral clock speed for APB2
uint32_t RCC_GetPCLK2Value(void)
{
	uint32_t pclk2, SystemClk;
	uint32_t clksrc, temp, ahbp, apb2p;

	clksrc = ((RCC->CFGR >> 2) & 0x3);

	if(clksrc == 0)
	{
		//HSI selected as system clock
		SystemClk = 16000000;
	}
	else if(clksrc == 1)
	{
		//HSE selected as system clock
		SystemClk = 8000000;
	}
	else if(clksrc == 2)
	{
		//PLL selected as system clock
		//Modify this according to your PLL configuration
		SystemClk = 16000000;
	}
	else
	{
		SystemClk = 16000000;
	}

	//AHB prescaler
	temp = (RCC->CFGR >> 4) & 0xF;

	if(temp < 8)
	{
		ahbp = 1;
	}
	else
	{
		ahbp = AHB_PreScalar[temp - 8];
	}

	//APB2 prescaler
	temp = (RCC->CFGR >> 13) & 0x7;

	if(temp < 4)
	{
		apb2p = 1;
	}
	else
	{
		apb2p = APB2_PreScalar[temp - 4];
	}

	pclk2 = (SystemClk / ahbp) / apb2p;

	return pclk2;
}
