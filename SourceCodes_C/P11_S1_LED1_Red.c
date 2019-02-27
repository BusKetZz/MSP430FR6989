#include <msp430.h>

#define RED_LED         0x0001      // P1.0 is the Red LED
#define CLEAR_RED_LED   0x00FE      // Used to turn off the Red LED
#define BUTTON11        0x0002      // P1.1 is the button
#define DEVELOPMENT     0x5A80      // Stop the watchdog timer
#define ENABLE_PINS     0xFFFE      // Required to use inputs and outputs

int main()
{
    WDTCTL  =   DEVELOPMENT;        // Need for the development mode

    PM5CTL0 =   ENABLE_PINS;        // Prepare pins for I/O usage

    P1DIR   =   RED_LED;            // Pin connected to Red LED
                                    // will be an output

    P1OUT   =   BUTTON11;           // Button needs a pull-up resistor
    P1REN   =   BUTTON11;

    while(1)    // Keep looping forever
    {
        while((BUTTON11 & P1IN) == 0)   // Is P1.1 button pushed?
        {
            P1OUT = P1OUT   |   RED_LED; // Turn on the Red LED
        }

        P1OUT = P1OUT & CLEAR_RED_LED;  // Turn off the Red LED
    }

}
