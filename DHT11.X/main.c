/*
 * File:        main.c
 * Author:      Yogesh Tamhane
 * Date:        4 April, 2019
 * Modified:    None
 * © Yogesh Tamhane, 2019
 * Desc:        DHT11 sensor communication with pic18f45k22.
 */

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <pic18f45k22.h>
#include "pragmas.h"
#include "myConfig.h"

#define LED_ON LATB|=0x08
#define LED_OFF LATB&=0xF7

#define _XTAL_FREQ 4000000

#define DIR     TRISAbits.TRISA1
#define IN      1
#define OUT     0
#define READ    PORTAbits.RA1
#define WRITE   LATAbits.LATA1

char sensorOutpur[4] = {'null'};

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
	set_osc_p18f45k22_4MHz();			
	portConfig();						
	serialComm();					//initializing the serial port	
    adcConfig();
}	
// eo initializeSystem::**********************************************************

void DHT11start()
{
    DIR = OUT;
    //LED_ON;
    WRITE = FALSE;
    __delay_ms(18);
    //LED_OFF;
    WRITE = TRUE;
    __delay_us(10);
    DIR = IN;
}

void responseWait()
{
    while(READ);
    while(!READ);
    while(READ);
}

char readByte()
{
    char data=0, index=0;
    DIR = IN;
    
    for(index=0;index<8;index++)
    {
        while(READ==0);
        __delay_us(30);
        if(READ==1)
        {
            data = ((data<<1)|1);
        }
        else
        {
            data = (data<<1);
        }
        while(READ==1);
    }
    
    return data;
}

void main(void) 
{
    systemInitialize();

    while(1)
    {
        DIR = OUT;
        WRITE = TRUE;
        __delay_ms(1000);
    
        DHT11start();
        responseWait();
        char RHI = readByte();
        char RHD = readByte();
        char TI = readByte();
        char TD = readByte();
        char checksum = readByte();
        char verify = RHI+RHD+TI+TD;
        
        if(checksum==verify)
        {
            printf("Temp: %d.%d",TI,TD);
            printf("\tHum: %d.%d\n\r",RHI,RHD);
        }
        else
        {
            printf("Checksum Error!\n\r");
        }
        __delay_ms(1000);
    }
}