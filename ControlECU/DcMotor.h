/******************************************************************************
 *
 * Module: DcMotor
 *
 * File Name: DcMotor.h
 *
 * Description: Source file for the DC Motor driver
 *
 * Author: Kirollos Medhat
 *
 *******************************************************************************/
#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include "std_types.h"
#include "gpio.h"
/*******************************************************************************
 *                          Definitions                              *
 *******************************************************************************/
#define DcMotor_CONNECTION_PORT PORTB_ID
#define DcMotor_CONNECTION_IN1_PIN PIN0_ID
#define DcMotor_CONNECTION_IN2_PIN PIN1_ID
/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum{
	STOP,CLOCKWISE,ANTI_CLOCKWISE
}DcMotor_State;
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description: Function to Initialize DC Motor Connected to the MC
 * 				Setting the start motion as Stopped
 * Arguments  : void
 * Return Type: void
 */
void DcMotor_Init(void);


/*
 * Description: Function to Configure the state of the DC motor and the speed
 * 				of the Motor according to the generated signal
 * 				from PWM module (0-100)%
 * Arguments  : DC Motor state Configuration structure, Char byte speed of motor
 * Return Type: void
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed);

#endif /* DCMOTOR_H_ */
