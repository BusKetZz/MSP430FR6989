/*
 * Program that turns on the red LED
 * when both buttons S1 and S2 are pressed
 * at the same time
 *
 */

#include <msp430.h>

#define RED_ON      0b00000001      // Used to turn-on/enable P1.0 red LED
#define RED_OFF     0b11111110      // Used to turn-off the P1.0 red LED
#define BUTTONS     0b00000110      // P1.1 and P1.2 are the S1 and S2 buttons

#define DEVELOPMENT 0x5A80          // Stop the watchdog timer
#define ENABLE_PINS 0xFFFE          // Enable I/O of the pins

int main()
{
    WDTCTL  =   DEVELOPMENT;    // Stop the watchdog timer for development process

    PM5CTL0 =   ENABLE_PINS;    // Required to use I/O

    P1DIR   =   RED_ON;         // Pin connected to red LED (P1.0) will be an output
    P1OUT   =   BUTTONS;        // Buttons need a pull-up resistor
    P1REN   =   BUTTONS;

    while(1)    // Keep looping forever
    {
        while((BUTTONS & P1IN) == 0) // Are P1.1 and P1.2 buttons pressed?
        {
            P1OUT = P1OUT | RED_ON; // Turn on the red LED
        }
        P1OUT = P1OUT & RED_OFF;    // Turn off the red LED
    }
}
