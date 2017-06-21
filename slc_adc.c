/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

#include "slc_adc.h"
#include "slc_util.h"

#define ANALOG_INPUTS 16
static volatile struct slc_ADCHandler _adc_aquisitions[ANALOG_INPUTS];
void isr_adc(void);

int slc_ADCInit(void)
{
    int i;
    for(i = 0; i < ANALOG_INPUTS; i++)
    {
        _adc_aquisitions[i].active = 0;
        _adc_aquisitions[i].channel = i;
        _adc_aquisitions[i].value = i;
    }
	/* Interruptions */
	IFS1bits.AD1IF = 0;
	IEC1bits.AD1IE = 1;
	IPC6bits.AD1IP = 6;
	/*end - Interruptions*/
	/*ADC*/
	AD1CON1bits.SSRC = 7;       // Internal counter ends sampling and starts conversion
	AD1CON1bits.CLRASAM = 1;    //Stop conversion when 1st A/D converter interrupt is generated and clears ASAM bit automatically
	AD1CON1bits.FORM = 0;       // Integer 16 bit output format
    AD1CON2bits.VCFG = 0;       // VR+=AVdd; VR-=AVss
    AD1CON2bits.SMPI = -1;        // Number (+1) of consecutive conversions, stored in ADC1BUF0...ADCBUF{SMPI}
    AD1CON3bits.ADRC = 1;       // ADC uses PBCLOCK with prescaler
    //AD1CON3bits.ADCS = 0xFF;    // precaler at maximum 256
    AD1CON3bits.SAMC = 16;      // Sample time is 16TAD ( TAD = 100ns)
    AD1CHSbits.CH0NA =  0;
    AD1CON2bits.CSCNA = 1;
    
}

void slc_ADCStart(void)
{
    AD1CON1bits.ON = 1;         // Enable A/D module (This must be the ***last instruction of configuration phase***)
	IFS1bits.AD1IF = 0;
    AD1CON1bits.ASAM = 1;
}

void slc_ADCReset(void)
{
    AD1CON1bits.ASAM = 0;
    AD1CON1bits.ON = 0;
}
void slc_ADCQueueInput(uint16_t channel)
{
    slc_clamp(&channel, 0, 15);
    AD1CON2bits.SMPI += 1;
    
    _adc_aquisitions[channel].active = true;
    /* Config entradas como analogicas*/
	TRISB |= ( 1 << channel); // primeiro dizer que é um input
	AD1PCFG &= !(1 << channel); // depois dizer que é analogico 16 bits
    //AD1CHSbits.CH0SA = channel;         // canal. 4 bits de AN0 a AN15
    AD1CSSL |= ( 1 << channel);
	/*end*/
}
int3float slc_ADCGetLatestValue(uint16_t channel)
{
    slc_clamp(&channel, 0, 15);
	return _adc_aquisitions[channel].value;	
}
void __attribute__( (interrupt(IPL6AUTO), vector(_ADC_VECTOR))) isr_adc(void)
{
	uint16_t *p_buff = (uint16_t*)(&ADC1BUF0);
    int i;
    for(p_buff = (uint16_t*)(&ADC1BUF0), i = 0; p_buff <= (uint16_t*)(&ADC1BUFF) && i < ANALOG_INPUTS; p_buff += 8, i++)
    {
        _adc_aquisitions[i].value = ((*p_buff*3300)/1023 + _adc_aquisitions[i].value)/2;
    }
    AD1CON1bits.ASAM = 1;
	IFS1bits.AD1IF = 0;
}

float slc_CurrentValue()
{
    float b_correction = 0.06;
    float x_correction = 0.366;
    return ((slc_ADCGetLatestValue(ADC_CURRENT)/1000.0)+b_correction)*x_correction;
}
float slc_VredValue()
{
    float b_correction = 0.034;
    float x_correction = 4.61;
    return ((slc_ADCGetLatestValue(ADC_VRED)/1000.0)+b_correction)*x_correction;
}
float slc_BattValue()
{
    float b_correction = 0.034;
    float x_correction = 4.61;
    return ((slc_ADCGetLatestValue(ADC_BATT)/1000.0)+b_correction)*x_correction;
}
float slc_TempIntValue()
{
    float b_correction = 0;
    float x_correction = 17.604;
    return ((slc_ADCGetLatestValue(ADC_TEMP_INT)/1000.0)+b_correction)*x_correction;
}
float slc_TempBattValue()
{
    float b_correction = 0;
    float x_correction = 17.604;
    return ((slc_ADCGetLatestValue(ADC_TEMP_BATT)/1000.0)+b_correction)*x_correction;
}

int3float slc_Current()
{
    int3float b_correction = 60;
    int3float x_correction = 366;
    return i3fM((slc_ADCGetLatestValue(ADC_CURRENT)+b_correction) , x_correction);
}
int3float slc_Vred()
{
    int3float b_correction = 34;
    int3float x_correction = 4610;
    return i3fM((slc_ADCGetLatestValue(ADC_VRED)+b_correction), x_correction);
}
int3float slc_Batt()
{
    int3float b_correction = 34;
    int3float x_correction = 4610;
    return i3fM((slc_ADCGetLatestValue(ADC_BATT)+b_correction), x_correction);
}
int3float slc_TempInt()
{
    int3float b_correction = 0;
    int3float x_correction = 17604;
    return i3fM((slc_ADCGetLatestValue(ADC_TEMP_INT)+b_correction), x_correction);
}
int3float slc_TempBatt()
{
    int3float b_correction = 0;
    int3float x_correction = 17604;
    return i3fM((slc_ADCGetLatestValue(ADC_TEMP_BATT)+b_correction), x_correction);
}
/* *****************************************************************************
 End of File
 */
