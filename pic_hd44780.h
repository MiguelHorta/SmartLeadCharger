/**
 * @author  Tilen Majerle
 * @email   tilen@majerle.eu
 * @website http://stm32f4-discovery.com
 * @link    http://stm32f4-discovery.com/2014/06/library-16-interfacing-hd44780-lcd-controller-with-stm32f4/
 * @version v1.2
 * @ide     Keil uVision
 * @license GNU GPL v3
 * @brief   HD44780 LCD driver library for STM32F4xx
 *	
@verbatim
   ----------------------------------------------------------------------
    Copyright (C) Tilen Majerle, 2015
    Portions (C) Miguel Horta, 2017
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.
     
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
   ----------------------------------------------------------------------
@endverbatim
 */
#ifndef TM_HD44780_H
#define TM_HD44780_H 120

#include <stdint.h>
#define _SUPPRESS_PLIB_WARNING 1
#include <p32xxxx.h>
#include <plib.h>
#include "slc_util.h"

/**
 * Originaly this lib target STM32F4xx MCU series, ported to pic32
 */

/**
 * @defgroup TM_HD44780
 * @brief    HD44780 LCD driver library for STM32F4xx - http://stm32f4-discovery.com/2014/06/library-16-interfacing-hd44780-lcd-controller-with-stm32f4/
 * @{
 *
 *	It also supports all HD44780 compatible LCD drivers.
 *
 *	\par Default pinout
 *	
@verbatim
LCD		STM32F4XX		DESCRIPTION

GND		GND				Ground
VCC		+5V				Power supply for LCD
V0		Potentiometer	Contrast voltage. Connect to potentiometer
RS		PB2				Register select, can be overwritten in your project's defines.h file
RW		GND				Read/write
E		PB7				Enable pin, can be overwritten in your project's defines.h file
D0		-				Data 0 - doesn't care
D1		-				Data 1 - doesn't care
D2		-				Data 2 - doesn't care
D3		-				Data 3 - doesn't  care
D4		PC12			Data 4, can be overwritten in your project's defines.h file
D5		PC13			Data 5, can be overwritten in your project's defines.h file
D6		PB12			Data 6, can be overwritten in your project's defines.h file
D7		PB13			Data 7, can be overwritten in your project's defines.h file
A		+3V3			Back light positive power
K		GND				Ground for back light
@endverbatim	
 */
//RS - Register select pin
#define HD44780_RS_PORT         LATEbits
#define HD44780_RS_PIN          LATE0
#define HD44780_RS_PORT_CFG     TRISEbits
#define HD44780_RS_PIN_CFG      TRISE0
//E - Enable pin
#define HD44780_E_PORT          LATEbits
#define HD44780_E_PIN           LATE1
#define HD44780_E_PORT_CFG      TRISEbits
#define HD44780_E_PIN_CFG       TRISE1
//D4 - Data 4 pin
#define HD44780_D4_PORT         LATEbits
#define HD44780_D4_PIN          LATE2
#define HD44780_D4_PORT_CFG     TRISEbits
#define HD44780_D4_PIN_CFG      TRISE2
//D5 - Data 5 pin
#define HD44780_D5_PORT         LATEbits
#define HD44780_D5_PIN          LATE3
#define HD44780_D5_PORT_CFG     TRISEbits
#define HD44780_D5_PIN_CFG      TRISE3
//D6 - Data 6 pin
#define HD44780_D6_PORT         LATEbits
#define HD44780_D6_PIN          LATE4
#define HD44780_D6_PORT_CFG     TRISEbits
#define HD44780_D6_PIN_CFG      TRISE4
//D7 - Data 7 pin
#define HD44780_D7_PORT         LATEbits
#define HD44780_D7_PIN          LATE5
#define HD44780_D7_PORT_CFG     TRISEbits
#define HD44780_D7_PIN_CFG      TRISE5

/**
 * @defgroup TM_HD44780_Functions
 * @brief    Library Functions
 * @{
 */

/**
 * @brief  Initializes HD44780 LCD
 * @brief  cols: width of lcd
 * @param  rows: height of lcd
 * @retval None
 */
void TM_HD44780_Init(uint8_t cols, uint8_t rows);

/**
 * @brief  Turn display on
 * @param  None
 * @retval None
 */
void TM_HD44780_DisplayOn(void);

/**
 * @brief  Turn display off
 * @param  None
 * @retval None
 */
void TM_HD44780_DisplayOff(void);

/**
 * @brief  Clears entire LCD
 * @param  None
 * @retval None
 */
void TM_HD44780_Clear(void);

/**
 * @brief  Puts string on lcd
 * @param  x location
 * @param  y location
 * @param  *str: pointer to string to display
 * @retval None
 */
void TM_HD44780_Puts(uint8_t x, uint8_t y, char* str);

/**
 * @brief  Enables cursor blink
 * @param  None
 * @retval None
 */
void TM_HD44780_BlinkOn(void);

/**
 * @brief  Disables cursor blink
 * @param  None
 * @retval None
 */
void TM_HD44780_BlinkOff(void);
/**
 * @brief  Move cursor
 * @param  col col
 * * @param  row row
 * @retval None
 */
void TM_HD44780_CursorSet(uint8_t col, uint8_t row);
/**
 * @brief  Shows cursor
 * @param  None
 * @retval None
 */
void TM_HD44780_CursorOn(void);

/**
 * @brief  Hides cursor
 * @param  None
 * @retval None
 */
void TM_HD44780_CursorOff(void);

/**
 * @brief  Scrolls display to the left
 * @param  None
 * @retval None
 */
void TM_HD44780_ScrollLeft(void);

/**
 * @brief  Scrolls display to the right
 * @param  None
 * @retval None
 */
void TM_HD44780_ScrollRight(void);

/**
 * @brief  Creates custom character
 * @param  location: Location where to save character on LCD. LCD supports up to 8 custom characters, so locations are 0 - 7
 * @param *data: Pointer to 8-bytes of data for one character
 * @retval None
 */
void TM_HD44780_CreateChar(uint8_t location, uint8_t* data);

/**
 * @brief  Puts custom created character on LCD
 * @param  location: Location on LCD where character is stored, 0 - 7
 * @retval None
 */
void TM_HD44780_PutCustom(uint8_t x, uint8_t y, uint8_t location);

/**
 * @}
 */
 
/**
 * @}
 */
 
/**
 * @}
 */

#endif

