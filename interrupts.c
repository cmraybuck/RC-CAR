/*
 * interrupts.c
 * Author: Micah Raybuck
 * This file contains all interrupts used throughout the program.
 */



#include  "msp430.h"
#include  <string.h>
#include  "Include/functions.h"
#include  "Include/LCD.h"
#include "Include/macros.h"
#include  "Include/ports.h"
volatile unsigned int start_timer = 0;
volatile unsigned int char_counter = 0;
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
    iot_ping_counter++; // counter for pinging Google to keep device on the network
    if(!booted) { // if iot has not yet been booted keep using iot_reset_counter
        iot_reset_counter++; // increment counter
    }
    if(timeVar > 50000) {
        timeVar = 0;
    }
    timeVar++; // increment general time variable
    if(calibration_mode) { // 11/26 if we are in calibration mode
        sensorTicks++; // increment sensorTicks (determine rate at which sensor information is displayed)
    }
}



#pragma vector = TIMER1_B0_VECTOR // Initialize Timer1_B0
__interrupt void Timer1_B0_ISR(void) {
    update_display = 1;
    display_ticks++;
    if (display_ticks > 4999)
        display_ticks = 0;
}


#pragma vector=TIMER2_B0_VECTOR
__interrupt void Timer2_B0_ISR(void){}




#pragma vector = PORT4_VECTOR
__interrupt void SW1_ISR(void) // SW1 interrupt service routine
{
    if(P4IFG & SW1) { // if switch 1 is pressed
        if(calibration_mode) { // if we're in calibration mode
            if (!whiteCalibrated) { // if white has not yet been calibrated
                whiteCalibrated = ADC_Left_Detect + 10; // set whiteCalibrated var equal to the left detector value (more sensitive)
            }
            else if (!blackCalibrated) { // if black has not yet been calibrated
                blackCalibrated = ADC_Left_Detect - 200; // set blackCalibrated var to the left detector value with an offset for sensitivity
                calibration_mode = FALSE; // exit calibration mode
                calibration_complete = TRUE; // calibration complete
            }
        }

    }
    P4IFG &= ~SW1; // clear interrupt flag
}



#pragma vector = PORT2_VECTOR
__interrupt void SW2_ISR(void) // SW2 isr
{
    if(P2IFG & SW2) { // if switch is pressed
        if(!ip_found) { // if we haven't found the IP yet
            ip_requested = TRUE; // set flag that indicating user has requested the IP address
            strcpy(A0_TX_BUF, "AT+CIFSR\r\n"); // store transmit message into A0 transfer buffer so we can get the IP
            USCI_A0_transmit(); // manually trigger a transmission
        }
        P2IFG &= ~SW2; // clear interrupt flag
    }
}




#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
    switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
        case ADCIV_NONE:
            break;
        case ADCIV_ADCOVIFG: // When a conversion result is written to the ADCMEM0
        // before its previous conversion result was read.
            break;
        case ADCIV_ADCTOVIFG: // ADC conversion-time overflow
            break;
        case ADCIV_ADCHIIFG: // Window comparator interrupt flags
            break;
        case ADCIV_ADCLOIFG: // Window comparator interrupt flag
            break;
        case ADCIV_ADCINIFG: // Window comparator interrupt flag
            break;
        case ADCIV_ADCIFG: // ADCMEM0 memory register with the conversion result
            ADCCTL0 &= ~ADCENC; // Disable ENC bit.
            switch (ADC_Channel++){
                case 0x00: // Channel A2 Interrupt
                    ADC_Left_Detect = ADCMEM0; // Move result into Global
                    ADC_Left_Detect = ADC_Left_Detect >> 2; // Divide the result by 4
                    ADCMCTL0 &= ~ADCINCH_2; // Disable Last channel A2
                    ADCMCTL0 |= ADCINCH_3; // Enable Next channel A3
                    break;
                case 0x01:
                    ADC_Right_Detect = ADCMEM0; // move result into global
                    ADC_Right_Detect = ADC_Right_Detect >> 2; // divide by 4
                    ADCMCTL0 &= ~ADCINCH_3; // disable last channel A3
                    ADCMCTL0 |= ADCINCH_5; // enable next channel A5
                    break;
                case 0x02:
                    ADC_Thumb_Detect = ADCMEM0; // move result to global
                    ADC_Thumb_Detect = ADC_Thumb_Detect >> 2; // divide by 4
                    ADCMCTL0 &= ~ADCINCH_5; // disable last channel A5
                    ADCMCTL0 |= ADCINCH_2; // enable next channel A2
                    ADC_Channel = 0; // set ADC_Channel low
                    break;
                default:
                    break;
            }
            ADCCTL0 |= ADCENC; // Enable Conversions
            ADCCTL0 |= ADCSC; // Start next sample
            break;
        default:
            break;
    }
}


