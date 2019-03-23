#include <msp430.h>

#include <driverlib.h>                          // Required for the LCD
#include "myGpio.h"                             // Required for the LCD
#include "myCLocks.h"                           // Required for the LCD
#include "myLcd.h"                              // Required for the LCD

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;                  // Stop watchdog timer

    initGPIO();                                 // Initialize General Purpose Inputs and Outputs for LCD

    initClocks();                               // Initialize clocks for LCD

    myLCD_init();                               // Prepare LCD to receive commands

    myLCD_showChar('H', 1);                     // Display "H" in space 1
    myLCD_showChar('E', 2);                     // Display "E" in space 2
    myLCD_showChar('L', 3);                     // Display "L" in space 3
    myLCD_showChar('L', 4);                     // Display "L" in space 4
    myLCD_showChar('O', 5);                     // Display "O" in space 5
    myLCD_showChar(' ', 6);                     // Display blank space in space 6

    while(1);
}
