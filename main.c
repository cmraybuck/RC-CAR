/*
 * main.c
 * Author: Micah Raybuck
 * This file contains the main while loop of this project.
 * It serves as the primary driver of the program, initializing all necessary
 * conditions to accomplish our project's goal.
 */


//AT+CWJAP=”Wifi”,”Password”
//AT+CWHOSTNAME="Car"
//AT+CWAUTOCONN=1

// #include statements added to top of each file for clarity and ease of use
#include  "msp430.h"
#include  <string.h>
#include  "Include/functions.h"
#include  "Include/LCD.h"
#include "Include/macros.h"
#include  "Include/ports.h"

// Initial function declarations
void main(void);
void Init_Conditions(void);
void Display_Process(void);
void Init_LEDs(void);

void main(void) {
    __enable_interrupt();
    WDTCTL = WDTPW | WDTHOLD;
    PM5CTL0 &= ~LOCKLPM5;
    Init_Ports(); // initialize ports
    Init_Clocks(); // initialize clocks
    Init_Timers(); // initialize timers
    Init_Conditions(); // initialize default conditions
    Init_LCD(); // initialize LCD
    Init_ADC(); // initialize ADC
    Init_DAC(); // initialize DAC
    Init_Serial_UCA0(speed); // initialize serial port communication for UCA0
    Init_Serial_UCA1(speed); // initialize serial port communication for UCA1
    AllMotorsOff(); // turn all motors off
    ResetIOT(); // reset the IOT device on startup
    // ------------------------ //
    display_changed = TRUE;
    update_display = TRUE;
    black_line_detected = FALSE;
    state = NONE;
    demoStatus = RESET_STATE;
    detectionDelay = RESET_STATE;
    loopCounter = RESET_STATE;
    command_received = RESET_STATE;
    in_auto_mode = FALSE;
    command_changed = FALSE;
    bootStep = RESET_STATE;
    // Variables above initialized to necessary values for functionality
    // ------------------------ //
    strcpy(currentCommand, "   WAIT   "); // set placeholder value to currentCommand
    while(ALWAYS) { // "infinite" while loop
        USB_Process(); // function to monitor USB/PC communication
        IOT_Process(); // function to monitor IOT communication
        Display_Process(); // function to manage display changes and updates
        AutomatonMode(); // function for black line detection and following activated via IOT commands
        P3OUT ^= TEST_PROBE;
    }
}









