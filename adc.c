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

#include "adc.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
int global_data;


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
static int ExampleLocalFunction(int param1, int param2) {
    return 0;
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
int ExampleInterfaceFunction(int param1, int param2) {
    return 0;
}
volatile int _last_value = 0;
int _amostras = 0;
void adc1_init(unsigned int channel, unsigned int amostras)
{
	channel = channel > 15 ? 15 : channel;
	channel = channel < 0  ? 0 : channel;
	amostras = amostras > 16 ? 16 : amostras;
	amostras = amostras < 1 ? 1 :amostras;
	_amostras = amostras;
	/* Config entradas como analogicas*/
	TRISB = TRISB | ( 1 << channel); // primeiro dizer que é um input
	AD1PCFG = AD1PCFG & !(1 << channel); // depois dizer que é analogico 16 bits
	/*end*/
	/* Interruptions */
	IFS1bits.AD1IF = 0;
	IEC1bits.AD1IE = 1;
	IPC6bits.AD1IP = 5;
	/*end - Interruptions*/
	/*ADC*/
	AD1CON1bits.SSRC = 7;
	AD1CON1bits.CLRASAM = 1;
	AD1CON3bits.SAMC = 16;
	AD1CON2bits.SMPI = amostras-1; // numero de amostras. 4bits 1 a 16
	AD1CHSbits.CH0SA = channel;         // canal. 4 bits de AN0 a AN15
	AD1CON1bits.ON = 1;
	/*end - ADC*/
}

void adc1_start(void)
{
	AD1CON1bits.ASAM = 1;
}

int adc1_lastValue()
{
	return _last_value;	
}
void _int_(27) isr_adc(void)
{
	int *p = (int *)(&ADC1BUF0);
	int i,result = 0;
	for(i = 0; i < _amostras;i++)
	{
		result += (p[4*i]*33+511)/1023;;
	}
	IFS1bits.AD1IF = 0;
	_last_value = (result/_amostras);
}

/* *****************************************************************************
 End of File
 */
