/*
 *  @file  : delay.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 11 nov 2020
 */

#ifndef INC_DELAY_H_
#define INC_DELAY_H_

#include <stdint.h>
#include <stdbool.h>

extern void delay_init(void);
extern void delay_ms(uint16_t delay);
extern void delay_load_ms(uint16_t delay);
extern bool delay_has_expired(void);


#endif /* INC_DELAY_H_ */
