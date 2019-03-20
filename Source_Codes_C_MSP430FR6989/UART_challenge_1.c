/*
 * Task to perform:
 *
 * 1) Set up the peripheral and enable the transmit interrupt. Then transmit 0x0A (10 decimal).
 * 2) In ISR continue the countdown by sending 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00.
 *    When the countdown reaches 0x00, red LED should turn on.
 */

#include <msp430.h>

#define ENABLE_PINS         0xFFFE          // Required to use inputs and outputs
#define UART_CLK_SEL        0x0080          // Specifies accurate clock for UART peripheral
#define BR0_FOR_9600        0x34            // Value required to use 9600 baud
#define BR1_FOR_9600        0x00            // Value required to use 9600 baud
#define CLK_MOD             0x4911          // Microcontroller will "clean-up" clock signal

// *****************************************************************************************
// Function Prototypes

void select_clock_signals(void);            // Assigns microcontroller clock signals
void assign_pins_to_uart(void);             // P4.2 is for TXD, P4.3 is for RXD
void use_9600_baud(void);                   // UART operates at 9600 bits/second

// *****************************************************************************************

void main()
{
    WDTCTL  = WDTPW | WDTHOLD;              // Stop watchdog timer
    PM5CTL0 = ENABLE_PINS;                  // Enable pins

    P1DIR   = BIT0;                         // Make P1.0 (red LED) an output
    P1OUT   = 0x00;                         // Red LED is initially off

    select_clock_signals();
    assign_pins_to_uart();
    use_9600_baud();

    UCA0IE  = UCTXCPTIE;                    // Interrupt when TX stop bit complete
    _BIS_SR(GIE);                           // Activate enabled UART RXD interrupt

    UCA0TXBUF = 0x0A;                       // Send the UART message 0x0A out pin P4.2

    while(1);                               // Wait here unless UART interrupt comes

}


// *****************************************************************************************
// UART Interrupt Service Routine
// ISR for both the TX interrupt and the RX interrupt

#pragma vector=USCI_A0_VECTOR
__interrupt void UART_ISR(void)
{
    static unsigned countdown = 10;
    countdown--;

    if(countdown == 0)  // when countdown is finished, turn on red LED
    {
        P1OUT     = BIT0;                       // Turn on red LED
        UCA0TXBUF = 0x00;                       // Countdown complete
        UCA0IE    = UCA0IE & (~~UCTXCPTIE);     // Disable future UART interrupts
    }
    else                                        // Countdown is not over
    {
        UCA0TXBUF = countdown;                  // Transmit the next count
    }

    UCA0IFG    = UCA0IFG & (~UCTXCPTIFG);       // Clear TX ComPleTe Interrupt FlaG
}

// *****************************************************************************************
// Select CLock Signals

void select_clock_signals(void)
{
    CSCTL0  = 0xA500;           // "Password" to access clock calibration registers
    CSCTL1  = 0x0046;           // Specify frequency of main clock
    CSCTL2  = 0x0133;           // Assigns additional clock signals
    CSCTL3  = 0x0000;           // Use clocks at intended frequency, do not slow them down
}

// *****************************************************************************************
// Give UART Control of Appropriate Pins

void assign_pins_to_uart(void)
{
    P4SEL1  = 0x00;             // BIT2 assigns P4.2 to UART Transmit (TXD)
    P4SEL0  = BIT3 | BIT2;      // BIT3 assigns P4.3 to UART Receive  (RXD)
}

// *****************************************************************************************
// Specify UART Baud Rate

void use_9600_baud(void)
{
    UCA0CTLW0   = UCSWRST;                      // Put UART into SoftWare ReSeT
    UCA0CTLW0   = UCA0CTLW0 | UART_CLK_SEL;     // Specifies clock source for UART
    UCA0BR0     = BR0_FOR_9600;                 // Specifies bit rate (baud) of 9600
    UCA0BR1     = BR1_FOR_9600;                 // Specifies bit rate (baud) of 9600
    UCA0MCTLW   = CLK_MOD;                      // "Cleans" clock signal
    UCA0CTLW0   = UCA0CTLW0 & (~UCSWRST);       // Takes UART out of SoftWare ReSeT
}
