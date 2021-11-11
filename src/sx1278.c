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

void sx1278_init(void)
{
	spi_config();
}
