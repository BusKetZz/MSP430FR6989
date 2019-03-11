#include <msp430.h>

#define RED_LED             0x0001              // P1.0 is the red LED
#define GREEN_LED           0x0080              // P9.7 is the green LED
#define STOP_WATCHDOG       0x5A80              // Stop the watchdog timer
#define ACLK                0x0100              // Timer_A ACLK source
#define UP                  0x0010              // Timer_A UP mode
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs

int main()
{
    WDTCTL  =   STOP_WATCHDOG;                  // Stop the watchdog timer

    PM5CTL0 =   ENABLE_PINS;                    // Required to use inputs and outputs
    P1DIR   =   RED_LED;                        // Set red LED as an output
    P9DIR   =   GREEN_LED;                      // Set green LED as an output

    TA0CCR0 =   20000;                          // Set value of Timer_0
    TA0CTL  =   ACLK | UP;                      // Set ACLK for UP mode for Timer_0
    TA0CCTL0 =  CCIE;                           // Enable interrupt for Timer_0

    TA1CCR0 =   3000;                           // Set value of Timer_1
    TA1CTL  =   ACLK | UP;                      // Set ACLK for UP mode for Timer_1
    TA1CCTL0 =  CCIE;                           // Enable interrupt for Timer_1

    _BIS_SR(GIE);                               // Activate interrupts previously enabled

    while(1);                                   // Wait here for interrupt
}

// ****************************************************************************************
// Timer_0 Interrupt Service Routine
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_ISR(void)
{
    P1OUT = P1OUT ^ RED_LED;                    // Toggle red LED
}

// ****************************************************************************************
// Timer_1 Interrupt Service Routine
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_ISR(void)
{
    P9OUT = P9OUT ^ GREEN_LED;                  // Toggle green LED
}
