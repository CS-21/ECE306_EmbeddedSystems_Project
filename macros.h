/*
 * macros.h
 *
 *  Created on: Jan 30, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains macro definitions used throughout the program,
 *  including constants for states, timers, motor speeds, switch logic,
 *  ADC configurations, and other control values for the MSP430 system.
 *
 *  Functions included:
 *    - None (this file contains only macro definitions).
 *
 */


#ifndef MACROS_H_
#define MACROS_H_
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0
#define GRN_LED              (0x40) // GREEN LED 1
#define TEST_PROBE           (0x01) // 0 TEST PROBE
#define TRUE                 (0x01) //

#define P4PUD (P4OUT)

// STATES ======================================================================
#define NONE ('N')
#define STRAIGHT ('S')
#define TRIANGLE ('T')
#define FIGURE_EIGHT ('F')
#define TIMER ('M')
#define CIRCLE ('C')
#define WAIT ('W')
#define START ('S')
#define RUN ('R')
#define END ('E')

#define SSID ('S')
#define IP ('I')
#define SERVER ('E')
#define MUX ('M')
#define STORE ('T')
#define WAITSSID ('D')
#define WAITIP ('P')

#define FORWARD ('F')
#define BACKWARD ('B')
#define RIGHT ('R')
#define LEFT ('L')
#define STOP ('S')
#define BLACKLINE ('C')
#define BUMP ('P')

#define WHEEL_COUNT_TIME (20)

#define RIGHT_COUNT_TIME (20)
#define LEFT_COUNT_TIME (15)

#define RIGHT_COUNT_TIME_CIRCLE (10)
#define LEFT_COUNT_TIME_CIRCLE (3)

#define RIGHT_COUNT_TIME_FIGURE_EIGHT_1 (10)
#define LEFT_COUNT_TIME_FIGURE_EIGHT_1 (3)
#define RIGHT_COUNT_TIME_FIGURE_EIGHT_2 (1)
#define LEFT_COUNT_TIME_FIGURE_EIGHT_2 (25)
#define RIGHT_COUNT_TIME_FIGURE_EIGHT_3 (10)
#define LEFT_COUNT_TIME_FIGURE_EIGHT_3 (3)
#define RIGHT_COUNT_TIME_FIGURE_EIGHT_4 (1)
#define LEFT_COUNT_TIME_FIGURE_EIGHT_4 (25)

#define RIGHT_COUNT_TIME_TRIANGLE_F (10)
#define LEFT_COUNT_TIME_TRIANGLE_F (10)
#define RIGHT_COUNT_TIME_TRIANGLE_T (10)


#define TRAVEL_DISTANCE (30)
#define TRAVEL_DISTANCE_CIRCLE (150)
#define TRAVEL_DISTANCE_TRIANGLE (10)
#define TRAVEL_DISTANCE_FIGURE_EIGHT_1 (44)
#define TRAVEL_DISTANCE_FIGURE_EIGHT_2 (87)
#define TRAVEL_DISTANCE_FIGURE_EIGHT_3 (130)
#define TRAVEL_DISTANCE_FIGURE_EIGHT_4 (173)

#define WAITING2START (200)

// Switches
#define RELEASED (1)
#define PRESSED (0)
#define NOT_OKAY (0)
#define OKAY (1)
#define DEBOUNCE_RESTART (0)
#define SW1DEBOUNCE (50000)
#define SW2DEBOUNCE (50000)

// Timers

#define TB0CCR0_INTERVAL (5000)
#define TB0CCR1_INTERVAL (50000)
#define TB0CCR2_INTERVAL (50000)

//Project 6
#define PROJ6 ('6')
#define DETECT ('D')

#define PROJ7 ('7')

//PWM
#define SLOW (10000)
#define FAST (20000)
#define MEDIUM (15000)
#define LEFT_WHEEL_SLOW (8000)
#define RIGHT_WHEEL_SLOW (8000)
#define PERCENT_100 (50000)
#define PERCENT_80 (45000)

#define BEGINNING (0x00)

#define CALIBRATE (0x01)

// #define START ('S')
#define INTERCEPT ('I')
#define TURN ('T')
#define TRAVEL ('Y')
// #define CIRCLE ('C')
#define EXIT ('E')
#define DONE ('D')

#define LEFTCIRCLE ('W')
#define RIGHTCIRCLE ('X')
#define LEFTTRAVEL ('!')
#define RIGHTTRAVEL ('Z')



#endif /* MACROS_H_ */
