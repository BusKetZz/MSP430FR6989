#include <msp430.h>

#include <string.h>
#include <driverlib.h>
#include "myGpio.h"
#include "myClocks.h"
#include "myLcd.h"

// ****************************************************************************************
// Function Prototypes

void ScrollWords(char word[250]);               // Works for messages of up 250 characters

// ****************************************************************************************

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;                  // Stop the watchdog timer

    initGPIO();                                 // Initialize General Purpose Inputs and Outputs
    initClocks();                               // Initialize clocks for the LCD
    myLCD_init();                               // Prepare LCD for receive commands

    ScrollWords("I LOVE YOU SYLWIA");          // Scroll this message across the LCD

    while(1);
}

// ****************************************************************************************
// Function Definitions

void ScrollWords(char words[250])
{
    unsigned int length;                        // Contains length of the message to be displayed
    unsigned int slot;                          // Slot to be displayed on LCD: 1, 2, 3, 4, 5 or 6
    unsigned int amount_shifted;                // Number of times message shifted so far
    unsigned int offset;                        // Used with amount_shifted to get correct character to display
    unsigned long delay;                        // Used to implement delay between scrolling iterations
    unsigned char next_char;                    // Next character from message to be displayed

    length = strlen(words);                     // Get the length of the message stored in words
    amount_shifted = 0;                         // Message is not shifted yet
    offset = 0;                                 // There is no offset yet

    while(amount_shifted <= length + 6)         // Loop as long as you haven't shifted all
    {                                           // of the characters off the LCD screen

        offset = amount_shifted;                // Starting point in message for next LCD update

        for(slot=1; slot<=6; slot++)            // Loop 6 times to display 6 characters at a time
        {
            next_char = words[offset-6];        // Get the current character for LCD slot

            if(next_char && (offset>=6) && (offset<=length+6))  // If character is not null             AND
            {                                                   // LCD is not filled (offset>=6)        AND
                                                                // You have not reached end of message
                                                                // (offset<=length+6)

                myLCD_showChar(next_char, slot);                // Show the next character on the LCD
                                                                // screen in correct slot
            }
            else                                                // Else, slot on LCD should be blank
            {
                myLCD_showChar(' ', slot);                      // Add a blank space to slot
            }

            offset++;                                           // Update as you move across the message
        }

        for(delay=0; delay<123456; delay++);                    // Delay between shifts

        amount_shifted++;                                       // Update times words shifted across LCD
    }

}
