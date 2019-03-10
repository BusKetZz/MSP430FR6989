#include <msp430.h>

#define RED_LED         0x0001              // P1.0 is the red LED
#define ENABLE_PINS     0xFFFE              // Required to use inputs and outputs
#define STOP_WATCHDOG   0x5A80              // Stop the watchdog timer
#define ACLK            0x0100              // Timer_A ACLK source
#define UP              0x0010              // Timer_A UP mode
#define TAIFG           0x0001              // Used to look at Timer A Interrupt Flag

int main()
{
    unsigned char intervals = 0;            // Count number of 50 000 counts (from 0 to 10)

    WDTCTL  =   STOP_WATCHDOG;              // Stop the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                // Enable inputs and outputs

    TA0CCR0 =   50000;                      // Count up from 0 to 50000
    TA0CTL  =   ACLK | UP;                  // Use ACLK for UP mode

    P1DIR   =   RED_LED;                    // Set red LED as an output

    while(1)
    {
        if(TA0CTL & TAIFG)                  // If timer has counted to 50 000
        {
            intervals = intervals + 1;      // Update number of 50 000 counts
            TA0CTL = TA0CTL & (~TAIFG);     // Count again

            if(intervals == 10)             // If counted 10 * 50 000 = 500 000
            {
                intervals = 0;              // Reset interval count
                P1OUT = P1OUT ^ RED_LED;    // Toggle red LED (P1.0)
            }
        }
    }
}
