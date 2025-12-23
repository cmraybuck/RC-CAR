/*
 * Display.c
 * Author: Micah Raybuck
 * This file contains all functions and processes relevant to updating the display, modifying data to be
 * visible on the LCD, and displaying readings from the hardware installed on the MSP430FR2355.
 */
#include  "msp430.h"
#include  <string.h>
#include  "Include/functions.h"
#include  "Include/LCD.h"
#include "Include/macros.h"
#include  "Include/ports.h"
int i;

void Display_Process(void){ // function for data to display onto the LCD
    if(calibration_mode) { // if in calibration mode
        strcpy(display_line[0], " R:       "); // display R: on line with right sensor readings
        strcpy(display_line[1], " L:       "); // display L: on line with left sensor readings
        DisplaySensorReadings(); // call to function that displays sensor values
    }
    if(need_to_increment) { // if increment command has been received
        switch(num_to_display) { // state machine using num_to_display
        case(1): // if num to display is 1
                strcpy(display_line[0], "Arrived 01"); // update line on display to Arrived 01
                update_display = TRUE; // update display flag high
                display_changed = TRUE; // display changed flag high
                need_to_increment = FALSE; // no longer need to increment
                break;
        case(2): // same process as case 1 for cases 2 - 7
                strcpy(display_line[0], "Arrived 02");
                update_display = TRUE;
                display_changed = TRUE;
                need_to_increment = FALSE;
                break;
        case(3):
                strcpy(display_line[0], "Arrived 03");
                update_display = TRUE;
                display_changed = TRUE;
                need_to_increment = FALSE;
                break;
        case(4):
                strcpy(display_line[0], "Arrived 04");
                update_display = TRUE;
                display_changed = TRUE;
                need_to_increment = FALSE;
                break;
        case(5):
                strcpy(display_line[0], "Arrived 05");
                update_display = TRUE;
                display_changed = TRUE;
                need_to_increment = FALSE;
                break;
        case(6):
                strcpy(display_line[0], "Arrived 06");
                update_display = TRUE;
                display_changed = TRUE;
                need_to_increment = FALSE;
                break;
        case(7):
                strcpy(display_line[0], "Arrived 07");
                update_display = TRUE;
                display_changed = TRUE;
                need_to_increment = FALSE;
                break;
        case(8): // if num to display is 8 (last pad)
                strcpy(display_line[0], "Arrived 08"); // display appropriate message
                update_display = TRUE;
                num_to_display = 0; // reset num to display so the command can be reused if needed. If increment command used while Arrived 08 is on the display, it will go back to Arrived 01
                display_changed = TRUE;
                need_to_increment = FALSE;
                break;
        }
    }
  if(update_display){ // if update display flag is high
      if(calibration_complete) { // if calibration has been completed
          calibration_complete = FALSE; // reset calibration flag in case car needs to be recalibrated
      }

      if(command_changed) { // if command has been changed and needs to be displayed
          strcpy(display_line[0], "          "); // clear first line
          strcpy(display_line[3], "          "); // clear last line
          strcpy(display_line[0], "CMD: "); // display CMD: to indicate current command being executed
          strcat(display_line[0], currentCommand); // display current command
          strcpy(display_line[3], "PREV: "); // display PREV: to indicate previously executed command
          strcat(display_line[3], lastCommand); // display previous command
          display_changed = TRUE; // set display changed flag high
          command_changed = FALSE; // set command changed flag low
      }
    update_display = FALSE; // set update display flag low
    if(display_changed){ // if display changed flag is high
      display_changed = 0; // reset display changed flag
      Display_Update(0,0,0,0); // call function to update display
    }
  }
}


void DisplayTimer(void) {
    unsigned long total_ms = (unsigned long)display_ticks * 200; // convert display ticks to ms
    unsigned int seconds = total_ms / 1000; // divide ms by 1000 to get time in seconds
    HEXtoBCD(seconds); // convert hex val to BCD
    strcpy(display_line[3], "time: "); // update display to properly display the timer
    adc_line(4, 6); // function call to display timer vals
}


void HEXtoBCD(int hex_value){ // function for converting hexadecimal to BCD
    int value = 0; // initialize value var
    int i; // initialize i to be used in for loops
    for(i = 0; i < 4; i++) { // do the folllowing 4 times
        adc_char[i] = '0'; // set vals of adc_char to null
    }
    while (hex_value > 999){ // if hex_value exceeds 999
        hex_value = hex_value - 1000; // reset var
        value = value + 1; // add 1 to value var
        adc_char[0] = 0x30 + value; // add value var to the char array (1000s, 100s, and 10s places is how the conversion actually happens)
    }
    value = 0; // reset value
    while (hex_value > 99){ // same logic as first while loop but for 100s place
        hex_value = hex_value - 100;
        value = value + 1;
        adc_char[1] = 0x30 + value;
    }
    value = 0;
    while (hex_value > 9){ // same logic as first while loop but for 10s place
        hex_value = hex_value - 10;
        value = value + 1;
        adc_char[2] = 0x30 + value;
    }
    adc_char[3] = 0x30 + hex_value;
}




void adc_line(char line, char location){ // function to display char values parsed from HEXtoBCD function
//-------------------------------------------------------------
    int i; // initialize var to be used in for loops
    unsigned int real_line; // var to represent the actual line that's being manipulated
    real_line = line - 1; // set real line to line argument -1
    for(i=0; i < 4; i++) { // do the following  4 times
        display_line[real_line][i+location] = adc_char[i]; // display chars onto display
    }
    update_display = TRUE; // set update display flag high
    display_changed = TRUE; // set display changed flag high
}



void DisplaySensorReadings(void){ // function for displaying readings from IR sensors
    if(sensorTicks >= 250) { // if 250ms have passed
        sensorTicks = 0; //reset sensor timer
        HEXtoBCD(ADC_Left_Detect); // convert left detector value to BCD
        adc_line(1, 3); // display value
        HEXtoBCD(ADC_Right_Detect); // convert right detector value to BCD
        adc_line(2, 3); // display value
        HEXtoBCD(ADC_Thumb_Detect); // convert thumb wheel detector value to BCD
        adc_line(3, 3); // display value
        display_changed = TRUE; // set display changed flag high
        update_display = TRUE; // set update display flag high
    }
}


void ClearDisplay(void) { // function for clearing display values
    strcpy(display_line[0], "          "); // clear line 1 of the display
    strcpy(display_line[1], "          "); // clear line 2 of the display
    strcpy(display_line[2], "          "); // clear line 3 of the display
    strcpy(display_line[3], "          "); // clear line 4 of the display
}
