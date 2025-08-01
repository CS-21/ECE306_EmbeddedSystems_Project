/*
 * led.c
 *
 *  Created on: January 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains the LED initialization function, which configures
 *  and turns on both LEDs connected to the microcontroller.
 *
 *  Functions included:
 *    - Init_LEDs: Initializes and turns on both LEDs.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"


void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}
