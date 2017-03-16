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
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct slc_ADCHandler
    {
        uint8_t active;
        uint8_t channel; //irrelevant
        int16_t value;
        
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
    uint16_t slc_ADCGetLatestValue(uint16_t channel);



#ifdef __cplusplus
}
#endif

#endif /* SLC_ADC_H */

/* *****************************************************************************
 End of File
 */
