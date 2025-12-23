/*
 * movement.c
 * Author: Micah Raybuck
 * This file contains all functions related to vehicle movement, including
 * using sensor readings to follow the black line
 */




#include  "msp430.h"
#include  <string.h>
#include  "Include/functions.h"
#include  "Include/LCD.h"
#include "Include/macros.h"
#include  "Include/ports.h"

void AllMotorsOff(void) { // function to shut off all motors
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // right reverse off
    LEFT_REVERSE_SPEED = WHEEL_OFF; // left reverse off
    LEFT_FORWARD_SPEED = WHEEL_OFF; // left forward off
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // right forward off
}
void GoForward(void) { // function to initiate forward movement
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // turn off right reverse
    LEFT_REVERSE_SPEED = WHEEL_OFF; // turn off left reverse
    LEFT_FORWARD_SPEED = LEFT_FORWARD; // set left motor to forward at speed specified via PWM configuration
    RIGHT_FORWARD_SPEED = RIGHT_FAST; // set right motor speed to forward at speed specified via PWM configuration
    movement_status = MOVING_FORWARD; // variable to track system's active movement state
}
void GoClockwise(void) { // function to initiate clockwise movement
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // right forward off
    LEFT_REVERSE_SPEED = WHEEL_OFF; // left reverse off
    RIGHT_REVERSE_SPEED = RIGHT_FAST; // right reverse set to fast
    LEFT_FORWARD_SPEED = LEFT_FAST; // left forward set to fast
}
void GoCounterClockwise(void){ // function to initiate counter clockwise movement
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // right reverse off
    LEFT_FORWARD_SPEED = WHEEL_OFF; // left forward off
    LEFT_REVERSE_SPEED = LEFT_FAST; // left reverse set to fast
    RIGHT_FORWARD_SPEED = RIGHT_FAST; // right forward set to fast
}
void GoReverse(void) { // function to initiate reverse movement
    LEFT_FORWARD_SPEED = WHEEL_OFF; // left forward off
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // right forward off
    RIGHT_REVERSE_SPEED = RIGHT_FAST; // right reverse on at PWM speed
    LEFT_REVERSE_SPEED = LEFT_FAST; // left reverse on at PWM speed
}
void Forward_Off(void) { // function to turn off forward movement
    LEFT_FORWARD_SPEED = WHEEL_OFF; // left forward off
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // right forward off
}
void Reverse_Off(void) { // function to turn off reverse movement
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // right reverse off
    LEFT_REVERSE_SPEED = WHEEL_OFF; // left reverse off
}

void SlowRightTurn(void) { // function to initiate a slow right turn
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // right reverse off
    LEFT_REVERSE_SPEED = WHEEL_OFF; // left reverse off
    LEFT_FORWARD_SPEED = COURSE_TURN; // left forward set to course turn speed which has a PWM value that is much slower than the regular speed
    RIGHT_FORWARD_SPEED = WHEEL_OFF; // right forward off
    movement_status = MOVING_RIGHT; // active movement status is moving to the right
}

void SlowLeftTurn(void) { // function to initiate a slow left turn
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // right reverse off
    LEFT_REVERSE_SPEED = WHEEL_OFF; // left reverse off
    LEFT_FORWARD_SPEED = WHEEL_OFF; // left forward off
    RIGHT_FORWARD_SPEED = COURSE_TURN; // right forward set to course turn (lower PWM value)
    movement_status = MOVING_LEFT; // active movement status is moving to the lft
}

void GoForwardCourse(void) { // function for moving forward on the black line course
    RIGHT_REVERSE_SPEED = WHEEL_OFF; // right reverse off
    LEFT_REVERSE_SPEED = WHEEL_OFF; // left reverse off
    LEFT_FORWARD_SPEED = COURSE_SPEED; // left forward set to course speed (lower PWM)
    RIGHT_FORWARD_SPEED = COURSE_SPEED_RIGHT; // right forward set to a course speed tailored to right motor to achieve straight movement (motor differences)
}



void SearchForBoard(void) { // function to search for white board
        if ((ADC_Right_Detect <= whiteCalibrated || ADC_Left_Detect <= whiteCalibrated) && !white_board_found) { // if either detectors detect the board and we have not found the white board
                AllMotorsOff(); // shut off motors
                timeVar = 0; // reset time var
                while(timeVar < POST_DETECTION_CRAWL) { // for specified amount of time to continue crawling onto whiteboard
                    GoForwardCourse(); // move forward to position self onto the white board
                }
                AllMotorsOff(); // turn motors off
                white_board_found = TRUE; // white board found flag set to high
                return;
        }
        else if (!white_board_found){ // if the white board has not been found
            GoForwardCourse(); // move forward
        }

}

