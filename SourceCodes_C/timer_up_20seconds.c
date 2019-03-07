/*
 * Program that turns on the red LED for approximately 20 seconds
 * and then turns it off.
 * The program should then keep the red LED off forever
 * (or, at least until user stop the program)
 *
 */

#include <msp430.h>

#define RED_LED         0x0001              // P1.0 is the red LED
#define ENABLE_PINS     0xFFFE              // Required to use inputs and outputs
#define STOP_WATCHDOG   0x5A80              // Stop the watchdog timer
#define ACLK            0x0100              // Timer_A ACLK source
#define UP              0x0010              // Timer_A UP mode
#define TAIFG           0x0001              // Used to look at Timer A Interrupt Flag

int main()
{
    WDTCTL  =   STOP_WATCHDOG;              // Stop the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                // Allow to use inputs and outputs

    unsigned char intervals = 0;            // It counts number of iteration of 40 000 counts
    TA0CCR0 = 40000;                        // Value stored in this register will be compared with
                                            // the peripheral count value

    TA0CTL  =   ACLK | UP;                  // Timer_A0 Control Register is set up to use ACLK
                                            // for UP mode

    P1DIR   =   RED_LED;                    // Set red LED (P1.0) as an output

    P1OUT   =   RED_LED;                    // Turn on the red LED at the beginning

    while(1)                                // Infinite loop (stay here forever)
    {
        if(TA0CTL & TAIFG)                  // Check if the Interrupt Flag has been set
        {
            intervals++;                    // Increment number of counts of 40 000
            TA0CTL = TA0CTL & (~TAIFG);     // Count again from 0 to 40 000

            if(intervals == 20)              // If counted 20 * 40 000 = 800 000
            {
                P1OUT = P1OUT ^ RED_LED;    // Turn off the red LED
                while(1);                   // Stay here forever
            }
        }
    }
}
