/*
 * iot.c
 * Author: Micah Raybuck
 * This file contains all functions relating to the IOT device, including transmitting
 * and receiving data via serial port communication, command processing, and a state machine
 * used to configure the IOT with console commands without needing to do so manually via a terminal
 * emulator.
 */

#include  "msp430.h"
#include  <string.h>
#include  "Include/functions.h"
#include  "Include/LCD.h"
#include "Include/macros.h"
#include  "Include/ports.h"

void IOT_Process(void){ // function for most IOT interaction
    if(!booted) { // if IOT hasn't been booted yet
        if(iot_reset_counter > BOOT_TIMER) { // if reset counter exceeds the specified boot timer
            iot_reset_counter = BEGINNING; // reset the iot reset counter
            BootIOT(); // call function to boot the IOT device
        }
    }
    if(iot_ping_counter > PING_TIMER) { // if iot ping counter is greater than specified ping timer
        IOT_Ping(); // ping function
    }
    unsigned int iot_rx_wr_temp; // var for holding temporary write values
    iot_rx_wr_temp = iot_rx_wr; // set temp value equal to write value
    while(iot_rx_wr_temp != iot_rx_rd) { // while temp value does not equal iot read value
        IOT_Data[line][character] = A0_RX_BUF[iot_rx_rd++]; // Use two dimensional array to store data received from IOT device
        if(iot_rx_rd >= sizeof(A0_RX_BUF)) { // if read data exceeds size of buffer
            iot_rx_rd = BEGINNING; // set read data back to 0
        }
        command = IOT_Data[line][character]; // define current character on line as "command" for later use
        if(!ip_found && ip_requested) { // if we have not received the IP and user has requested the IP
            if(search_for_ip && command != '"') { // if we have found the first double quote and the current char is not another double quote (currently parsing the actual IP in between quotes)
                Parse_IP(line, character); // pass line and character into function to parse the IP
                continue; // continue iterating through the while loop
            }
            if(command == '"' && !search_for_ip) { // if first set of double quotes is detected
                search_for_ip = TRUE; // set flag to indicate we are now actively parsing the IP address
                continue; // continue iterating through the while loop
            }
            if(search_for_ip && command == '"') { // if we have been parsing the IP and the current char is the second set of double quotes
                ip_found = TRUE; // set flag indicating that we have all necessary data to display the IP
                Parse_IP(line, character); // make final call to parse IP function
                continue; // continue iterating through while loop
            }
        }
        if(lastChar == '^' && !exe_command) { // if char variable lastChar was a ^ and we are not currently executing a command
            ProcessCommand(command); // process the command sent
        }

        lastChar = IOT_Data[line][character]; // set lastChar equal to the current char so that the next iteration will check for a ^
        if(IOT_Data[line][character] == END_OF_ARRAY){ // if we've reached the capacity of the array
            character = 0; // set character to 0
            line++; // move to the next line
            if(line >= 4){ // if line is greater than or equal to 4
                line = 0; // reset line array
            }
        }
        else { // if above conditional was not met
            character++; // move to the next character position in the array
        }
    }
}

void USB_Process(void) { // function for processing most USB interaction
    unsigned int usb_rx_wr_temp; // create temp write var
    usb_rx_wr_temp = usb_rx_wr; // set equal to current write char
    while(usb_rx_wr_temp != usb_rx_rd) { // while temp write val does not equal read val
        USB_Data[character] = A1_RX_BUF[usb_rx_rd++]; // set USB_Data at certain index to the char received in the A1 receive buffer
        if(usb_rx_rd >= sizeof(A1_RX_BUF)){ // if read val exceeds size of A1 receive buffer
            usb_rx_rd = BEGINNING; // reset read value
        }
        if(USB_Data[character] == END_OF_ARRAY){ // if we've reached the capacity of the array
            character = RESET_STATE; // reset char index
        }
        else { // if above conditional was not met
            character++; // move to next character position in the array
        }
    }
}


void IOT_Ping(void) { // function for sending a ping from IOT device to prevent being kicked off of network
    iot_ping_counter = 0; // reset ping counter
    strcpy(A0_TX_BUF, "AT+PING=\"www.google.com\"\r\n"); // load A0 transfer buffer with the proper command to ping google
    USCI_A0_transmit(); // initiate manual transmit
}

