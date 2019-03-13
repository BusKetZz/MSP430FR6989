#include <msp430.h>

#define STOP_WATCHDOG           0x5A80          // Stop the watchdog timer
#define ENABLE_PINS             0xFFFE          // Required to use inputs and outputs
#define ACLK                    0x0100          // Timer ACLK source
#define UP                      0x0010          // Timer UP mode
#define SLOW                    0x00C0          // Slow down ACLK by factor of 8

int main()
{
    WDTCTL  =   STOP_WATCHDOG;                  // Stop the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                    // Required to use inputs and outputs
    P1DIR   =   BIT0;                           // Set pin for red LED as an output
    P1OUT   =   0x00;                           // Make sure red LED is off to start

    TA0CCR0 =   40000;                          // 40K*200us ~ 8 seconds to ISR
    TA0CTL  =   ACLK | UP | SLOW;               // Set ACLK, UP mode and slow down incrementing
    TA0CCTL0=   CCIE;                           // Enable interrupt for Timer0

    _BIS_SR(LPM0_bits | GIE);                   // Enter Low Power Mode 0 and activate interrupts

    while(1);
}

// ************************************************************************************************
// Timer0 Interrupt Service Routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_ISR(void)
{
    if(TA0CCR0 == 40000)                        // If LED was off for 1 second
    {
        TA0CCR0 =   125;                        // Then turn on LED for short time
        P1OUT   =   BIT0;
    }
    else                                        // Else LED was on for a short time
    {
        TA0CCR0 =   40000;                      // Then turn off LED for long time
        P1OUT   =   0x00;
    }
}
