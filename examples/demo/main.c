#include <pigpio.h>
#include <pilcd.h>
#include <stdio.h>

#define PIN_LED 21

#define PIN_VO 27
#define PIN_RS 2
#define PIN_RW 3
#define PIN_E  4
#define PIN_DB4 14
#define PIN_DB5 15
#define PIN_DB6 17
#define PIN_DB7 18

extern const char _binary_text1_txt_start[], _binary_text1_txt_end[];
extern const char _binary_text1_txt_size;
extern const char _binary_text2_txt_start[], _binary_text2_txt_end[];
extern const char _binary_text2_txt_size;

int main() {
  if (gpioInitialise() < 0) {
    return -1;
  }
  printf("hello\n");

  gpioSetMode(PIN_LED, PI_OUTPUT);
  gpioPWM(PIN_VO, 80);

  struct LCD lcd = {
    PIN_RS, PIN_RW, PIN_E,
    PIN_DB4, PIN_DB5, PIN_DB6, PIN_DB7
  };
  lcdInitialize(&lcd);

  FILE* thermal_file = fopen("/sys/class/thermal/thermal_zone0/temp", "r");
  int temp = 0;
  fscanf(thermal_file, "%d", &temp);

  lcdPrintf(&lcd, "CPU temp = %d.%d", temp / 1000, (temp / 100) % 10);

  gpioSleep(0, 2, 0);

  lcdCursorAt(&lcd, LCD_DDRAM_LINE2);
  lcdPutStr(&lcd, _binary_text1_txt_start);
  for (int i = 0; i < 5; ++i) {
    lcdCommand(&lcd, LCD_CURSOR_DISPLAY | LCD_SHIFT_DISPLAY | LCD_SHIFT_LEFT);
    gpioSleep(0, 0, 500 * 1000);
  }

  for (int i = 0; i < 5; ++i) {
    lcdCommand(&lcd, LCD_CURSOR_DISPLAY | LCD_SHIFT_DISPLAY | LCD_SHIFT_RIGHT);
    gpioSleep(0, 0, 500 * 1000);
  }

  int addr_text1_end = lcdGetCursorAt(&lcd);

  lcdPutStr(&lcd, " ");
  lcdPutStr(&lcd, _binary_text2_txt_start);

  lcdCursorAt(&lcd, addr_text1_end);

  for (int i  = 0, ms = 100; i < 20; ++i) {
    lcdCommand(&lcd, LCD_CURSOR_DISPLAY | LCD_SHIFT_DISPLAY | LCD_SHIFT_LEFT);
    gpioSleep(0, 0, ms * 1000);
    ms *= 1.1;
  }

  for (int i = 0; i < 10; ++i) {
    lcdCommand(&lcd, LCD_CURSOR_DISPLAY | LCD_SHIFT_CURSOR | LCD_SHIFT_RIGHT);
    gpioSleep(0, 0, 200 * 1000);
  }

  gpioSleep(0, 1, 0);

  for (int i = 0; i < 10; ++i) {
    lcdCommand(&lcd, LCD_CURSOR_DISPLAY | LCD_SHIFT_CURSOR | LCD_SHIFT_LEFT);
    gpioSleep(0, 0, 100 * 1000);
  }

  gpioSleep(0, 1, 0);

  lcdCommand(&lcd, LCD_SET_CGRAM_ADDR | LCD_CGRAM_CHARSIZE * 1);
  lcdPutMem(&lcd, "\x04\x0a\x11\x15\x15\x11\x0a\x04", 8);

  lcdCursorAt(&lcd, addr_text1_end + 4);
  lcdPutMem(&lcd, "\x01\x01\x01\x01\x01\x01\x01\x01", 8);

  gpioSleep(0, 2, 0);

  gpioTerminate();
}
