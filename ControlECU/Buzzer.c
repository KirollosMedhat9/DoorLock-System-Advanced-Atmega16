/*
 * Buzzer.c
 *
 *  Created on: Nov 3, 2021
 *      Author: Kirollos Medhat
 */

#include "Buzzer.h"
#include "gpio.h"
void BUZZER_init(){
	GPIO_setupPinDirection(PORTD_ID, PIN2_ID, PIN_OUTPUT);
}


void BUZZER_ON(){
	GPIO_writePin(PORTD_ID, PIN2_ID, LOGIC_HIGH);
}

void BUZZER_OFF(){
	GPIO_writePin(PORTD_ID, PIN2_ID, LOGIC_LOW);
}
