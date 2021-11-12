/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "lib_uart.h"
#include "sx1278.h"

int main(void)
{
	t_sx1278 module;

	UART_lib_config(e_UART_2, DISABLE, 0, 0);
	sx1278_init(&module);

	while(1)
	{

	}
}
