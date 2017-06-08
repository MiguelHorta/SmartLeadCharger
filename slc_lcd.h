#ifndef SLC_LCD_H    /* Guard against multiple inclusion */
#define SLC_LCD_H


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
#include "pic_hd44780.h"

#define MINUS_BTN   PORTDbits.RD4
#define PLUS_BTN    PORTDbits.RD5
#define CANCEL_BTN  PORTDbits.RD6
#define OK_BTN      PORTDbits.RD7
#define MINUS_BTN_CFG   TRISDbits.TRISD4
#define PLUS_BTN_CFG    TRISDbits.TRISD5
#define CANCEL_BTN_CFG  TRISDbits.TRISD6
#define OK_BTN_CFG      TRISDbits.TRISD7
/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    typedef void (*button_press)(void);
    typedef void (*lcd_event)(void);
    typedef struct
    {
        lcd_event new_state;
        lcd_event on_tick;
        button_press minus;
        button_press plus;
        button_press cancel;
        button_press ok;
    }LcdScreen;
    
    void slc_InitLCD(void);
    uint32_t getCurrentTick(void);
#ifdef __cplusplus
}
#endif

#endif /* SLC_LCD_H */

/* *****************************************************************************
 End of File
 */
