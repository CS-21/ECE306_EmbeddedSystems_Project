/*
 * main.c
 *
 *  Created on: January 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains the main routine ("while" operating system) for the
 *  MSP430 system. It initializes peripherals, clears the display, and
 *  manages the main control flow using state machines, periodic updates,
 *  and IOT command handling.
 *
 *  Functions included:
 *    - main: Initializes the system and runs the main control loop.
 *
 */


#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

// Function Prototypes
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);
void Carlson_StateMachine(void);

// Global Variables
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
//unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;

unsigned int test_value;
extern char chosen_direction;
extern char change;
extern volatile unsigned int start_instruction;
volatile unsigned int wheel_move;
volatile unsigned char forward;
volatile unsigned char event;
volatile unsigned char choice;
volatile unsigned char state;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int IR;

extern volatile unsigned int time_change;
extern volatile unsigned int instruction;
extern volatile unsigned int Backlite;
extern volatile unsigned int count_debounce_SW1;
extern volatile unsigned int count_debounce_SW2;
extern volatile unsigned int sw1_position;
extern volatile unsigned int sw2_position;
extern unsigned int ADC_Channel;
extern unsigned int ADC_Left_Det;
extern unsigned int ADC_Right_Det;
extern unsigned int ADC_Thumb;
extern unsigned int p6;
volatile unsigned int proj7timer;
volatile unsigned int proj7timer2;
volatile unsigned int proj7timerdisplay;
unsigned int calibration;
unsigned int avg;

volatile unsigned int proj8timer;
volatile unsigned int proj8display;
char NCSUArray [9];
extern char usb_tx_buf [160];
extern char usb_rx_buf [160];
char process_buf [11];
extern unsigned int usb_tx;
extern unsigned int usb_rx;
unsigned int transmit;


extern unsigned int iot_tx;
extern unsigned int iot_rx;
extern char iot_rx_buf [160];
extern char iot_tx_buf [160];
unsigned int process_buf_ptr;
unsigned int read_ptr;

unsigned int clear_usb_rx;
unsigned int clear_iot_rx;
unsigned int clear_process;

unsigned int iot_boot_timer;
unsigned int cmdFram;
unsigned int iot_init_cmd;

extern unsigned int ssid_ptr;
extern unsigned int ip_ptr;

extern char iotState;
extern unsigned int bigCmd_ptr;
extern unsigned int smCmd_ptr;
extern unsigned int startCaret;

char movement;
unsigned int timeLength;
extern unsigned int padNum;

extern volatile unsigned int tenmsCounter;
unsigned int secondsCounter;
unsigned int timer_start;

extern unsigned int BLStart;

void main(void) {
//    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // Enable GPIOs

    // Initialize System
    Init_Ports();
    Init_Clocks();
    Init_Conditions();
    Init_Timers();
    Init_LCD();
    Init_ADC();
    Init_Serial();


    // Clear Display
    strcpy(display_line[0], "          ");
    strcpy(display_line[1], "          ");
    strcpy(display_line[2], "          ");
    strcpy(display_line[3], "          ");
    display_changed = TRUE;
    state = WAIT;
    proj8timer = 0;
    UCA0IE |= UCRXIE;
    read_ptr = 0;
    transmit = 0;

    iot_boot_timer = 0;
    iotState = STORE;
    ssid_ptr = 0;
    ip_ptr = 0;
    bigCmd_ptr = 0;
    smCmd_ptr = 0;
    startCaret = 0;
    movement = NONE;
    timeLength = 0;
    padNum = 0;
    timer_start = 0;
    tenmsCounter = 0;
    secondsCounter = 0;
    BLStart = 0;

    // Main Loop
    while(ALWAYS) {                      // Can the Operating system runs

        bootIOT();
        movement_machine();

        if(!timer_start){
            tenmsCounter = 0;
        } else {
            if(tenmsCounter > 9){
                secondsCounter++;
                tenmsCounter = 0;

                HEXtoBCD(secondsCounter);
                adc_line(4,6);
                display_line[3][9] = 's';
                display_changed = TRUE;
            }
        }




        if(iot_boot_timer >= 15){
            P3OUT |= IOT_EN;
            P3OUT |= IOT_LINK_GRN;
        }

        Display_Process();                 // Update Display
        P3OUT ^= TEST_PROBE;               // Change State of TEST_PROBE OFF
    }
  }
