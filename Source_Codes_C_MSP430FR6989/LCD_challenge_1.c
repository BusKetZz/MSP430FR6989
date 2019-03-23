#include <msp430.h>

#include <driverlib.h>                      // Required for the LCD
#include "myGpio.h"                         // Required for the LCD
#include "myClocks.h"                       // Required for the LCD
#include "myLcd.h"                          // Required for the LCD

#define ENABLE_PINS     0xFFFE              // Required to use inputs and outputs

// *******************************************************************************
// Function Prototypes

void ADC_SETUP();

// *******************************************************************************

int main(void)
{
    WDTCTL  = WDTPW | WDTHOLD;              // Stop the watchdog timer
    PM5CTL0 = ENABLE_PINS;                  // Enable pins

    initGPIO();                             // Initialize General Purpose Inputs and Outputs for LCD
    initClocks();                           // Initialize clocks for LCD
    myLCD_init();                           // Prepare LCD to receive commands

    ADC_SETUP();                            // Set up ADC peripheral

    ADC12IER0 = ADC12IE0;                   // Enable ADC interrupt

    _BIS_SR(GIE);                           // Activate interrupts

    ADC12CTL0 = ADC12CTL0 | ADC12ENC;       // Enable conversion
    ADC12CTL0 = ADC12CTL0 | ADC12SC;        // Start conversion

    while(1);
}

// *******************************************************************************
// ADC12 Interrupt Service Routine

#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
{
    myLCD_showSymbol(LCD_UPDATE, LCD_BRACKETS, 0);      // Brackets on

    if(ADC12MEM0 > 0x800)                               // If input > 1.65V
    {
        myLCD_showSymbol(LCD_UPDATE, LCD_B1, 0);        // Bar level 1 on
    }
    else
    {
        myLCD_showSymbol(LCD_CLEAR, LCD_B1, 0);         // Bar level 1 off
    }

    if(ADC12MEM0 > 0xA00)
    {
        myLCD_showSymbol(LCD_UPDATE, LCD_B2, 0);        // Bar level 2 on
    }
    else
    {
        myLCD_showSymbol(LCD_CLEAR, LCD_B2, 0);         // Bar level 2 off
    }

    if(ADC12MEM0 > 0xC00)
    {
        myLCD_showSymbol(LCD_UPDATE, LCD_B3, 0);        // Bar level 3 on
    }
    else
    {
        myLCD_showSymbol(LCD_CLEAR, LCD_B3, 0);         // Bar level 3 off
    }

    if(ADC12MEM0 > 0xD00)
    {
        myLCD_showSymbol(LCD_UPDATE, LCD_B4, 0);        // Bar level 4 on
    }
    else
    {
        myLCD_showSymbol(LCD_CLEAR, LCD_B4, 0);         // Bar level 4 off
    }

    if(ADC12MEM0 > 0xE00)
    {
        myLCD_showSymbol(LCD_UPDATE, LCD_B5, 0);        // Bar level 5 on
    }
    else
    {
        myLCD_showSymbol(LCD_CLEAR, LCD_B5, 0);         // Bar level 5 off
    }

    if(ADC12MEM0 > 0xF00)
    {
        myLCD_showSymbol(LCD_UPDATE, LCD_B6, 0);        // Bar level 6 on
    }
    else
    {
        myLCD_showSymbol(LCD_CLEAR, LCD_B6, 0);         // Bar level 6 off
    }

    ADC12CTL0 = ADC12CTL0 | ADC12SC;                    // Start next conversion

}

// *******************************************************************************
// Configure Analog-to-Digital Converter Peripheral

void ADC_SETUP(void)
{
    #define ADC12_SHT_16        0x0200      // 16 clock cycles for sample and hold
    #define ADC12_ON            0x0010      // Used to turn ADC12 peripheral on
    #define ADC12_SHT_SRC_SEL   0x0200      // Select source for sample & hold
    #define ADC12_12BIT         0x0020      // Select 12-bits of resolution
    #define ADC12_P92           0x000A      // Use input P9.2 for analog input

    ADC12CTL0   = ADC12_SHT_16 | ADC12_ON;  // Turn on, set sample & hold time
    ADC12CTL1   = ADC12_SHT_SRC_SEL;        // Specify sample & hold clock source
    ADC12CTL2   = ADC12_12BIT;              // 12-bits conversion result
    ADC12MCTL0  = ADC12_P92;                // P9.2 is analog input
}
