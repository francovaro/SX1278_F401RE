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

/* for module */
#include "sx1278.h"
#include "sx1278_parameter_define.h"

static void setup_sx1278(t_sx1278* module);

/**
 *
 * @return
 */
int main(void)
{
	t_sx1278 module;
	uint8_t version;

	UART_lib_config(e_UART_2, DISABLE, 0, 0);
	setup_sx1278(&module);

	version = sx1278_get_version();

	while(1)
	{

	}
}

/**
 *
 */
static void setup_sx1278(t_sx1278* module)
{

	module->freq._freq_8[0] = gfrequency_434[0];
	module->freq._freq_8[1] = gfrequency_434[1];
	module->freq._freq_8[2] = gfrequency_434[2];

	module->power = gpower[0];				/* 20 dBm */
	module->bandwith = ((0b0101) << 4);		/* 41.7kHz */

	module->spread_factor = ((7u) << 4);		/* */


	sx1278_init(module);
}
