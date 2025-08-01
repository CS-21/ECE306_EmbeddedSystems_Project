/*
 * commands.c
 *
 *  Created on: Apr 3, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains the logic for handling IOT boot commands, parsing
 *  incoming IOT data, and executing movement commands. It processes SSID
 *  and IP information from the IOT module and interprets special command
 *  sequences to control the robot’s movement and display output.
 *
 *  Functions included:
 *    - bootIOT: Manages the IOT boot process and extracts SSID and IP info.
 *    - iot_commands: Parses incoming IOT commands and sets system actions.
 *    - movement_machine: Executes robot movement based on the current command.
 *
 */


#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

extern volatile unsigned char display_changed;
extern char display_line[4][11];

extern unsigned int iot_tx;
extern unsigned int iot_rx;
extern char iot_rx_buf [160];
extern char iot_tx_buf [160];
extern unsigned int usb_tx;
extern unsigned int usb_rx;
extern char usb_rx_buf [160];
extern char usb_tx_buf [160];
extern char process_buf [4][40];
extern unsigned int process_buf_ptr1;
extern unsigned int process_buf_ptr2;
extern unsigned int read_ptr;
extern unsigned int cmdFram;

char iotState;
unsigned int startSSID;
char ssidName [20];
unsigned int checkInput;
unsigned int ssid_ptr;
extern unsigned int iot_boot_timer;
unsigned int startIP;
char ipName[20];
unsigned int ip_ptr;

char iotCmd[3][15];
unsigned int bigCmd_ptr;
unsigned int smCmd_ptr;
unsigned int startCaret;
unsigned int time;

char movement;
unsigned int timeLength;

unsigned int startMove;

unsigned int padNum;

unsigned int timer_start;
volatile unsigned int wait;

extern char BLState;


void bootIOT(void){
    if(iot_boot_timer >= 1500){
        iot_commands();
        timer_start = 1;
        P2OUT |= IR_LED;
    } else if(iotState == WAITIP && iot_boot_timer >= 1100){
        strcpy(iot_tx_buf,"AT+CIFSR\r\n");
        UCA0IE |= UCTXIE;
        iotState = IP;
    } else if(iotState == WAITSSID && iot_boot_timer >= 900){
        strcpy(iot_tx_buf, "AT+CWJAP?\r\n");
        UCA0IE |= UCTXIE;
        iotState = SSID;
    } else if(iotState == SERVER && iot_boot_timer >= 700){
        strcpy(iot_tx_buf, "AT+CIPSERVER=1,22222\r\n");
        UCA0IE |= UCTXIE;
        iotState = WAITSSID;
    } else if(iotState == MUX && iot_boot_timer >= 500){
        strcpy(iot_tx_buf, "AT+CIPMUX=1\r\n");
        UCA0IE |= UCTXIE;
        iotState = SERVER;
    } else if(iotState == STORE && iot_boot_timer >= 300){
        strcpy(iot_tx_buf, "AT+SYSSTORE=0\r\n");
        UCA0IE |= UCTXIE;
        iotState = MUX;
    }

    switch(iotState){
        case SSID:
            if(read_ptr != iot_rx){
                if(!startSSID && iot_rx_buf[read_ptr++] == '"'){
                    startSSID = 1;

                } else if(startSSID == 1){
                    if(iot_rx_buf[read_ptr] != '"'){
                        ssidName[ssid_ptr++] = iot_rx_buf[read_ptr++];
                    } else {
                        startSSID = 2;
                    }
                }
            }

            if(read_ptr >= sizeof(iot_rx_buf)){
                read_ptr = BEGINNING;
            }


            if(startSSID == 2){
                strncpy(display_line[0],ssidName,10);
                display_changed = TRUE;
                iotState = WAITIP;
            }

            break;
        case IP:
            if(read_ptr != iot_rx){
                if(startIP <= 3 && iot_rx_buf[read_ptr++] == '"'){
                    startIP++;

                } else if(startIP == 4){
                    if(iot_rx_buf[read_ptr] != '"'){
                        ipName[ip_ptr++] = iot_rx_buf[read_ptr++];
                    } else {
                        startIP = 5;
                    }
                }
            }

            if(read_ptr >= sizeof(iot_rx_buf)){
                read_ptr = BEGINNING;
            }


            if(startIP == 5){
                strcpy(display_line[1], "    IP    ");
                strncpy(display_line[2],ipName, 10);
                display_line[3][0] = ipName[10];
                display_line[3][1] = ipName[11];
                display_line[3][2] = ipName[12];
                display_line[3][3] = ipName[13];
                display_line[3][4] = ipName[14];
                display_changed = TRUE;
                iotState = NONE;
            }
            break;
        default:
            break;

    }
}


//-----------------------------------------------------------------
// Command logic sent from Magic Smoke
// Parse commands sent from IOT, looking for a '^' to signal a valid command
// Switch statement to determine what each command should do
//
//-----------------------------------------------------------------

