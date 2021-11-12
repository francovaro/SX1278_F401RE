/*
 *  @file  : sx1278_pin.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 12 nov 2021
 */
#include "sx1278_pin.h"
#include <stdint.h>
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"

/**
 *
 */
void sx1278_pin_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* init clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Enable SYSCFG's APB interface clock */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_EXTIT, ENABLE);	/* useful ? */

	GPIO_StructInit(&GPIO_InitStructure);
	EXTI_StructInit(&EXTI_InitStructure);

	/* GPIO Init */
	/* init GPIO as input */
	GPIO_InitStructure.GPIO_Pin = (SX1278_DIO0_PIN | SX1278_DIO1_PIN);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_Init(GPIOA , &GPIO_InitStructure);

	/* connect pin to ext */
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, SX1278_DIO0_PIN_SOURCE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, SX1278_DIO1_PIN_SOURCE);

	/* EXTINT Init */
	/* set DIO0  10 */
	EXTI_InitStructure.EXTI_Line = SX1278_DIO0_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;		/* yeah rising or falling ? */
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* set DIO1 */
	EXTI_InitStructure.EXTI_Line = SX1278_DIO1_LINE;
	EXTI_Init(&EXTI_InitStructure);

	/* INTERRUPT Init */
	/* Enable the USARTx Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;	/* to check ! */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;			/* to check ! */
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
 *
 */
void EXTI15_10_IRQHandler(void)
{
	uint8_t i;

	if (EXTI_GetITStatus(SX1278_DIO0_LINE) == SET)
	{
		EXTI_ClearITPendingBit(SX1278_DIO0_LINE);
	}

	if (EXTI_GetITStatus(SX1278_DIO1_LINE) == SET)
	{
		EXTI_ClearITPendingBit(SX1278_DIO1_LINE);
	}
}
