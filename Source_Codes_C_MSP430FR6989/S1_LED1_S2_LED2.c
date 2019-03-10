/*
 * Following program will turn on the red LED if P1.1 push-button (S1)
 * is pressed while independently allowing P1.2 push-button (S2) to turn on
 * the green LED
 *
 */

#include <msp430.h>

#define RED_ON      0b00000001      // Used to turn-on/enable P1.0 red LED
#define RED_OFF     0b11111110      // Used to turn-off the P1.0 red LED
#define GREEN_ON    0b10000000      // Used to turn-on/enable P9.7 green LED
#define GREEN_OFF   0b01111111      // Used to turn-off the P9.7 green LED
#define BUTTONS     0b00000110      // P1.1 and P1.2 are the S1 and S2 buttons
#define BUTTON11    0b00000010
#define BUTTON12    0b00000100

#define DEVELOPMENT 0x5A80          // Stop the watchdog timer
#define ENABLE_PINS 0xFFFE          // Enable I/O of the pins

int main()
{
    WDTCTL  =   DEVELOPMENT;    // Stop the watchdog for development process

    PM5CTL0 =   ENABLE_PINS;    // Required to use I/O

    P1DIR   =   RED_ON;         // Pin connected to Red LED (P1.0) will be an output
    P9DIR   =   GREEN_ON;       // Pin connected to Green LED (P9.7) will be an output
    P1OUT   =   BUTTONS;        // Buttons need a pull-up resistor
    P1REN   =   BUTTONS;


    while(1)    // Keep looping forever
    {
        while((BUTTON11 & P1IN) == 0)   // Is P1.1 button pushed?
        {
            P1OUT = P1OUT | RED_ON;     // Turn on the Red LED

            if((BUTTON12 & P1IN) == 0)  // Is P1.2 button pushed?
            {
                P9OUT = P9OUT | GREEN_ON;   // Turn on the Green LED
            }
            P9OUT = P9OUT & GREEN_OFF;  // Turn off the Green LED
        }
        P1OUT = P1OUT & RED_OFF;    // Turn off the Red LED

    }
}
