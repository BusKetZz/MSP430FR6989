#include <msp430.h>

#define RED_ON      0x0001  // Enable and turn on the red LED
#define RED_OFF     0x0000  // Turn off the red LED
#define DEVELOPMENT 0x5A80  // Stop the watchdog timer
#define ENABLE_IO   0xFFFE  // Used to ensure outputs are ready

int main()
{
    WDTCTL = DEVELOPMENT;   // Stop the watchdog timer

    PM5CTL0 = ENABLE_IO;    // Enable to tun on LED
    P1DIR = RED_ON;         // Red LED pin will be an output
    P1OUT = RED_OFF;        // Start with red LED off

    long x;                 // Creates variable

    for(x=0; x<30000; x++)  // Loop 30 000 times
    {
        P1OUT = P1OUT ^ RED_ON; // Toggle LED when x < 30 000
    }

    P1OUT = RED_OFF;    // Ensures LED is off

    while(1);   // Stay here when complete
}
