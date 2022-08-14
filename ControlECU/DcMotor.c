/******************************************************************************
 *
 * Module: DcMotor
 *
 * File Name: DcMotor.c
 *
 * Description: Source file for the DC Motor driver
 *
 * Author: Kirollos Medhat
 *
 *******************************************************************************/
#include "DcMotor.h"
#include "gpio.h"
#include "PWM.h"
#include "std_types.h"
#include <avr/io.h>
/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
/*
 * Description: Function to Initialize DC Motor Connected to the MC
 * 				Setting the start motion as Stopped
 * Arguments  : void
 * Return Type: void
 */
void DcMotor_Init(void){
	/*
	 * Setting the DC Motor which the fan will be connected to
	 * as output pin
	 */
	GPIO_setupPinDirection(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN1_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN2_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(DcMotor_CONNECTION_PORT, PIN3_ID, PIN_OUTPUT);
	/*
	 * Setting the Motor to be stopped at the start
	 *
	 */

	GPIO_writePin(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN1_PIN, LOGIC_LOW);
	GPIO_writePin(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN2_PIN, LOGIC_LOW);
}


/*
 * Description: Function to Configure the state of the DC motor and the speed
 * 				of the Motor according to the generated signal
 * 				from PWM module (0-100)%
 * Arguments  : DC Motor state Configuration structure, Char byte speed of motor
 * Return Type: void
 */
void DcMotor_Rotate(DcMotor_State state,uint8 speed){

	uint8 duty_cycle=((float)speed/100)*255;


	/*
	 * Calling Start timer function from PWM.h module
	 * to start the motion of the motor according to given speed
	 */
	PWM_Timer0_Start(duty_cycle);

	/*
	 * Setting the State of motor according the given structure Configuration
	 */

	if(state == STOP){
		GPIO_writePin(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN1_PIN, LOGIC_LOW);
		GPIO_writePin(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN2_PIN, LOGIC_LOW);
	}
	else if(state == CLOCKWISE){
		GPIO_writePin(DcMotor_CONNECTION_PORT,DcMotor_CONNECTION_IN1_PIN, LOGIC_LOW);
		GPIO_writePin(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN2_PIN, LOGIC_HIGH);
	}
	else{
		GPIO_writePin(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN1_PIN, LOGIC_LOW);
		GPIO_writePin(DcMotor_CONNECTION_PORT, DcMotor_CONNECTION_IN2_PIN, LOGIC_HIGH);
	}

}
