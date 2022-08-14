/********************************************************************************
 * File Name: CONTROLECU.c
 *
 * Author: Kirollos Medhat
 *
 * Description: Main file of Control mc
 *******************************************************************************/
#define F_CPU 8000000
#include "uart.h"
#include <avr/io.h>
#include "Buzzer.h"
#include "twi.h"
#include "external_eeprom.h"
#include "util/delay.h"
#include "DcMotor.h"
#include "common_macros.h"
#include "timer.h"
#include <avr/interrupt.h>
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 g_UARTValue =0;
uint16 g_tick = 0; /*Global variable indicates number of overflows*/
uint8 g_calc_sec_flag=0; /*Global variable indicates number of seconds*/

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/

#define SAVE 'q'
#define M2_Ready 'w'
#define SAVED 'e'
#define PASSWORDLIMIT 5
#define NOTFIRSTTIME 0x01
#define M1_ready 'r'
#define buzzerFlag 't'
#define checkPasswordEntered 'y'
#define wrongPassword 'u'
#define correctPassword 'i'
#define correctPassword1 4
#define ledon 'o'
#define getKey 'p'
#define unlockDoor 's'

/*******************************************************************************
 *                            FUNCTIONS DEFINITIONS                            *
 *******************************************************************************/

/*Function responsible for calculating seconds*/
void Calculate_time(void)
{
	g_tick ++; /*Every overflow increase ticks*/

	/*Once ticks become 30(according to the used prescalar),increase seconds flag and tick =0*/
	if(g_tick == 30)
	{
		g_calc_sec_flag++; /*Every 30 tick, increase seconds flag*/
		g_tick =0; /*Put tick=0 to start counting new second*/
	}
}
void CONTROL_recievePassword(void){
	uint8 arrayPass[5];
	uint8 index=0;
	while(UART_recieveByte() != SAVE);

	for(int i=0; i<PASSWORDLIMIT;i++){
		UART_sendByte(M2_Ready);
		arrayPass[index] = UART_recieveByte();
		EEPROM_writeByte((0x0311+i), arrayPass[index]);
		_delay_ms(70);
		index++;
	}
	EEPROM_writeByte(0x0420, NOTFIRSTTIME);
	_delay_ms(70);
	UART_sendByte(SAVED);
}

void ledOn(void){
	GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_writePin(PORTD_ID, PIN5_ID, LOGIC_HIGH);
}
void ledoff(void){
	GPIO_setupPinDirection(PORTD_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_writePin(PORTD_ID, PIN5_ID, LOGIC_LOW);
}
void ledOn2(void){
	GPIO_setupPinDirection(PORTB_ID, PIN5_ID, PIN_OUTPUT);
	GPIO_writePin(PORTB_ID, PIN5_ID, LOGIC_HIGH);
}

void CONTROL_mainMenu(void){
	/*
	 *Call this function where it check whether the entered password is right or not
	 *and it handle all the cases
	 */
	CONTROL_checkPassword();

	if(UART_recieveByte() == 0)
	{
		if(UART_recieveByte() == '*'){
			CONTROL_recievePassword();
		}
		else{
			CONTROL_openTheDoor();
		}
	}
}

void CONTROL_openTheDoor(void)
{
	while(UART_recieveByte() != unlockDoor);
	_delay_ms(1000);

	DcMotor_Rotate(CLOCKWISE, 90);
	TIMER_Config Config = {CPU_1024,COMPARE,0,65000};
	TIMER_init(&Config);


}
void CONTROL_timerCallFunction3(void)
{
	DcMotor_Rotate(ANTI_CLOCKWISE, 90);
	Timer_DeInit();


}
void CONTROL_timerCallFunction2(void)
{
	DcMotor_Rotate(STOP, 0);
	Timer_DeInit();
	Timer_setCallBack(CONTROL_timerCallFunction3);
	TIMER_Config Config = {CPU_1024,COMPARE,0,20000};
	TIMER_init(&Config);
	Timer_setCallBack(CONTROL_timerCallFunction3);

}
void CONTROL_buzzerCallBack(void)
{

	BUZZER_OFF();
	Timer_DeInit();

}
void CONTROL_checkPassword(void)
{
	/*arrayPass[5] to save the password getting from HMI from keypad
	 *arraypass2[5] to save the password getting from EEPROM
	 *uartReceive to get the receive byte UART
	 *wrongEntry to count the wrong trials
	 */
	uint8 arrayPass[5],arrayPass2[5];
	uint8 wrongEntry=0;


	/*Looping until HMI sends the ready handshake for checking the password with EEPROM*/

	while(UART_recieveByte() != checkPasswordEntered);


	/*looping to get the password entered in array and get the array saved in EEPROM
	 * in other array to prepare for comparing them
	 */

	for(int i=0;i<PASSWORDLIMIT;i++){
		arrayPass[i] = UART_recieveByte();
		EEPROM_readByte((0x0311+i), &arrayPass2[i]);
		_delay_ms(70);
		/*if the arrays don't match send to the HMI by UART that there is an Error
		 *with the saved array in EEPROM so that it appears in LCD
		 */
		if(arrayPass[i] != arrayPass2[i])
		{
			UART_sendByte(wrongPassword);

		}
		else
		{
			UART_sendByte(correctPassword);

		}
	}

	/*Receiving the numbers of wrong entries to HMI to decide the buzzer action*/
	wrongEntry = UART_recieveByte();
	/*Asking to try again of the number of trials is less than 3*/
	if(wrongEntry <= 2)
	{
		CONTROL_checkPassword();
	}
	else if (wrongEntry ==3)
	{
		SREG |= (1<<7);

		//BUZZER_init();
		Timer_setCallBack(Calculate_time);
		TIMER_Config Config = {CPU_1024,COMPARE,0,250};
		TIMER_init(&Config);
		Timer_setCallBack(Calculate_time);
//		while(g_calc_sec_flag != 5)
//		{
//			//BUZZER_ON();

//		}
		if(g_calc_sec_flag == 2) {
			ledOn();
		}
		ledoff();
		g_calc_sec_flag =0;
	}
	else
	{
		CONTROL_openTheDoor();
		ledOn2();
	}


}



int main(void){
	SREG |= (1<<7);
	uint8 firstTimeFlag =0;
	UART_Config Config = {EIGHT,ONE,DISABLE_PARITY,9600};
	UART_init(&Config);
	TWI_ConfigType TWI_Config = {F_CPU_CLOCK_1,0x02,0x01};
	TWI_init(&TWI_Config);
	DcMotor_Init();
	while(1){
		g_UARTValue = UART_recieveByte();
		while(g_UARTValue != M1_ready);
		EEPROM_readByte(0x0420, &firstTimeFlag);
		UART_sendByte(firstTimeFlag);
		if(firstTimeFlag != NOTFIRSTTIME)
		{
			CONTROL_recievePassword();
		}
		else
		{
			CONTROL_mainMenu();
		}

	}
	return 0;
}

