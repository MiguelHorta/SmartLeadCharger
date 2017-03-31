#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>

#define SYSCLK 80000000L // System clock frequency, in Hz
#define PBUSCLK 40000000L // Peripheral bus clock
#define CALIBRATION_VALUE 4998
void delay(unsigned int n_intervals)
{
	volatile unsigned int i;
	for(; n_intervals != 0; n_intervals--)
		for(i = CALIBRATION_VALUE; i != 0; i--);
}

int main(int argc, char** argv)
{
     // Performance optimization (flash access time, enable instruct and data cache,... and PBClock setup
    SYSTEMConfigPerformance(SYSCLK);
    mOSCSetPBDIV(OSC_PB_DIV_2); // This is necessary since SYSTEMConfigPerformance defaults FPBDIV to DIV_1

        // Remove input/output buffering
    setvbuf ( stdin , NULL , _IONBF , 0 );
    setvbuf ( stdout , NULL , _IONBF , 0 );
    
    TRISEbits.TRISE0 = 0;
    
    printf("Start!");
    slc_PWMInit(1000, 50);
    slc_ADCInit();
    slc_ADCQueueInput(0);
    slc_ADCStart();
    LATEbits.LATE0 = 0;
    while(1)
    {
        delay(1000);
        int i;
        for(i = 0; i < 16; i++)
        {
              printf("Reads: %d\n",slc_ADCGetLatestValue(i));
        }
    }
    return (EXIT_SUCCESS);
}