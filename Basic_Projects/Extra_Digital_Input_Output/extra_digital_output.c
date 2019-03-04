#include <msp430.h>                 // Allows to use "short-cut" names

#define ENABLE_PINS     0xFFFE      // Used to enable microcontroller's pins
#define DEVELOPMENT     0x5A80      // Used to disable some of the security features

int main()
{
    WDTCTL  =   DEVELOPMENT;        // Disables some security features

    PM5CTL0 =   ENABLE_PINS;        // Enables the pins to the outside world

    P9DIR   =   BIT6;               // P9.6 will be connected to yellow LED

    long x;                          // Will be used to slow down blinking

    while(1)                        // Continuously repeat everything below
    {
        for(x = 0; x < 30000; x++); // Count from 0 to 30 000 for a delay

        P9OUT = BIT6;               // Turn on new yellow LED

        for(x = 0; x < 30000; x++); // Count from 0 to 30 000 for a delay

        P9OUT = 0x00;               // Turn off new yellow LED
    }
}

