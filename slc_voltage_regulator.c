#include "slc_voltage_regulator.h"
#include "slc_adc.h"

uint16_t f_freq;
int3float f_max_voltage;
int3float f_max_current;
extern volatile slc_ADCHandler _adc_aquisitions;
void slc_PWMInit(uint16_t freq, uint8_t dutyCycle);

void slc_InitVoltageRegulator()
{
    f_freq = 1000; // TODO attempt load from SD
    f_max_voltage = 0;
    f_max_current = 0;
    slc_PWMInit(f_freq, 0);
}

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

//TODO warn/fail if frequency not possible
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
	IPC2bits.T2IP = 5;
	// PWM
	OC1CONbits.OCM = 6; // Output Compare Mode Select bits; 6 -> PWM mode on OCx; Fault pin disabled
	OC1CONbits.OCTSEL = 0; // 0 -> T2 1-> T3 
	slc_SetDutyCycle(dutyCycle);
	OC1CONbits.ON = 1;
	
	//_timer4_callback = callback;
	
}

void onTick(void)
{
    //_adc_aquisitions[ADC_EMITTER];
    
    slc_SetDutyCycle(0);
}
void __attribute__( (interrupt(IPL5AUTO), vector(_TIMER_2_VECTOR))) isr_pwm(void)
{
	IFS0bits.T2IF = 0;
}