// This program will blink a red LED

#include <msp430.h> // Allows to use "short-cut" names
                    // to make code easier to read

#define RED 0x0001 // Specifies the red LED light

#define RED_OFF 0x00FE // Used to turn the red LED off

#define ENABLE_PINS 0xFFFE // Used to enable microcontroller's pins

#define DEVELOPMENT 0x5A80 // Used to disable some of the security features
                           // (only for learning process)

int main() // "Main" block of the program
{
    WDTCTL = DEVELOPMENT; // Disables some security features

    PM5CTL0 = ENABLE_PINS; // Enables the pins to the outside world

    P1DIR = RED; // Makes a pin an output

    long x = 0; // Will be used to slow down blinking

    while(1) // Continuously repeat everything below
    {
        for(x=0; x<30000; x++); // Count from 0 to 30 000 for a delay

        P1OUT = RED; // Turns red LED light on

        for(x=0; x<30000; x++); // Count from 0 to 30 000 for a delay

        P1OUT = RED_OFF; // Turns off the red LED light
    }

}
