#include <msp430.h>

#include <driverlib.h>                  // Required for the LCD
#include "myGpio.h"                     // Required for the LCD
#include "myClocks.h"                   // Required for the LCD
#include "myLcd.h"                      // Required for the LCD

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;          // Stop the watchdog timer

    signed long int i = -5;             // Number to be displayed
    unsigned long int j = 0;            // For delay

    initGPIO();                         // Initialize General Purpose Inputs and Outputs for LCD
    initClocks();                       // Initialize clocks for LCD
    myLCD_init();                       // Prepare LCD to receive commands

    while(1)
    {
        if(i < 0)                       // If number is negative
        {
            myLCD_displayNumber(-i);    // Display absolute value
            myLCD_showSymbol(LCD_UPDATE, LCD_NEG, 0);   // Turn on negative sign
        }
        else                            // If number is positive
        {
            myLCD_showSymbol(LCD_CLEAR, LCD_NEG, 0);    // Turn off negative sign
            myLCD_displayNumber(i);     // Display number
        }

        i++;                            // Increment number

        for(j=0; j<654321; j++);        // Delay
    }
}
