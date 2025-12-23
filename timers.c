/*
 * timers.c
 * Author: Micah Raybuck
 * This file contains all functions relevant to configuring timers used in
 * interrupts throughout the program.
 */





#include  "msp430.h"
#include  <string.h>
#include  "Include/functions.h"
#include  "Include/LCD.h"
#include "Include/macros.h"
#include  "Include/ports.h"

extern volatile unsigned int timeVar;

void tick(void);


void Init_Timers(void){
    Init_Timer_B0();
    Init_Timer_B1();
    Init_Timer_B3();
}

void Init_Timer_B0(void) {
    TB0CCTL0 = CCIE; // enable interrupt
    TB0CCR0 = TIMER_B0_TICK; // set ccr0 equal to desired tick speed
    TB0EX0 = 0; // set expansion register low
    TB0CTL = TBCLR; // set control register
    TB0CTL = TBSSEL__SMCLK | ID__8 | MC__UP; // using SMCLK, divide by 8, in up mode
}


void Init_Timer_B1(void) {
    TB1CCTL0 = CCIE; // enable interrupt
    TB1CCR0  = TB1_TICK_200MS; // set desired tick rate of timer
    TB1CTL   = TBSSEL__SMCLK | ID__8 | MC__UP | TBCLR; // using SMCLK, divide by 8, up mode, and set control register
}

void Init_Timer_B3(void) {
    TB3CTL = TBSSEL__SMCLK; // SMCLK
    TB3CTL |= MC__UP; // Up Mode
    TB3CTL |= TBCLR; // Clear TAR
    TB3CCTL0 = CCIE;   // Enable interrupt

    PWM_PERIOD = WHEEL_PERIOD; // PWM Period [Set this to 50005]

    TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
    LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Left Forward PWM duty cycle

    TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.2 Right Forward PWM duty cycle

    TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
    LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM duty cycle

    TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.4 Right Reverse PWM duty cycle

    TB3CCTL5 = OUTMOD_7; // CCR5 reset/set
    LCD_BACKLITE_DIMING = PERCENT_20; // P6.5 LCD_BACKLITE On Diming percent
}