void iot_commands(void){

    if(read_ptr != iot_rx){
        if(!startCaret && iot_rx_buf[read_ptr++] == '^'){
            startCaret = 1;
        } else if(startCaret == 1){
            if(iot_rx_buf[read_ptr] != '\r'){
                iotCmd[bigCmd_ptr][smCmd_ptr++] = iot_rx_buf[read_ptr++];
            } else {
                startCaret = 2;
            }
        }
    }
    if(startCaret == 2){
        if(iotCmd[bigCmd_ptr][0] == '0' && iotCmd[bigCmd_ptr][1] == '0' && iotCmd[bigCmd_ptr][2] == '0' && iotCmd[bigCmd_ptr][3] == '0'){

            if(iotCmd[bigCmd_ptr][4] == 'F'){
                P6OUT &= ~LCD_BACKLITE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[3], "          ");
                strcpy(display_line[3], &iotCmd[bigCmd_ptr][4]);
                display_changed = TRUE;
                movement = FORWARD;
                timeLength = (iotCmd[bigCmd_ptr][5] - 0x30) * 25;
                time = 0;
            } else if(iotCmd[bigCmd_ptr][4] == 'B'){
                P6OUT &= ~LCD_BACKLITE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[3], "          ");
                strcpy(display_line[3], &iotCmd[bigCmd_ptr][4]);
                display_changed = TRUE;
                movement = BACKWARD;
                timeLength = (iotCmd[bigCmd_ptr][5] - 0x30) * 25;
                time = 0;
            } else if(iotCmd[bigCmd_ptr][4] == 'R'){
                P6OUT &= ~LCD_BACKLITE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[3], "          ");
                strcpy(display_line[3], &iotCmd[bigCmd_ptr][4]);
                display_changed = TRUE;
                movement = RIGHT;
                timeLength = (iotCmd[bigCmd_ptr][5] - 0x30) * 20;
                time = 0;
            } else if(iotCmd[bigCmd_ptr][4] == 'L'){
                P6OUT &= ~LCD_BACKLITE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[3], "          ");
                strcpy(display_line[3], &iotCmd[bigCmd_ptr][4]);
                display_changed = TRUE;
                movement = LEFT;
                timeLength = (iotCmd[bigCmd_ptr][5] - 0x30) * 20;
                time = 0;
            } else if (iotCmd[bigCmd_ptr][4] == 'S'){
                movement = STOP;
            } else if (iotCmd[bigCmd_ptr][4] == 'C'){
                strcpy(display_line[0], "          ");
                strcpy(display_line[3], "          ");
                strcpy(display_line[3], &iotCmd[bigCmd_ptr][4]);
                display_changed = TRUE;
                movement = BLACKLINE;
            } else if(iotCmd[bigCmd_ptr][4] == '+'){
                P6OUT |= LCD_BACKLITE;
                strcpy(display_line[0], "ARRIVED 0 ");
                padNum++;
                display_line[0][9] = padNum + 0x30;
                display_changed = TRUE;
            } else if (iotCmd[bigCmd_ptr][4] == 'D'){
                P6OUT &= ~LCD_BACKLITE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[1], "  CHINMAY ");
                strcpy(display_line[2], "  SHENDE  ");
                strcpy(display_line[3], "          ");
                display_changed = TRUE;
            } else if (iotCmd[bigCmd_ptr][4] == 'E'){
                wait = 0;
                BLState = EXIT;
            } else if (iotCmd[bigCmd_ptr][4] == 'P'){
                P6OUT &= ~LCD_BACKLITE;
                strcpy(display_line[0], "          ");
                strcpy(display_line[3], "          ");
                strcpy(display_line[3], &iotCmd[bigCmd_ptr][4]);
                display_changed = TRUE;
                movement = BUMP;
                timeLength = (iotCmd[bigCmd_ptr][5] - 0x30) * 5;
                time = 0;
            }
        }
        if(bigCmd_ptr++ >= 2){
            bigCmd_ptr = 0;
        }
        smCmd_ptr = 0;
        startCaret = 0;
    }
    if(read_ptr >= sizeof(iot_rx_buf)){
        read_ptr = BEGINNING;
    }
    if(smCmd_ptr >= sizeof(iotCmd[bigCmd_ptr])){
        smCmd_ptr = 0;
        if(bigCmd_ptr++ >= 2){
            bigCmd_ptr = 0;
        }
    }
}

void movement_machine(void){
    switch(movement){
        case FORWARD:
            if(time <= timeLength){
                forward_fast();
            } else {
                turn_off_motors();
                movement = NONE;
            }
            break;
        case BACKWARD:
            if(time <= timeLength){
                reverse_fast();
            } else {
                turn_off_motors();
                movement = NONE;
            }
            break;
        case LEFT:
            if(time <= timeLength){
                turn_right();
            } else {
                turn_off_motors();
                movement = NONE;
            }
            break;
        case RIGHT:
            if(time <= timeLength){
               turn_left();
            } else {
                turn_off_motors();
                movement = NONE;
            }
            break;
        case STOP:
            turn_off_motors();
            movement = NONE;
            break;
        case BLACKLINE:
            BlackLineIntercept();
            break;
        case BUMP:
            if(time <= timeLength){
                turn_left();
            } else {
                turn_off_motors();
                movement = NONE;
            }
            break;
        default:
            break;
    }
}
