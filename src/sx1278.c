/**
 *  @file  : sx1278.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 10 nov 2021
 */
#include "spi.h"
#include "sx1278.h"
#include "sx1278_register.h"
#include "sx1278_parameter_define.h"

#define CRC_ENABLE	(1u)

/**
 *
 */
void sx1278_init(t_sx1278* module)
{
	uint8_t tx_data;
	spi_config();

	spi_single_write(SX1278_REGISTER_OPMODE, 0x88);	/* set LoRa mode */
	spi_multiple_write(SX1278_REGISTER_FRF_MSB, &module->freq._freq_8[0], 3);	/* set working freq */

	spi_single_write(SX1278_REGISTER_PA_CONFIG, 0x88);	/* set LoRa mode */

	//spi_single_write(SX1278_REGISTER_OCP,  0x0B);	/* ocp - default 0x0B*/
	spi_single_write(SX1278_REGISTER_LNA, 0x23);	/* lna - enabled, max gain*/


	if (module->spread_factor == 6)
	{
		spi_single_write(SX1278_REGISTER_MODEMCONFIG1, module->bandwith | (1 << 1) | (1));		/* bandwith coding rate 4/5 */

		spi_read(SX1278_REGISTER_DETECTOPTIMIZE, &tx_data,1);
		tx_data &= 0xF8;
		tx_data |= 0x05;
		spi_single_write(SX1278_REGISTER_DETECTOPTIMIZE, tx_data);
		spi_single_write(SX1278_REGISTER_DETECTIONTHRESHOLD, 0x0C);
	}
	else
	{
		spi_single_write(SX1278_REGISTER_MODEMCONFIG1, module->bandwith | (1 << 1));		/* bandwith coding rate 4/5 */
	}

	spi_single_write(SX1278_REGISTER_MODEMCONFIG2, module->spread_factor | (CRC_ENABLE << 2) | 0x03 );	/* spread factor  - crc enable */

	spi_single_write(SX1278_REGISTER_SYMBTIMEOUTLSB, 0xFF); 	/* Timeout = 0x3FF(Max) */
	spi_single_write(SX1278_REGISTER_PREAMBLEMSB, 0x00); 		//RegPreambleMsb
	spi_single_write(SX1278_REGISTER_PREAMBLELSB, 12); 			//RegPreambleLsb 8+4=12byte Preamble

	sx1278_set_mode(module, mode_stdby);
	// spi_single_write(SX1278_REGISTER_DIO_MAPPING1, 0x01); //RegDioMapping2 DIO5=00, DIO4=01
}

/**
 *
 * @param new_mode
 */
void sx1278_set_mode(t_sx1278* module, t_mode new_mode)
{
	module->mode = new_mode;
	if (new_mode > mode_stdby)
	{
		spi_single_write(SX1278_REGISTER_OPMODE, OP_MODE_LORA_ON | new_mode);	/* set LoRa mode */
	}
	else
	{
		spi_single_write(SX1278_REGISTER_OPMODE, OP_MODE_LORA_OFF | new_mode);	/* set LoRa mode */
	}

}

void sx1278_write(t_sx1278* module)
{

}

void sx1278_read(t_sx1278* module)
{

}