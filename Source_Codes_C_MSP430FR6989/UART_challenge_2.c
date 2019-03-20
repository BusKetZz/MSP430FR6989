/*
 * Task to perform:
 *
 * 1) Repeat UART_Challenge_1 but have the microcontroller pause for approximately 1 second
 *    between each step in countdown.
 */

#include <msp430.h>

#define ENABLE_PINS             0xFFFE          // Required to use inputs and outputs
#define UART_CLK_SEL            0x0080          // Specifies accurate clock for UART peripheral
#define BR0_FOR_9600            0x34            // Value required to use 9600 baud
#define BR1_FOR_9600            0x00            // Value required to use 9600 baud
#define CLK_MOD                 0x4911          // Microcontroller will "clean-up" clock signal
#define ACLK                    0x0100          // ACLK source for Timer
#define UP                      0x0010          // UP mode for Timer
#define ONE_SECOND              0x2710          // 0x2710 = 10000 (decimal) -> 100us each count

// ********************************************************************************************
// Function Prototypes

void select_clock_signals(void);                // Assigns microcontroller clock signals
void assign_pins_to_uart(void);                 // P4.2 is for TXD, P4.3 is for RXD
void use_9600_baud(void);                       // UART operates at 9600 bits/second

// ********************************************************************************************

int main()
{
    WDTCTL  = WDTPW | WDTHOLD;                  // Stop watchdog timer
    PM5CTL0 = ENABLE_PINS;                      // Enable pins

    P1DIR   = BIT0;                             // Make P1.0 (red LED) an output
    P1OUT   = 0x00;                             // Red LED is initially off

    select_clock_signals();
    assign_pins_to_uart();
    use_9600_baud();

    TA0CCR0 = ONE_SECOND;                       // Timer_0 will count up from 0 to 40000 (0x9C40)
    TA0CTL  = ACLK | UP;                        // Set ACLK source for UP mode
    TA0CCTL0= CCIE;                             // Enable interrupt for Timer_0


    _BIS_SR(GIE);                               // Activate enabled UART TXD interrupt

    UCA0TXBUF = 0x0A;                           // Send the UART message 0x0A out pin P4.2

    while(1);                                   // Wait here unless UART interrupt starts
}

// ********************************************************************************************
// Timer_0 Interrupt Service Routine

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_ISR(void)
{
    static unsigned char countdown = 9;         // Countdown starts with 9 because program waits 1 second for interrupt

    if(countdown == 0)                          // If, countdown has been finished
    {
        P1OUT     = BIT0;                       // Turn on red LED (launch racket)
        UCA0TXBUF = 0x00;                       // Transmit "Zero", countdown completed
        TA0CCTL0  = TA0CCTL0 & (~CCIE);         // Disable future Timer interrupts
    }
    else                                        // Else, still counting down
    {
        UCA0TXBUF = countdown;                  // Transmit present count state
        countdown--;                            // Decrement count after every 1 second
    }

}

// ********************************************************************************************
// Select Clock Signals

void select_clock_signals(void)
{
    CSCTL0  = 0xA500;       // "Password" to access clock calibration registers
    CSCTL1  = 0x0046;       // Specifies frequency of main clock
    CSCTL2  = 0x0133;       // Assigns additional clock signals
    CSCTL3  = 0x0000;       // Use clocks at intended frequency, do not slow them down
}

// ********************************************************************************************
// Give UART Control of Appropriate Pins

void assign_pins_to_uart(void)
{
    P4SEL1   = 0x00;
    P4SEL0   = BIT3 | BIT2;
}


// ********************************************************************************************
// Specify UART Baud Rate

void use_9600_baud(void)
{
    UCA0CTLW0   = UCSWRST;                              // Put UART into SoftWare ReSeT
    UCA0CTLW0   = UCA0CTLW0 | UART_CLK_SEL;             // Specifies clock source for UART
    UCA0BR0     = BR0_FOR_9600;                         // Specifies bit rate (baud) of 9600
    UCA0BR1     = BR1_FOR_9600;                         // Specifies bit rate (baud) of 9600
    UCA0MCTLW   = CLK_MOD;                              // "Cleans" clock signal
    UCA0CTLW0   = UCA0CTLW0 & (~UCSWRST);               // Take UART out of SoftWare ReSeT
}
