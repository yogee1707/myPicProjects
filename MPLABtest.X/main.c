//*** Program Header ***
/***********************************************************************************************
	File Name:	TempLoggerStart.c
	Author:		
	Date:		
	Modified:	
	© Fanshawe College, 2016

	Description: This program will create and control a temperature logger sensor system. 
				Structures, arrays and ADC will be practiced and executed.
	
***********************************************************************************************/

// Preprocessor ===================================================================

#include "pragmas.h"
#include <stdlib.h>
//#include <delays.h>
#include <stdio.h>
#include <xc.h>

// Constants ======================================================================

#define TRUE			1
#define	FALSE			0

#define LED_ON			1				// Simple way to handle the LED operation,
#define LED_OFF 		0				//	Set for the logic type and use defined

#define SW1 PORTDbits.RD7
#define SW2 PORTDbits.RD6
#define SW3 PORTDbits.RD5
#define SW4 PORTDbits.RD4 

#define PORTDMASK 0xF0
#define LED PORTBbits.RB3
#define _XTAL_FREQ 4000000
					
// Global Variables ===============================================================

	
// Functions ======================================================================

/*** set_osc_p18f45k22_4MHz: ******************************************************
Author:
Date:		18 Oct, 2016		
Modified:	
Desc:		Sets the internal Oscillator of the Pic 18F45K22 to 4MHz.
Input: 		None
Returns:	None
**********************************************************************************/
void set_osc_p18f45k22_4MHz(void)
{
	//***TODO 1
	
	OSCCON = 0x52;  						// Sleep on slp cmd, HFINT 4MHz, INT OSC Blk
	OSCCON2 = 0x04;							// PLL No, CLK from OSC, MF off, Sec OSC off, Pri OSC
	OSCTUNE = 0x80;							// PLL disabled, Default factory freq tuning
	
	while (OSCCONbits.HFIOFS != 1); 	// wait for osc to become stable
}
//eo: set_osc_p18f45k22_4MHz:: ***************************************************


/***   portConfig: ***************************************************************
Author:		Yogesh Tamhane
Date:		25 March, 2019
Modified:	None
Desc:		Initalizes the ports required for the hardware components.
Input: 		None
Returns:	None
**********************************************************************************/
void portConfig(void)
{
	//***TODO 2
	ANSELA = 0x01;			//analog 0 i.e. AN0 is enabled
	LATA = 0x00;
	TRISA = 0xFF;			//setting entire porta as the input
  	
	ANSELB = 0x00;
	LATB = 0x00;
	TRISB = 0xF0;			//portb bit3 output

	ANSELD = 0x00;
	LATD = 0x00;
	TRISD = 0xFF;			//all portd bits are input

	//Serial comm portconfig
	TRISCbits.TRISC6 = 1;
	TRISCbits.TRISC7 = 1;

}	
// eo  portConfig::***************************************************************

/*** serialComm9600: ***************************************************************
Author:		Yogesh Tamhane
Date:		25 March, 2019
Modified:	None
Desc:		Initialize serial port to communicate at 9600 baud
Input: 		None
Returns:	None
**********************************************************************************/
void serialComm9600()
{
	SPBRG1 = 25;
	RCSTA1 = 0x90;
	TXSTA1 = 0x26;
	BAUDCON1 = 0x40; 
}
// eo serialComm9600::***************************************************************


/***  initializeSystem: ***********************************************************
Author:		CTalbot
Date:		14 Sept, 2016
Modified:	None
Desc:		Initalizes the Temperature Logger System.
Input: 		None
Returns:	None
**********************************************************************************/
void initializeSystem(void)
{
	set_osc_p18f45k22_4MHz();			
	portConfig();						
	serialComm9600();					//initializing the serial port	
}	
// eo initializeSystem::**********************************************************

void putch(unsigned char data) {
    while( ! PIR1bits.TXIF);          // wait until the transmitter is ready
    TXREG = data;                     // send one character
}

/*********************************************************************************
***** MAIN FUNCTION **************************************************************
**********************************************************************************/
void main(void)	
{
	initializeSystem();	//initialize all peripherals
    printf("\33[?25l");
	while(1)
	{
        
		LED = FALSE;
        printf("\33[2J \33[0;0H \33[0;0f");
		printf("LED OFF");
		while((PORTD&PORTDMASK)==PORTDMASK);

		LED = TRUE;
        printf("\33[2J \33[0;0H \33[0;0f");
		printf("LED ON");
		while((PORTD&PORTDMASK)!=PORTDMASK);
	}//eo while
}