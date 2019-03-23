#include <msp430.h>
#include <driverlib.h>
#include <string.h>

#include "myGpio.h"
#include "myClocks.h"
#include "myLcd.h"

// **************************************************************************************
// Function Prototypes

void DisplayWord(char word[6]);                 // Display words (6 characters or less)


int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;                  // Stop the watchdog timer

    initGPIO();                                 // Initialize General Purpose Inputs and Outputs for LCD
    initClocks();                               // Initialize clocks for LCD
    myLCD_init();                               // Prepare LCD to receive commands

    DisplayWord("MSP430");                      // Display word on LCD

    while(1);
}


// **************************************************************************************
// Function Definitions

void DisplayWord(char word[6])
{
    unsigned int length;                        // Used to store length of the word
    unsigned int i;                             // Used to "step" through word, 1 character at a time
    char next_char;                             // The character in word presently displaying

    length = strlen(word);                      // Get length of the desired word

    if(length <= 6)                             // If 6 or less characters
    {
        for(i=0; i <= length; i++)              // Loop through each of characters
        {
            next_char = word[i];                // Get character for the ith slot

            if(next_char)                       // Check if character exists (not null)
            {
                myLCD_showChar(next_char, i+1); // Show character on LCD at proper place
            }
        }
    }
    else                                        // Else, word hase more than 6 characters
    {
        myLCD_showChar('E', 1);
        myLCD_showChar('R', 2);
        myLCD_showChar('R', 3);
        myLCD_showChar('O', 4);
        myLCD_showChar('R', 5);
        myLCD_showChar(' ', 6);
    }
}
