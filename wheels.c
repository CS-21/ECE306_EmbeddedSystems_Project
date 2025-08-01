/*
 * wheels.c
 *
 *  Created on: Jan 31, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains motor control functions for the MSP430, including
 *  movement commands for driving forward, reversing, spinning, and turning.
 *  PWM signals are used to control motor speeds and directions.
 *
 *  Functions included:
 *    - turn_off_motors: Stops all motor movement.
 *    - turn_on_forward: Starts forward motion at slow speed.
 *    - forward_fast: Drives forward at fast speed.
 *    - forward_medium: Drives forward at medium speed.
 *    - turn_on_reverse: Starts reverse motion at slow speed.
 *    - reverse_fast: Drives backward at fast speed.
 *    - spin_clockwise: Spins the robot clockwise.
 *    - spin_clockwise_medium: Spins clockwise at medium speed.
 *    - spin_counterclockwise: Spins the robot counterclockwise.
 *    - turn: Turns in place using differential drive.
 *    - turn_left: Executes a left turn.
 *    - turn_right: Executes a right turn.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"


void turn_off_motors(void){
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

void turn_on_forward(void){
    turn_off_motors(); // Call turn_off function to turn off the motors
    LEFT_FORWARD_SPEED = LEFT_WHEEL_SLOW;
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_SLOW;
}

void forward_fast(void) {
    turn_off_motors();
    RIGHT_FORWARD_SPEED = FAST;
    LEFT_FORWARD_SPEED = MEDIUM;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void forward_medium(void) {
    turn_off_motors();
    RIGHT_FORWARD_SPEED = MEDIUM;
    LEFT_FORWARD_SPEED = MEDIUM;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void turn_on_reverse(void){
    turn_off_motors(); // Call turn_off function to turn off the motors
    LEFT_REVERSE_SPEED = LEFT_WHEEL_SLOW;
    RIGHT_REVERSE_SPEED = RIGHT_WHEEL_SLOW;
}

void reverse_fast(void) {
    turn_off_motors();
    RIGHT_REVERSE_SPEED = FAST;
    LEFT_REVERSE_SPEED = FAST;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}

void spin_clockwise(void){
    turn_off_motors();
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_SLOW;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = LEFT_WHEEL_SLOW;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
}

void spin_clockwise_medium(void){
    turn_off_motors();
    RIGHT_REVERSE_SPEED = MEDIUM;
    LEFT_FORWARD_SPEED = MEDIUM;
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void spin_counterclockwise(void){
    turn_off_motors();
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = RIGHT_WHEEL_SLOW;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = LEFT_WHEEL_SLOW;
}

void turn(void){
    turn_off_motors();
    RIGHT_FORWARD_SPEED = RIGHT_WHEEL_SLOW;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = LEFT_WHEEL_SLOW;
}

void turn_left(void){
    turn_off_motors();
    RIGHT_FORWARD_SPEED = 6000;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}

void turn_right(void){
    turn_off_motors();
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = 6000;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
}



