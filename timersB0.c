/*
 * timersB0.c
 *
 *  Created on: Feb 17, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains timer initialization and interrupt service routines
 *  for the MSP430. It sets up Timer B0 for periodic interrupts used for
 *  timing, debouncing switches, and managing system counters. It also
 *  configures Timer B3 for PWM generation to control motor speeds and
 *  backlight brightness.
 *
 *  Functions included:
 *    - Init_Timers: Initializes both Timer B0 and Timer B3.
 *    - Init_Timer_B0: Configures Timer B0 for timing and debounce interrupts.
 *    - Timer0_B0_ISR: Handles periodic tasks every 10 ms.
 *    - TIMER0_B1_ISR: Handles switch debounce and overflow events.
 *    - Init_Timer_B3: Sets up PWM outputs for motor and backlight control.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

extern volatile unsigned int wait;
extern volatile unsigned int proj7timer;
extern volatile unsigned int proj7timer2;
extern volatile unsigned int proj8timer;
extern volatile unsigned int proj8display;
volatile unsigned char update_display;
volatile unsigned int Time_Sequence;
volatile unsigned int time_change;
volatile unsigned int start_instruction;
volatile unsigned int instruction;
volatile unsigned int count_debounce_SW1;
volatile unsigned int count_debounce_SW2;
volatile unsigned int hex;
volatile unsigned int counter;
volatile unsigned int overflow_ctr;
volatile unsigned int DAC_data;


extern unsigned int iot_boot_timer;
extern unsigned int time;
volatile unsigned int timer10s;
volatile unsigned int tenmsCounter;

void Init_Timers(void){
    Init_Timer_B0();
    Init_Timer_B3();
}

//------------------------------------------------------------------------------
// Timer B0 initialization sets up both B0_0, B0_1-B0_2 and overflow
void Init_Timer_B0(void) {
    TB0CTL = TBSSEL__SMCLK;         // SMCLK source
    TB0CTL |= TBCLR;                // Resets TB0R, clock divider, count direction
    TB0CTL |= MC__CONTINOUS;        // Continuous up
    TB0CTL |= ID__2;                // Divide clock by 2

    TB0EX0 = TBIDEX__8; // Divide clock by an additional 8


    TB0CCR0 = TB0CCR0_INTERVAL;     // CCR0
    TB0CCTL0 |= CCIE;               // CCR0 enable interrupt
    // TB0CCR1 = TB0CCR1_INTERVAL; // CCR1
    // TB0CCTL1 |= CCIE; // CCR1 enable interrupt
    // TB0CCR2 = TB0CCR2_INTERVAL; // CCR2
    // TB0CCTL2 |= CCIE; // CCR2 enable interrupt
    TB0CTL &= ~TBIE;     // Disable Overflow Interrupt
    TB0CTL &= ~TBIFG;   // Clear Overflow Interrupt flag

}
//------------------------------------------------------------------------------

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
//------------------------------------------------------------------------------
// TimerB0 0 Interrupt handler
//----------------------------------------------------------------------------
//...... Add What you need happen in the interrupt ......
    // Interrupt runs every 10 ms

    iot_boot_timer++;
    time++;

    proj8timer++;
    proj8display++;
    tenmsCounter++;
    timer10s++;
    wait++;

    if(count_debounce_SW1){
        count_debounce_SW1--;
    }

    if(count_debounce_SW2){
        count_debounce_SW2--;
    }
    update_display = 1;
    TB0CCR0 += TB0CCR0_INTERVAL; // Add Offset to TBCCR0
//----------------------------------------------------------------------------
}

#pragma vector = TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
    //----------------------------------------------------------------------------
    // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
    //----------------------------------------------------------------------------
    switch(__even_in_range(TB0IV,14)){
        case 0: break; // No interrupt
        case 2: // SW1 Debounce
            TB0CCTL1 &= ~CCIE;
            P4IFG &= ~SW1;
            P4IE |= SW1;
            TB0CCR1 += TB0CCR1_INTERVAL;
            break;
        case 4: // SW2 Debounce
            TB0CCTL2 &= ~CCIE;
            P4IFG &= ~SW2;
            P4IE |= SW2;

            //...... Add What you need happen in the interrupt ......
            TB0CCR2 += TB0CCR2_INTERVAL; // Add Offset to TBCCR2
            break;
        case 14: // overflow
            //...... Add What you need happen in the interrupt ......
            break;
        default: break;
}
//----------------------------------------------------------------------------
}


void Init_Timer_B3(void) {
    //------------------------------------------------------------------------------
    // SMCLK source, up count mode, PWM Right Side
    // TB3.1 P6.0 LCD_BACKLITE
    // TB3.2 P6.1 R_FORWARD
    // TB3.3 P6.2 R_REVERSE
    // TB3.4 P6.3 L_FORWARD
    // TB3.5 P6.4 L_REVERSE
    //------------------------------------------------------------------------------
     TB3CTL = TBSSEL__SMCLK; // SMCLK
     TB3CTL |= MC__UP; // Up Mode
     TB3CTL |= TBCLR; // Clear TAR

     PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]

     TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
     LCD_BACKLITE_DIMING = PERCENT_80; // P6.0 Right Forward PWM duty cycle

     TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
     RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Right Forward PWM duty cycle

     TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
     LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Left Forward PWM duty cycle

     TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
     RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Right Reverse PWM duty cycle

     TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
     LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Left Reverse PWM duty cycle
    //------------------------------------------------------------------------------
}

