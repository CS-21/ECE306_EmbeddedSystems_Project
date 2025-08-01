/*
 * init.c
 *
 *  Created on: January 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file initializes system conditions, including LCD display buffers
 *  and enabling interrupts. It resets the display buffers, assigns display
 *  pointers, and ensures interrupts are enabled for system operation.
 *
 *  Functions included:
 *    - Init_Conditions: Resets display buffers, sets display pointers,
 *                       and enables interrupts.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char update_display;

void Init_Conditions(void){
//------------------------------------------------------------------------------

  int i;
  for(i=0;i<11;i++){
    display_line[0][i] = RESET_STATE;
    display_line[1][i] = RESET_STATE;
    display_line[2][i] = RESET_STATE;
    display_line[3][i] = RESET_STATE;
  }
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  display[0] = &display_line[0][0];
  display[1] = &display_line[1][0];
  display[2] = &display_line[2][0];
  display[3] = &display_line[3][0];
  update_display = 0;


// Interrupts are disabled by default, enable them.
  enable_interrupts();
//------------------------------------------------------------------------------
}

