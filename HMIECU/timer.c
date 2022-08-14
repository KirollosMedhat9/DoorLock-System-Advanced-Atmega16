/******************************************************************************
 *
 * Module: DcMotor
 *
 * File Name: DcMotor.c
 *
 * Description: Source file for the timer driver
 *
 * Author: Kirollos Medhat
 *
 *******************************************************************************/

#include "timer.h"

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
static volatile void (*g_callBackTimerPtr)(void) = NULL_PTR;
/*******************************************************************************
 *                       Interrupt Service Routines                            *
 *******************************************************************************/
#if (TIMERNUMBER  == 0)
ISR(TIMER0_OVF_vect)
{
	if(g_callBackTimerPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_callBackTimerPtr)();
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackTimerPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_callBackTimerPtr)();
	}
}
#endif
#if (TIMERNUMBER  == 1)

ISR(TIMER1_OVF_vect)
{
	if(g_callBackTimerPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application after each overflow */
		(*g_callBackTimerPtr)();
	}
}

ISR(TIMER1_COMP_vect)
{
	if(g_callBackTimerPtr != NULL_PTR)
	{
		/* Call the Call Back function in the application
		 * after the timer reach the compare value
		 */
		(*g_callBackTimerPtr)();
	}
}

#endif
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description:Description: Function to initialize timer zero according
 * 			   to passed structure configuration address
 * Arguments  : &TIMER_ConfigType
 * Return Type: void
 */
#if TIMERNUMBER  == 0
void TIMER_init(const TIMER_Config * TIMER_ConfigType)
{

	SREG &= ~(1<<7); /*Disable I-bit at the beginning*/

	/*FOC0=1:Non_PWM mode always
	 * and setting all other bits zero
	 */
	TCCR0 = (1<<FOC0);
	if (TIMER_ConfigType->TIMER_MODE == 1)  /*compare mode*/
	{
		/*WGM00=0: for compare mode
		 *COM01:0=0: No need for OC0*/
		TCCR0 |= (1<WGM01);
		/*Setting the compare value by config*/
		OCR0 = TIMER_ConfigType->compareValue;
		/*setting the reset value*/
		TCNT0 = TIMER_ConfigType->initialValue;

		/*Masking the first 3 bits of TCCR0 to insert the Clock freq*/
		TCCR0 = (TCCR0 & 0xF8) | (TIMER_ConfigType->CLOCK_FREQ); /*Put the prescalar in the first 3-bits*/
		TIMSK = (1<<OCIE0); /*OCIE0=1: Enable output compare match interrupt*/
	}
	else
	{
		TIMSK = (1<<TOIE0); /*TOIE0=1: Enable overflow interrupt*/
	}

	SREG |= (1<<7); /*Enable I-bit*/
}
#endif
#if TIMERNUMBER == 1
void TIMER_init(const TIMER_Config * TIMER_ConfigType)
{

	SREG &= ~(1<<7); /*Disable I-bit at the beginning*/

	/*FOC1A,FOC1B=1: Non_PWM mode
	 *WGM10,WGM11=0: Whether it is normal mode or compare mode
	 *COM1A1:0,COM1B1:0=0: No need for OC1
	 */
	TCCR1A = (1<<FOC1A) | (1<<FOC1B);
	if (TIMER_ConfigType->TIMER_MODE == 1)  /*compare mode*/
	{
		TCCR1B = (1<<WGM12); /*WGM12=1: Compare mode (Mode 12)*/
		/*Setting the compare value by config*/
		OCR1A = TIMER_ConfigType->compareValue;
		/*setting the reset value*/
		TCNT1 = TIMER_ConfigType->initialValue;

		/*Masking the first 3 bits of TCCR0 to insert the Clock freq*/
		TCCR1B = (TCCR1B & 0xF8) | (TIMER_ConfigType->CLOCK_FREQ); /*Put the prescalar in the first 3-bits*/
		TIMSK = (1<<OCIE1A); /*OCIE0=1: Enable output compare match interrupt*/
	}
	else
	{
		TIMSK = (1<<TOIE1); /*TOIE1=1: Enable overflow interrupt*/
	}

	SREG |= (1<<7); /*Enable I-bit*/
}
#endif
/*
 * Description:Function to call back function in app layer
 * Arguments  : void(*aTimer_ptr)(void)
 * Return Type: void
 */
void Timer_setCallBack( void(*aTimer_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackTimerPtr = aTimer_ptr;
}



/*
 * Description: Function to disable the Timer1
 * Arguments  : void
 * Return Type: void
 */
void Timer_DeInit(void)
{
	/* Clear All Timer1 Registers */
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	OCR1A = 0 ;
	OCR1B = 0 ;


}
