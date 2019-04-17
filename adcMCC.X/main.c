/*
 * File:        main.c
 * Author:      Yogesh Tamhane
 * Date:        4 April, 2019
 * Modified:    None
 * © Yogesh Tamhane, 2019
 * Desc:        This program is created to test the ADC and TAD relationship in PIC18F45K22
 */

#include <xc.h>
#include <stdlib.h>
#include <stdio.h>
#include <pic18f45k22.h>
#include "pragmas.h"
#include "myConfig.h"

#define SW1 PORTDbits.RD7
#define SW2 PORTDbits.RD6
#define SW3 PORTDbits.RD5
#define SW4 PORTDbits.RD4 

#define PORTDMASK 0xF0
#define LED PORTBbits.RB3

#define LED_ON LATB|=0x08
#define LED_OFF LATB&=0xF7

#define SAMPLES 20

#define _XTAL_FREQ 16000000

#define TRUE 1
#define FALSE 0
#define FADE 100

void PWMRB3()
{
    TRISBbits.RB3 = TRUE;
    CCPTMRS0 = 0x00;
    PR2 = 0x09;           //10k pwm frequncy
    CCP2CON = 0b00001100;
    CCPR2L = 0x32;
    PIR1bits.TMR2IF = FALSE;
    T2CONbits.T2CKPS0 = 1;
    T2CONbits.T2CKPS1 = 0;
    T2CONbits.TMR2ON = TRUE;
    while(!PIR1bits.TMR2IF);
    PIR1bits.TMR2IF = FALSE;
    TRISBbits.RB3 = FALSE;
}

void main(void) 
{
    systemInitialize();
    
    while(1)
    {
        //de-bouncing issues solved
        while(PORTDbits.RD4);
        LED_ON;
        while(!PORTDbits.RD4);
        
        //take 10 samples from ADC
        for(int index=0; index<SAMPLES; index++)
        {   
            if(index!=0)
                printf(",");

            printf("%d",getADRES());
            __delay_ms(10);
        }
        printf("\n\r");
        LED_OFF;
    }
}