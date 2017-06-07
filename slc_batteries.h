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

#ifndef SLC_BATTERIES_H    /* Guard against multiple inclusion */
#define SLC_BATTERIES_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdbool.h>
#include <stdint.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif
    typedef enum
    {
        BV_NONE,
        BV_14V7,
    }BatteryVoltage;
    typedef enum
    {
        BC_NONE,
        BC_2200,
    }BatteryCapacity;
    /* Provide C++ Compatibility */
    float getActualVoltagePercentage(float v);
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
