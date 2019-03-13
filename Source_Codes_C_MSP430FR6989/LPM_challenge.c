/*
 * Tasks to perform:
 *
 * 1) Stop the watchdog;
 * 2) Enable P1.0 to be an output with the red LED initially off;
 * 3) Enable P1.1 to be an input for the push-button switch. (Do not forget to enable the pull-up resistor!);
 * 4) Set up the timer to generate an interrupt every 50ms (0.05s). This will require a TA0CCR0 value of 2000;
 * 5) Put the microcontroller into Low Power Mode 0;
 * 6) Every 50ms, the program will jump to the Timer0 interrupt service routine;
 * 7) Each time you are in the ISR, check to see if the P1.1 push-button is pushed;
 * 8) If the button is not pushed, make sure the red LED is off, and end the ISR to go back to main() to return to Low Power Mode;
 * 9) If the button is ever pushed, turn on the red LED and end the ISR to go back to main() to return to Low Power Mode;
 * 10) Keep repeating steps 6-9.
 *
 */

#include <msp430.h>

#define STOP_WATCHDOG           0x5A80          // Stop the watchdog timer
#define ENABLE_PINS             0xFFFE          // Required to use inputs and outputs
#define ACLK                    0x0100          // Timer ACLK source
#define UP                      0x0010          // Timer UP mode
#define BUTTON11                0x0002          // P1.1 is the push-button S1
#define RED_LED                 0x0001          // P1.0 is the red LED
#define RED_OFF                 0x0000          // Used to turn of red LED


int main()
{
    WDTCTL  =   STOP_WATCHDOG;                  // Stop the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                    // Required to use inputs and outputs

    P1DIR   =   RED_LED;                        // Set P1.0 red LED as an output
    P1OUT   =   RED_OFF;                        // Make sure red LED is initially off

    P1OUT   =   BUTTON11;                       // Establish pull-up resistor for the push-button
    P1REN   =   BUTTON11;

    TA0CCR0 =   2000;                           // Timer count up to 2000 -> 2000*25us = 0.05s to ISR
    TA0CTL  =   ACLK | UP;                      // Timer0 ACLK source, UP mode
    TA0CCTL0=   CCIE;                           // Enable Timer0 Interrupt Service Routine

    _BIS_SR(LPM0_bits | GIE);                   // Enter Low Power Mode 0 and activate interrupts

    while(1);


}

//****************************************************************************************************
// Timer0 Interrupt Service Routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_ISR(void)
{
    if((BUTTON11 & P1IN) == 0)                  // If push-button S1 is pushed
    {
        P1OUT   =   P1OUT | RED_LED;            // Turn on red LED and hold pull-up resistor for the push-button
    }
    else                                        // Else push-button S1 is not pushed
    {
        P1OUT   =   P1OUT & (~RED_LED);         // Turn off red LED and hold pull-up resistor for the push-button
    }
}
