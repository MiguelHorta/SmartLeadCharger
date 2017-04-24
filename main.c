#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "slc_adc.h"
#include "slc_voltage_regulator.h"

#define SYSCLK 80000000L // System clock frequency, in Hz
#define PBUSCLK 40000000L // Peripheral bus clock

int main(int argc, char** argv)
{
     // Performance optimization (flash access time, enable instruct and data cache,... and PBClock setup
    SYSTEMConfigPerformance(SYSCLK);
    mOSCSetPBDIV(OSC_PB_DIV_2); // This is necessary since SYSTEMConfigPerformance defaults FPBDIV to DIV_1
    INTEnableSystemMultiVectoredInt();
    
    // Remove input/output buffering
    setvbuf ( stdin , NULL , _IONBF , 0 );
    setvbuf ( stdout , NULL , _IONBF , 0 );
    
    
    puts("Start!");
    slc_ADCInit();
    slc_ADCQueueInput(ADC_EMITTER);
    slc_ADCStart();
    slc_InitVoltageRegulator();
    while(1)
    {
        int i;
        for(i = 0;  i < 3; i++)
        {
        printf("R%d: %d\n", i, slc_ADCGetLatestValue(i));
        }
    }
    return (EXIT_SUCCESS);
}