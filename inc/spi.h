/**
 *  @file  : spi.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 10 nov 2021
 */

#ifndef INC_SPI_H_
#define INC_SPI_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

#define SPIx                           SPI2

#define SPIx_CLK                       RCC_APB1Periph_SPI2
#define SPIx_CLK_INIT                  RCC_APB1PeriphClockCmd
#define SPIx_IRQn                      SPI2_IRQn
#define SPIx_IRQHANDLER                SPI2_IRQHandler

/* CLK - GPIOB 10*/
#define SPIx_SCK_PIN                   GPIO_Pin_10
#define SPIx_SCK_GPIO_PORT             GPIOB
#define SPIx_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SPIx_SCK_SOURCE                GPIO_PinSource10
#define SPIx_SCK_AF                    GPIO_AF_SPI2

/* MOSI/SDA -  GPIOC 03 */
#define SPIx_MOSI_PIN                  GPIO_Pin_3
#define SPIx_MOSI_GPIO_PORT            GPIOC
#define SPIx_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOC
#define SPIx_MOSI_SOURCE               GPIO_PinSource3
#define SPIx_MOSI_AF                   GPIO_AF_SPI2

/* CHIP SELECT - GPIOC 02*/
#define SPIx_CS_PIN                    GPIO_Pin_2
#define SPIx_CS_GPIO_PORT              GPIOC
#define SPIx_CS_GPIO_CLK               RCC_AHB1Periph_GPIOC

// CS pin macros
#define CS_L() GPIO_ResetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)
#define CS_H() GPIO_SetBits(SPIx_CS_GPIO_PORT, SPIx_CS_PIN)

#define CS_LOW		GPIO_WriteBit(GPIOC, GPIO_Pin_2, 0)
#define CS_HIGH		GPIO_WriteBit(GPIOC, GPIO_Pin_2, 1)

extern void spi_config(void);
extern void spi_read(uint8_t address, uint8_t* rx_buffer, uint16_t n_byte);
extern void spi_multiple_write(uint8_t address, const uint8_t* tx_buffer, uint16_t n_byte);
extern void spi_single_write(uint8_t address, uint8_t data);

#endif /* INC_SPI_H_ */
