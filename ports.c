/*
 * ports.c
 * Author: Micah Raybuck
 * This file contains functions that initialize the ports and their pins
 * on the MSP430FR2355.
 */

#include  "msp430.h"
#include  <string.h>
#include  "Include/functions.h"
#include  "Include/LCD.h"
#include "Include/macros.h"
#include  "Include/ports.h"


void Init_Ports(void) { // function to initialize each port
    Init_Port1(); // initialize port 1
    Init_Port2(); // initialize port 2
    Init_Port3(); // initialize port 3
    Init_Port4(); // initialize port 4
    Init_Port5(); // initialize port 5
    Init_Port6(); // initialize port 6
}

void Init_Port1(void) { // Port 1 configuration
    P1OUT = 0x00;
    P1DIR = 0x00;

    P1SEL0 &= ~RED_LED; // RED_LED -> GPIO output
    P1SEL1 &= ~RED_LED;
    P1OUT &= ~RED_LED;
    P1DIR |= RED_LED;

    P1SEL0 &= ~A1_SEEED; // A1_SEEED -> GPIO input
    P1SEL1 &= ~A1_SEEED;
    P1OUT &= ~A1_SEEED;
    P1DIR &= ~A1_SEEED;

    P1SEL0 |= V_DETECT_L; // V_DETECT_L -> Analog input
    P1SEL1 |= V_DETECT_L;
    P1OUT &= ~V_DETECT_L;
    P1DIR &= ~V_DETECT_L;
    P1REN &= ~V_DETECT_L;

    P1SEL0 |= V_DETECT_R; // V_DETECT_R -> Analog input
    P1SEL1 |= V_DETECT_R;
    P1OUT &= ~V_DETECT_R;
    P1DIR &= ~V_DETECT_R;
    P2REN &= ~V_DETECT_R;

    P1SEL0 &= ~A4_SEEED; // A4_SEEED -> GPIO input
    P1SEL1 &= ~A4_SEEED;
    P1OUT &= ~A4_SEEED;
    P1DIR &= ~A4_SEEED;


    P1SEL0 |= V_THUMB; // V_THUMB -> Analog input
    P1SEL1 |= V_THUMB;
    P1OUT &= ~V_THUMB;
    P1DIR &= ~V_THUMB;
    P1REN &= ~V_THUMB;

    P1SEL0 |= UCA0RXD; // UCA0RXD -> Enabled as a function
    P1SEL1 &= ~UCA0RXD;
    P1OUT &= ~UCA0RXD;
    P1DIR &= ~UCA0RXD;

    P1SEL0 |= UCA0TXD; // UCA0TXD -> Enabled as a function
    P1SEL1 &= ~UCA0TXD;
    P1OUT &= ~UCA0TXD;
    P1DIR &= ~UCA0TXD;

}


void Init_Port2(void) { // Port 2 configuration
    P2OUT = 0x00;
    P2DIR = 0x00;

    P2SEL0 &= ~SLOW_CLK; // SLOW_CLK -> GPIO output
    P2SEL1 &= ~SLOW_CLK;
    P2OUT &= ~SLOW_CLK;
    P2DIR |= SLOW_CLK;

    P2SEL0 &= ~CHECK_BAT; // CHECK_BAT -> GPIO output
    P2SEL1 &= ~CHECK_BAT;
    P2OUT &= ~CHECK_BAT;
    P2DIR |= CHECK_BAT;

    P2SEL0 &= ~IR_LED; // IR_LED -> GPIO output initialized to turn on immediately
    P2SEL1 &= ~IR_LED;
    P2OUT |= IR_LED;
    P2DIR |= IR_LED;

    P2SEL0 &= ~SW2; // SW2 -> GPIO input, pull-up resistor AND interrupt enabled
    P2SEL1 &= ~SW2;
    P2OUT |= SW2;
    P2DIR &= ~SW2;
    P2REN |= SW2;
    P2IFG &= ~SW2;
    P2IE |= SW2;
    P2IES |= SW2;

    P2SEL0 &= ~IOT_RUN_CPU; // IOT_RUN_CPU -> GPIO output
    P2SEL1 &= ~IOT_RUN_CPU;
    P2OUT &= ~IOT_RUN_CPU;
    P2DIR |= IOT_RUN_CPU;

    P2SEL0 &= ~DAC_ENB; // DAC_ENB -> GPIO output initialized high
    P2SEL1 &= ~DAC_ENB;
    P2OUT |= DAC_ENB;
    P2DIR |= DAC_ENB;

    P2SEL0 &= ~LFXOUT; // LFXOUT -> SEL0 low, SEL1 high
    P2SEL1 |= LFXOUT;

    P2SEL0 &= ~LFXIN; // LFXIN -> SEL0 low, SEL1 high
    P2SEL1 |= LFXIN;

}

