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
    float slc_CurrentValue();
    float slc_VredValue();
    float slc_BattValue();
    float slc_TempIntValue();
    float slc_TempBattValue();
    int3float slc_Current();
    int3float slc_Vred();
    int3float slc_Batt();
    int3float slc_TempInt();
    int3float slc_TempBatt();

#ifdef __cplusplus
}
#endif

#endif /* SLC_ADC_H */

/* *****************************************************************************
 End of File
 */
