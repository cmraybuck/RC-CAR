/*
 * macros.h
 *
 *  Created on: Sep 9, 2025
 *      Author: Micah Raybuck
 *
 *      This file contains defines for our .c files alongside global variables so they can be accessed throughout the workspace
 */

#ifndef MACROS_H_
#define MACROS_H_

// Defines
#define BOOT_TIMER (3000)
#define PING_TIMER (50000)
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01)
#define GRN_LED              (0x40)
#define TEST_PROBE           (0x01)
#define TRUE                 (0x01)
#define FALSE (0)
#define LEFT_ROTATE ('Y')
#define RIGHT_ROTATE ('Z')
#define DONE ('X')
#define GO ('G')
#define BLACK (500)
#define NONE                  ('N')
#define STRAIGHT              ('L')
#define CIRCLE                ('C')
#define WAIT                  ('W')
#define START                 ('S')
#define RUN                   ('R')
#define END                   ('E')
#define WHEEL_COUNT_TIME      (10)
#define RIGHT_COUNT_TIME      (7)
#define LEFT_COUNT_TIME       (8)
#define TRAVEL_DISTANCE       (2)
#define WAITING2START         (150)
#define RIGHT_STRAIGHT_TIME   (6)
#define LEFT_STRAIGHT_TIME    (6)
#define STRAIGHT_TRAVEL_DISTANCE (10)
#define RIGHT_CIRCLE_TIME (70)
#define LEFT_CIRCLE_TIME (1)
#define CIRCLE_TRAVEL_DISTANCE (18)
#define CIRCLE_COUNT_TIME (20)
#define FIGURE8               ('8')
#define RIGHT_LOOP1_TIME (60)
#define LEFT_LOOP1_TIME (1)
#define RIGHT_LOOP2_TIME (1)
#define LEFT_LOOP2_TIME (60)
#define LOOP1_TRAVEL_DISTANCE (8)
#define LOOP2_TRAVEL_DISTANCE (16)
#define LOOP_COUNT_TIME (20)
#define LOOP2_COUNT_TIME (20)
#define TRIANGLE ('T')
#define RIGHT_TRIANGLE_TIME (9)
#define LEFT_TRIANGLE_TIME (9)
#define TRIANGLE_SIDE_LENGTH (7)
#define TRIANGLE_COUNT_TIME (10)
#define PIVOT_TIME (30)
#define PIVOT_RIGHT (12)
#define PIVOT_LEFT (0)
#define BLINK_TIME (200)
#define SW1_HOLD_TIME (1000)
#define SW2_HOLD_TIME (1000)
#define TIMER_B0_TICK (999)
#define WHEEL_PERIOD (50005)
#define PWM_PERIOD (TB3CCR0)
#define LEFT_FORWARD_SPEED (TB3CCR1)
#define RIGHT_FORWARD_SPEED (TB3CCR2)
#define LEFT_REVERSE_SPEED (TB3CCR3)
#define RIGHT_REVERSE_SPEED (TB3CCR4)
#define LCD_BACKLITE_DIMING (TB3CCR5)
#define WHEEL_OFF (0)
#define SLOW (35000)
#define FAST (50000)
#define PERCENT_100 (50000)
#define PERCENT_80 (45000)
#define PERCENT_20 (10000)
#define LEFT_FAST (35000)
#define RIGHT_FAST (50000)
#define LEFT_FORWARD (25000)
#define LEFT_SLOW (20000)
#define RIGHT_SLOW (35000)
#define COURSE_SPEED (10000)
#define COURSE_SPEED_RIGHT (11500)
#define COURSE_TURN (10000)
#define TB1_TICK_200MS (20000)
#define MOVING_LEFT ('L')
#define MOVING_RIGHT ('R')
#define MOVING_FORWARD ('F')
#define BAUD_LOW (115220)
#define BAUD_HIGH (460800)
#define BEGINNING (0)
#define SETUP_STEP1 (2000)
#define SETUP_STEP2 (215)
#define SETUP_STEP3 (2750)
#define SETUP_STEP4 (215)
#define END_OF_ARRAY (0x0A)
#define BOW_STEP1 (400)
#define BOW_STEP2 (200)
#define BOW_STEP3 (1500)
#define DISPLAY_DEMO_PAUSE (10000)
#define POST_DETECTION_CRAWL (50)
#define RIGHT_TURN_TIME (50)
#define LEFT_TURN_TIME (50)
#define IOT_RESET_NUM (100)





