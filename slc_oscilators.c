#include "slc_oscilators.h"
#include "slc_adc.h"
#include "slc_charge_plan.h"
#include "slc_control_loop.h"

uint16_t f_freq;
static int3float f_max_voltage;
static int3float f_max_current;
static bool f_disabled;
void slc_TimerInit(uint16_t freq);
void slc_SetFanPWM(uint8_t dutyCycle);
void slc_SetBasePWM(uint8_t dutyCycle);

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
void slc_QueueBaseRegulator()
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

static float working_temp;
void slc_QueueFanRegulator(float initial_working_temp)
{
    working_temp = initial_working_temp;
    OC2CONbits.OCM = 6; // Output Compare Mode Select bits; 6 -> PWM mode on OCx; Fault pin disabled
	OC2CONbits.OCTSEL = 0; // 0 -> T2 1-> T3 
	slc_SetFanPWM(0);
	OC2CONbits.ON = 1;
}

uint8_t getFanPWM(void)
{
    return fan_duty_cycle;
}
uint8_t getBasePWM(void)
{
    return base_duty_cycle;
}
    
void setFanPWM(uint8_t v){
    slc_SetFanPWM(v);
}
void setBasePWM(uint8_t v){
    slc_SetBasePWM(v);
}
static float k_fan = 2.0;
void onFanTick(void)
{
    if(f_disabled)
        return;
    uint8_t duty_cycle = 0;
    duty_cycle = 80.0 + k_fan*(slc_TempIntValue() - working_temp);
    slc_SetFanPWM(duty_cycle);
}

static float Kp=10;     // TODO calibrar estes valore
static float Ti=1;
static float Td=0.1;

static float u_p=0;
static float u=0;
static float u_i=0;
static float u_d=0;
static float u_i_a=0;
static float e_a=0;
static float e=0;
static float h = 1.0/CONTROL_LOOP_FREQ;   // tempo da chamada da interrupção  

void onBaseTick(void)
{
    if(f_disabled)
        return;
    static ControlType ct = VOLTAGE;
    ct = getControlType();
    if(ct == VOLTAGE){
        e=12-slc_BattValue();
        u_p=Kp*(e);
        u_d=Kp*Td*(e-e_a)/h;
        u_i=e*Ti*h/Kp+u_i_a;
        if(u_i>10)  // TODO ajustar estes valores das limitações
            u_i=10;     
        if(u_i<-10)
            u_i=-10;
        u=u_p+u_d+u_i;
        slc_SetBasePWM(u);
        u_i_a=u_i;
        e_a=e;
    }else if(ct == CURRENT){
        e=0.5-slc_CurrentValue();
        u_p=Kp*(e);
        u_d=Kp*Td*(e-e_a)/h;
        u_i=e*Ti*h/Kp+u_i_a;
        if(u_i>10)  // TODO ajustar estes valores das limitações
            u_i=10;
        if(u_i<-10)
            u_i=-10;
        u=u_p+u_d+u_i;
        slc_SetBasePWM(u);
        u_i_a=u_i;
        e_a=e;
    }
}
void __attribute__( (interrupt(IPL5AUTO), vector(_TIMER_2_VECTOR))) isr_pwm(void)
{
	IFS0bits.T2IF = 0;
}