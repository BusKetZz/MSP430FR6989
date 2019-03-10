#include <msp430.h>               // Allows to use "short-cut" names

#define ENABLE_PINS     0xFFFE  // Used to enable microcontroller's pins
#define DEVELOPMENT     0x5A80  // Used to disable some of the security features

int main()
{
    WDTCTL  =   DEVELOPMENT;    // Disables some security features

    PM5CTL0 =   ENABLE_PINS;    // Enables the pins to the outside world

    P9DIR   =   BIT6;           // P9.6 will be connected to the output (yellow LED)

    P9OUT   =   BIT5;           // Enables pull-up resistor for pin 5
    P9REN   =   BIT5;

    while(1)                    // Continuously repeat everything below
    {
        if((BIT5 & P9IN) == 0)  // If button connected to P9.5 is pushed (Logic 0)
        {
            P9OUT = P9OUT | BIT6; // Turn on the new yellow LED
        }
        else    // Else, if P9.5 is not pushed (Logic 1)
        {
            P9OUT = BIT5;   // Turn off the yellow LED, but leave the P9.5 pull-up resistor in place
        }

    }
}
