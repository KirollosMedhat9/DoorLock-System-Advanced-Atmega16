/******************************************************************************
 *
 * Module: DcMotor
 *
 * File Name: DcMotor.h
 *
 * Description: Source file for timer header
 *
 * Author: Kirollos Medhat
 *
 *******************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_
#include "gpio.h"
#include "std_types.h"
#include <avr/io.h>
#include <avr/interrupt.h>

#define TIMERNUMBER 0
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	TIMER0,TIMER1,TIMER2
}TIMER_Num;
typedef enum {
	NO_CLOCK,CPU_CLOCK,CPU_8,CPU_64,CPU_256,CPU_1024
}TIMER_Clock_Freq;
typedef enum {
	OVERFLOW,COMPARE
}TIMER_Mode;
typedef struct{
//	TIMER_Num Timer;
	TIMER_Clock_Freq CLOCK_FREQ;
	TIMER_Mode TIMER_MODE;
	uint8 initialValue;
	uint8 compareValue;
}TIMER_Config;

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/



/*
 * Description:Description: Function to initialize timer zero according
 * 			   to passed structure configuration address
 * Arguments  : &TIMER_ConfigType
 * Return Type: void
 */
void TIMER_init(const TIMER_Config * TIMER_ConfigType);

/*
 * Description:Function to call back function in app layer
 * Arguments  : void(*aTimer_ptr)(void)
 * Return Type: void
 */

void Timer_setCallBack( void(*aTimer_ptr)(void));

/*
 * Description: Function to disable the Timer1
 * Arguments  : void
 * Return Type: void
 */

void TIMER_DeInit(void);
#endif /* TIMER_H_ */
