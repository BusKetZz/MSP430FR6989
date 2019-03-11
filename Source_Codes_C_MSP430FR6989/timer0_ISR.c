#include <msp430.h>

#define RED_LED             0x0001          // P1.0 is the red LED
#define STOP_WATCHDOG       0x5A80          // Stop the watchdog timer
#define ACLK                0x0100          // Timer ACLK source
#define UP                  0x0010          // Timer UP mode
#define ENABLE_PINS         0xFFFE          // Required to use inputs and outputs

int main()
{
    WDTCTL  =   STOP_WATCHDOG;              // Stop the watchdog timer

    PM5CTL0 =   ENABLE_PINS;                // Required to use inputs and outputs
    P1DIR   =   RED_LED;                    // Set red LED as an output

    TA0CCR0   =   20000;                    // Set value of Timer_0
    TA0CTL    =   ACLK | UP;                // Set ACLK for UP mode
    TA0CCTL0  =   CCIE;                     // Enable interrupt for Timer_0

    _BIS_SR(GIE);                           // Activate interrupts previously enabled

    while(1);                               // Wait here for interrupt
}

// ***********************************************************************************
// Timer0 Interrupt Service Routine

#pragma vector = TIMER0_A0_VECTOR           // The ISR must be put into a special place
                                            // in the microcontroller program memory.

__interrupt void Timer0_ISR (void)          // This officially names this ISR as "Timer0_ISR"
{
    P1OUT = P1OUT ^ RED_LED;                // Toggle the red LED
}                                           // When all the code is here done, the ISR ends and
                                            // the program jumps back to wherever it was before