void Parse_IP(int line, char character) { // function for parsing IP address
    if(!ip_found) { // if ip has not been found
        if(dots < 2) { // if dots is less than 2
            ip_address[ip_len++] = IOT_Data[line][character]; // ip_address (first display line of IP, need to have it displayed on two lines due to length) now holds current char of IOT_Data
        }
        if(IOT_Data[line][character] == '.' && dots < 2) { // if the char is a period and we have detected less than 2 periods
            dots++; // increment dots (counter for how many periods have been received which helps break the IP into two separate lines)
        }
        if(dots == 2) { // if dots is exactly 2 meaning we need to move to the next line of the display
            ip_address2[ip_len2++] = IOT_Data[line][character]; // ip_address2 (second display line of IP) now holds current char of IOT_Data
        }
    }
    if(ip_found) { // ip has been parsed successfully
        strcpy(display_line[1], ip_address); // copy array from ip_address onto the 2nd line of the display
        strcpy(display_line[2], ip_address2); // copy array from ip_address onto the 3rd line of the display
        update_display = TRUE; // flag to indicate display needs to be updated
        display_changed = TRUE; // flag to indicate display has been changed
    }
}



void ProcessCommand(char command) { // function for processing commands and taking action
    exe_command = TRUE; // exe_command indicates that we are currently executing a command which prevents two commands from being executed at once
    switch(command){ // state machine for command processing
    case('~'): // Forward
            AllMotorsOff(); // turn all motors off
            GoForward(); // initialize forward movement
            SwapCommandString(); // swap the previous command with the current command so we can display the last executed command
            strcpy(currentCommand, "FWRD "); // copy command to var so we can display it
            exe_command = FALSE; // finished executing command so set flag low
            break;
    case('@'): // Stop
            AllMotorsOff(); // turn all motors off
            SwapCommandString(); // swap strings
            strcpy(currentCommand, "STOP "); // current command is stop
            exe_command = FALSE; // set flag low
            break;
    case('F'): // Reverse
            AllMotorsOff(); // turn all motors off
            GoReverse(); // initialize reverse movement
            SwapCommandString(); // swap previous command with current command
            strcpy(currentCommand, "RVRS "); // current command is reverse
            exe_command = FALSE; // set flag low
            break;
    case('R'): // Calibrate
            AllMotorsOff(); // turn all motors off
            calibration_mode = TRUE; // set calibration mode flag high
            exe_command = FALSE; // set flag low
            break;
    case('X'): // movement from Pad 8 and aligning with white board
            AllMotorsOff(); // turn off motors
            timeVar = RESET_STATE; // reset general time var
            while(timeVar < SETUP_STEP1) { // for specified time necessary for step 1
                GoForward(); // move forward
            }
            timeVar = RESET_STATE; // reset time var
            AllMotorsOff(); // turn off motors
            while(timeVar < SETUP_STEP2) { // for specified time necessary for step 2
                GoClockwise(); // pivot to the right about 90 degrees
            }
            AllMotorsOff(); // turn off motors
            timeVar = RESET_STATE; // reset time var
            while(timeVar < SETUP_STEP3) { // for specified time necessary for step 3
                GoForward(); // move forward
            }
            AllMotorsOff(); // turn off motors
            timeVar = RESET_STATE; // reset time var
            while(timeVar < SETUP_STEP4) { // for specified time necessary for step 4
                GoClockwise(); // pivot right about 90 degrees
            }
            AllMotorsOff(); // turn off all motors
            exe_command = FALSE; // set flag low
            break;
    case('C'): // Automaton mode
            AllMotorsOff(); // turn off motors
            timeVar = RESET_STATE; // reset time var
            automaton_power_on = TRUE; // set flag high indicating we're entering automaton mode
            white_board_found = FALSE; // set white board flag low
            black_line_found = FALSE; // set black line flag low
            exit_loop = FALSE; // set exit loop flag low
            exit_left = FALSE; // set left loop exit flag low
            exit_right = FALSE; // set right loop exit flag low
            reset_timer_ticks = FALSE; // set reset_timer_ticks flag low
            detectionDelay = RESET_STATE; // set detection delay to 0
            state = NONE; // set state flag low
            black_line_detected = FALSE; // set black line detection flag low
            exe_command = FALSE; // set execution flag low
            break;
    case('G'): // Exit Loop Left
            in_auto_mode = FALSE; // exit automaton mode
            exit_loop = TRUE; // set exit var high to indicate exiting the circle
            exit_left = TRUE; // set left exit var high to indicate the need to exit to the left
            exe_command = FALSE; // set execution flag low
            break;
    case('D'): // Exit Loop Right
            in_auto_mode = FALSE; // exit automaton mode
            exit_loop = TRUE; // set exit var high to indicate exiting the circle
            exit_right = TRUE; // set right exit var high to indicate the need to exit to the right
            exe_command = FALSE; // set execution flag low
            break;
    case('I'): // Get IP
            strcpy(A0_TX_BUF, "AT+CIFSR\r\n"); // copy command to receive IP to the transmit buffer
            USCI_A0_transmit(); // initiate manual transmission
            SwapCommandString(); // swap previous command with current command
            strcpy(currentCommand, "GTIP "); // current command is get IP
            exe_command = FALSE; // set execution flag low
            ip_requested = TRUE; // set ip request flag high
            break;
    case('W'): // Turn Left
            AllMotorsOff(); // turn off motors
            timeVar = RESET_STATE; // reset time var
            while(timeVar < LEFT_TURN_TIME){ // for 0.05 seconds
                GoCounterClockwise(); // turn to the left (about 45 degrees)
            }
            SwapCommandString(); // swap previous command with current command
            strcpy(currentCommand, "TRNL"); // current command is turn left
            AllMotorsOff(); // turn off all motors to prevent continuous movement
            exe_command = FALSE; // set execution flag low
            break;
    case('E'): // Turn Right
            AllMotorsOff(); // turn off motors
            timeVar = 0; // reset time var
            while(timeVar < RIGHT_TURN_TIME){ // for 0.05 seconds
                GoClockwise(); // turn to the right (about 45 degrees)
            }
            SwapCommandString(); // swap previous command with current command
            strcpy(currentCommand, "TRNR"); // current command is turn right
            AllMotorsOff(); // turn off all motors to prevent continous movement
            exe_command = FALSE; // set execution flag low
            break;
    case('P'): // Increment Number
            need_to_increment = TRUE; // set increment flag high
            num_to_display++; // increment what number needs to be displayed
            exe_command = FALSE; // set execution flag low
            break;

    default: break;
    }
}


