/**
 *  @file  : delay.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 12 nov 2020
 */
#include "stm32f4xx.h"
#include "delay.h"

volatile uint32_t _delay_ms;

/**
 * @brief Initializes the sys tick freq
 */
void delay_init(void)
{
	_delay_ms = 0;
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
}

/**
 * @brief
 * @param delay
 */
void delay_ms(uint16_t new_delay)
{
	_delay_ms = new_delay;
	do
	{
		_delay_ms--;
	}while(_delay_ms);
}

/**
 *
 * @param new_delay
 */
void delay_load_ms(uint16_t new_delay)
{
	_delay_ms = new_delay;
}

/**
 *
 * @return
 */
bool delay_has_expired(void)
{
	return (_delay_ms == 0);
}


/**
 * @brief The systick handler. In this case it lower the delay variable if was set
 * @note period is 1us
 */
void SysTick_Handler(void)
{
	if(_delay_ms != 0)
	{
		_delay_ms--;
	}
}
