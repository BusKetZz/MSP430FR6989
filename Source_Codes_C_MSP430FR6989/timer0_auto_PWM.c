#include <msp430.h>

#define ENABLE_PINS                 0xFFFE              // Required to use inputs and outputs
#define ACLK                        0x0100              // Timer_A ACLK source
#define UP                          0x0010              // Timer_A UP mode

int main()
{
    WDTCTL  =   WDTPW | WDTHOLD;                        // Stop WDT

    PM5CTL0 =   ENABLE_PINS;                            // Enable inputs and outputs

    P1DIR   =   BIT0;                                   // P1.0 will be an output for the red LED

    P1SEL0  =   BIT0;                                   // Gives Timer0 control over P1.0

    TA0CCTL1=   OUTMOD_3;                               // Use Timer mode 3
                                                        // Timer starts at 0, P1.0 starts LO
                                                        // When timer reaches TA0CCR1, P1.0 goes HI
                                                        // When timer reaches TA0CCR0, P1.0 goes LO
                                                        // Count starts over at 0 with P1.0 LO

    TA0CCR1 =   45000;                                  // P1.0 LO from     0 - 45000
    TA0CCR0 =   50000;                                  // P1.0 HI from 45000 - 50000

    TA0CTL  =   ACLK | UP;                              // Count up to TA0CCR1 and TA0CCR0 at 25us/count

    while(1);                                           // Program stays here and never leaves
                                                        // Timer0 handles everything without needing an interrupt service routine

}
