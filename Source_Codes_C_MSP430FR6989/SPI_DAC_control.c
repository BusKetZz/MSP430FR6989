// ***********************************************************************************
// TLC5615 is a 10-bit DAC, but it is configured a little bit strangely to be forward
// and backward compatible with other DACs.
//
// For the way we are connecting our DAC to our Launchpad, we can send values between
// 0 and 2047 decimal. The decimal value sent will linearly relate to the DAC analog
// output voltage.
//      Data Sent =     0   VOUT = 0.00V    (0/8 of 5V)
//      Data Sent =   256   VOUT = 0.62V    (1/8 of 5V)
//      Data Sent =   511   VOUT = 1.25V    (1/4 of 5V)
//      Data Sent =  1023   VOUT = 2.50V    (1/2 of 5V)
//      Data Sent =  2047   VOUT = 5.00V    (1/1 of 5V)
//
// Here are the SPI pins:
//       P1.4           UCB0 SPI Clock
//       P1.6           UCB0 SPI SIMO
//       P1.7           UCB0 SPI SOMI (not used in this application)
//       P2.0           Chip Select
// ***********************************************************************************

#include <msp430.h>

#define ENABLE_PINS             0xFFFE          // Required to use inputs and outputs

// ***********************************************************************************
// Function Prototypes

void Setup_GPIO_Pins(void);                     // Setup GPIO pins for SPI operation
void Setup_Clocks(void);                        // Setup clocks for synch operation
void Setup_SPI_B0(void);                        // Setup USCI type B, number 0 for SPI
void Send_SPI_B0_16(unsigned int msg);          // Send 16-bit data on SPI

// ***********************************************************************************
// main() sets everything up and send 1 message out SPI port

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;                  // Stop the watchdog timer
    Setup_GPIO_Pins();                          // Setup GPIO pins for SPI communication
    Setup_Clocks();                             // Setup clocks for synch communication
    Setup_SPI_B0();                             // Setup USCI port type B, number 0 as SPI

    Send_SPI_B0_16(511);                        // Send message out SPI port
                                                // For our DAC, data which can be sent
                                                // varies between 0 - 2047 decimal

    while(1);                                   // Do not anything after message was sent
}

// ***********************************************************************************
// Setup general purpose input and output pins for SPI operation
// SOMI pin will not be use in this application

void Setup_GPIO_Pins(void)
{
    PM5CTL0 = ENABLE_PINS;                      // Enable use of inputs and outputs

    P1SEL0  = P1SEL0 | BIT4;                    // P1.4 ==> SPI clock signal
    P1SEL0  = P1SEL0 | BIT6;                    // P1.6 ==> SPI Slave In / Master Out (SIMO)
    P1SEL0  = P1SEL0 | BIT7;                    // P1.7 ==> SPI Slave Out / Master In (SOMI)

    P2OUT   = BIT0;                             // Want CS to start HI to avoid possible glitches
    P2DIR   = BIT0;                             // Make CS pin an output

    PJSEL0  = BIT4 | BIT5;                      // Enable some clock pins for SPI's synch operation
}

// ***********************************************************************************
// Enable usage of a new clock signal for SPI

void Setup_Clocks(void)
{
    CSCTL0_H= CSKEY >> 8;                                       // Unlock CS registers

    CSCTL1  = DCOFSEL_0;                                        // Set DCO to 1Mhz

    CSCTL2  = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;      // ACLK: 32.768kHz
                                                                // SMCLK will use DCO (1MHz)
                                                                // MCLK  will use DCO (1MHz)

    CSCTL3  = DIVA__1 | DIVS__1 | DIVM__1;                      // Do change any frequencies

    CSCTL4  = CSCTL4 & (~LFXTOFF);                              // Enable 32.768kHz crystal

    do                                                          // Wait here until 32.768kHz clock is ready
    {
        CSCTL5  = CSCTL5  & (~LFXTOFFG);                        // Clear 32.768kHz fault flags
        SFRIFG1 = SFRIFG1 & (~OFIFG);                           // Clear 32.768kHz fault flags
    }while(SFRIFG1 & OFIFG);                                    // Test 32.768kHz oscillator fault flag

    CSCTL0_H = 0;                                               // Lock CS registers
}


// ***********************************************************************************
// Configure USCI_B0 for SPI operation

void Setup_SPI_B0(void)
{
    UCB0CTLW0 = UCSWRST;                                        // Put Universal Communication (UC) peripheral
                                                                // into SoftWare (SW) ReSeT (RST) -- UCSWRST
                                                                // to Disables USCI so it can be setup
                                                                // Most of these modifications can only be made when the
                                                                // USCI is disabled

    UCB0CTLW0 = UCB0CTLW0 | UCMST;                              // Microcontroller will be the master,
                                                                // Master (M) mode SelecTed (ST) -- UCMST

    UCB0CTLW0 = UCB0CTLW0 | UCSYNC;                             // SPI needs clock so SYNChronous mode selected

    UCB0CTLW0 = UCB0CTLW0 | UCCKPH;                             // Specify ClocK (CK) PHase (PH)
                                                                // UCCKPH: data captured on first CLK edge and changed on next

    UCB0CTLW0 = UCB0CTLW0 | UCMSB;                              // Specify the Most Significant Bit of data
                                                                // to be transmitted first -- UCMSB

    UCB0CTLW0 = UCB0CTLW0 | UCSSEL__ACLK;                       // Clock Source (S) is SELected (SEL) to be ACLK

    UCB0BR1   = 0x00;                                           // Set up clock divider to slow data transmission
    UCB0BR0   = 0x02;                                           // Div by 2: CLK is 16.4kHz, data bit 61us wide

    UCB0CTLW0 = UCB0CTLW0 & (~UCSWRST);                         // Take Universal Communication (UC) peripheral
                                                                // out of SoftWare (SW) ReSeT (RST) -- UCSWRST
                                                                // since peripheral setting is done
}

// ***********************************************************************************
// Send 16-bit DataToSend on SPI port in two, 8-bit pieces

void Send_SPI_B0_16(unsigned int DataToSend)
{
    while( (UCB0STATW & UCBUSY) != 0);                          // Check the UCBUSY bit in the UCB0STATW (UCB0 STATus Word) register)
                                                                // This bit will be HI if the peripheral is sending or receiving
                                                                // Therefore, wait here until the UCBUSY bit goes LO

    P2OUT = 0x00;                                               // Pull the Chip Select line LO to tell the DAC to pay attention

    UCB0TXBUF = (DataToSend>>8);                                // Shift 8 most significant bits over to lower 8 slots and then
                                                                // load into the transmit (TX) BUFfer to send

    while( (UCB0IFG & UCTXIFG) == 0);                           // Test the TX IFG flag to see when the 8 most significant bits have been send

    UCB0TXBUF = (DataToSend & 0xFF);                            // Clear out 8 most significant bits, the 8 least significant bits will be sent

    while( (UCB0STATW & UCBUSY) != 0);                          // Wait until signal that everything is complete

    P2OUT = BIT0;                                               // Raise Chip Select line to HI to tell the DAC to stop listening
}
