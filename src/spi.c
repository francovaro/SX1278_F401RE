/**
 *  @file  : spi.c
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 10 nov 2021
 */

#include "spi.h"
#include "stm32f4xx_spi.h"

#define SPI_IS_BUSY(spi) 			(((spi)->SR & (SPI_SR_TXE | SPI_SR_RXNE)) == 0 || ((spi)->SR & SPI_SR_BSY))
#define SPI_WAIT_TILL_BUSY(spi) 	while(SPI_IS_BUSY(spi))

/**
 *
 */
void spi_config(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Peripheral Clock Enable -------------------------------------------------*/
  /* Enable GPIO clocks */
  RCC_AHB1PeriphClockCmd(SPIx_SCK_GPIO_CLK | SPIx_MOSI_GPIO_CLK | SPIx_CS_GPIO_CLK, ENABLE);

  /* Enable the SPI clock */
  SPIx_CLK_INIT(SPIx_CLK, ENABLE);


  /* SPI GPIO Configuration --------------------------------------------------*/
  /* GPIO Deinitialisation */
  GPIO_DeInit(SPIx_SCK_GPIO_PORT);
  GPIO_DeInit(SPIx_MOSI_GPIO_PORT);
  GPIO_DeInit(SPIx_CS_GPIO_PORT);

  /* Connect SPI pins to AF5 */
  GPIO_PinAFConfig(SPIx_SCK_GPIO_PORT, SPIx_SCK_SOURCE, SPIx_SCK_AF);
  GPIO_PinAFConfig(SPIx_MOSI_GPIO_PORT, SPIx_MOSI_SOURCE, SPIx_MOSI_AF);
  GPIO_PinAFConfig(SPIx_MISO_GPIO_PORT, SPIx_MISO_SOURCE, SPIx_MISO_AF);

  GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; //GPIO_PuPd_NOPULL;

  /* SPI SCK pin configuration */
  GPIO_InitStructure.GPIO_Pin = SPIx_SCK_PIN;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  /* SPI  MOSI pin configuration */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MOSI_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* MISO pin */
  GPIO_InitStructure.GPIO_Pin =  SPIx_MISO_PIN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL; //GPIO_PuPd_DOWN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

  /* SPI Chip Select pin configuration */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		/* checked */
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Pin =  SPIx_CS_PIN;
  GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStructure);

  CS_HIGH;		/* deselect SPI closed */

  /* SPI configuration -------------------------------------------------------*/
  SPI_I2S_DeInit(SPIx);
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		/* tx and rx */
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;							/* 1 byte */
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								/* */
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;								/* */
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft 									/* */;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;

  //SPI_NSSInternalSoftwareConfig(SPIx, SPI_NSSInternalSoft_Set);
  SPI_Init(SPIx, &SPI_InitStructure);

  /* Enable the Tx empty interrupt */
  //SPI_I2S_ITConfig(SPIx, SPI_I2S_IT_TXE, ENABLE);

  /*
   * TODO
   * use DMA
   */
  //SPI_I2S_DMACmd(SPIx, SPI_DMAReq_Rx, ENABLE);
  //SPI_I2S_DMACmd(SPIx, SPI_DMAReq_Tx, ENABLE);

  /* Enable the SPI peripheral */
  SPI_Cmd(SPIx, ENABLE);

  /* Configure the Priority Group to 1 bit */
#ifdef _SPI_IRQ
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Configure the SPI interrupt priority */
  NVIC_InitStructure.NVIC_IRQChannel = SPIx_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;	/* don't need it*/
  NVIC_Init(&NVIC_InitStructure);
#endif


}

/**
 *
 * @param rx_buffer
 * @param n_byte
 */
void spi_read(uint8_t address, uint8_t* rx_buffer, uint16_t n_byte)
{
	uint8_t	i;
	uint8_t *p_rx_buffer = rx_buffer;
	uint8_t dummy_read;

	(void)dummy_read;

	CS_LOW;

	SPI_WAIT_TILL_BUSY(SPIx);
	SPIx->DR = address;			/* address */

	SPI_WAIT_TILL_BUSY(SPIx);
	dummy_read = SPIx->DR;

	for (i = 0; i < (n_byte); i++)
	{
		SPI_WAIT_TILL_BUSY(SPIx);
		SPIx->DR = 0xFF;			/* dummy send */

		SPI_WAIT_TILL_BUSY(SPIx);
		*p_rx_buffer = (uint8_t)SPIx->DR;	/* receives data */
		p_rx_buffer++;
	}

	CS_HIGH;
}

/**
 *
 * @param tx_buffer
 * @param n_byte
 */
void spi_multiple_write(uint8_t address, const uint8_t* tx_buffer, uint16_t n_byte)
{
	uint8_t	i;
	uint8_t dummy_rx;
	const uint8_t *p_tx_buffer = tx_buffer;

	(void)dummy_rx;	/* no warning */

	CS_LOW;

	SPI_WAIT_TILL_BUSY(SPIx);
	SPIx->DR = (uint8_t)((1 << 7) | address);	/* W_BIT address */

	SPI_WAIT_TILL_BUSY(SPIx);
	dummy_rx = SPIx->DR;		/* dummy receive */

	for (i = 0; i < n_byte; i++)
	{
		SPI_WAIT_TILL_BUSY(SPIx);
		/* whay about SPI_I2S_FLAG_BSY ? */
		SPIx->DR = *p_tx_buffer;	/* send data*/

		SPI_WAIT_TILL_BUSY(SPIx);
		dummy_rx = SPIx->DR;		/* dummy receive */
		p_tx_buffer++;
	}

	CS_HIGH;
}

void spi_single_write(uint8_t address, uint8_t data)
{
	uint8_t dummy_rx;

	CS_LOW;

	(void)dummy_rx;	/* no warning */

	SPI_WAIT_TILL_BUSY(SPIx);
	SPIx->DR = (uint8_t)((1 << 7) | address);	/* W_BIT address */

	SPI_WAIT_TILL_BUSY(SPIx);
	dummy_rx = SPIx->DR;		/* dummy receive */

	SPI_WAIT_TILL_BUSY(SPIx);
	SPIx->DR = (uint8_t)data;

	SPI_WAIT_TILL_BUSY(SPIx);
	dummy_rx = SPIx->DR;		/* dummy receive */

	CS_HIGH;
}
