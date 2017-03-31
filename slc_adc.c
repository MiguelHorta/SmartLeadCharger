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
static volatile struct slc_ADCHandler f_inputs[ANALOG_INPUTS];
void isr_adc(void);

int slc_ADCInit(void)
{
    int i;
    for(i = 0; i < ANALOG_INPUTS; i++)
    {
        f_inputs[i].active = 0;
        f_inputs[i].channel = i;
        f_inputs[i].value = i;
    }
	/* Interruptions */
    void __attribute__( (interrupt(IPL2AUTO), vector(_ADC_VECTOR))) isr_adc(void);
	IFS1bits.AD1IF = 0;
	IEC1bits.AD1IE = 1;
	IPC6bits.AD1IP = 5;
	/*end - Interruptions*/
	/*ADC*/
	AD1CON1bits.SSRC = 7;       // Internal counter ends sampling and starts conversion
	AD1CON1bits.CLRASAM = 1;    //Stop conversion when 1st A/D converter interrupt is generated and clears ASAM bit automatically
	AD1CON1bits.FORM = 0;       // Integer 16 bit output format
    AD1CON2bits.VCFG = 0;       // VR+=AVdd; VR-=AVss
    AD1CON2bits.SMPI = 1;        // Number (+1) of consecutive conversions, stored in ADC1BUF0...ADCBUF{SMPI}
    AD1CON3bits.ADRC = 1;       // ADC uses internal RC clock
    AD1CON3bits.SAMC = 16;      // Sample time is 16TAD ( TAD = 100ns)
    AD1CON2bits.CSCNA = 1;
    
}

void slc_ADCStart(void)
{
    // Enable module
    AD1CON1bits.ON = 1;         // Enable A/D module (This must be the ***last instruction of configuration phase***)
	IFS1bits.AD1IF = 0;
    /*end - ADC*/
    f_inputs[1].value = 18;
    AD1CON1bits.ASAM = 1;
}

void slc_ADCReset(void)
{
    AD1CON1bits.ASAM = 0;
    AD1CON1bits.ON = 0;         // Enable A/D module (This must be the ***last instruction of configuration phase***)
}
void slc_ADCQueueInput(uint16_t channel)
{
    slc_clamp(&channel, 0, 15);
    f_inputs[channel].active = true;
    /* Config entradas como analogicas*/
	TRISB = TRISB | ( 1 << (channel)); // primeiro dizer que é um input
	AD1PCFG = AD1PCFG & !(1 << (channel)); // depois dizer que é analogico 16 bits
    AD1CSSL |= ( 1 << channel);
	/*end*/
}
uint16_t slc_ADCGetLatestValue(uint16_t channel)
{
    slc_clamp(&channel, 0, 15);
	return f_inputs[channel].value;	
}
void isr_adc(void)
{
	uint16_t *p_buff = (uint16_t*)(&ADC1BUF0);
    int i;
    for(i = 0; i < ANALOG_INPUTS; i++)
    {
        if(f_inputs[i].active)
        {
            LATEbits.LATE0 = !LATEbits.LATE0;
            f_inputs[i].value = 5; //(*p_buff + f_inputs[i].value)/2;
            p_buff++;
        }
    }
	IFS1bits.AD1IF = 0;
}

/* *****************************************************************************
 End of File
 */
