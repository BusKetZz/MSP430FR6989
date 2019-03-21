#include <msp430.h>

#define ENABLE_PINS         0xFFFE              // Required to use inputs and outputs

// ***********************************************************************************
// Function Prototypes

void ADC_SETUP(void);                           // Used to setup ADC12 peripheral

// ***********************************************************************************

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;                  // Stop the watchdog timer
    PM5CTL0 = ENABLE_PINS;                      // Enable pins

    P1DIR   = BIT0;                             // Make P1.0 (red LED) an output

    ADC_SETUP();                                // Set up ADC peripheral

    while(1)
    {
        ADC12CTL0 = ADC12CTL0 | ADC12ENC;       // Enable conversion
        ADC12CTL0 = ADC12CTL0 | ADC12SC;        // Start conversion

                                                // Looking for threshold of 50% of 3.3V
                                                // with binary equivalent of
                                                // 1000 0000 0000 = 0x8000

        if(ADC12MEM0 > 0x800)                   // If input > 1.65V
        {
            P1OUT = BIT0;                       // Turn on red LED
        }
        else                                    // Else input <= 1.65V
        {
            P1OUT = 0x00;                       // Turn off red LED
        }

    } // end while(1)

} // end main()


// ***********************************************************************************
// Configure Analog-to-Digital Converter Peripheral

void ADC_SETUP(void)
{
    #define ADC12_SHT_16            0x0200      // 16 clock cycles for sample and hold
    #define ADC12_ON                0x0010      // Used to turn ADC12 peripheral on
    #define ADC12_SHT_SRC_SEL       0x0200      // Select source for sample & hold
    #define ADC12_12BIT             0x0020      // Select 12-bits of resolution
    #define ADC12_P92               0x000A      // Use input P9.2 for analog input

    ADC12CTL0   = ADC12_SHT_16 | ADC12_ON;      // Turn on, set sample & hold time
    ADC12CTL1   = ADC12_SHT_SRC_SEL;            // Specify sample & hold clock source
    ADC12CTL2   = ADC12_12BIT;                  // 12-bit conversion results
    ADC12MCTL0  = ADC12_P92;                    // P9.2 is analog input
}
