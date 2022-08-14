/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: Source file for the LCD driver
 *
 * Author: Kirollos Medhat
 *
 *******************************************************************************/

#ifndef PWM_H_
#define PWM_H_
#include "std_types.h"
/*******************************************************************************
 *                          Definitions                              *
 *******************************************************************************/
#define PWM_TIMER0_MAXBITS 255
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description: Function to generate a PWM Signal According to duty cycle argument
 * 				through timer0
 * Arguments : Char of 1 byte
 * Return type : void
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
