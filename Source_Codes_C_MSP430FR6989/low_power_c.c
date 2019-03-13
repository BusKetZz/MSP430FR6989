#include <msp430.h>

#define STOP_WATCHDOG       0x5A80          // Stop the watchdog timer
#define ENABLE_PINS         0xFFFE          // Required to use inputs and outputs
#define ACLK                0x0100          // Timer ACLK source
#define UP                  0x0010          // Timer UP mode

int main()
{
    WDTCTL  =   STOP_WATCHDOG;              // Stop the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                // Required to use inputs and outputs

    P1DIR   =   BIT0;                       // Set pin for red LED as an output
    P1OUT   =   0x00;                       // Make sure red LED is off to start

    P9DIR   =   BIT7;                       // Set pin for green LED as an output
    P9OUT   =   0x00;                       // Make sure green LED is off to start

    TA0CCR0 =   40000;                      // 40K*25us ~ 1 second to ISR
    TA0CTL  =   ACLK | UP;                  // Set ACLK, UP mode
    TA0CCTL0=   CCIE;                       // Enable interrupt for Timer0

    _BIS_SR(LPM0_bits | GIE);               // Enter Low Power Mode 0 and activate interrupts

    P9OUT   =   BIT7;                       // If the microcontroller executes any additional
                                            // instructions in the main() function, it will turn on
                                            // the green LED
    while(1);
}

// **************************************************************************************************
// Timer0 Interrupt Service Routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_ISR(void)
{
    P1OUT   =   P1OUT ^ BIT0;               // Toggle red LED on P1.0
}
