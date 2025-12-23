// Functions
#include "macros.h"
// Main
void main(void);

// Initialization Functions
void Init_Conditions(void);
void Init_ADC(void);
void Init_DAC(void);
void Init_Serial_UCA0(char speed);
void Init_Serial_UCA1(char speed);s

// Interrupt-Related Functions
void enable_interrupts(void);
__interrupt void Timer0_B0_ISR(void);
__interrupt void switch_interrupt(void);
void SW1_ISR(void);
void SW2_ISR(void);
void Timer_B1_ISR(void);

// Clocks
void Init_Clocks(void);

// LED Configurations
void Init_LEDs(void);
void IR_LED_control(char selection);
void Backlite_control(char selection);

// Display Functions
void Display_Process(void);
void Display_Update(char p_L1,char p_L2,char p_L3,char p_L4);
void enable_display_update(void);
void update_string(char *string_data, int string);
void Init_LCD(void);
void lcd_clear(void);
void lcd_putc(char c);
void lcd_puts(char *s);
void lcd_power_on(void);
void lcd_write_line1(void);
void lcd_write_line2(void);
void lcd_enter_sleep(void);
void lcd_exit_sleep(void);
void Write_LCD_Ins(char instruction);
void Write_LCD_Data(char data);
void ClrDisplay(void);
void ClrDisplay_Buffer_0(void);
void ClrDisplay_Buffer_1(void);
void ClrDisplay_Buffer_2(void);
void ClrDisplay_Buffer_3(void);
void SetPostion(char pos);
void DisplayOnOff(char data);
void lcd_BIG_mid(void);
void lcd_BIG_bot(void);
void lcd_120(void);
void lcd_4line(void);
void lcd_out(char *s, char line, char position);
void lcd_rotate(char view);
void lcd_write(unsigned char c);
void lcd_write_line1(void);
void lcd_write_line2(void);
void lcd_write_line3(void);
void lcd_command( char data);
void LCD_test(void);
void LCD_iot_meassage_print(int nema_index);
void HEXtoBCD(int hex_value);
void adc_line(char line, char location);
void DisplaySensorReadings(void);
void DisplayTimer(void);
void ClearDisplay();
void PauseForDisplay(void);
void SwapCommandString(void);


// Ports
void Init_Ports(void);
void Init_Port1(void);
void Init_Port2(void);
void Init_Port3(void);
void Init_Port4(void);
void Init_Port5(void);
void Init_Port6(void);

// SPI
void Init_SPI_B1(void);
void SPI_B1_write(char byte);
void spi_rs_data(void);
void spi_rs_command(void);
void spi_LCD_idle(void);
void spi_LCD_active(void);
void SPI_test(void);
void WriteIns(char instruction);
void WriteData(char data);

// Switches
void Init_Switches(void);
void switch_control(void);
void enable_switch_SW1(void);
void enable_switch_SW2(void);
void disable_switch_SW1(void);
void disable_switch_SW2(void);
void Switches_Process(void);
void Init_Switch(void);
void Switch_Process(void);
void Switch1_Process(void);
void Switch2_Process(void);
void Default_Switch(void);
void menu_act(void);
void menu_select(void);
void CalibrateSensors(void);

// Timers
void Init_Timers(void);
void Init_Timer_B0(void);
void Init_Timer_B1(void);
void Init_Timer_B2(void);
void Init_Timer_B3(void);

void usleep(unsigned int usec);
void usleep10(unsigned int usec);
void five_msec_sleep(unsigned int msec);
void measure_delay(void);
void out_control_words(void);

void initialize_1ms(void);
void tick(void);
int timeCheck(void);
void Timer0_B0_ISR(void);
void Timer1_B0_ISR(void);
void Timer2_B0_ISR(void);
void Timer3_B0_ISR(void);
// ------------------- //

// Movement Functions
void AllMotorsOff(void);
void GoForward(void);
void GoReverse(void);
void GoClockwise(void);
void GoCounterClockwise(void);
void Forward_Off(void);
void Reverse_Off(void);
void Run_Straight(void);
void Run_Circle(void);
void Run_Loop(void);
void Run_Triangle(void);
void MotorCheck(void);
void PivotLeft(void);
void PivotRight(void);
void MovementTest(void);
void SlowLeftTurn(void);
void SlowRightTurn(void);
void GoForwardCourse(void);
void FollowBlackLine(void);
void SearchForLine(void);
void GoForwardLeft(void);
void GoForwardRight(void);
void AutomatonMode(void);
void TakeABow(void);
void SearchForBoard(void);



// IOT Functions
void IOT_Process(void);
void USCI_A0_transmit(void);
void USCI_A1_transmit(void);
void ProcessCommand(char command);
void SetBaudRateOneFifteen(void);
void SetBaudRateNineSix(void);
void ResetIOT(void);
void USB_Process(void);
void IOT_Ping(void);
void Parse_IP(int line, char character);
void Parse_SSID(int line, char character);
void BootIOT(void);

