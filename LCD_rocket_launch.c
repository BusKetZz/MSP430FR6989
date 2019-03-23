#include <msp430.h>
#include <driverlib.h>                      // Required for the LCD
#include "myGpio.h"                         // Required for the LCD
#include "myClocks.h"                       // Required for the LCD
#include "myLcd.h"                          // Required for the LCD

int main(void)
{
    signed long i, j;

    WDTCTL  = WDTPW | WDTHOLD;              // Stop the watchdog timer

    initGPIO();                             // Initialize General Purpose Inputs and Outputs for LCD
    initClocks();                           // Initialize clocks for LCD
    myLCD_init();                           // Prepare LCD to receive commands

    myLCD_showChar( ' ', 1);                // Display blank space in space 1
    myLCD_showChar( ' ', 2);                // Display blank space in space 2
    myLCD_showChar( ' ', 3);                // Display blank space in space 3
    myLCD_showChar( ' ', 4);                // Display blank space in space 4
    myLCD_showChar( ' ', 5);                // Display blank space in space 5
    myLCD_showChar( ' ', 6);                // Display blank space in space 6

    for(i=9; i>=0; i--)
    {
        myLCD_showChar( i+48, 1);           // Display number from 9 to 0
        for(j=0; j<654321; j++);            // Delay
    }

    myLCD_showChar( 0x42, 1);               // Display B in space 1
    myLCD_showChar( 0x4C, 2);               // Display L in space 2
    myLCD_showChar( 0x41, 3);               // Display A in space 3
    myLCD_showChar( 0x53, 4);               // Display S in space 4
    myLCD_showChar( 0x54, 5);               // Display T in space 5
    myLCD_showChar( 0x20, 6);               // Display blank space in space 6

    for(j=0; j<654321; j++);                // Delay

    myLCD_showChar( 0x4F, 1);               // Display O in space 1
    myLCD_showChar( 0x46, 2);               // Display F in space 2
    myLCD_showChar( 0x46, 3);               // Display F in space 3
    myLCD_showChar( 0x20, 4);               // Display blank space in space 4
    myLCD_showChar( 0x20, 5);               // Display blank space in space 5
    myLCD_showChar( 0x20, 6);               // Display blank space in space 6

    while(1);
}