void SearchForLine(void) { // function to search for black line
        if (!detectionDelay) { // if detection delay flag has not been set high
            timeVar = RESET_STATE; // reset time var
            in_auto_mode = TRUE; // flag to indicate we are in auto mode
            strcpy(display_line[0], " BL Start "); // display required message
            Display_Update(0,0,0,0); // update display
            PauseForDisplay(); // call to function that waits 10 seconds to give TA time to read display (10 seconds)
            detectionDelay = TRUE; // detection delay flag set high
        }
        if ((ADC_Right_Detect >= blackCalibrated || ADC_Left_Detect >= blackCalibrated) && !black_line_detected && state != DONE) { // if either sensor detects black, black line hasn't been detected, state is not done, and left sensor is not black
            AllMotorsOff(); // turn off motors
            strcpy(display_line[0], "Intercept "); // display required message
            Display_Update(0,0,0,0); // update display
            PauseForDisplay(); // wait for 10 seconds
            black_line_detected = TRUE; // set black line detected flag high
        }
        else if (!black_line_detected && state != DONE){ // if black line has not been detected and state is not done
            GoForwardCourse(); // move forward slowly
        }
        if (black_line_detected) { // if black line has been detected
            timeVar = RESET_STATE; // reset var
            strcpy(display_line[0], " BL Turn  "); // display required message
            strcpy(display_line[3], "          "); // clear last line of display to be used later
            Display_Update(0,0,0,0); // update display
            AllMotorsOff(); // turn off motors
            while(ADC_Left_Detect < blackCalibrated || ADC_Right_Detect < blackCalibrated){ // while both sensors are not on the black line
                SlowLeftTurn(); // turn left slowly
            }
            AllMotorsOff(); // turn motors off
            PauseForDisplay(); // wait 10 seconds
            black_line_found = TRUE; // set black line found flag high
            timeVar = RESET_STATE; // reset time var
        }
}


void FollowBlackLine(void) { // function for following the black line based on IR sensor readings
    if(!reset_timer_ticks) { // if reset timer ticks flag is low
        display_ticks = RESET_STATE; // reset display_ticks
        reset_timer_ticks = TRUE; // set reset timer ticks flag high
    }
    DisplayTimer(); // display timer to count time on black line
    if(!displaying_travel) { // if not display BL Travel message
        strcpy(display_line[0], "BL Travel "); // display required message
        Display_Update(0,0,0,0); // update display
        displaying_travel = TRUE; // set displaying travel var high
        PauseForDisplay();
        timeVar = RESET_STATE; // reset time var
    }
    else if(displaying_travel && !displaying_circle && timeVar >= 4000) { // if travel message is being displayed, circle is not, and 4 seconds have passed from displaying travel msg
        strcpy(display_line[0], "BL Circle "); // display required message
        AllMotorsOff(); // Turn off motors to display message
        Display_Update(0,0,0,0); // update display
        displaying_circle = TRUE; // set displaying circle var high
        PauseForDisplay();
    }

    if(!exit_loop) { // if the command to exit the loop has not been given
        if (ADC_Left_Detect >= blackCalibrated && ADC_Right_Detect >= blackCalibrated) { // if both detectors are on the black line
            GoForwardCourse(); // move forward slowly
        }
        else if (ADC_Left_Detect >= blackCalibrated && ADC_Right_Detect < blackCalibrated) { // if the left detector is on the black line but the right is not
            SlowRightTurn(); // turn right slowly
        }
        else if (ADC_Left_Detect < blackCalibrated && ADC_Right_Detect >= blackCalibrated) { // if the right detector is on black but left is not
            SlowLeftTurn(); // turn left slowly
        }
    }


}



void AutomatonMode(void) { // function for executing appropriate actions when command to enter automated mode is received
    if(automaton_power_on) { // if automaton power on flag is high
        if(!white_board_found) { // if white board has not been found
            SearchForBoard(); // search for the white board
        }
        else if(!black_line_found && white_board_found) { // if the white board has been found but the black line has not
            SearchForLine(); // search for the black line
        }
        else if(black_line_found && !exit_loop) { // if the black line has been found and a command to exit automaton mode has not been given
            FollowBlackLine(); // follow the black line
        }
        else if(exit_loop) { // if command to exit automaton mode has been received
            in_auto_mode = FALSE; // set in auto mode flag low
            automaton_power_on = FALSE; // set automaton power on flag low
            black_line_found = FALSE; // reset black line found state
            white_board_found = FALSE; // reset white board found state
            AllMotorsOff(); // turn all motors off
            TakeABow(); // initiate final movement
        }
    }
}



void TakeABow(void) { // function to handle final course movement/command
    if(exit_loop) { // if command to exit loop has been given
        strcpy(display_line[0], " BL Exit  "); // display required message
        Display_Update(0,0,0,0); // update display
        PauseForDisplay(); // wait 10 seconds
        if(exit_right) { // if the command given requests an exit to the right
            timeVar = 0; // reset time var
            while(timeVar < BOW_STEP1) { // for specified step time
                AllMotorsOff(); // turn motors off
            }
            timeVar = 0; // reset time var
            while(timeVar < BOW_STEP2) { // for specified step time
                GoClockwise(); // pivot clockwise
            }
        }
        if(exit_left) { // if command given requests an exit to the left
            timeVar = 0; // reset time var
            while(timeVar < BOW_STEP1) { // for specified step time
                AllMotorsOff(); // turn motors off
            }
            timeVar = 0; // reset time var
            while(timeVar < BOW_STEP2) { // for specified step time
                GoCounterClockwise(); // pivot counter clockwise
            }
        }
        AllMotorsOff(); // turn off motors
        timeVar = 0; // reset time var
        while(timeVar < BOW_STEP3) { // for specified step time
            GoForward(); // move forward to get off of the track
        }
        strcpy(display_line[0], " BL Stop  "); // display required message
        strcpy(display_line[1], " RELEASE  "); // display "victory" message
        strcpy(display_line[2], "   ME     "); // ^^^
        Display_Update(0,0,0,0); // update display
        AllMotorsOff(); // turn off motors
    }

}


void PauseForDisplay(void) { // function to call for allowing TAs to read message displayed onto car
    timeVar = 0; // reset time var
    while(timeVar < DISPLAY_DEMO_PAUSE) { // for 10 seconds...
        continue; // do nothing
    }
}






