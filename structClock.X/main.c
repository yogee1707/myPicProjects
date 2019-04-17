/*
 * File:        main.c
 * Author:      Yogesh Tamhane
 * Date:        10 April, 2019
 * Modified:    None
 * © Yogesh Tamhane, 2019
 * Desc:        The program uses structure, to generate clock along with the Timer 0.
 */

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <pic18f45k22.h>
#include "pragmas.h"
#include "myConfig.h"

/** CONSTANTS ********************************************************************/
#define SW1 PORTDbits.RD7
#define SW2 PORTDbits.RD6
#define SW3 PORTDbits.RD5
#define SW4 PORTDbits.RD4 

#define PORTDMASK 0xF0

#define LED_ON LATB|=0x08
#define LED_OFF LATB&=0xF7
#define LED_TOGGLE LATB^=0x08

#define _XTAL_FREQ 4000000

#define TRUE 1
#define FALSE 0

#define TMR0FLAG INTCONbits.TMR0IF 

/** STRUCTURES *******************************************************************/
struct clock
{
    char seconds;
    char minutes;
    char hours;
    char isPM;
    char is24;
};

/** VARIABLES ********************************************************************/
char AMPM[2][10] = {"AM","PM"};

/***  systemInitialize: ***********************************************************
Author:		Yogesh Tamhane
Date:		4 April, 2019
Modified:	None
Desc:		Initalizes all the peripherals
Input: 		None
Returns:	None
**********************************************************************************/
void systemInitialize(void)
{
	set_osc_p18f45k22_4MHz();			
	portConfig();						
	serialComm();					//initializing the serial port	
    timer0Config();                 //one second tick of timer enabled 8-bits, 9600 baud
}	
// eo systemInitialize::**********************************************************

/*********************************************************************************
***** MAIN FUNCTION **************************************************************
**********************************************************************************/
void main(void) 
{
    systemInitialize();
    struct clock myClock;
     
    //set timing and format
    myClock.seconds = 50;
    myClock.minutes = 59;
    myClock.hours = 11;      
    myClock.isPM = FALSE; 
    myClock.is24 = FALSE;
    
    while(1)
    {
        printf("\33[2J \33[?25l \33[H");               //clear screen, hide cursor, cursor to home
        
        printf("%02d : %02d : %02d ",myClock.hours,myClock.minutes,myClock.seconds);
        if(!myClock.is24)
        {
            printf("%s",AMPM[myClock.isPM]);
        }
        
        while(!TMR0FLAG);                               //wait for timer flag to raise
        LED_TOGGLE;                                     //heart beat of clock
        myClock.seconds++;
        if(myClock.seconds==60)
        {
            myClock.seconds = 0;
            myClock.minutes++;
            if(myClock.minutes==60)
            {
                myClock.minutes=0;
                myClock.hours++;
                if((myClock.hours>12)&(!myClock.is24))
                {
                    myClock.hours=1;
                }
                else if(myClock.is24)
                {
                    if(myClock.hours>23)
                    {
                        myClock.hours = 0;
                    }
                }
                if(myClock.hours==12)
                {
                        myClock.isPM^=TRUE;         //TOGGLE using XOR
                }
            }
        }   
        //reset timer0
        TMR0FLAG = FALSE;
        TMR0H=0x0B;
        TMR0L=0xDC;
    }//eo while
}//eo main