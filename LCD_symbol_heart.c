#include <msp430.h>

#include <driverlib.h>                  // Required for the LCD
#include "myGpio.h"                     // Required for the LCD
#include "myClocks.h"                   // Required for the LCD
#include "myLcd.h"                      // Required for the LCD

int main(void)
{
    unsigned long i;                    // Use for delay

    WDTCTL  = WDTPW | WDTHOLD;          // Stop the watchdog timer

    initGPIO();                         // Initialize General Purpose Inputs and Outputs for LCD

    initClocks();                       // Initialize clocks for LCD

    myLCD_init();                       // Prepare LCD to receive commands

    while(1)
    {
        myLCD_showSymbol(LCD_TOGGLE, LCD_HRT, 0);       // Toggle heart symbol
        myLCD_showSymbol(LCD_TOGGLE, LCD_ANT, 0);       // Toggle antenna symbol
        myLCD_showSymbol(LCD_TOGGLE, LCD_TMR, 0);       // Toggle stopwatch symbol
        myLCD_showSymbol(LCD_TOGGLE, LCD_A4COL, 0);     // Toggle colon symbol

        for(i=0; i<234567; i++);                        // Delay
    }
}
