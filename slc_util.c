#include "slc_util.h"
void slc_clamp(uint16_t *n, int lower, int upper )
{
    *n = *n <= lower ? lower : *n >= upper ? upper : *n;
}
void Delay_us(unsigned int ums)
/*
 * Core timer uses a 32 bit HW register. Max value is 4,294,967,295 and counts at FOSC/2
 * For a 80 MHz FOSC the core timer ticks every 25 nS and will roll over in 107.374 seconds.
 * OpenCoreTimer() and UpdateCoreTimer() are used only if you want to generate interrupts when the
 * core timer reaches a given value.  None of the functions alters the value of the core timer.
 * You don't have to use them to use ReadCoreTimer().
 *
 * Info: PIC32MX Family reference manual, Section 2.2.3
 *
 * 1 tick - 25ns
 * x ticks - 1us
 * x=1e-6/25e-9=0.04e6=40 - CoreTimet ticks that correspond to one microsecond
 *
 */
{
    unsigned int t;
    t=ReadCoreTimer()+40*ums;
    while(ReadCoreTimer() < t);
}
