/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 3.15.0
        Device            :  PIC18F4550
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.20
*/

#include "mcc.h"
#include "stdio.h"
#include "stdlib.h"
#include <xlcd.h>

unsigned char ADC_Buffer[10];
unsigned int duty = 0;
unsigned char ADC_Flag = 0;
unsigned int  ADC_Value;

adc_result_t ADC_GetConversionResult(void)
{
    // Conversion finished, return the result
    return ((ADRESH << 8) + ADRESL);
}

void ADC_StartConversion(adc_channel_t channel)
{
    // select the A/D channel
    ADCON0bits.CHS = channel;    
    // Turn on the ADC module
    ADCON0bits.ADON = 1;


    // Start the conversion
    ADCON0bits.GO_nDONE = 1;
    
}

void ADC_ISR(void)
{
   //read ADC conversion value
    ADC_Value = ADC_GetConversionResult();
    
    //set user's flag to continue processing in while loop
    ADC_Flag = 1; 
    
}

void main(void)
{
    // Initialize the device
    SYSTEM_Initialize();
    
    
    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();
    
    //prints a message
    while(BusyXLCD());
    putrsXLCD("**ADC Value**");
    
    PWM2_LoadDutyValue(100);
    PR2 = 49;
    
    //start first ADC conversion with AN0
    ADC_StartConversion(0);
    
    while (1)
    {
        if(ADC_Flag == 1)
        {
            //print ADC value on LCD
            ADC_Flag = 0;
            sprintf(ADC_Buffer, "%d    ", ADC_Value);
            
            //set cursor con 2nd row of the LCD
            while(BusyXLCD());
            SetDDRamAddr(0x40);
            
            //
            while(BusyXLCD());
            putsXLCD((unsigned char *) ADC_Buffer);
            
            //change the duty value according to the potentiometer
            duty = (int)(20+(ADC_Value/5.75));
            PWM2_LoadDutyValue(duty);
            
            //start new conversion
            ADC_StartConversion(0);
        }
        
    }
}

/**
 End of File
*/
