#include <msp430.h>

#define GREEN_ON        0b10000000      // P9.7 is the green LED (LED2)
#define GREEN_OFF       0b01111111      // Used to turn off the green LED (LED2)
#define BUTTON12        0b00000100      // P1.2 is the lower-right push button
#define DEVELOPMENT     0x5A80          // Stop the watchdog timer
#define ENABLE_PINS     0xFFFE          // Required to use inputs and outputs

int main()
{
    WDTCTL  =   DEVELOPMENT;            // Need for development mode

    PM5CTL0 =   ENABLE_PINS;            // Prepare pins for I/O usage

    P9DIR   =   GREEN_ON;               // Green LED connected to P9.7 as output
    P1OUT   =   BUTTON12;               // Button needs a pull-up resistor
    P1REN   =   BUTTON12;

    while(1)    // Keep looping forever
    {
        while((BUTTON12 & P1IN) == 0)   // Is P1.2 button pushed?
        {
            P9OUT = P9OUT | GREEN_ON;   // Turn on the green LED
        }

        P9OUT = P9OUT & GREEN_OFF;      // Turn off the green LED
    }
}
