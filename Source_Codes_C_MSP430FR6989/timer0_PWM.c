#include <msp430.h>

#define STOP_WATCHDOG           0x5A80              // Stop the watchdog timer
#define ACLK                    0x0100              // Timer ACLK source
#define UP                      0x0010              // Timer UP mode
#define ENABLE_PINS             0xFFFE              // Required to use inputs and outputs

int main()
{
    WDTCTL  =   STOP_WATCHDOG;                      // Stop the watchdog timer

    PM5CTL0 =   ENABLE_PINS;                        // Required to use inputs and outputs
    P1DIR   =   BIT0;                               // Set red LED as an output
    P1OUT   =   0x00;                               // Start with red LED off

    TA0CCR0 =   45000;                              // Timer0 counts up to 45000
    TA0CTL  =   ACLK | UP;                          // Set ACLK, UP mode for Timer
    TA0CCTL0=   CCIE;                               // Enable interrupt for Timer0

    _BIS_SR(GIE);                                   // Activate interrupts previously enabled

    while(1);                                       // Wait here for interrupt
}

// *******************************************************************************************
// Timer0 Interrupt Service Routine (ISR)
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR(void)
{
    if(TA0CCR0 == 45000)                            // If counted to 45000
    {
        P1OUT   =   BIT0;                           // Turn on red LED
        TA0CCR0 =   5000;                           // Count to 5000 next time
    }
    else                                            // Else, counted to 5000
    {
        P1OUT   =   0x00;                           // Turn off the red LED
        TA0CCR0 =   45000;                          // Count to 45000 next time
    }
}
