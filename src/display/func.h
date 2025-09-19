/* ************************************************************************
 *
 *   Display functions
 *
 *   (c) 2025 by gadefox@EEVblog
 *   based on code from Markus Frejek and Karl-Heinz Kübbeler and Markus Reschke
 *
 * ************************************************************************ */

#ifndef DISPLAY_FUNC_H
#define DISPLAY_FUNC_H


extern void LCD_BusSetup(void);
extern void LCD_Cmd(uint8_t Cmd);
extern void LCD_AddressWindow(void);
extern void LCD_CharPos(uint8_t x, uint8_t y);
extern void LCD_ClearLine(uint8_t Line);
extern void LCD_Clear(void);
extern void LCD_Init(void);
extern void LCD_Char(unsigned char Char);
extern void LCD_Cursor(uint8_t Mode);
extern void LCD_Contrast(uint8_t Contrast);
extern void LCD_Band(uint16_t Color, uint8_t Align);

#ifdef SW_SYMBOLS
extern void LCD_Symbol(uint8_t ID);
#endif

#if !defined(UI_SERIAL_COPY) && !defined(UI_SERIAL_COMMANDS)
/* make Display_Char() an alias for LCD_Char() */
#define Display_Char LCD_Char
#endif


#endif // DISPLAY_FUNC_H
