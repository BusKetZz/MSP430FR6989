#include <msp430.h>

#define ENABLE_PINS         0xFFFE          // Required to use inputs and outputs

void main(void)
{
    WDTCTL  =   WDTPW   | WDTHOLD;          // Stop the watchdog timer

    PM5CTL0 =   ENABLE_PINS;                // Enable inputs and outputs

    P1DIR   =   BIT0;                       // P1.0 will be output for red LED

    P1OUT   =   BIT1;                       // P1.1 will be input with a pull-up resistor
    P1REN   =   BIT1;                       // Additionally, the red LED will initially be off

    P1IE    =   BIT1;                       // Enable interrupt for P1.1
    P1IES   =   BIT1;                       // Interrupt for transitions from HI -> LO

    P1IFG   =   0x00;                       // Ensure no ISRs are pending

    _BIS_SR(LPM0_bits | GIE);                           // Activate all interrupts

    while(1);                               // Infinite loop

}

// ********************************************************************************************
// Port 1 Interrupt Service Routine
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    unsigned long delay;
    for(delay=0; delay<20000; delay++);      // Wait for bouncing to end

    P1OUT   =   P1OUT ^ BIT0;               // Toggle LED after delay

    P1IFG  &=   (~BIT1);                    // Clear P1.1 interrupt flag
}
