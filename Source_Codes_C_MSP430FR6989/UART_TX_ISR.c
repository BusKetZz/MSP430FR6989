#include <msp430.h>

#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define UART_CLK_SEL        0x0080              // Specifies accurate clock for UART peripheral
#define BR0_FOR_9600        0x34                // Value required to use 9600 baud
#define BR1_FOR_9600        0x00                // Value required to use 9600 baud
#define CLK_MOD             0x4911              // Microcontroller will "clean-up" clock signal

// ********************************************************************************************
// Function Prototypes

void select_clock_signals(void);                // Assigns microcontroller clock signals
void assign_pins_to_uart(void);                 // P4.2 is for TXD, P4.3 is for RXD
void use_9600_baud(void);                       // UART operates at 9600 bits/second

void main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;                  // Stop the watchdog timer
    PM5CTL0 = ENABLE_PINS;                      // Enable pins

    P1DIR   = BIT0;                             // Make P1.0 (red LED) an output
    P1OUT   = 0x00;                             // Red LED initially off

    select_clock_signals();                     // Assigns microcontroller clock signals
    assign_pins_to_uart();                      // P4.2 is for TXD, P4.3 is for RXD
    use_9600_baud();                            // UART operates at 9600 bits/second

    UCA0IE  = UCTXCPTIE;                        // Interrupt when TX stop bit complete

    _BIS_SR(GIE);                               // Activate enabled UART TXD interrupt

    UCA0TXBUF = 0x56;                           // Send the UART message 0x56 out pin P4.2

    while(1);                                   // Wait here for UART interrupt
}

// ********************************************************************************************
// UART Interrupt Service Routine
// This is the ISR for both the TX interrupt and the RX interrupt

#pragma vector = USCI_A0_VECTOR
__interrupt void UART_ISR(void)
{
    P1OUT ^= BIT0;                              // Turn on the red LED
    UCA0IFG = UCA0IFG & (~UCTXCPTIFG);          // Clear TX ComPleTe Interrupt FlaG
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
// Used to Give UART Control of Appropriate Pins

void assign_pins_to_uart(void)
{
    P4SEL1 = 0x00;
    P4SEL0 = BIT3 | BIT2;   // BIT2 assigns P4.2 to UART Transmit (TXD)
                            // BIT3 assigns P4.3 to UART Receive  (RXD)
}

// ********************************************************************************************
// Specify UART Baud Rate

void use_9600_baud(void)
{
    UCA0CTLW0 = UCSWRST;                    // Put UART into SoftWare ReSeT
    UCA0CTLW0 = UCA0CTLW0 | UART_CLK_SEL;   // Specifies clock source for UART
    UCA0BR0   = BR0_FOR_9600;               // Specifies bit rate (baud) of 9600
    UCA0BR1   = BR1_FOR_9600;               // Specifies bit rate (baud) of 9600
    UCA0MCTLW = CLK_MOD;                    // "Cleans" clock signal
    UCA0CTLW0 = UCA0CTLW0 & (~UCSWRST);     // Takes UART out of SoftWare ReSeT
}
