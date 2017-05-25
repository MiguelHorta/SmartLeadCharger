#include "slc_oscilators.h"
#include "slc_control_loop.h"
static void getConstants(int freq,int *prescaler, int *value)
{
	const int MAX_VALUE = 65535;
	const int prescaler_values[] = {1,2,4,8,16,32,64,256};
	int i;
	for(i = 0; i < 8; i++) 
	{
		*value = PBUSCLK/prescaler_values[i]/freq -1 ;
		if(*value < MAX_VALUE)
			break;
	}
	*prescaler = i;
}

//TODO warn/fail if frequency not possible
void slc_InitControlLoop(uint16_t freq)
{
	
	int prescaler,value;
	getConstants(freq,&prescaler,&value);
	
	T4CONbits.ON = 0; // Stop timer
	IFS0bits.T4IF=0; // Reset interrupt flag 
	T5CONbits.TCKPS = prescaler;
	PR4 = value;
	TMR4 = 0;
	T4CONbits.TON = 1;
	// INterrupçoes para o timer "
	IFS0bits.T4IF = 0;
	IEC0bits.T4IE = 1;
	IPC4bits.T4IP = 5;
	
}

void __attribute__( (interrupt(IPL5AUTO), vector(_TIMER_4_VECTOR))) isr_control_loop(void)
{
    OnCheckFaultyConditions();
    onBaseTick();
    onFanTick();
	IFS0bits.T4IF = 0;
}