/*
 * menu.c
 *
 *  Created on: Apr 16, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains the menu system for the MSP430, allowing selection
 *  between different operation modes based on ADC thumb input. It initializes
 *  the menu display and handles user input from switches to navigate or trigger
 *  actions in each menu section.
 *
 *  Functions included:
 *    - run_menu: Executes actions based on the current menu selection.
 *    - init_menu: Initializes the menu based on ADC thumb value and updates the display.
 *
 */



#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern volatile unsigned char display_changed;
extern unsigned int menu;
extern unsigned int sw1_position;
extern unsigned int sw2_position;
unsigned int ADC_Channel;
unsigned int ADC_Left_Det;
unsigned int ADC_Right_Det;
unsigned int ADC_Thumb;


void run_menu(void){
    switch(menu){
        case CALIBRATE:
            if(sw1_position){
                sw1_position = 0;
                Calibration();

            }
            if(sw2_position){
                sw2_position = 0;

            }

            break;
        case 2:
            if(sw1_position){
                sw1_position = 0;

            }
            if(sw2_position){
                sw2_position = 0;

            }
            break;
        case 3:
            if(sw1_position){
                sw1_position = 0;

            }
            if(sw2_position){
                sw2_position = 0;

            }
            break;
        default:
            break;
    }
}

void init_menu(void){
    if(ADC_Thumb < 340){
        strcpy(display_line[0], "          ");
        strcpy(display_line[1], " CALIBRATE");
        strcpy(display_line[2], " SECTION 2");
        menu = CALIBRATE;
    } else if (ADC_Thumb < 680) {
        strcpy(display_line[0], " CALIBRATE");
        strcpy(display_line[1], " SECTION 2");
        strcpy(display_line[2], " SECTION 3");
        menu = 2;
    } else {
        strcpy(display_line[0], " SECTION 2");
        strcpy(display_line[1], " SECTION 3");
        strcpy(display_line[2], "          ");
        menu = 3;
    }
    lcd_BIG_mid();
    display_changed = TRUE;

}
