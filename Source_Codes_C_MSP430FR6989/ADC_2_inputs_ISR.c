#include <msp430.h>

#define ENABLE_PINS         0xFFFE                  // Required to use inputs and outputs

// ***************************************************************************************
// Function Prototypes

void ADC_SETUP();                                   // Used to setup ADC12 peripheral

// ***************************************************************************************

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;                      // Stop the watchdog timer
    PM5CTL0 = ENABLE_PINS;                          // Enable inputs and outputs
    P1DIR   = BIT0;                                 // Make P1.0 (red LED) an output
    P9DIR   = BIT7;                                 // Make P9.7 (green LED) an output

    ADC_SETUP();                                    // Set up ADC peripheral

    ADC12IER0 = ADC12IE0;                           // Enable ADC interrupt

    _BIS_SR(GIE);                                   // Activate interrupts

    ADC12CTL0 = ADC12CTL0 | ADC12ENC;               // Enable conversion
    ADC12CTL0 = ADC12CTL0 | ADC12SC;                // Start conversion

    while(1);
}

// ***************************************************************************************
// ADC12 Interrupt Service Routine

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    #define ADC12_P84           0x0007              // Use pin P8.4 for analog input
    #define ADC12_P92           0x000A              // Use pin P9.2 for analog input

    static unsigned char input = 84;                // input = 84 if P8.4 sampled
                                                    // input = 92 if P9.2 sampled

    if(input == 84)                                 // If sample was from P8.4
    {
        if(ADC12MEM0 > 0x800)                       // If input > 1.65V (50%)
        {
            P1OUT = BIT0;                           // Turn on red LED
        }
        else                                        // Else input <= 1.65V
        {
            P1OUT = 0x00;                           // Turn off red LED
        }

        input = 92;                                 // Next sample from P9.2

        ADC12CTL0 = ADC12CTL0 & (~ADC12ENC);        // Need to disable peripheral
        ADC12MCTL0= ADC12_P92;                      // to change to input from P9.2
    }

    else                                            // Else sample was from P9.2
    {
        if(ADC12MEM0 > 0x800)                       // If input > 1.65V (50%)
        {
            P9OUT = BIT7;                           // Turn on green LED
        }
        else                                        // Else input <= 1.65V
        {
            P9OUT = 0x00;                           // Turn off green LED
        }

        input = 84;                                 // Next sample from P8.4

        ADC12CTL0 = ADC12CTL0 & (~ADC12ENC);        // Need to disable peripheral
        ADC12MCTL0= ADC12_P84;                      // to change to input from P8.4
    }

    ADC12CTL0 = ADC12CTL0 | ADC12ENC;               // Re-enable conversion
    ADC12CTL0 = ADC12CTL0 | ADC12SC;                // Start next conversion
}

// ***************************************************************************************
// Configure Analog-to-Digital Converter Peripheral

void ADC_SETUP(void)
{
    #define ADC12_SHT_16            0x0200          // 16 clock cycles for sample and hold
    #define ADC12_ON                0x0010          // Used to turn ADC12 peripheral on
    #define ADC12_SHT_SRC_SEL       0x0200          // Select source for sample & hold
    #define ADC12_12BIT             0x0020          // Select 12-bits of resolution

    #define ADC12_P84               0x0007          // Use pin P8.4 for analog input
    #define ADC12_P85               0x0006          // Use pin P8.5 for analog input
    #define ADC12_P86               0x0005          // Use pin P8.6 for analog input
    #define ADC12_P87               0x0004          // Use pin P8.7 for analog input

    #define ADC12_P90               0x0008          // Use pin P9.0 for analog input
    #define ADC12_P91               0x0009          // Use pin P9.1 for analog input
    #define ADC12_P92               0x000A          // Use pin P9.2 for analog input
    #define ADC12_P93               0x000B          // Use pin P9.3 for analog input
    #define ADC12_P95               0x000D          // Use pin P9.5 for analog input
    #define ADC12_P96               0x000E          // Use pin P9.6 for analog input

    ADC12CTL0   = ADC12_SHT_16 | ADC12_ON;          // Turn on, set sample & hold time
    ADC12CTL1   = ADC12_SHT_SRC_SEL;                // Specify sample & hold clock source
    ADC12CTL2   = ADC12_12BIT;                      // 12-bit conversion results
    ADC12MCTL0  = ADC12_P84;                        // Initially P8.4 is analog input
}