void BootIOT(void) {
    switch(bootStep) {
    case(0):
            strcpy(A0_TX_BUF, "AT+SYSSTORE=0\r\n"); // step 1 of booting IOT
            USCI_A0_transmit(); // manually transmit
            bootStep++; // go to next step in boot process
            break;
    case(1):
            strcpy(A0_TX_BUF, "AT+CIPMUX=1\r\n"); // allow multiple connections to IOT
            USCI_A0_transmit(); // manually transmit
            bootStep++; // go to next step in boot process
            break;
    case(2):
            strcpy(A0_TX_BUF, "AT+CIPSERVER=1,13872\r\n"); // register a port number to IOT device
            USCI_A0_transmit(); // manually transmit
            bootStep++; // increment boot step
            strcpy(display_line[0], "  WAITING "); // display message indicating successful connection
            update_display = TRUE; // set flag to update display high
            display_changed = TRUE; // set flag to indicate the display has been changed high
            booted = TRUE; // IOT has now booted so set this var high to prevent repeatedly rerunning the boot process
            break;
    default: break;
    }
}

void SwapCommandString(void) { // function to manipulate previous / current command data for displaying commands
    strcpy(tempCommand, currentCommand); // copy current command into a temporary command var
    strcpy(lastCommand, tempCommand); // copy temp command into last command
    command_changed = TRUE; // indicate a command change has taken place
    update_display = TRUE; // need to update the display so set the flag high
    return;
}
