/*
 * movement.c
 *
 *  Created on: Feb 13, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains the Black Line Intercept state machine for the MSP430.
 *  It implements the logic for detecting and following a black line, executing
 *  specific maneuvers (start, intercept, turn, travel, and circle), and
 *  managing transitions between these states based on sensor input and timers.
 *
 *  Functions included:
 *    - BlackLineIntercept: State machine that controls the black line following routine.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern char display_changed;
extern volatile unsigned char update_display;

extern unsigned int sw1_position;
extern unsigned int sw2_position;
unsigned int calibration;
unsigned int Wcal;
unsigned int Bcal;
unsigned int avg;


unsigned int BLStart;
char BLState;

extern volatile unsigned int proj7timerDisplay;
extern volatile unsigned int proj7timer;
extern volatile unsigned int timer10s;

extern unsigned int secondsCounter;
extern unsigned int timer_start;
unsigned int ADC_Channel;
unsigned int ADC_Left_Det;
unsigned int ADC_Right_Det;
unsigned int ADC_Thumb;
volatile unsigned int wait;

//-----------------------------------------------------------------
// State Machine for Black Line Intercept
// Follows instructions for intercepting the black line from pad 8
// Updates display following instructions from Project 10
//
//-----------------------------------------------------------------
void BlackLineIntercept(void){
    if(!BLStart){
        BLStart++;
        BLState = START;
        timer10s = 0;
    }

    switch(BLState){
        case START:
            if(timer10s < 600){
                P6OUT  |= LCD_BACKLITE;
                strcpy(display_line[0], " BL START ");
                display_changed = TRUE;
                wait = 0;
            } else {
                P6OUT  &= ~LCD_BACKLITE;
                if(wait <= 48){         // 41 = 90 deg
                    spin_counterclockwise();
                } else {
                    forward_fast();
                    if(wait >= 350 && (ADC_Left_Det > 600 || ADC_Right_Det > 600)){
                        turn_off_motors();
                        BLState = INTERCEPT;
                        timer10s = 0;
//                        break;
                    }

                }

            }
            break;
        case INTERCEPT:
            strcpy(display_line[0], " INTERCEPT ");
            display_changed = TRUE;
            if(timer10s >= 600){
                P6OUT  &= ~LCD_BACKLITE;
                BLState = TURN;
                wait = 0;
                timer10s = 0;
            } else {
                P6OUT  |= LCD_BACKLITE;
            }

            break;
        case TURN:
            strcpy(display_line[0], " BL TURN  ");
            display_changed = TRUE;
            if(timer10s <= 600){
                P6OUT  |= LCD_BACKLITE;
                wait = 0;
                break;
            } else {
                P6OUT  &= ~LCD_BACKLITE;
            }
            if(wait <= 5){
                spin_counterclockwise();
            } else if(ADC_Right_Det > 600){
                turn_off_motors();
                BLState = TRAVEL;
                timer10s = 0;
                break;
            }
            break;
        case TRAVEL:
            strcpy(display_line[0], " BL TRAVEL");
            display_changed = TRUE;
            if(timer10s < 600){
                P6OUT  |= LCD_BACKLITE;
                break;
            } else {
                P6OUT  &= ~LCD_BACKLITE;
            }
            if(timer10s >= 5000){       // change to circle for display after 40 seconds
                BLState = CIRCLE;
                timer10s = 0;
                turn_off_motors();
                break;
            }
            turn_on_forward();
            if (ADC_Right_Det < 600) BLState = RIGHTTRAVEL;
            else if (ADC_Left_Det < 600) BLState = LEFTTRAVEL;
            break;
        case RIGHTTRAVEL:
            spin_clockwise();
            if (ADC_Right_Det > 600) BLState = TRAVEL;
            break;
        case LEFTTRAVEL:
            spin_counterclockwise();
            if (ADC_Left_Det > 600) BLState = TRAVEL;
            break;
        case CIRCLE:

            strcpy(display_line[0], " BL CIRCLE");
            display_changed = TRUE;
            if(timer10s < 600){
                P6OUT  |= LCD_BACKLITE;
                turn_off_motors();
                break;
            } else {
                P6OUT  &= ~LCD_BACKLITE;
            }


            turn_on_forward();
            if (ADC_Right_Det < 600) BLState = RIGHTCIRCLE;
            else if (ADC_Left_Det < 600) BLState = LEFTCIRCLE;
            break;
        case RIGHTCIRCLE:
            spin_clockwise();
            if (ADC_Right_Det > 600) BLState = CIRCLE;
            break;
        case LEFTCIRCLE:
            spin_counterclockwise();
            if (ADC_Left_Det > 600) BLState = CIRCLE;
            break;
        case EXIT:
            if(wait < 600){
                turn_off_motors();
                P6OUT |= LCD_BACKLITE;
                strcpy(display_line[0], " BL EXIT  ");
                display_changed = TRUE;
                break;
            } else {
                P6OUT &= ~LCD_BACKLITE;
            }

            if(wait <= 633){
                spin_clockwise();
                break;
            }
            if(wait <= 1000){
                forward_fast();
                break;
            }

            BLState = DONE;
            break;


        case DONE:
            turn_off_motors();
            P6OUT  |= LCD_BACKLITE;
            strcpy(display_line[0], " BL STOP  ");
            strcpy(display_line[1], " FINISHED ");
            strcpy(display_line[2], " COURSE   ");
            strcpy(display_line[3], "TIME:     ");
            HEXtoBCD(secondsCounter);
            adc_line(4,6);
            display_line[3][9] = 's';
            display_changed = TRUE;
            timer_start = 0;
            BLState = NONE;
            break;

        default:
            break;


    }

}
