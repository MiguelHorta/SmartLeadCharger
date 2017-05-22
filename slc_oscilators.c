#include "slc_oscilators.h"
#include "slc_adc.h"

uint16_t f_freq;
static int3float f_max_voltage;
static int3float f_max_current;
static bool f_disabled;
void slc_TimerInit(uint16_t freq);

void slc_InitOscilators(uint16_t freq)
{
    slc_TimerInit(freq);
    f_disabled = false;
}

void slc_DisableOscilators(void)
{
    f_disabled = true;
    slc_SetFanPWM(0);
    slc_SetBasePWM(0);
}
void slc_EnableOscilators(void)
{
    f_disabled = false;
    onFanTick();
    onBaseTick();
}
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
void slc_TimerInit(uint16_t freq)
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
	
}
static uint8_t base_duty_cycle;
void slc_SetBasePWM(uint8_t dutyCycle)
{
	dutyCycle = dutyCycle > 100 ? 100 : dutyCycle;
	dutyCycle = dutyCycle < 0 ? 0 : dutyCycle;
	base_duty_cycle = dutyCycle;
	OC1RS = (PR2+1)*(dutyCycle)/100;
}
void slc_QueueBaseRegulator(int3float initial_max_voltage, int3float initial_max_current)
{
	OC1CONbits.OCM = 6; // Output Compare Mode Select bits; 6 -> PWM mode on OCx; Fault pin disabled
	OC1CONbits.OCTSEL = 0; // 0 -> T2 1-> T3 
	slc_SetBasePWM(0);
	OC1CONbits.ON = 1;
}

static uint8_t fan_duty_cycle;
void slc_SetFanPWM(uint8_t dutyCycle)
{
	dutyCycle = dutyCycle > 100 ? 100 : dutyCycle;
	dutyCycle = dutyCycle < 0 ? 0 : dutyCycle;
	fan_duty_cycle = dutyCycle;
	OC2RS = (PR2+1)*(dutyCycle)/100;
}

static int3float working_temp;
void slc_QueueFanRegulator(int3float initial_working_temp)
{
    working_temp = initial_working_temp;
    OC2CONbits.OCM = 6; // Output Compare Mode Select bits; 6 -> PWM mode on OCx; Fault pin disabled
	OC2CONbits.OCTSEL = 0; // 0 -> T2 1-> T3 
	slc_SetFanPWM(0);
	OC2CONbits.ON = 1;
}

static float k_fan = 0.01;
void onFanTick(void)
{
    if(f_disabled)
        return;
    uint8_t duty_cycle = 0;
    duty_cycle = k_fan*(slc_TempIntValue() - working_temp);
    slc_SetFanPWM(duty_cycle);
}

void onBaseTick(void)
{
    if(f_disabled)
        return;
    slc_SetBasePWM(60);
}
void __attribute__( (interrupt(IPL5AUTO), vector(_TIMER_2_VECTOR))) isr_pwm(void)
{
	IFS0bits.T2IF = 0;
}