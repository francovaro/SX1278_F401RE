/**
 *  @file  : spx1278_parameter_define.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 12 nov 2021
 */

#ifndef INC_SPX1278_PARAMETER_DEFINE_H_
#define INC_SPX1278_PARAMETER_DEFINE_H_

#include <stdint.h>

/* const define for settings */
#define	OP_MODE_LORA_ON	 ((1 << 7) | (1 << 3))
#define	OP_MODE_LORA_OFF (1 << 3)

/* freq setting, for now just 434MHz */
static const uint8_t gfrequency_434[] = {0x6C, 0x80, 0x00}; /* 434MHz */

/* power setting */
static const uint8_t gpower[] =
{
		0xFF, //20dbm
		0xFC, //17dbm
		0xF9, //14dbm
		0xF6, //11dbm
};


#endif /* INC_SPX1278_PARAMETER_DEFINE_H_ */