void Init_Port3(void) { // Port 3 configuration
    P3OUT = 0x00;
    P3DIR = 0x00;

    P3SEL0 &= ~TEST_PROBE; // TEST_PROBE -> GPIO input
    P3SEL1 &= ~TEST_PROBE;
    P3OUT &= ~TEST_PROBE;
    P3DIR &= ~TEST_PROBE;

    P3SEL0 &= ~DAC_CTRL_2; // DAC_CTRL_2 -> GPIO input
    P3SEL1 &= ~DAC_CTRL_2;
    P3OUT &= ~DAC_CTRL_2;
    P3DIR &= ~DAC_CTRL_2;

    P3SEL0 &= ~OA2N; // OA2N -> GPIO input
    P3SEL1 &= ~OA2N;
    P3OUT &= ~OA2N;
    P3DIR &= ~OA2N;

    P3SEL0 &= ~OA2P; // OA2P -> GPIO input
    P3SEL1 &= ~OA2P;
    P3OUT &= ~OA2P;
    P3DIR &= ~OA2P;

    P3SEL0 &= ~SMCLK_OUT; // SMCLK_OUT -> GPIO input
    P3SEL1 &= ~SMCLK_OUT;
    P3OUT &= ~SMCLK_OUT;
    P3DIR &= ~SMCLK_OUT;

    P3SEL0 &= ~DAC_CTRL_3; // DAC_CTRL_3 -> GPIO input
    P3SEL1 &= ~DAC_CTRL_3;
    P3OUT &= ~DAC_CTRL_3;
    P3DIR &= ~DAC_CTRL_3;

    P3SEL0 &= ~IOT_LINK_CPU; // IOT_LINK_CPU -> GPIO output
    P3SEL1 &= ~IOT_LINK_CPU;
    P3OUT &= ~IOT_LINK_CPU;
    P3DIR |= IOT_LINK_CPU;

    P3SEL0 &= ~IOT_RN_CPU; // IOT_RN_CPU -> GPIO output
    P3SEL1 &= ~IOT_RN_CPU;
    P3OUT &= ~IOT_RN_CPU;
    P3DIR |= IOT_RN_CPU;
}

void Init_Port4(void) { // Port 4 configuration
    P4OUT = 0x00;
    P4DIR = 0x00;

    P4SEL0 &= ~RESET_LCD; // RESET_LCD -> GPIO output
    P4SEL1 &= ~RESET_LCD;
    P4OUT &= ~RESET_LCD;
    P4DIR |= RESET_LCD;

    P4SEL0 &= ~SW1; // SW1 -> GPIO input, pull-up resistor AND interrupt enabled
    P4SEL1 &= ~SW1;
    P4OUT |= SW1;
    P4DIR &= ~SW1;
    P4REN |= SW1;
    P4IFG &= ~SW1;
    P4IE |= SW1;
    P4IES |= SW1;

    P4SEL0 |= UCA1RXD; // UCA1RXD -> Enabled as a function
    P4SEL1 &= ~UCA1RXD;

    P4SEL0 |= UCA1TXD; // UCA1TXD -> Enabled as a function
    P4SEL1 &= ~UCA1TXD;

    P4SEL0 &= ~UCB1_CS_LCD; // UCB1_CS_LCD -> GPIO output
    P4SEL1 &= ~UCB1_CS_LCD;
    P4OUT |= UCB1_CS_LCD;
    P4DIR |= UCB1_CS_LCD;

    P4SEL0 |= UCB1CLK; // UCB1CLK -> SEL0 high, SEL1 low
    P4SEL1 &= ~UCB1CLK;

    P4SEL0 |= UCB1SIMO; // UCB1SIMO -> SEL0 high, SEL1 low
    P4SEL1 &= ~UCB1SIMO;

    P4SEL0 |= UCB1SOMI; // UCB1SOMI -> SEL0 high, SEL1 low
    P4SEL1 &= ~UCB1SOMI;
}

void Init_Port5(void) { // Port 5 configuration
    P5OUT = 0x00;
    P5DIR = 0x00;

    P5SEL0 &= ~V_BAT; // V_BAT -> GPIO input
    P5SEL1 &= ~V_BAT;
    P5OUT &= ~V_BAT;
    P5DIR &= ~V_BAT;

    P5SEL0 &= ~V_5; // V_5 -> GPIO input
    P5SEL1 &= ~V_5;
    P5OUT &= ~V_5;
    P5DIR &= ~V_5;

    P5SEL0 &= ~V_DAC; // V_DAC -> GPIO input
    P5SEL1 &= ~V_DAC;
    P5OUT &= ~V_DAC;
    P5DIR &= ~V_DAC;

    P5SEL0 &= ~V3_3; // V3_3 -> GPIO input
    P5SEL1 &= ~V3_3;
    P5OUT &= ~V3_3;
    P5DIR &= ~V3_3;

    P5SEL0 &= ~IOT_BOOT_CPU; // IOT_BOOT_CPU -> GPIO output and initialized high
    P5SEL1 &= ~IOT_BOOT_CPU;
    P5OUT |= IOT_BOOT_CPU;
    P5DIR |= IOT_BOOT_CPU;
}

void Init_Port6(void) { // Port 6 configuration
    P6OUT = 0x00;
    P6DIR = 0x00;

    P6SEL0 |= L_FORWARD; // L_FORWARD -> Set to output and configured for PWM control
    P6SEL1 &= ~L_FORWARD;
    P6DIR |= L_FORWARD;

    P6SEL0 |= R_FORWARD; // R_FORWARD -> Set to output and configured for PWM control
    P6SEL1 &= ~R_FORWARD;
    P6DIR |= R_FORWARD;

    P6SEL0 |= L_REVERSE; // L_REVERSE -> Set to output and configured for PWM control
    P6SEL1 &= ~L_REVERSE;
    P6DIR |= L_REVERSE;

    P6SEL0 |= R_REVERSE; // R_REVERSE -> Set to output and configured for PWM control
    P6SEL1 &= ~R_REVERSE;
    P6DIR |= R_REVERSE;

    P6SEL0 |= LCD_BACKLITE; // LCD_BACKLITE -> Set to output and configured for PWM control
    P6SEL1 &= ~LCD_BACKLITE;
    P6DIR |= LCD_BACKLITE;

    P6SEL0 &= ~P6_5; // P6_5 -> GPIO input
    P6SEL1 &= ~P6_5;
    P6OUT &= ~P6_5;
    P6DIR &= ~P6_5;

    P6SEL0 &= ~GRN_LED; // GRN_LED -> GPIO output
    P6SEL1 &= ~GRN_LED;
    P6OUT &= ~GRN_LED;
    P6DIR |= GRN_LED;

}




