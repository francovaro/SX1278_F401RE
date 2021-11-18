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
#include "sx1278_pin.h"
#include "delay.h"

#include <string.h>

#define CRC_ENABLE	(1u)

/**
 *
 */
void sx1278_init(t_sx1278* module)
{
	uint8_t tx_data;
	uint8_t rx_buffer[10];

	spi_config();
	sx1278_pin_init();

	module->cs_pin = GPIO_Pin_1;	/* should be a parameter */
	module->cs_port = GPIOB;		/* should be a parameter */


	delay_init();

	sx1278_hw_reset();	/* reset */

	sx1278_set_mode(module, mode_sleep);
	delay_ms(15u);

	spi_single_write(SX1278_REGISTER_OPMODE, 0x88);	/* set LoRa mode */

	spi_multiple_write(SX1278_REGISTER_FRF_MSB, &module->freq._freq_8[0], 3);	/* set working freq */
	spi_read(SX1278_REGISTER_FRF_MSB, &rx_buffer[0], 3u);
	(void)rx_buffer;

	spi_single_write(SX1278_REGISTER_PA_CONFIG, 0xF9);	/* set LoRa mode */

	spi_single_write(SX1278_REGISTER_OCP,  module->power);	/* ocp - default 0x0B*/
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

/**
 *
 * @return The version (should be 0x12)
 */
uint8_t sx1278_get_version(t_sx1278* module)
{
	uint8_t ver;

	spi_read(SX1278_REGISTER_VER, &ver, 1u);

	return ver;
}

/**
 *
 * @param mask
 */
void sx1278_set_irq_mask(t_sx1278* module, uint8_t mask)
{
	spi_single_write(SX1278_REGISTER_IRQFLAGSMASK, mask);
}

/**
 *
 */
void sx1278_clears_irq(t_sx1278* module)
{
	/*
	 * irq are cleared by writing a 1
	 */
	spi_single_write(SX1278_REGISTER_IRQFLAGS, 0xFF);
}

/**
 * @brief prepare the module to go into rx mode
 * @param module
 */
void sx1278_start_rx_mode(t_sx1278* module, uint8_t payload, uint32_t rx_timeout)
{
	/*
	 * set stdby
	 * set irq, if needed
	 * set hop period
	 * diox mapping
	 * set mode
	 * - if continuous, set addr ptr read from rx fifo address
	 */
	uint8_t address;

	if (module->mode != mode_stdby)
	{
		sx1278_set_mode(module, mode_stdby);	/* set standby before doing stuff  */
	}

	//spi_single_write(SX1278_REGISTER_IRQFLAGS, 0x84);
	module->freq_hop = DISABLE;
	spi_single_write(SX1278_REGISTER_HOPPERIOD, 0xFF);	/* no freq hop ! const for now... */

	sx1278_set_irq_mask(module, 0xFF);
	sx1278_clears_irq(module);

	spi_single_write(SX1278_REGISTER_PAYLOADLENGTH, payload);
	//spi_single_write(SX1278_REGISTER_PAYLOADMAXLENGTH, payload);	/* equal  ?*/

	spi_read(SX1278_REGISTER_FIFORXBASEADDR, &address, 1);		/* get rx fifo address */
	spi_single_write(SX1278_REGISTER_FIFOADDRPTR, address);		/* and set to fifo add */

	spi_single_write(SX1278_REGISTER_DIO_MAPPING1, 0x00); /* RxDone */

	sx1278_set_mode(module, mode_receive_continuous);	/* rx cont mode ! */
	module->total_rx = 0;

	memset(module->rx_buffer, 0, sizeof(module->rx_buffer));

	module->packet_timeout = rx_timeout;

}

/**
 * @brief Put the module in continuos RX mode
 * @param module
 */
void sx1278_receive_packet(t_sx1278* module)
{
	uint8_t		cur_addr;
	uint8_t		byte_rec;

	if (dio0_status == 1)
	{

		spi_read(SX1278_REGISTER_FIFORXCURRENTADDR, &cur_addr, 1);		/* get curr addr */
		spi_single_write(SX1278_REGISTER_FIFOADDRPTR, cur_addr);		/* and set to fifo add */

		spi_read(SX1278_REGISTER_RXNBBYTES, &byte_rec, 1); 				/* get rx size - Number of payload bytes of latest packet received*/

		module->total_rx += byte_rec;
		spi_read(SX1278_REGISTER_FIFO, &module->rx_buffer[0], byte_rec);	/* get RX fifo */

		if (module->freq_hop == ENABLE)
		{
			/*
			 * todo
			 */
		}

		dio0_status = 0;
		sx1278_clears_irq(module);
	}
}

/**
 *
 * @param module
 */
void sx1278_start_tx_mode(t_sx1278* module, uint8_t payload)
{
	uint8_t	address;

	/* we hope module has already been configured... ? */
	if (module->mode != mode_stdby)
	{
		sx1278_set_mode(module, mode_stdby);	/* set standby before doing stuff  */
	}

	module->freq_hop = DISABLE;
	spi_single_write(SX1278_REGISTER_HOPPERIOD, 0x00);	/* no freq hop ! const for now... */

	sx1278_set_irq_mask(module, 0x3F);
	sx1278_clears_irq(module);

	spi_single_write(SX1278_REGISTER_PAYLOADLENGTH, payload);
	spi_read(SX1278_REGISTER_FIFOTXBASEADDR, &address, 1);		/* get rx fifo address */
	spi_single_write(SX1278_REGISTER_FIFOADDRPTR, address);		/* and set to fifo add */

	spi_single_write(SX1278_REGISTER_DIO_MAPPING1, 0x01); /* TxDone */
}
/**
 *
 * @param module
 */
void sx1278_send_packet(t_sx1278* module, uint8_t* tx_buffer, uint32_t byte_to_send, uint32_t timeout_ms)
{
	bool end_loop = false;

	sx1278_set_mode(module, mode_tx);	/* set standby before doing stuff  */
	spi_multiple_write(SX1278_REGISTER_FIFO, tx_buffer, byte_to_send);

	delay_load_ms(timeout_ms);

	do
	{
		if (dio0_status == 1)
		{
			dio0_status = 0;
			end_loop = true;
			sx1278_clears_irq(module);
		}


		if (delay_has_expired())
		{
			end_loop = true;
			/*
			 * signal an error ?
			 */
		}
	}while(end_loop == false);
}
