/*
 * Tasks to perform:
 *
 * 1) Disable the watchdog timer;
 * 2) Use an interrupt on Timer_0 to toggle the red LED every second;
 * 3) Monitor the status of the P1.1 push-button (do this in the main() function);
 * 4) When the button is pressed, the green LED is on (do this in the main() function);
 * 5) When the button is not pressed, the green LED is off (do this in the main() function).
 *
 */

#include <msp430.h>

#define RED_LED             0x0001              // P1.0 is the red LED
#define GREEN_LED            0x0080             // P9.7 is the green LED
#define GREEN_OFF           0x007F              // Used to turn off the green LED
#define BUTTON11            0x0002              // P1.1 is the push-button S1 (lower left corner)
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define STOP_WATCHDOG       0x5A80              // Stop the watchdog timer
#define ACLK                0x0100              // ACLK source for Timer
#define UP                  0x0010              // UP mode for Timer
#define TAIFG               0x0001              // Inform about status of the flag (HI or LOW)

int main()
{
    WDTCTL  =   STOP_WATCHDOG;                  // Disable the watchdog timer

    PM5CTL0 =   ENABLE_PINS;                    // Used to enable inputs and outputs

    P1DIR   =   RED_LED;                        // Set red LED as an output
    P9DIR   =   GREEN_LED;                      // Set green LED as an output

    P1OUT   =   BUTTON11;                       // Establish the pull-up resistor for the button
    P1REN   =   BUTTON11;

    TA0CCR0 =   40000;                          // Timer_0 will count up to 40000 -> ~1second
    TA0CTL  =   ACLK | UP;                      // Set ACLK source for UP mode
    TA0CCTL0=   CCIE;                           // Enable interrupt for Timer_0

    _BIS_SR(GIE);                               // Activate interrupts previously enabled

    while(1)
    {
        while((BUTTON11 & P1IN) == 0)           // Is P1.1 button pushed
        {
            P9OUT = P9OUT | GREEN_LED;          // Turn on the green LED
        }

        P9OUT = P9OUT & GREEN_OFF;              // Turn off the green LED
    }

}

// ***********************************************************************************************
// Timer0 Interrupt Service Routine

#pragma vector = TIMER0_A0_VECTOR               // The ISR must be put into a special place
                                                // in the microcontroller program memory

__interrupt void Timer0_ISR(void)               // This officially names this ISR as "Timer0_ISR"
{
    P1OUT   =   P1OUT ^ RED_LED;                // Toggle the red LED
}
