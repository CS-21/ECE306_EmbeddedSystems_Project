/*
 * adc.c
 *
 *  Created on: Feb 25, 2025
 *      Author: chinmayshende
 *
 *  Description:
 *  ------------
 *  This file configures and operates the ADC (Analog-to-Digital Converter)
 *  for the MSP430 microcontroller. It initializes the ADC, cycles through
 *  three analog input channels (left detector, right detector, and thumb),
 *  and stores the converted values. The ADC results are scaled and can be
 *  converted to BCD format for display on an LCD.
 *
 *  Functions included:
 *    - Init_ADC: Initializes ADC settings and starts conversion.
 *    - HEXtoBCD: Converts a hexadecimal value to a 4-digit BCD format.
 *    - adc_line: Places BCD digits into the desired line and location on the display.
 *    - ADC_ISR: Handles ADC conversions and cycles through the input channels.
 *
 */


#include  "msp430.h"
#include  <string.h>
#include  "functions.h"
#include  "LCD.h"
#include  "ports.h"
#include "macros.h"

unsigned int ADC_Channel;
unsigned int ADC_Left_Det;
unsigned int ADC_Right_Det;
unsigned int ADC_Thumb;

char display_line[4][11];
volatile unsigned char display_changed;
unsigned int adc_char[4];

void HEXtoBCD(int hex_value){
     int value;
     int i;
     for(i=0; i < 4; i++) {
         adc_char[i] = '0' ;
    }
     while (hex_value > 999){
         hex_value = hex_value - 1000;
         value = value + 1;
         adc_char[0] = 0x30 + value;
     }
     value = 0;
     while (hex_value > 99){
         hex_value = hex_value - 100;
         value = value + 1;
         adc_char[1] = 0x30 + value;
     }
     value = 0;
     while (hex_value > 9){
         hex_value = hex_value - 10;
         value = value + 1;
         adc_char[2] = 0x30 + value;
     }
     adc_char[3] = 0x30 + hex_value;
}
//-----------------------------------------------------------------

//-------------------------------------------------------------
// ADC Line insert
// Take the HEX to BCD value in the array adc_char and place it
// in the desired location on the desired line of the display.
// char line => Specifies the line 1 thru 4
// char location => Is the location 0 thru 9
//
//-------------------------------------------------------------
void adc_line(char line, char location){
//-------------------------------------------------------------
     int i;
     unsigned int real_line;
     real_line = line - 1;
     for(i=0; i < 4; i++) {
         display_line[real_line][i+location] = adc_char[i];
     }
     display_changed = 1;
}
//-------------------------------------------------------------



void Init_ADC(void) {
  //------------------------------------------------------------------------------
  // V_DETECT_L (0x04) // Pin 2 A2
  // V_DETECT_R (0x08) // Pin 3 A3
  // V_THUMB    (0x20) // Pin 5 A5
  //------------------------------------------------------------------------------

  // ADCCTL0 Register
  ADCCTL0 = 0;                  // Reset
  ADCCTL0 |= ADCSHT_2;         // 16 ADC clocks
  ADCCTL0 |= ADCMSC;           // MSC
  ADCCTL0 |= ADCON;            // ADC ON

  // ADCCTL1 Register
  ADCCTL1 = 0;                 // Reset
  ADCCTL1 |= ADCSHS_0;         // 00b = ADCSC bit
  ADCCTL1 |= ADCSHP;           // ADC sample-and-hold SAMPCON signal from sampling timer.
  ADCCTL1 &= ~ADCISSH;         // ADC invert signal sample-and-hold.
  ADCCTL1 |= ADCDIV_0;         // ADC clock divider - 000b = Divide by 1
  ADCCTL1 |= ADCSSEL_0;        // ADC clock MODCLK
  ADCCTL1 |= ADCCONSEQ_0;      // ADC conversion sequence 00b = Single-channel single-conversion
  // ADCCTL1 & ADCBUSY identifies a conversion is in process

  // ADCCTL2 Register
  ADCCTL2 = 0;                 // Reset
  ADCCTL2 |= ADCPDIV0;         // ADC pre-divider 00b = Pre-divide by 1
  ADCCTL2 |= ADCRES_2;         // ADC resolution 10b = 12 bit (14 clock cycle conversion time)
  ADCCTL2 &= ~ADCDF;           // ADC data read-back format 0b = Binary unsigned.
  ADCCTL2 &= ~ADCSR;           // ADC sampling rate 0b = ADC buffer supports up to 200 ksps

  // ADCMCTL0 Register
  ADCMCTL0 |= ADCSREF_0;       // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
  ADCMCTL0 |= ADCINCH_5;       // V_THUMB (0x20) Pin 5 A5

  ADCIE |= ADCIE0;             // Enable ADC conv complete interrupt
  ADCCTL0 |= ADCENC;           // ADC enable conversion.
  ADCCTL0 |= ADCSC;            // ADC start conversion.
}

#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void) {
  switch(__even_in_range(ADCIV, ADCIV_ADCIFG)) {
    case ADCIV_NONE:
      break;

    case ADCIV_ADCOVIFG:       // When a conversion result is written to the ADCMEM0
                               // before its previous conversion result was read.
      break;

    case ADCIV_ADCTOVIFG:      // ADC conversion-time overflow
      break;

    case ADCIV_ADCHIIFG:       // Window comparator interrupt flags
      break;

    case ADCIV_ADCLOIFG:       // Window comparator interrupt flag
      break;

    case ADCIV_ADCINIFG:       // Window comparator interrupt flag
      break;

    case ADCIV_ADCIFG:         // ADCMEM0 memory register with the conversion result
      ADCCTL0 &= ~ADCENC;      // Disable ENC bit.

      switch (ADC_Channel++) {
        case 0x00: {
          ADC_Left_Det = ADCMEM0;           // Move result into Global
          ADC_Left_Det = ADC_Left_Det >> 2; // Divide the result by 4

          ADCMCTL0 &= ~ADCINCH_2;              // Disable Last channel A2
          ADCMCTL0 |= ADCINCH_3;               // Enable Next channel A3
        } break;

        case 0x01: {                            // ADC_Right_Det
          ADC_Right_Det = ADCMEM0;          // Move result into Global
          ADC_Right_Det = ADC_Right_Det >> 2; // Divide the result by 4

          ADCMCTL0 &= ~ADCINCH_3;              // Disable Last channel A2
          ADCMCTL0 |= ADCINCH_5;               // Enable Next channel A3
        } break;

        case 0x02: {
          ADC_Thumb = ADCMEM0;                 // Move result into Global
          ADC_Thumb = ADC_Thumb >> 2;          // Divide the result by 4

          ADCMCTL0 &= ~ADCINCH_5;              // Disable Last channel A2
          ADCMCTL0 |= ADCINCH_2;               // Enable Next channel A3

          ADC_Channel = 0;                     // Reset channel index
          // Do not start the next sample
        } break;

        default:
          break;
      }

      ADCCTL0 |= ADCENC; // Enable Conversions
      break;

    default:
      break;
  }
}


