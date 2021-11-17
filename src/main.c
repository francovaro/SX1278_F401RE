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
#include "delay.h"

/* for module */
#include "sx1278.h"
#include "sx1278_parameter_define.h"

#include <string.h>

#define SX1278_VERSION		0x12

static void setup_sx1278(t_sx1278* module);
static const char initial_string[] = "sx1278_demo\n";

static const char found_sx1278[] = 		"sx1278 with version 0x12 found\n";
static const char notfound_sx1278[] = 	"sx1278 not found :( \n";

/**
 *
 * @return
 */
int main(void)
{
	t_sx1278 module;
	RCC_ClocksTypeDef rcc;

	RCC_GetClocksFreq(&rcc);

	UART_lib_config(e_UART_2, DISABLE, 0, 0);
	UART_lib_sendData(e_UART_2, (char*)initial_string, strlen(initial_string));

	setup_sx1278(&module);



	while(1)
	{

		__WFE();
		delay_ms(10);
	}
}

/**
 *
 */
static void setup_sx1278(t_sx1278* module)
{
	uint8_t version = 0;

	module->freq._freq_8[0] = gfrequency_434[0];
	module->freq._freq_8[1] = gfrequency_434[1];
	module->freq._freq_8[2] = gfrequency_434[2];

	module->power = gpower[0];				/* 20 dBm */
	module->bandwith = ((0b0101) << 4);		/* 41.7kHz */

	module->spread_factor = ((7u) << 4);		/* */

	sx1278_init(module);

	version = sx1278_get_version(module);
	if (version == SX1278_VERSION)
	{
		UART_lib_sendData(e_UART_2, (char*)found_sx1278, strlen(found_sx1278));
	}
	else
	{
		UART_lib_sendData(e_UART_2, (char*)notfound_sx1278, strlen(notfound_sx1278));
	}
}
