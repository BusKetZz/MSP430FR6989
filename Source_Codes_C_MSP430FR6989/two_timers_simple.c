/*
 * Timer_A0 will count up from 0 to 33 000 (33000 * 25us = 0.825 seconds)
 *
 * Timer_A1 will count up from 0 to 5 000 (5000 * 25us = 0.125 seconds)
 *
 */

#include <msp430.h>

#define RED_LED             0x0001              // P1.0 is the red LED
#define GREEN_LED           0x0080              // P9.7 is the green LED
#define STOP_WATCHDOG       0x5A80              // Stop the watchdog timer
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define ACLK                0x0100              // Timer_A ACLK source
#define UP                  0x0010              // Timer_A UP mode
#define TAIFG               0x0001              // Used to check the Timer A Interrupt Flag

int main()
{
    WDTCTL  =   STOP_WATCHDOG;                  // Stop the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                    // Enable inputs and outputs

    TA0CCR0 =   33000;                          // Timer_A0 will count up from 0 to 33000
    TA0CTL  =   ACLK | UP;                      // Use ACLK for UP mode

    TA1CCR0 =   5000;                           // Timer_A1 will count up from 0 to 5000
    TA1CTL  =   ACLK | UP;                      // Use ACLK for UP mode

    P1DIR   =   RED_LED;                        // Set red LED as an output
    P9DIR   =   GREEN_LED;                      // Set green LED as an output

    while(1)
    {
        if(TA0CTL & TAIFG)                      // If Timer_A0 has counted to 33000
        {
            P1OUT   =   P1OUT   ^   RED_LED;    // Toggle the red LED (P1.0)
            TA0CTL  =   TA0CTL  &   (~TAIFG);   // Clear the flag and count again
        }

        if(TA1CTL & TAIFG)                      // If Timer_A1 has counted to 5000
        {
            P9OUT   =   P9OUT   ^   GREEN_LED;  // Toggle the green LED (P9.7)
            TA1CTL  =   TA1CTL  &   (~TAIFG);   // Clear the flag and count again
        }
    } // end while(1)

} // end main()
