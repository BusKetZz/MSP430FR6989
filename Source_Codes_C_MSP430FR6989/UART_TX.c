#include <msp430.h>

#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs
#define UART_CLK_SEL        0x0080              // Specifies accurate SMCLK clock for UART
#define BR0_FOR_9600        0x34                // Value required to use 9600 baud rate
#define BR1_FOR_9600        0x00                // Value required to use 9600 baud rate
#define CLK_MOD             0x4911              // Microcontroller will "clean-up" clock signal

// *********************************************************************************************
// Function Prototypes

void select_clock_signals(void);                // Assigns microcontroller clock signals
void assign_pins_to_uart(void);                 // P4.2 is for TXD, P4.3 is for RXD
void use_9600_baud(void);                       // UART operates at 9600 bits/second


void main(void)
{
    WDTCTL  =   WDTPW | WDTHOLD;                // Stop the watchdog timer
    PM5CTL0 =   ENABLE_PINS;                    // Enable inputs and outputs pins

    select_clock_signals();
    assign_pins_to_uart();
    use_9600_baud();

    UCA0TXBUF = 0x56;                           // Send the UART message 0x56 out pin P4.2

    while(1);
}

// *********************************************************************************************
// Select Clock Signals
void select_clock_signals(void)
{
    CSCTL0  =   0xA500;                         // "Password" to access clock calibration registers
    CSCTL1  =   0x0046;                         // Specifies frequency of main clock
    CSCTL2  =   0x0133;                         // Assigns additional clock signals
    CSCTL3  =   0x0000;                         // Use clocks at inteded frequency, do not slow them down
}

// *********************************************************************************************
// Used to give UART control of appropriate pins
void assign_pins_to_uart(void)
{
    P4SEL1  =   0x00;                           // 0000 0000
    P4SEL0  =   BIT3 | BIT2;                    // 0000 1100
                                                // BIT2 assigns P4.2 to UART Transmit (TXD)
                                                // BIT3 assigns P4.3 to UART Receive (RXD)
}

// *********************************************************************************************
// Specify UART baud rate
void use_9600_baud(void)
{
    UCA0CTLW0   =   UCSWRST;                    // Put UART into SoftWare ReSeT
    UCA0CTLW0   =   UCA0CTLW0 | UART_CLK_SEL;   // Specifies clock source for UART
    UCA0BR0     =   BR0_FOR_9600;               // Specifies bit rate (baud) of 9600
    UCA0BR1     =   BR1_FOR_9600;               // Specifies bit rate (baud) of 9600
    UCA0MCTLW   =   CLK_MOD;                    // "Cleans" clock signal
    UCA0CTLW0   =   UCA0CTLW0 & (~UCSWRST);     // Take UART out of SoftWare ReSeT
}
