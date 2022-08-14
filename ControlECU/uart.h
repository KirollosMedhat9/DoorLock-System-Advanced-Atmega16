 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Kirollos Medhat
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "std_types.h"
extern volatile uint8 g_recievedValue;
extern volatile uint8 g_recievedFlag ;
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum  {
	ONE,TWO
}UART_STOP_BIT;
typedef enum {
	DISABLE_PARITY,RESERVED,ENABLED_EVEN,ENABLED_ODD
}UART_PARITY_MODE;
typedef enum  {
	FIVE,SIX,SEVEN,EIGHT,NINE=7
}UART_BIT_DATA;

/*******************************************************************************
 *                         Configuration Declaration                                   *
 *******************************************************************************/
typedef struct UART_Configuartion{
	UART_BIT_DATA dataBits;
	UART_PARITY_MODE parityMode;
	UART_STOP_BIT stopBit;
	uint32 baurdRate;
}UART_Config;
/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/


/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_Config * UART_ConfigType);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str); // Receive until #

#endif /* UART_H_ */
