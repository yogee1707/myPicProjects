/***********************************************************************************************
	File Name:	myConfig.h
	Author:		Yogesh Tamhane
	Date:		4 April, 2019
	Modified:	None
	© Yogesh Tamhane, 2019

	Description: This configuration file contains the peripheral configuration functions.
	
***********************************************************************************************/

// Constants ======================================================================

#define TRUE			1
#define	FALSE			0

// Functions ======================================================================

/*** set_osc_p18f45k22_4MHz: ******************************************************
Author:		Yogesh Tamhane
Date:		4 April, 2019		
Modified:	None
Desc:		Sets the internal Oscillator of the Pic 18F45K22 to 4MHz.
Input: 		None
Returns:	None
**********************************************************************************/
void set_osc_p18f45k22_4MHz(void)
{	
	OSCCON = 0x52;  						// Sleep on slp cmd, HFINT 4MHz, INT OSC Blk
	OSCCON2 = 0x04;							// PLL No, CLK from OSC, MF off, Sec OSC off, Pri OSC
	OSCTUNE = 0x80;							// PLL disabled, Default factory freq tuning
	
	while (OSCCONbits.HFIOFS != 1); 	// wait for osc to become stable
}
//eo: set_osc_p18f45k22_4MHz:: ***************************************************

/*** set_osc_p18f45k22_16MHz: *****************************************************
Author:		Yogesh Tamhane
Date:		4 April, 2019		
Modified:	None
Desc:		Sets the internal Oscillator of the Pic 18F45K22 to 16MHz.
Input: 		None
Returns:	None
**********************************************************************************/
void set_osc_p18f45k22_16MHz(void)
{	
	OSCCON = 0x72;  						// Sleep on slp cmd, HFINT 4MHz, INT OSC Blk
	OSCCON2 = 0x04;							// PLL No, CLK from OSC, MF off, Sec OSC off, Pri OSC
	OSCTUNE = 0x80;							// PLL disabled, Default factory freq tuning
	
	while (OSCCONbits.HFIOFS != 1); 	// wait for osc to become stable
}
//eo: set_osc_p18f45k22_16MHz:: **************************************************


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
	TRISB = 0xF7;			//portb bit3 output

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
void serialComm()
{
	SPBRG1 = 8;        //baudrate 115200, 8bit, no parity
	RCSTA1 = 0x90;
	TXSTA1 = 0x26;
	BAUDCON1 = 0x40; 
}
// eo serialComm9600::***************************************************************

/***  putch: *********************************************************************
Author:		Yogesh Tamhane
Date:		4 April, 2019
Modified:	None
Desc:		This act as the supporting function for the printf function, redirects stdio to EUART1
Input: 		None
Returns:	None
**********************************************************************************/
void putch(unsigned char data) {
    while( ! PIR1bits.TXIF);          // wait until the transmitter is ready
    TXREG = data;                     // send one character
}
// eo putch::**********************************************************************

/***  adcConfig: ******************************************************************
Author:		Yogesh Tamhane
Date:		4 April, 2019
Modified:	None
Desc:		This function configures the ADC and enables the AN0 at RA0
Input: 		None
Returns:	None
**********************************************************************************/
void adcConfig()
{
    ADCON0 = 0x01;
    ADCON1 = 0x00;
    ADCON2 = 0xC0; //10 111 000
}
// eo adcConfig::******************************************************************

/***  initializeSystem: ***********************************************************
Author:		Yogesh Tamhane
Date:		4 April, 2019
Modified:	None
Desc:		Starts ADC conversion and waits for result
Input: 		None
Returns:	int - ADRES value from the ADC conversion
**********************************************************************************/
int getADRES()
{
    ADRES = 0;
    ADCON0bits.GO=TRUE;
    while(ADCON0bits.GO);
    return ADRES;
}
// eo getADRES::*******************************************************************

/***  initializeSystem: ***********************************************************
Author:		Yogesh Tamhane
Date:		4 April, 2019
Modified:	None
Desc:		Initalizes all the peripherals
Input: 		None
Returns:	None
**********************************************************************************/
void systemInitialize(void)
{
	set_osc_p18f45k22_16MHz();			
	portConfig();						
	serialComm();					//initializing the serial port	
    adcConfig();
}	
// eo initializeSystem::**********************************************************