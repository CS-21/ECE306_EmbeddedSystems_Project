/*
 * functions.c
 *
 *  Created on: January 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains system configuration settings and utility functions
 *  for enabling interrupts and managing assembly-level operations for the
 *  MSP430 microcontroller.
 *
 *  Functions included:
 *    - enable_interrupts: Enables global interrupts using the bis_SR_register intrinsic.
 *    - (Commented out) READ_AND_DISCARD: Prevents optimization of certain register reads.
 *    - (Commented out) disable_interrupts: Intended to disable global interrupts.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

void enable_interrupts(void);

//------------------------------------------------------------------------------
// System Configurations

// Tells the compiler to provide the value in reg as an input to an
// inline assembly block.  Even though the block contains no instructions,
// the compiler may not optimize it away, and is told that the value
// may change and should not be relied upon.

//inline void READ_AND_DISCARD(unsigned int reg) __attribute__((always_inline));
//void READ_AND_DISCARD(unsigned int reg){
//  asm volatile ("" : "=m" (reg) : "r" (reg));
//}

//inline void enable_interrupts(void) __attribute__((always_inline));
void enable_interrupts(void){
  __bis_SR_register(GIE);     // enable interrupts
//  asm volatile ("eint \n");
}

//inline void disable_interrupts(void) __attribute__((always_inline));
//void disable_interrupts(void){
//  asm volatile ("dint \n");
//}
