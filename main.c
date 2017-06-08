#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <plib.h>
#include "slc_adc.h"
#include "slc_oscilators.h"
#include "slc_control_loop.h"
#include "slc_lcd.h"
#include "slc_charge_plan.h"

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
    slc_QueueBaseRegulator();
    slc_QueueFanRegulator(30);
    slc_DisableOscilators();
    
    /* Control Loop */
    slc_InitControlLoop();
    slc_InitLCD();
    char command;
    char inst[24];
    int value;
    extern volatile int3float e;
    extern volatile int3float u;
    extern uint8_t f_charge_step;
    while(1)
    {
        puts("Command:");
        command = getchar();
        switch(command)
        {
            case 'b':
                puts("Base PWM: ");
                gets(inst);
                value = atoi(inst);
                printf("Set PWM: %d\n", value);
                setBasePWM(value);
            break;
            case 'f':
                puts("Fan PWM: ");
                gets(inst);
                value = atoi(inst);
                printf("Set PWM: %d\n", value);
                setFanPWM(value);
            break;
            case 't':
                printf("TMP INT: %f\n", slc_TempIntValue());
                break;
            case 'y':
                printf("TMP BATT: %f\n", slc_TempBattValue());
            break;
            case 'r':
                while(!U1STAbits.URXDA)
                {
                    printf("%3.2f, %3.2f, %2.2f, %2.2f\n", slc_VredValue(), slc_CurrentValue(), slc_TempIntValue(),slc_TempBattValue());
                    Delay_us(100000);
                }
                break;
            case 'c':
                while(!U1STAbits.URXDA)
                {
                    printf("%3.2f, %3.2f, %4.3f, %4.3f, %u\n", slc_VredValue(), slc_CurrentValue(),  e/1000.0, u/1000.0, f_charge_step);
                    Delay_us(100000);
                }
                break;
        }
    };
    return (EXIT_SUCCESS);
}