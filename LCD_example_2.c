#include <msp430.h>

#include <driverlib.h>                      // Required for the LCD
#include "myGpio.h"                         // Required for the LCD
#include "myClocks.h"                       // Required for the LCD
#include "myLcd.h"                          // Required for the LCD

int main(void)
{
    unsigned long i;                        // Use for delay

    WDTCTL  = WDTPW | WDTHOLD;              // Stop the watchdog timer

    initGPIO();                             // Initialize General Purpose Inputs and Outputs for LCD
    initClocks();                           // Initialize clocks for LCD
    myLCD_init();                           // Prepare LCD to receive commands

    myLCD_showSymbol(LCD_UPDATE, LCD_BRACKETS, 0);      // Brackets on

    for(i=0; i<987654; i++);                            // Long delay

    while(1)
    {
        myLCD_showSymbol(LCD_UPDATE, LCD_B1, 0);        // Bar level 1
        for(i=0; i<234567; i++);                        // Delay

        myLCD_showSymbol(LCD_UPDATE, LCD_B2, 0);        // Bar level 2
        for(i=0; i<234567; i++);                        // Delay

        myLCD_showSymbol(LCD_UPDATE, LCD_B3, 0);        // Bar level 3
        for(i=0; i<234567; i++);                        // Delay

        myLCD_showSymbol(LCD_UPDATE, LCD_B4, 0);        // Bar level 4
        for(i=0; i<234567; i++);                        // Delay

        myLCD_showSymbol(LCD_UPDATE, LCD_B5, 0);        // Bar level 5
        for(i=0; i<234567; i++);                        // Delay

        myLCD_showSymbol(LCD_UPDATE, LCD_B6, 0);        // Bar level 6

        for(i=0; i<987654; i++);                        // Long delay

        myLCD_showSymbol(LCD_CLEAR, LCD_B1, 0);         // Clear bar 1
        myLCD_showSymbol(LCD_CLEAR, LCD_B2, 0);         // Clear bar 2
        myLCD_showSymbol(LCD_CLEAR, LCD_B3, 0);         // Clear bar 3
        myLCD_showSymbol(LCD_CLEAR, LCD_B4, 0);         // Clear bar 4
        myLCD_showSymbol(LCD_CLEAR, LCD_B5, 0);         // Clear bar 5
        myLCD_showSymbol(LCD_CLEAR, LCD_B6, 0);         // Clear bar 6

        for(i=0; i<987654; i++);                        // Long delay

    }
}
