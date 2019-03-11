#include <msp430.h>

#define RED_LED             0x0001              // P1.0 is the red LED
#define STOP_WATCHDOG       0x5A80              // Stop the watchdog timer
#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define ACLK                0x0100              // Timer_A ACLK source
#define UP                  0x0010              // Timer_A UP mode
#define TAIFG               0x0001              // Used to look at Timer A Interrupt Flag

/***************************************************************
 *
 * Function Prototypes
 *
 */

// These functions do not have neither input nor output
void stop_watchdog_timer(void);
void enable_inputs_and_outputs(void);
void timerA0_will_count_up_for_500ms(void);
void make_P10_red_LED_an_output(void);
void toggle_red_LED(void);
void clear_timerA0_elapsed_flag(void);

// This function has an output, but no input
unsigned int timerA0_500ms_elapsed(void);

// *************************************************************

int main()
{
    stop_watchdog_timer();

    enable_inputs_and_outputs();

    timerA0_will_count_up_for_500ms();

    make_P10_red_LED_an_output();

    while(1)
    {
        if(timerA0_500ms_elapsed())
        {
            toggle_red_LED();
            clear_timerA0_elapsed_flag();
        }

    } // end while(1)

} // end main()

/* *************************************************************
 *
 * Function Definitions
 *
 */

void stop_watchdog_timer(void)
{
    WDTCTL  =   STOP_WATCHDOG;          // Disables watchdog timer for development
}

void enable_inputs_and_outputs(void)
{
    PM5CTL0 =   ENABLE_PINS;            // Enables inputs and outputs
}

void timerA0_will_count_up_for_500ms(void)
{
    TA0CCR0 =   20000;                  // ACLK will increment every 25us (0.000025), 20000 * 25us = 0.5 seconds
    TA0CTL  =   ACLK | UP;
}

void make_P10_red_LED_an_output(void)
{
    P1DIR   =   RED_LED;                // Make P1.0 an output
}

void toggle_red_LED(void)
{
    P1OUT   ^=  RED_LED;                // Toggle the red LED
}

void clear_timerA0_elapsed_flag(void)
{
    TA0CTL  =   TA0CTL & (~TAIFG);      // This command first looks at the value of TAIFG
                                        // which was defined as:
                                        // TAIFG = 0x0001 = 0000 0000 0000 0001

                                        // Then, it bit-wise inverts the value
                                        // ~TAIFG = 0xFFFE = 1111 1111 1111 1110

                                        // Then, it bit-wise ANDs the 0xFFFE value with
                                        // the contents of TA0CTL. This clears the TAIFG bit
                                        // (bit 0 of TA0CTL) without modifying any of the other bits
}

unsigned int timerA0_500ms_elapsed(void)
{
    return TA0CTL & TAIFG;              // This takes the bit-wise logic AND of the value defined for TAIFG
                                        // TAIFG = 0x0001 = 0000 0000 0000 0001
                                        // and the contents of the TA0CTL register

                                        // The result will be returned as the output back to the main program
                                        // 0x0000   If TAIFG is LO and the timer has not yet counted up to TA0CCR0
                                        // 0x0001   If TAIFG is HI and the timer has counted up to TA0CCR0
}
