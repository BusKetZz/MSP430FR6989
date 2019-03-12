/*
 * Tasks to perform:
 *
 * 1) Do not disable the watchdog timer - instead, set up Timer1 to use an interrupt every 0,01 seconds (10ms) to pet the watchdog;
 * 2) Create a function (not an ISR) to setup the inputs and outputs;
 * 3) Create a function (not an ISR) to setup and start Timer0 counting;
 * 4) Create a function (not an ISR) to setup and start Timer1 counting.
 *
 */

#include <msp430.h>

#define RED_LED                 0x0001              // P1.0 is the red LED
#define GREEN_LED               0x0080              // P9.7 is the green LED
#define GREEN_OFF               0x007F              // Used to turn off the green LED
#define BUTTON11                0x0002              // P1.1 is the push-button S1 (lower left corner)
#define ENABLE_PINS             0xFFFE              // Required to use inputs and outputs
#define PET_WATCHDOG            0x5A08              // WDT password and pet
#define ACLK                    0x0100              // ACLK source for Timer
#define UP                      0x0010              // UP mode for Timer

// *******************************************************************************************************
// Function Prototypes
void enable_pins_LEDs_button(void);
void set_timer0(void);
void set_timer1(void);



int main()
{
   enable_pins_LEDs_button();
   set_timer0();
   set_timer1();

   _BIS_SR(GIE);                                    // Activate interrupt previously enabled

   while(1)
   {
       while((BUTTON11 & P1IN) == 0)                // Is P1.1 button pushed
       {
           P9OUT = P9OUT | GREEN_LED;               // Turn on the green LED
       }

       P9OUT = P9OUT & GREEN_OFF;                   // Turn off the green LED
   }

}

// *******************************************************************************************************
// Timer0 Interrupt Service Routine
#pragma vector=TIMER0_A0_VECTOR                  // ISR is put into special place in microcontroller program memory

__interrupt void Timer0_ISR(void)               // This officially names this ISR as "Timer0_ISR"
{
    P1OUT = P1OUT ^ RED_LED;                    // Toggle the red LED
}

#pragma vector=TIMER1_A0_VECTOR

__interrupt void Timer1_ISR(void)
{
    WDTCTL  =   PET_WATCHDOG;                   // Pet watchdog
}


// *******************************************************************************************************
// Function Definitions
void enable_pins_LEDs_button(void)
{
    PM5CTL0 =   ENABLE_PINS;
    P1DIR   =   RED_LED;                        // Set red LED as an output
    P9DIR   =   GREEN_LED;                      // Set green LED as an output

    P1OUT   =   BUTTON11;                       // Establish the pull-up resistor for the button
    P1REN   =   BUTTON11;
}

void set_timer0(void)
{
    TA0CCR0 =   40000;                          // Timer_0 will count up to 40000 -> ~1second
    TA0CTL  =   ACLK | UP;                      // Set ACLK source for UP mode
    TA0CCTL0=   CCIE;                           // Enable interrupt for Timer_0
}

void set_timer1(void)
{
    TA1CCR0 =   400;                            // Timer_1 will count up to 400 -> ~10ms
    TA1CTL  =   ACLK | UP;                      // Set ACLK source for UP mode
    TA1CCTL0=   CCIE;                           // Enable interrupt for Timer_0
}


