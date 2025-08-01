/*
 * switches.c
 *
 *  Created on: Feb 13, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains the switch interrupt service routines for the MSP430.
 *  It handles debouncing and state updates for SW1 (Port 4) and SW2 (Port 2),
 *  setting flags when each switch is pressed and preparing the system to
 *  process the input.
 *
 *  Functions included:
 *    - switchP4_interrupt: ISR for SW1 (Port 4) with debounce handling.
 *    - switchP2_interrupt: ISR for SW2 (Port 2) with debounce handling.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

char display_line[4][11];
volatile unsigned char display_changed;
extern unsigned int clear_usb_rx;
extern unsigned int clear_iot_rx;
extern unsigned int clear_process;

volatile unsigned char event;
volatile unsigned char choice;
volatile unsigned char state;
unsigned int okay_to_look_at_switch1 = OKAY;
volatile unsigned int sw1_position;
volatile unsigned int count_debounce_SW1;
unsigned int okay_to_look_at_switch2 = OKAY;
volatile unsigned int sw2_position;
volatile unsigned int count_debounce_SW2;
volatile unsigned int start_instruction;
volatile unsigned int Backlite;


#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void){          // Switch 1
    if (P4IFG & SW1) {
        P4IFG &= ~SW1; // IFG SW1 cleared
        P4IE &= ~SW1;
        TB0CCR1 = TB0R + SW1DEBOUNCE;
        TB0CCTL1 |= CCIE;

        sw1_position = 1;
        count_debounce_SW1 = 10;
    }


}


#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void){          // Switch 2
    if (P2IFG & SW2) {
        P2IFG &= ~SW2; // IFG SW2 cleared
        P4IE &= ~SW2;
        TB0CCR2 = TB0R + SW2DEBOUNCE;
        TB0CCTL2 |= CCIE;

        sw2_position = 1;
        count_debounce_SW2 = 10;
    }
}


