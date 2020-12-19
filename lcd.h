#pragma once

#include <stddef.h>
#include <stdint.h>

#define LCD_RS_CMD  0
#define LCD_RS_DATA 1

#define LCD_CLEAR_DISPLAY   0x01
#define LCD_RETURN_HOME     0x02

#define LCD_ENTRY_MODE_SET  0x04
#define LCD_CURSOR_INC      0x02
#define LCD_CURSOR_DEC      0
#define LCD_DISPLAY_SHIFT   0x01
#define LCD_DISPLAY_NOSHIFT 0

#define LCD_DISPLAY_ONOFF   0x08
#define LCD_ENTIRE_ON       0x04
#define LCD_ENTIRE_OFF      0
#define LCD_CURSOR_ON       0x02
#define LCD_CURSOR_OFF      0
#define LCD_BLINK_ON        0x01
#define LCD_BLINK_OFF       0

#define LCD_CURSOR_DISPLAY  0x10
#define LCD_SHIFT_DISPLAY   0x08
#define LCD_SHIFT_CURSOR    0
#define LCD_SHIFT_RIGHT     0x04
#define LCD_SHIFT_LEFT      0

#define LCD_FUNCTION_SET    0x20
#define LCD_8BITS           0x10
#define LCD_4BITS           0
#define LCD_2LINES          0x08
#define LCD_1LINE           0
#define LCD_FONT_5x11       0x04
#define LCD_FONT_5x8        0

#define LCD_SET_CGRAM_ADDR  0x40
#define LCD_SET_DDRAM_ADDR  0x80

#define LCD_DDRAM_LINE1     0x00
#define LCD_DDRAM_LINE2     0x40
#define LCD_DDRAM_LINE3     0x14
#define LCD_DDRAM_LINE4     0x54

#define LCD_CGRAM_CHARSIZE  0x08

struct LCD {
  unsigned int pin_rs, pin_rw, pin_e;
  unsigned int pin_db4, pin_db5, pin_db6, pin_db7;
};

/*******************
 * basic functions *
 *******************/
void lcdInitialize(struct LCD* lcd);
uint8_t lcdRead(struct LCD* lcd, unsigned rs);
void lcdWrite(struct LCD* lcd, unsigned rs, uint8_t data);

#define lcdCommand(lcd, cmd) \
  lcdWrite((lcd), LCD_RS_CMD, (cmd))
#define lcdData(lcd, data) \
  lcdWrite((lcd), LCD_RS_DATA, (data))

/*********************
 * utility functions *
 *********************/
#define lcdCursorAt(lcd, pos) \
  lcdCommand((lcd), LCD_SET_DDRAM_ADDR | (pos))
#define lcdGetCursorAt(lcd) \
  (lcdRead((lcd), LCD_RS_CMD) & 0x7f)

void lcdPutStr(struct LCD* lcd, const char* s);
void lcdPutMem(struct LCD* lcd, const void* buf, size_t n);
void lcdPrintf(struct LCD* lcd, const char* format, ...);