// Global Variables
volatile unsigned int debounce_SW1;
volatile unsigned int debounce_SW2;
volatile unsigned char debounce_status_SW1;
volatile unsigned char debounce_status_SW2;
volatile unsigned char blink_active;
volatile unsigned int blink_counter;
volatile unsigned int whiteCalibrated;
volatile unsigned int blackCalibrated;
volatile char slow_input_down;
extern char display_line[4][11];
extern char *display[4];
unsigned char display_mode;
extern volatile unsigned char display_changed;
extern volatile unsigned char update_display;
extern volatile unsigned int update_display_count;
extern volatile unsigned int Time_Sequence;
volatile unsigned int ADC_Channel;
volatile unsigned int ADC_Left_Detect;
volatile unsigned int ADC_Right_Detect;
volatile unsigned int ADC_Thumb_Detect;
volatile unsigned int timeVar;
extern volatile char one_time;
unsigned int test_value;
char chosen_direction;
char change;
char adc_char[4];
unsigned int delay_start;
unsigned int Last_Time_Sequence;
unsigned int cycle_time;
unsigned int time_change;
unsigned int right_motor_count;
unsigned int left_motor_count;
unsigned int segment_count;
char shape;
char state;
volatile char demoStatus;
unsigned int wheel_move;
char forward;
int loopCounter;
int detectionDelay;
int rotationDelay;
int switch1Released;
int switch2Released;
int ok_to_look_at_sw1;
int ok_to_look_at_sw2;
int black_line_detected;
volatile unsigned int ir_led_status;
char movement_status;
volatile unsigned int display_ticks;
volatile unsigned int sensorTicks;
volatile unsigned int baud_rate;
volatile char IOT_2_PC[10];
volatile unsigned int iot_rx_wr;
char speed;
unsigned int boot_state;
char character;
volatile unsigned int direct_iot;
volatile unsigned int IOT_parse;
volatile char IOT_Ring_Rx[10];
volatile char IOT_Ring_Tx[10];
volatile unsigned int line;
volatile char USB_Ring_Rx[10];
volatile char USB_Ring_Tx[10];
volatile char iot_TX_buf[10];
volatile char IOT_Data[4][30];
volatile char A0_RX_BUF[50];
volatile char A0_TX_BUF[30];
volatile char A1_RX_BUF[30];
volatile char A1_TX_BUF[30];
volatile unsigned int iot_ping_counter;
volatile char USB_Data[30];
volatile char IOT_2_PC[10];
volatile char PC_2_IOT[10];
volatile unsigned int iot_tx;
volatile unsigned int iot_index;
volatile unsigned int ip_address_found;
volatile unsigned int usb_rx_wr;
volatile unsigned int usb_rx_rd;
volatile char ip_mac[10];
volatile unsigned int iot_rx_rd;
volatile unsigned int iot_rx_wr;
volatile char AT[10];
volatile unsigned int iot_ticks;
volatile unsigned int pc_power_on;
volatile char command;
volatile char response[25];
int test_Value;
volatile unsigned int need_specific_response;
volatile unsigned int command_received;
volatile unsigned int baud_rate_changed;
volatile unsigned int clear_command;
volatile unsigned int transmit_command;
volatile unsigned int temp;
volatile unsigned int ip_len;
volatile unsigned int stringTimer;
volatile int tempValue;
volatile int tempValue2;
volatile char process_buffer[25];
volatile unsigned int pb_index;
volatile unsigned int search_for_ip;
volatile unsigned int ip_found;
volatile unsigned int num_of_quotes;
volatile char ip_address[10];
char lastChar;
unsigned int dots;
volatile char ip_address2[10];
volatile unsigned int ip_len2;
volatile char ssid_val[10];
volatile char ssid_val2[10];
volatile unsigned int ssid_len;
volatile unsigned int ssid_len2;
unsigned int colons;
volatile unsigned int calibration_mode;
volatile unsigned int automaton_power_on;
volatile unsigned int black_line_found;
volatile unsigned int exit_loop;
volatile unsigned int iot_reset;
volatile int ssid_found;
volatile int search_for_ssid;
volatile unsigned int exe_command;
volatile unsigned int calibration_complete;
volatile unsigned int bootStep;
volatile unsigned int booted;
volatile unsigned int iot_reset_counter;
volatile unsigned int ip_requested;
volatile unsigned int exit_left;
volatile unsigned int exit_right;
volatile unsigned int need_to_increment;
volatile unsigned int num_to_display;
volatile unsigned int default_display;
volatile unsigned int white_board_found;
volatile char currentCommand[10];
volatile char lastCommand[10];
volatile char tempCommand[10];
volatile int command_changed;
volatile int in_auto_mode;
volatile int displaying_travel;
volatile int displaying_circle;
volatile int reset_timer_ticks;


#endif /* MACROS_H_ */
