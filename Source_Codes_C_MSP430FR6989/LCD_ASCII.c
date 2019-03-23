#include <msp430.h>

#include <driverlib.h>                          // Required for the LCD
#include "myGpio.h"                             // Required for the LCD
#include "myClocks.h"                           // Required for the LCD
#include "myLcd.h"                              // Required for the LCD

int main(void)
{
    unsigned long i;                            // Used to scroll ASCII codes
    unsigned long j;                            // Used to implement simple delay

    WDTCTL  = WDTPW | WDTHOLD;                  // Stop watchdog timer

    initGPIO();                                 // Initialize General Purpose Inputs and Outputs for LCD

    initClocks();                               // Initialize clocks for LCD

    myLCD_init();                               // Prepare LCD to receive commands

    myLCD_showChar(' ', 1);                     // Display blank space in space 1
    myLCD_showChar(' ', 2);                     // Display blank space in space 2
    myLCD_showChar(' ', 3);                     // Display blank space in space 3
    myLCD_showChar(' ', 4);                     // Display blank space in space 4
    myLCD_showChar(' ', 5);                     // Display blank space in space 5
    myLCD_showChar(' ', 6);                     // Display blank space in space 6

    for(i=48; i<91; i++)                        // To scroll through ASCII codes
    {
        myLCD_showChar( i, 1);                  // Display ASCII codes 48-90

        for(j=0; j<345678; j++);                // Delay
    }

    myLCD_showChar(' ', 1);                     // Display blank space in space 1
    myLCD_showChar(' ', 2);                     // Display blank space in space 2
    myLCD_showChar('D', 3);                     // Display D in space 3
    myLCD_showChar('O', 4);                     // Display O in space 4
    myLCD_showChar('N', 5);                     // Display N in space 5
    myLCD_showChar('E', 6);                     // Display E in space 6

    while(1);
}
