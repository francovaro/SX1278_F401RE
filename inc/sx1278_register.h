/**
 *  @file  : sx1278_register.h
 *	
 *  @brief	
 *
 *  @author: franc
 *  @date  : 11 nov 2021
 */

#ifndef INC_SX1278_REGISTER_H_
#define INC_SX1278_REGISTER_H_

/* register values */
#define SX1278_REGISTER_FIFO								0x00
#define SX1278_REGISTER_OPMODE								0x01
#define SX1278_REGISTER_FRF_MSB								0x06
#define SX1278_REGISTER_FRF_MID								0x07
#define SX1278_REGISTER_FRF_LSB								0x08
#define SX1278_REGISTER_PA_CONFIG							0x09
#define SX1278_REGISTER_PA_RAMP								0x0A
#define SX1278_REGISTER_OCP									0x0B
#define SX1278_REGISTER_LNA                                 0x0C
#define SX1278_REGISTER_FIFOADDRPTR                         0x0D
#define SX1278_REGISTER_FIFOTXBASEADDR                      0x0E
#define SX1278_REGISTER_FIFORXBASEADDR                      0x0F
#define SX1278_REGISTER_FIFORXCURRENTADDR                   0x10
#define SX1278_REGISTER_IRQFLAGSMASK                        0x11
#define SX1278_REGISTER_IRQFLAGS                            0x12
#define SX1278_REGISTER_RXNBBYTES                           0x13
#define SX1278_REGISTER_RXHEADERCNTVALUEMSB                 0x14
#define SX1278_REGISTER_RXHEADERCNTVALUELSB                 0x15
#define SX1278_REGISTER_RXPACKETCNTVALUEMSB                 0x16
#define SX1278_REGISTER_RXPACKETCNTVALUELSB                 0x17
#define SX1278_REGISTER_MODEMSTAT                           0x18
#define SX1278_REGISTER_PKTSNRVALUE                         0x19
#define SX1278_REGISTER_PKTRSSIVALUE                        0x1A
#define SX1278_REGISTER_RSSIVALUE                           0x1B
#define SX1278_REGISTER_HOPCHANNEL                          0x1C
#define SX1278_REGISTER_MODEMCONFIG1                        0x1D
#define SX1278_REGISTER_MODEMCONFIG2                        0x1E
#define SX1278_REGISTER_SYMBTIMEOUTLSB                      0x1F
#define SX1278_REGISTER_PREAMBLEMSB                         0x20
#define SX1278_REGISTER_PREAMBLELSB                         0x21
#define SX1278_REGISTER_PAYLOADLENGTH                       0x22
#define SX1278_REGISTER_PAYLOADMAXLENGTH                    0x23
#define SX1278_REGISTER_HOPPERIOD                           0x24
#define SX1278_REGISTER_FIFORXBYTEADDR                      0x25
#define SX1278_REGISTER_MODEMCONFIG3                        0x26
#define SX1278_REGISTER_FEIMSB                              0x28
#define SX1278_REGISTER_FEIMID                              0x29
#define SX1278_REGISTER_FEILSB                              0x2A
#define SX1278_REGISTER_RSSIWIDEBAND                        0x2C
#define SX1278_REGISTER_TEST2F                              0x2F
#define SX1278_REGISTER_TEST30                              0x30
#define SX1278_REGISTER_DETECTOPTIMIZE                      0x31
#define SX1278_REGISTER_INVERTIQ                            0x33
#define SX1278_REGISTER_TEST36                              0x36
#define SX1278_REGISTER_DETECTIONTHRESHOLD                  0x37
#define SX1278_REGISTER_SYNCWORD							0x39
#define SX1278_REGISTER_TEST3A								0x3A
#define SX1278_REGISTER_INVERTIQ2							0x3B

#define SX1278_REGISTER_DIO_MAPPING1						0x40
#define SX1278_REGISTER_DIO_MAPPING2						0x41

#define SX1278_REGISTER_VER									0x42



#endif /* INC_SX1278_REGISTER_H_ */
