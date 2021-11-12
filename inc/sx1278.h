/**
 *  @file  : sx1278.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 10 nov 2021
 */

#ifndef INC_SX1278_H_
#define INC_SX1278_H_

#include <stdint.h>

typedef enum
{
	state_sleep,
	state_max
}t_state;

/**
 *
 */
typedef enum
{
	mode_sleep 						= 0,            //!< mode_sleep
	mode_stdby 						= 1,            //!< mode_stdby
	mode_frequency_synteshis_tx		= 2,			//!< mode_frequency_synteshis_tx
	mode_tx							= 3,            //!< mode_tx
	mode_frequency_synteshis_rx		= 4,			//!< mode_frequency_synteshis_rx
	mode_receive_continuous			= 5,   			//!< mode_receive_continuous
	mode_receive_single				= 6,      		//!< mode_receive_single
	mode_channel_detection			= 7,    		//!< mode_channel_detection
	mode_max                         				//!< mode_max
}t_mode;

typedef struct
{
	void(*START_SPI)(void);		/* cs lo*/
	void(*STOP_SPI)(void);		/* cs high */

	uint8_t _freq_carrier;
	t_state _state;
}t_sx1278;

extern void sx1278_init(t_sx1278* module);
extern void sx1278_set_mode(t_sx1278* module, t_mode new_mode);
extern void sx1278_write(t_sx1278* module);
extern void sx1278_read(t_sx1278* module);

#endif /* INC_SX1278_H_ */
