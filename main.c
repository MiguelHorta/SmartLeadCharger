#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "slc_adc.h"
#include "slc_oscilators.h"
#include "slc_control_loop.h"

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
    
    /* Readings */
    slc_ADCInit();
    slc_ADCQueueInput(ADC_CURRENT);
    slc_ADCQueueInput(ADC_VRED);
    slc_ADCQueueInput(ADC_BATT);
    slc_ADCQueueInput(ADC_TEMP_INT);
    slc_ADCQueueInput(ADC_TEMP_BATT);
    slc_ADCStart();
    
    /* Actuators */
    //ODCDbits.ODCD0 = 1;
    slc_InitOscilators(5000);
    slc_QueueBaseRegulator(5000, 0200);
    slc_QueueFanRegulator(40000);
    
    /* Control Loop */
    slc_InitControlLoop(1000);
    
    while(1)
    {
        int i;
        for(i = 0;  i < 5; i++)
        {
        printf("R%d: %d\n", i, slc_ADCGetLatestValue(i));
        }
    }
    return (EXIT_SUCCESS);
}