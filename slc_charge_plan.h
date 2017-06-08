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

#ifndef SLC_CHARGE_PLAN_H    /* Guard against multiple inclusion */
#define SLC_CHARGE_PLAN_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdbool.h>
#include <stdint.h>
#include "slc_adc.h"
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    typedef bool (*callback)(int3float,int3float,int3float);
    /* Provide C++ Compatibility */
    typedef struct
    {
        char desc[17];
        int3float target_current;
        int3float target_voltage;
        callback next_step;
    }ChargeStep;
    typedef enum{
        CHARGE_TYPE_FIRST = 0x0,
        START_CHARGE_SMT = CHARGE_TYPE_FIRST,
        START_CHARGE_FST = 0x1,
        START_CHARGE_SLW = 0x2,
        CHARGE_TYPE_LAST = START_CHARGE_SLW,
    }ChargeType;
    typedef enum
    {
        VOLTAGE = 0x0,
        CURRENT = 0x1,
    }ControlType;
    
    #define SMART_CHARGE_STEPS 3
    #define FAST_CHARGE_STEPS 1
    #define SLOW_CHARGE_STEPS 1
    #define PLAN_COUNT 3

    void setType(ChargeType);
    void setBatteryVoltage(int3float);
    void setCapacity(unsigned int);
    void startCharge(void);
    void stopCharge(void);
    
    int3float getBatteryVoltage(void);
    unsigned int getCapacity(void);
    ChargeType getType(void);
    ChargeStep* getActiveStep();
    ControlType getControlType(void);
    int3float getControlValue(void);
    char const * getChargeTypeDesc(ChargeType);
    void OnControlTick(void);
    bool changedStep(void);
    bool OnCheckFaultyConditions(void);
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
