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

extern void sx1278_pin_init(void);

#define SX1278_DIO0_PIN			GPIO_Pin_10
#define SX1278_DIO1_PIN			GPIO_Pin_11

#define SX1278_DIO0_PIN_SOURCE	EXTI_PinSource10
#define SX1278_DIO1_PIN_SOURCE	EXTI_PinSource11

#define SX1278_DIO0_LINE		EXTI_Line10
#define SX1278_DIO1_LINE		EXTI_Line10


#endif /* INC_SX1278_PIN_H_ */
