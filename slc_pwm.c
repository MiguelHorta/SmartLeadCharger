#include "slc_pwm.h"


void getConstants(int freq,int *prescaler, int *value)
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
void slc_SetDutyCycle(uint8_t dutyCycle)
{
	dutyCycle = dutyCycle > 100 ? 100 : dutyCycle;
	dutyCycle = dutyCycle < 0 ? 0 : dutyCycle;
	
	OC1RS = (PR2+1)*(dutyCycle)/100;
}
void slc_PWMInit(uint16_t freq, uint8_t dutyCycle)
{
	
	int prescaler,value;
	getConstants(freq,&prescaler,&value);
	
	T2CONbits.ON = 0; // Stop timer
	IFS0bits.T2IF=0; // Reset interrupt flag 
	T2CONbits.TCKPS = prescaler;
	PR2 = value;
	TMR2 = 0;
	T2CONbits.TON = 1;
	// INterrupçoes para o timer "
	IFS0bits.T2IF = 0;
	IEC0bits.T2IE = 1;
	IPC2bits.T2IP = 6;
	// PWM
	OC1CONbits.OCM = 6; // Pin desejado OCx
	OC1CONbits.OCTSEL = 0; // 0 -> T2 1-> T3 
	slc_SetDutyCycle(dutyCycle);
	OC1CONbits.ON = 1;
	
	//_timer4_callback = callback;
	
}

void isr_pwm(void)
{
	IFS0bits.T2IF = 0;
}