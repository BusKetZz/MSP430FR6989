#include <msp430.h>

#define RED_LED         0x0001              // P1.0 is the red LED
#define STOP_WATCHDOG   0x5A80              // Stop the watchdog timer
#define ENABLE_PINS     0xFFFE              // Required to use inputs and outputs
#define ACLK            0x0100              // Timer_A ACLK source
#define UP              0x0010              // Timer_A UP mode
#define TAIFG           0x0001              // Used to look at Timer A Interrupt Flag

int main()
{
    WDTCTL  =   STOP_WATCHDOG;              // Stop the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                // Enable inputs and outputs

    TA0CCR0 =   65535;                      // Count up from 0 to 65 535
    TA0CTL  =   ACLK | UP;                  // Use ACLK for UP mode

    P1DIR   =   RED_LED;                    // Set red LED as an output

    while(1)
    {
        if(TA0CTL & TAIFG)                  // If timer has counted to 65 535
        {
            P1OUT   =   P1OUT  ^ RED_LED;   // Then, toggle red P1.0 LED (on/off)
            TA0CTL  =   TA0CTL & (~TAIFG);  // Count again from 0 to 65 535
        }
    }
}
