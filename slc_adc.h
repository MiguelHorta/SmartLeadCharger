/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Universidade de Aveiro

  @File Name
    slc_adc.h

  @Summary
    Base abstration for adc usage.

  @Description
    Base abstration for adc usage.
 */
/* ************************************************************************** */

#ifndef SLC_ADC_H    /* Guard against multiple inclusion */
#define SLC_ADC_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stdint.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <p32xxxx.h>
#include <plib.h>
#include "slc_util.h"

#define ADC_CURRENT   0 //RB0
#define ADC_VRED      1 //RB1
#define ADC_BATT      2 //RB2
#define ADC_TEMP_INT  3 //RB3
#define ADC_TEMP_BATT 4 //RB4

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct slc_ADCHandler
    {
        uint8_t active;
        uint8_t channel; //irrelevant
        volatile int3float value;
        
    } slc_ADCHandler;
    
    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************
    int slc_ADCInit(void);
    void slc_ADCStart(void);
    void slc_ADCReset(void);
    void slc_ADCQueueInput(uint16_t channel);

    int3float slc_ADCGetLatestValue(uint16_t channel);
    int3float slc_CurrentValue();
    int3float slc_VredValue();
    int3float slc_BattValue();
    int3float slc_TempIntValue();
    int3float slc_TempBattValue();

#ifdef __cplusplus
}
#endif

#endif /* SLC_ADC_H */

/* *****************************************************************************
 End of File
 */
