/********************************************************************************
 * File Name: HMIECU.c
 *
 * Author: Kirollos Medhat
 *
 * Description: Main file of HMI mc
 *******************************************************************************/
#define F_CPU 8000000
#include "uart.h"
#include "keypad.h"
#include "lcd.h"
#include <avr/io.h>
#include "util/delay.h"
#include "gpio.h"
#include "timer.h"
#include <avr/interrupt.h>

/*******************************************************************************
 *                                  Definitions                                *
 *******************************************************************************/
#define PASSWORDLIMIT 5
#define SAVE 'q'
#define M2_Ready 'w'
#define SAVED 'e'
#define M1_Ready 'r'
#define buzzerFlag 't'
#define NOTFIRSTTIME 0x01
#define checkPasswordEntered 'y'
#define wrongPassword 'u'
#define correctPassword 'i'
#define correctPassword1 4
#define ledon 'o'
#define getKey 'p'
#define unlockDoor 's'
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/
uint8 g_compareResult=0;
uint8 g_returnMainOptions =0;
uint8 wrongEntry=0;

uint8 correctPasswordFlag=0;

/*******************************************************************************
 *                            FUNCTIONS DEFINITIONS                            *
 *******************************************************************************/
void APP_newPassword(void){
	uint8 arrayPass[5],arrayPass2[5];
	uint8 key=0,index=0,index2=0,compareResults=0;
	LCD_clearScreen();
	LCD_displayString("Enter New Pass:");
	LCD_moveCursor(1, 0);
	for(int i=0; i<PASSWORDLIMIT ; i++){
		key = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		arrayPass[index] = key;
		index++;
		_delay_ms(500);
	}
	LCD_clearScreen();
	LCD_displayString("ReEnter new Pass:");
	LCD_moveCursor(1, 0);
	for(int i=0; i<PASSWORDLIMIT ; i++){
		key = KEYPAD_getPressedKey();
		LCD_displayCharacter('*');
		arrayPass2[index2] = key;
		index2++;
		_delay_ms(500);
	}

	for(int i=0;i<PASSWORDLIMIT;i++){
		if(arrayPass[i]!=arrayPass2[i]){
			compareResults++;
		}
		else
		{
			compareResults=0;
		}
	}

	if(compareResults==0){
		UART_sendByte(SAVE);
		for(int i=0;i<PASSWORDLIMIT;i++){
			while(UART_recieveByte()!= M2_Ready);
			UART_sendByte(arrayPass[i]);
		}
		while(UART_recieveByte() != SAVED);
		g_returnMainOptions=0;
		UART_sendByte(g_returnMainOptions);
	}
	else {
		LCD_clearScreen();
		LCD_displayString("MissMatch");
		_delay_ms(1000);
		APP_newPassword();
	}
}




void APP_passwordCheck(void){
	/*creating array to save the password in for check*/
	uint8 arrayPass[5];
	_delay_ms(1000); /*delay to get the uart ready for avoiding sending any wrong bits*/
	/*creating local variable to get the pressed key by keypad*/
	uint8 key;

	/*declaring passwordErrors for detecting Miss Match passwords if not equal 0
	 *declaring wrongEntry for detecting wrong entries numbers for buzzer on decision
	 */
	uint8 passwordErrors =0;

	/*configure the screen for enter password*/
	LCD_clearScreen();
	LCD_displayString("Enter Password:");
	LCD_moveCursor(1, 0);

	/*getting the password entered by keypad and saving it in arrayPass*/
	for(int i=0;i<PASSWORDLIMIT;i++){
		key = KEYPAD_getPressedKey();

		if(key >= 0 && key <= 9){    /*get in the array integers only*/
			arrayPass[i] = key;
			LCD_displayCharacter('*');

		}

		_delay_ms(500);  /*delay to get the next key pressed correctly*/

	}
	/*sending a handshake with the HMI to get READY for checking the password*/
	UART_sendByte(checkPasswordEntered);


	/*sending the array to CONTROL to check with the one already saved in EEPROM  address*/
	for(int i=0;i<PASSWORDLIMIT;i++){
		UART_sendByte(arrayPass[i]); /*sending the password array*/


		/*detecting if the HMI send that the password in wrong compared to the one saved in eeprom*/
		if(UART_recieveByte() == wrongPassword){
			passwordErrors++;
		}

	}


	if(passwordErrors != 0) /*which means that the password is not correct and errors found */
	{
		/*increment the number of wrong entries to decide about the buzzzer*/
		wrongEntry++;
		/*sending the number of wrong Entries to Control as if 1 or 2 will ask to try again
		 *but if 3 times wrong will activate the buzzer
		 */

		UART_sendByte(wrongEntry);

		/*handle the LCD in HMI for the wrong password trials*/
		if(wrongEntry < 3)
		{
			LCD_clearScreen();
			LCD_displayString("Wrong Password");
			LCD_displayStringRowColumn(1, 0, "Try Again");
			_delay_ms(500); /*delay for appearing on screen that it is wrong password*/
			APP_passwordCheck(); /*asking to check password again if wrong trials is less than 3*/

		}
		else /*if the number of wrong trials is 3 will appear and error message on lcd*/
		{
			LCD_clearScreen();
			LCD_displayString("ERROR!");
			wrongEntry=0; /*reseting the wrong trials */
			g_returnMainOptions =1;
			_delay_ms(5000); //timer
			APP_mainMenu();
		}


	}
	else {

		passwordErrors=0;
		LCD_clearScreen();
		LCD_displayString("Right password!");
		UART_sendByte(correctPassword1); /*sending to control that no Errors*/
		/*reseting the wrong Entry to zero in order if the password typed from second time correctly
		 *and the application is still on so that if entered again wrong is not considered to
		 *decide about the buzzer
		 */
		wrongEntry =0;
		_delay_ms(1000);
		g_returnMainOptions=0;


	}
}




void APP_mainMenu(void){
	uint8 keyPressed;

	LCD_clearScreen(); /*clear the screen for appearing main menu*/
	LCD_displayStringRowColumn(0, 1, "x:Change Password"); /*display change password option*/
	LCD_displayStringRowColumn(1, 1, "-:Open The Door");/*display open door option*/
	/*get the key pressed by keypad in local variable*/
	while((keyPressed != '*') && (keyPressed != '-')) { /*if the input was not proper get another input until it is right 'x' or '-'*/
		keyPressed = KEYPAD_getPressedKey();
	}
	APP_passwordCheck();
	UART_sendByte(g_returnMainOptions);
	if(g_returnMainOptions == 0)
	{
		UART_sendByte(keyPressed);
		if(keyPressed == '*')
		{
			APP_newPassword();
		}
		else
		{
			APP_openDoor();
		}
	}
	else
	{
		g_returnMainOptions =0;
	}


}


void APP_openDoor(void){
	UART_sendByte(unlockDoor);

	LCD_clearScreen();
	LCD_displayString("Door is Opening!");
	_delay_ms(15000);
	LCD_clearScreen();
	LCD_displayString("Door is open!");
	LCD_clearScreen();
	_delay_ms(3000);
	LCD_displayString("Door is Closing!");
	_delay_ms(15000);
}

int main(void){
	uint8 firstTimeFlag =0;
	UART_Config Config = {EIGHT,ONE,DISABLE_PARITY,9600};
	UART_init(&Config);
	SREG |= (1 << 7);
	LCD_init();


	while(1){
		UART_sendByte(M1_Ready);
		firstTimeFlag = UART_recieveByte();
		if(firstTimeFlag != 0x01){
			APP_newPassword();
		}
		else {
			APP_mainMenu();
		}


		return 0;
	}
}
