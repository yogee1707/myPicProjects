
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
/*
//***TODO  This is a live, in Lab, exercise.

		You must use the PIC18F45K22 datasheet in order to complete:
		OSCILLATOR MODULE:
			Section 2.2 Oscillator Control, Page 29
			Section 2.2.1 Main System Clock Selection, Page 29
			Section 2.2.2 Internal Frequency Selection, Page 29
			Section 2.5 Internal Clock Modes, Page 36
			
		Registry pages: 32,33 & 37
			
	1. Configure the registries to set the PIC18F45K22's internal oscillator to 4MHz.
			There is already a function created where the registries are identified,
			set_osc_p18f45k22_4MHz().
	2. Call the set_osc_p18f45k22_4MHz() function within initializeSystem().
	
	3. Delete these comments when complete.
*/


// Preprocessor ===================================================================

#include "pragmas.h"
//#include <adc.h>
#include <stdlib.h>
//#include <delays.h>
#include <stdio.h>

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
										//	 keywords
					
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


/*** initializeADC: **************************************************************
Author:		CTalbot
Date:		14 Sept, 2016
Modified:	CTalbot
Desc:		Initalizes the ADC Module in the Processor to sample from A0 pin.
Input: 		None
Returns:	None
**********************************************************************************/
void initializeADC(void)
{
  TRISAbits.TRISA0= 1;			 	//config pin as input
  ANSELAbits.ANSA0 = 1; 			//disable digital buffer, set to analog

  /*OpenADC( 	ADC_FOSC_4 &          //config1
			ADC_RIGHT_JUST &
			ADC_12_TAD,
			ADC_CH0 &              //config2
			ADC_INT_OFF,
			ADC_REF_VDD_VDD &      //config3
			ADC_REF_VDD_VSS
                      
		);*/
}
//eo initializeADC:: *************************************************************

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
	//***TODO 2
	set_osc_p18f45k22_4MHz();				// Set the processor speed
	portConfig();								
	serialComm9600();					//initializing the serial port	
	initializeADC();					// Prepare the ADC module of the processor
  

}	
// eo initializeSystem::**********************************************************


/*** getTemp: ********************************************************************
Author:		CTalbot
Date:		14 Sept, 2016
Modified:	None
Desc:		Reads from the ADC at A0, returns the result
Input: 		None
Returns:	int - The read ADC result
**********************************************************************************/
int getTemp(void)
{
	int result;
	
	/*ConvertADC();                   // Start conversion    
	while( BusyADC() );             // Wait for completion 
	result = ReadADC();             // Read result         
   
	return result;*/

}
// eo getTemp:: ******************************************************************



/*********************************************************************************
***** MAIN FUNCTION **************************************************************
**********************************************************************************/

void main(void)	
{
	initializeSystem();	// Function call for setting the system I/Os and enabling the ADC module
						//	Many PIC library functions are called within
						// End of the initialization portion of this code
	
	// Begin indefinite loop for program

	while(1)
	{

		LED = FALSE;
		printf("led OFF\n\r");
		while((PORTD&PORTDMASK)==PORTDMASK);

		LED = TRUE;
		printf("led ON\n\r");
		while((PORTD&PORTDMASK)!=PORTDMASK);
		//Delay10KTCYx( 5 );		// Wait 50 ms if OSC is set to 4MHz.
	}//eo while
}