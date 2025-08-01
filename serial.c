/*
 * serial.c
 *
 *  Created on: Mar 25, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file contains functions to initialize and manage serial communication
 *  using the MSP430's USCI_A0 and USCI_A1 modules. It sets up UART communication
 *  for both the USB and IOT interfaces, handles transmission and reception of data,
 *  and includes interrupt service routines for both modules.
 *
 *  Functions included:
 *    - USCI_A0_transmit: Starts a transmission on UCA0.
 *    - Init_Serial: Initializes both UCA0 and UCA1 serial modules.
 *    - Init_Serial_UCA0: Configures UCA0 for UART communication.
 *    - Init_Serial_UCA1: Configures UCA1 for UART communication.
 *    - eUSCI_A0_ISR: Interrupt service routine for UCA0 RX and TX.
 *    - eUSCI_A1_ISR: Interrupt service routine for UCA1 RX and TX.
 *
 */


#include "msp430.h"
#include <string.h>
#include "functions.h"
#include "LCD.h"
#include "ports.h"
#include "macros.h"

unsigned int usb_tx;
unsigned int usb_rx;
char usb_tx_buf[160];
char usb_rx_buf[160];
unsigned int iot_tx;
unsigned int iot_rx;
char iot_tx_buf[160];
char iot_rx_buf[160];

extern unsigned int cmdFram;

// Global Variables
// Size for appropriate Command Length
// Index for process_buffer
void USCI_A0_transmit(void) {
    // Transmit Function for USCI_A0
    // Contents must be in process_buffer
    // End of Transmission is identified by NULL character in process_buffer
    // process_buffer includes Carriage Return and Line Feed
    UCA0IE |= UCTXIE; // Enable TX interrupt
}

void Init_Serial(void) {
    Init_Serial_UCA1('0');
    Init_Serial_UCA0('0');
}

void Init_Serial_UCA0(char speed) {
    UCA0CTLW0 = 0;
    UCA0CTLW0 |= UCSWRST;
    UCA0CTLW0 |= UCSSEL__SMCLK;
    UCA0CTLW0 &= ~UCMSB;
    UCA0CTLW0 &= ~UCSPB;
    UCA0CTLW0 &= ~UCPEN;
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |= UCMODE_0;

    UCA0BRW = 4; // 115200 baud
    UCA0MCTLW = 0x5551;

    UCA0CTLW0 &= ~UCSWRST;
    UCA0IE |= UCRXIE;
}

void Init_Serial_UCA1(char speed) {
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST;
    UCA1CTLW0 |= UCSSEL__SMCLK;
    UCA1CTLW0 &= ~UCMSB;
    UCA1CTLW0 &= ~UCSPB;
    UCA1CTLW0 &= ~UCPEN;
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;

    UCA1BRW = 4; // 115200 baud
    UCA1MCTLW = 0x5551;

    UCA1CTLW0 &= ~UCSWRST;
    UCA1IE |= UCRXIE;
}

#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void) {
    char iot_receive;

    switch (__even_in_range(UCA0IV, 0x08)) {
        case 0:
            break;
        case 2:
            iot_receive = UCA0RXBUF;
            iot_rx_buf[iot_rx++] = iot_receive;
            if (iot_rx >= sizeof(iot_rx_buf)) {
                iot_rx = BEGINNING;
            }
            UCA1TXBUF = iot_receive;
            break;
        case 4:
            if (iot_tx_buf[iot_tx] == '\n') {
                UCA0TXBUF = iot_tx_buf[iot_tx];
                iot_tx_buf[iot_tx] = 0x00;
                UCA0IE &= ~UCTXIE;
                iot_tx = 0;
                break;
            } else {
                UCA0TXBUF = iot_tx_buf[iot_tx];
                iot_tx_buf[iot_tx] = 0x00;
            }
            iot_tx++;
            break;
        default:
            break;
    }
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void) {
    char usb_receive;

    switch (__even_in_range(UCA1IV, 0x08)) {
        case 0:
            break;
        case 2:
            usb_receive = UCA1RXBUF;
            usb_rx_buf[usb_rx++] = usb_receive;
            if (usb_rx >= sizeof(usb_rx_buf)) {
                usb_rx = BEGINNING;
            }
            UCA0TXBUF = usb_receive;
            break;
        case 4:
            if (usb_tx_buf[usb_tx] == '\n') {
                UCA1TXBUF = usb_tx_buf[usb_tx];
                usb_tx_buf[usb_tx] = 0x00;
                UCA1IE &= ~UCTXIE;
                usb_tx = 0;
                break;
            } else {
                UCA1TXBUF = usb_tx_buf[usb_tx];
                usb_tx_buf[usb_tx] = 0x00;
            }
            usb_tx++;
            break;
        default:
            break;
    }
}



