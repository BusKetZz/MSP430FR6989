/*
 * Program that turn on the red LED for 3 seconds and then
 * turns off the red LED for 1 second before repeating
 */

#include <msp430.h>

#define RED_LED             0x0001              // P1.0 is the red LED
#define ENABLE_PINS         0xFFFE              // Used to enable pins as inputs and outputs
#define STOP_WATCHDOG       0x5A80              // Used to stop watchdog timer
#define ACLK                0x0100              // It tells the counter to use Auxiliary Clock (ACLK) as its timing source
#define UP                  0x0010              // It puts the peripheral timer into UP mode
#define TAIFG               0x0001              // It checks the Timer A Interrupt Flag

int main()
{
    WDTCTL  =   STOP_WATCHDOG;                  // Disable the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                    // Allow to use inputs and outputs

    P1DIR   =   RED_LED;                        // Set the red LED as an output
    P1OUT = RED_LED;                            // Turn on the LED at the beginning

    TA0CTL  =   ACLK | UP;                      // It setups the Timer_A0 peripheral to count UP
                                                // and how fast to count

    TA0CCR0 =   40000;                          // It tells the Timer_A0 to count up to 40 000

    unsigned char intervals = 0;                // It counts the number of counts of 40 000


    while(1)                                    // Infinite loop
    {

        if(TA0CTL & TAIFG)                      // TRUE if the counter counts to 40 000
        {
            intervals++;                        // Increment number of counts of 40 000
            TA0CTL = TA0CTL & (~TAIFG);         // Reset the Timer_A0 peripheral counter and count again
                                                // from 0 to 40 000

            if(P1OUT & RED_LED)                 // If red LED is on
            {
                if(intervals == 3)              // If counted 3 * 40 000 = 120 000
                {
                    P1OUT = 0x00;               // Turn off the red LED for 1 second
                    intervals = 0;              // Reset the interval value, and count again from 0 t0 3
                }
            }
            else
            {
                P1OUT = RED_LED;                // Turn on the red LED
                intervals = 0;                  // Reset the interval value, and count again
            }
        }
    } // end while(1)

} // end main()
