/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef SLC_OSCILATORS_H    /* Guard against multiple inclusion */
#define SLC_OSCILATORS_H

#include <stdint.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <p32xxxx.h>
#include <plib.h>
#include "slc_util.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

//#define VOLTAGE_REGULATOR_OUTPUT 0

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    void slc_InitOscilators(uint16_t freq);
    void slc_EnsableOscilators(void);
    void slc_DisableOscilators(void);
    void slc_QueueBaseRegulator(int3float initial_max_voltage, int3float initial_max_current);
    void slc_QueueFanRegulator(int3float initial_working_temp);
     
    void slc_SetMaxVoltage(int3float voltage); // the last 3 cases are decimal, 4 case is the unity
    void slc_SetMaxCurrent(int3float current); // the last 3 cases are decimal, 4 case is the unity
    // Calibration factors
    void slc_SetCFMinVoltage(int3float);
    void slc_SetCFMaxVoltage(int3float);
    //Variation in voltage when the duty-cycle is increased by 1%
    //XXX It assumed to be linear
    void slc_SetCFVoltageVariation(int3float);
    
    uint8_t getFanPWM(void);
    uint8_t getBasePWM(void);
    
    void setFanPWM(uint8_t);
    void setBasePWM(uint8_t);
    void slc_DisableOscilators(void);
    void slc_EnableOscilators(void);
    //update
    void onFanTick(void);
    void onBaseTick(void);
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
