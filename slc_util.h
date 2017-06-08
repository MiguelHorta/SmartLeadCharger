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

#ifndef SLC_UTIL_H    /* Guard against multiple inclusion */
#define SLC_UTIL_H


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
    #define SYSCLK 80000000L // System clock frequency, in Hz
    #define PBUSCLK 40000000L // Peripheral bus clock

    typedef long int int3float;
    #define i3fM(a,b) ((a)*(b)/(1000))
    #define i3fD(a,b) ((a*(1000))/(b))
    void slc_clamp(uint16_t *n, int lower, int upper );
    void Delay_ms(unsigned int dms);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
