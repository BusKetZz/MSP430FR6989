// This program will blink a red LED

#include <msp430.h>				    /*
									 * Allows to use "short-cut" names
									 * to make a code easier to read
									 */

#define RED 		0x0001			// Specifies the red LED light for user

#define RED_OFF 	0x00FE			// Used to turn the red LED off

#define ENABLE_RED  0xFFFE			// Used to enable microcontroller's pins

#define DEVELOPMENT 0x5A80			/*
									 * Used to disable some of the security
									 * features (only for learning process)
									 */

int main()							// All C programs have a main function
{
	WDTCTL = DEVELOPMENT;			// Disables some security features

	PM5CTL0 = ENABLE_RED;			// Enables the pins to the outside world

	P1DIR = RED;					// Make a pin an output

	long x = 0;						// Will be used to slow down blinking

	while(1)						// Continuously repeat everything below
	{
		for(x=0; x<10000; x++);		// Count from 0 to 30 000 for a delay

		P1OUT = RED;				// Turn red LED light on

		for(x=0; x<10000; x++);		// Count from 0 to 30 000 for a delay

		P1OUT = RED_OFF;			// Turn off the red LED light
	}
	
}
