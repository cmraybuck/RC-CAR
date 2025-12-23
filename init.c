/*
 * init.c
 * Author: Micah Raybuck
 * This file contains all functions relating to initializing required conditions
 * for proper program functionality.
 */

#include  "msp430.h"
#include  <string.h>
#include  "Include/functions.h"
#include  "Include/LCD.h"
#include "Include/macros.h"
#include  "Include/ports.h"



void Init_Conditions(void){
//------------------------------------------------------------------------------

  int i;
  for(i=0;i<11;i++){ // Reset all display line values
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

void Init_LEDs(void){
//------------------------------------------------------------------------------
// LED Configurations
//------------------------------------------------------------------------------
// Turns on both LEDs
  P1OUT &= ~RED_LED;
  P6OUT &= ~GRN_LED;
//------------------------------------------------------------------------------
}

void Init_ADC(void){
    ADCCTL0 = 0;                      // Reset
    ADCCTL0 |= ADCSHT_2;              // 16 ADC clocks
    ADCCTL0 |= ADCMSC;                // Multiple sample conversion
    ADCCTL0 |= ADCON;                 // Turn ADC ON

    ADCCTL1 = 0;                      // Reset
    ADCCTL1 |= ADCSHS_0;              // Sample-and-hold source = ADCSC bit
    ADCCTL1 |= ADCSHP;                // SAMPCON signal from sampling timer
    ADCCTL1 &= ~ADCISSH;              // Do not invert sample-and-hold
    ADCCTL1 |= ADCDIV_0;              // Clock divider = 1
    ADCCTL1 |= ADCSSEL_0;             // ADC clock source = MODCLK
    ADCCTL1 |= ADCCONSEQ_0;           // Single-channel, single-conversion mode

    ADCCTL2 = 0;                      // Reset
    ADCCTL2 |= ADCPDIV0;              // Pre-divide by 1
    ADCCTL2 |= ADCRES_2;              // 12-bit resolution
    ADCCTL2 &= ~ADCDF;                // Binary unsigned format
    ADCCTL2 &= ~ADCSR;                // 200 ksps mode

    ADCMCTL0 = 0;
    ADCMCTL0 |= ADCSREF_0;            // VR+ = AVCC, VR- = AVSS
    ADCMCTL0 |= ADCINCH_2;            // Start with channel A2 (V_DETECT_L)

    ADCIE |= ADCIE0;                  // Enable ADC conversion complete interrupt
    ADCCTL0 |= ADCENC;                // Enable ADC conversions
    ADCCTL0 |= ADCSC;                 // Start first conversion



}

void Init_DAC(void) {
    int DAC_data = 4000; // Value between 0x000 and 0x0FFF
    SAC3DAT = DAC_data; // Initial DAC data
    SAC3DAC = DACSREF_1; // Select int Vref as DAC reference
    SAC3DAC |= DACLSEL_0; // DAC latch loads when DACDAT written
    // SAC3DAC |= DACIE; // generate an interrupt
    SAC3DAC |= DACEN; // Enable DAC
    SAC3OA = NMUXEN; // SAC Negative input MUX controL
    SAC3OA |= PMUXEN; // SAC Positive input MUX control
    SAC3OA |= PSEL_1; // 12-bit reference DAC source selected
    SAC3OA |= NSEL_1; // Select negative pin input
    SAC3OA |= OAPM; // Select low speed and low power mode
    SAC3PGA = MSEL_1; // Set OA as buffer mode
    SAC3OA |= SACEN; // Enable SAC
    SAC3OA |= OAEN; // Enable OA
}

void Init_Serial_UCA0(char speed) {
    //------------------------------------------------------------------------------
    // TX error (%) RX error (%)
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
    // 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
    // 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
    // 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
    //------------------------------------------------------------------------------
    // Configure eUSCI_A0 for UART mode
    UCA0CTLW0 = 0;
    UCA0CTLW0 |= UCSWRST ; // Put eUSCI in reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
    UCA0CTLW0 &= ~UCMSB; // MSB, LSB select
    UCA0CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA0CTLW0 &= ~UCPEN; // No Parity
    UCA0CTLW0 &= ~UCSYNC;
    UCA0CTLW0 &= ~UC7BIT;
    UCA0CTLW0 |= UCMODE_0;
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // UCA?MCTLW = UCSx + UCFx + UCOS16
    UCA0BRW = 4; // 115,200 baud
    UCA0MCTLW = 0x5551 ;
    UCA0CTLW0 &= ~UCSWRST ; // release from reset
    UCA0TXBUF = 0x00; // Prime the Pump
    UCA0IE |= UCRXIE; // Enable RX interrupt
    baud_rate = 0;
    //------------------------------------------------------------------------------
}

void Init_Serial_UCA1(char speed) {
    //------------------------------------------------------------------------------
    // TX error (%) RX error (%)
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 4800 1 104 2 0xD6 -0.08 0.04 -0.10 0.14
    // 8000000 9600 1 52 1 0x49 -0.08 0.04 -0.10 0.14
    // 8000000 19200 1 26 0 0xB6 -0.08 0.16 -0.28 0.20
    // 8000000 57600 1 8 10 0xF7 -0.32 0.32 -1.00 0.36
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // 8000000 460800 0 17 0 0x4A -2.72 2.56 -3.76 7.28
    //------------------------------------------------------------------------------
    // Configure eUSCI_A0 for UART mode
    //pc_power_on = 0;
    UCA1CTLW0 = 0;
    UCA1CTLW0 |= UCSWRST ; // Put eUSCI in reset
    UCA1CTLW0 |= UCSSEL__SMCLK; // Set SMCLK as fBRCLK
    UCA1CTLW0 &= ~UCMSB; // MSB, LSB select
    UCA1CTLW0 &= ~UCSPB; // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
    UCA1CTLW0 &= ~UCPEN; // No Parity
    UCA1CTLW0 &= ~UCSYNC;
    UCA1CTLW0 &= ~UC7BIT;
    UCA1CTLW0 |= UCMODE_0;
    // BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
    // 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
    // UCA?MCTLW = UCSx + UCFx + UCOS16
    UCA1BRW = 4; // 115,200 baud
    UCA1MCTLW = 0x5551 ;
    UCA1CTLW0 &= ~UCSWRST ; // release from reset
    UCA1TXBUF = 0x00; // Prime the Pump
    UCA1IE |= UCRXIE; // Enable RX interrupt
    baud_rate = 0;
    //------------------------------------------------------------------------------
}




void USCI_A0_transmit(void){ // Transmit Function for USCI_A0
    pb_index = 0; // Set Array index to first location
    UCA0IE |= UCTXIE; // Enable TX interrupt
}

void USCI_A1_transmit(void){ // Transmit Function for USCI_A0
    pb_index = 0; // Set Array index to first location
    UCA1IE |= UCTXIE; // Enable TX interrupt
}



void SetBaudRateOneFifteen(void) { // Sets necessary values to set PC baud rate to 115,200
    UCA0BRW = 4;
    UCA1BRW = 4;
    UCA0MCTLW = 0x5551;
    UCA1MCTLW = 0x5551;

}

void SetBaudRateNineSix(void) { // Sets necessary values to set PC baud rate to 9,600
    UCA0BRW = 52;
    UCA1BRW = 52;
    UCA0MCTLW = 0x4911;
    UCA1MCTLW = 0x4911;

}












