/*
 *  @file  : sx1278_pin.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 12 nov 2021
 */

#ifndef INC_SX1278_PIN_H_
#define INC_SX1278_PIN_H_

#include <stdint.h>

extern volatile uint8_t dio0_status;

extern void sx1278_pin_init(void);
extern void sx1278_hw_reset(void);

#endif /* INC_SX1278_PIN_H_ */
