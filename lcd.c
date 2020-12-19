#include "lcd.h"

#include <pigpio.h>
#include <stdarg.h>
#include <stdio.h>

static void SetDBPinMode(struct LCD* lcd, unsigned mode) {
  gpioSetMode(lcd->pin_db7, mode);
  gpioSetMode(lcd->pin_db6, mode);
  gpioSetMode(lcd->pin_db5, mode);
  gpioSetMode(lcd->pin_db4, mode);
}

static uint8_t ReadOnce(struct LCD* lcd) {
  gpioWrite(lcd->pin_e, 1);
  gpioDelay(1);
  uint8_t inp_4bits = gpioRead(lcd->pin_db7) << 3 |
                      gpioRead(lcd->pin_db6) << 2 |
                      gpioRead(lcd->pin_db5) << 1 |
                      gpioRead(lcd->pin_db4);
  gpioWrite(lcd->pin_e, 0);
  gpioDelay(1);
  return inp_4bits;
}

static void WriteOnce(struct LCD* lcd, uint8_t data_4bits) {
  gpioWrite(lcd->pin_e, 1);
  gpioWrite(lcd->pin_db7, (data_4bits >> 3) & 1);
  gpioWrite(lcd->pin_db6, (data_4bits >> 2) & 1);
  gpioWrite(lcd->pin_db5, (data_4bits >> 1) & 1);
  gpioWrite(lcd->pin_db4, (data_4bits >> 0) & 1);
  gpioDelay(1);
  gpioWrite(lcd->pin_e, 0);
  gpioDelay(1);
}

static void WaitBusy(struct LCD* lcd) {
  while (lcdRead(lcd, LCD_RS_CMD) & 0x80) {
    gpioDelay(1);
  }
}

void lcdInitialize(struct LCD* lcd) {
  gpioSetMode(lcd->pin_rs, PI_OUTPUT);
  gpioSetMode(lcd->pin_rw, PI_OUTPUT);
  gpioSetMode(lcd->pin_e,  PI_OUTPUT);
  SetDBPinMode(lcd, PI_OUTPUT);
  gpioSetPullUpDown(lcd->pin_db4, PI_PUD_OFF);
  gpioSetPullUpDown(lcd->pin_db5, PI_PUD_OFF);
  gpioSetPullUpDown(lcd->pin_db6, PI_PUD_OFF);
  gpioSetPullUpDown(lcd->pin_db7, PI_PUD_OFF);

  gpioWrite(lcd->pin_rs, LCD_RS_CMD);

  WriteOnce(lcd, 0x3); // function set 8 bits
  gpioDelay(37);
  WriteOnce(lcd, 0x3); // function set 8 bits
  gpioDelay(37);
  WriteOnce(lcd, 0x2); // function set 4 bits
  gpioDelay(37);

  lcdWrite(lcd, LCD_RS_CMD,
           LCD_FUNCTION_SET | LCD_4BITS | LCD_2LINES | LCD_FONT_5x8);
  lcdWrite(lcd, LCD_RS_CMD, LCD_DISPLAY_ONOFF | LCD_ENTIRE_OFF);
  lcdWrite(lcd, LCD_RS_CMD, LCD_CLEAR_DISPLAY);
  lcdWrite(lcd, LCD_RS_CMD, LCD_RETURN_HOME);
  lcdWrite(lcd, LCD_RS_CMD,
           LCD_DISPLAY_ONOFF | LCD_ENTIRE_ON | LCD_CURSOR_ON | LCD_BLINK_OFF);
  lcdWrite(lcd, LCD_RS_CMD,
           LCD_ENTRY_MODE_SET | LCD_CURSOR_INC | LCD_DISPLAY_NOSHIFT);
}

uint8_t lcdRead(struct LCD* lcd, unsigned rs) {
  SetDBPinMode(lcd, PI_INPUT);
  gpioWrite(lcd->pin_rs, rs);
  gpioWrite(lcd->pin_rw, 1);
  uint8_t inp = ReadOnce(lcd) << 4;
  inp |= ReadOnce(lcd);
  gpioWrite(lcd->pin_rw, 0);
  gpioWrite(lcd->pin_rs, 1); // to save energy
  return inp;
}

void lcdWrite(struct LCD* lcd, unsigned rs, uint8_t data) {
  WaitBusy(lcd);
  SetDBPinMode(lcd, PI_OUTPUT);
  gpioWrite(lcd->pin_rs, rs);
  gpioWrite(lcd->pin_rw, 0);
  WriteOnce(lcd, data >> 4);
  WriteOnce(lcd, data);
  gpioWrite(lcd->pin_rs, 1); // to save energy
  SetDBPinMode(lcd, PI_INPUT); // to save energy
}

void lcdPutStr(struct LCD* lcd, const char* s) {
  while (*s >= 0x20) {
    lcdWrite(lcd, LCD_RS_DATA, *s);
    ++s;
  }
}

void lcdPutMem(struct LCD* lcd, const void* buf, size_t n) {
  const char* s = (const char*)buf;
  for (size_t i = 0; i < n; ++i) {
    lcdWrite(lcd, LCD_RS_DATA, s[i]);
  }
}

void lcdPrintf(struct LCD* lcd, const char* format, ...) {
  char s[41];
  va_list args;

  va_start(args, format);
  vsnprintf(s, sizeof(s), format, args);
  va_end(args);

  lcdPutStr(lcd, s);
}
