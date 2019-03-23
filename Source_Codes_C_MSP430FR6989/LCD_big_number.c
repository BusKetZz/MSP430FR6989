#include <msp430.h>

#include <driverlib.h>                  // Required for the LCD
#include "myGpio.h"                     // Required for the LCD
#include "myClocks.h"                   // Required for the LCD
#include "myLcd.h"                      // Required for the LCD

int main(void)
{
    signed long i=0;                    // Number to be displayed
    unsigned long j=0;                  // For delay

    WDTCTL  = WDTPW | WDTHOLD;          // Stop the watchdog timer

    initGPIO();                         // Initialize General Purpose Inputs and Outputs for LCD
    initClocks();                       // Initialize clocks for LCD
    myLCD_init();                       // Prepare LCD to receive commands

    while(1)
    {
        myLCD_displayNumber(i);         // Display the number
        i+=1;                           // Increment the number

        for(j=0; j<123456; j++);        // Delay
    }

}
