/*
 * Every 10ms, Timer 0 causes the watchdog to be petted.
 *
 * After ten periods of 10ms elapses, Timer 0 causes the red LED to toggle.
 *
 * After three periods of 1s elapses, Timer 1 causes the green LED to toggle.
 *
 */

#include <msp430.h>

#define RED_LED             0x0001              // P1.0 is the red LED
#define GREEN_LED           0x0080              // P9.7 is the green LED
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define STOP_WATCHDOG       0x5A80              // Stop the watchdog timer
#define PET_WATCHDOG        0x5A08              // WDT password and pet
#define ACLK                0x0100              // Timer_A ACLK source
#define UP                  0x0010              // Timer_A UP mode
#define TAIFG               0x0001              // Used to check the Timer A Interrupt Flag

int main()
{
    unsigned char t0_count = 0;
    unsigned char t1_count = 0;

    PM5CTL0 =   ENABLE_PINS;                    // Enable inputs and outputs

    TA0CCR0 =   400;                            // Count up from 0 to 400 (~10ms)
    TA0CTL  =   ACLK | UP;                      // Use ACLK for UP mode
    TA1CCR0 =   40000;                          // Count up from 0 to 40000 (~1s)
    TA1CTL  =   ACLK | UP;                      // Use ACLK for UP mode

    P1DIR   =   RED_LED;                        // Set red LED as an output
    P9DIR   =   GREEN_LED;                      // Set green LED as an output

    while(1)
    {
       if(TA0CTL & TAIFG)                       // If Timer_A0 has counted to 400
       {
           WDTCTL   =   PET_WATCHDOG;           // Pet the watchdog
           TA0CTL   =   TA0CTL & (~TAIFG);      // Clear the flag and count again
           t0_count++;                          // Increment 10ms counts

           if(t0_count == 10)                   // If ~100ms has elapsed
           {
               t0_count = 0;                    // Reset 10ms counter
               P1OUT ^= RED_LED;                // Toggle the red LED
           }

       } // end Timer_A0 if

       if(TA1CTL & TAIFG)                       // If Timer_A1 has counted to 5000
       {
           TA1CTL   =   TA1CTL & (~TAIFG);      // Clear the flag and count again
           t1_count++;                          // Increment 1s counts

           if(t1_count == 3)                    // If ~3s has elapsed
           {
               t1_count = 0;                    // Reset 1s counter
               P9OUT ^= GREEN_LED;              // Toggle the green LED
           }

       } // end Timer_A1 if

    } // end while(1)

} // end main()
