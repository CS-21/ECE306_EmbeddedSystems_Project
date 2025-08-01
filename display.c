/*
 * display.c
 *
 *  Created on: January 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains the Display Process function, which updates the
 *  LCD display when changes occur. It checks if an update is required
 *  and calls Display_Update accordingly to refresh the display content.
 *
 *  Functions included:
 *    - Display_Process: Checks for display update requests and triggers an update.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

extern volatile unsigned char update_display;
extern volatile unsigned char display_changed;

void Display_Process(void){
  if(update_display){
    update_display = 0;
    if(display_changed){
      display_changed = 0;
      Display_Update(0,0,0,0);
    }
  }
}

