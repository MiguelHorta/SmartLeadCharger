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

#ifndef SLC_CONTROL_LOOP_H    /* Guard against multiple inclusion */
#define SLC_CONTROL_LOOP_H

#include <stdint.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <p32xxxx.h>
#include <plib.h>
#include "slc_util.h"

#define CONTROL_LOOP_FREQ 1000

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
    void slc_InitControlLoop();
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