#pragma vector = TIMER3_B0_VECTOR
__interrupt void Timer3_B0_ISR(void) { // Timer3_B0 enables PWM implementation

    switch (TB3IV) {
        case 0x00: break;  // No interrupt
        case 0x02:         // CCR1 left forward

            break;
        case 0x04:         // CCR2 right forward

            break;
        case 0x06:         // CCR3 left reverse

            break;
        case 0x08:         // CCR4 right reverse

            break;
        case 0x0A:         // CCR5 lcd

            break;
        case 0x0E:         // Overflow
            break;
        default:
            break;
    }
}



#pragma vector = EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
    char iot_receive; // character received from IOT
    switch(__even_in_range(UCA0IV,0x08)){
        case 0: break;
        case 2:{
            iot_receive = UCA0RXBUF; // UCA0RXBUF holds character of the string transmitted from IOT
            A0_RX_BUF[iot_rx_wr++] = iot_receive; // set receive buffer at index iot_rx_wr to received char
            if(iot_rx_wr >= sizeof(A0_RX_BUF)){ // if index exceeds size of the receive buffer
                iot_rx_wr = BEGINNING; // set index back to 0
            }
            UCA1TXBUF = iot_receive; // transmit char to A1 transfer buffer
        }break;
        case 4:{
            UCA0TXBUF = A0_TX_BUF[pb_index]; // set transfer val equal to char received in A0 transfer buffer
            A0_TX_BUF[pb_index++] = 0; // clear next character in transfer buffer array
            if(A0_TX_BUF[pb_index] == 0x00){ // if current index is 0x00
                UCA0IE &= ~UCTXIE; // disable transfer interrupt
                pb_index = 0; // reset buffer index for next message
            }
        }break;
        default:
            break;
    }
}

#pragma vector = EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
        char usb_value;
            switch(__even_in_range(UCA1IV,0x08)){
                case 0: break;
                case 2:{
                    pc_power_on = TRUE; // indicates CPU is online
                    usb_value = UCA1RXBUF; // UCA1RXBUF holds character of the string transmitted from USB/PC
                    A1_RX_BUF[usb_rx_wr++] = usb_value; // set receive buffer at index usb_rx_wr to received char
                    if(usb_rx_wr >= sizeof(A1_RX_BUF)){ // if index exceeds size of the receive buffer
                        usb_rx_wr = BEGINNING; // set index back to 0
                    }
                    UCA0TXBUF = usb_value; // transmit char to A0 transfer buffer
                }break;
                case 4:{
                    UCA1TXBUF = A1_TX_BUF[direct_iot]; // set transfer val equal to char received in A1 transfer buffer
                    A1_TX_BUF[direct_iot++] = 0; // clear next character in transfer buffer array
                    if(direct_iot >= sizeof(A1_TX_BUF)){ // if index exceeds size of the transfer buffer
                        direct_iot = BEGINNING; // set direct_iot back to starting array position [0]
                    }
                    if(A1_TX_BUF[direct_iot] == 0x00){ // if current index is 0x00
                        UCA1IE &= ~UCTXIE; // disable transfer interrupt
                        direct_iot = 0; // reset buffer index for next message
                    }
                }break;
                default:
                    break;
            }
}

void ResetIOT(void) {
    iot_reset_counter = RESET_STATE; // reset iot counter
    while(iot_reset_counter < IOT_RESET_NUM) { // for specified amount of time to have the iot device off
        P3OUT &= ~IOT_RN_CPU; // disable the IOT
    }
    P3OUT |= IOT_RN_CPU; // turn on IOT
    iot_reset = TRUE; // flag indicating IOT has been reset
}



