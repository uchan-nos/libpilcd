#include <pigpio.h>
#include <pilcd.h>
#include <time.h>

int main() {
  if (gpioInitialise() < 0) {
    return -1;
  }

  gpioSetMode(27, PI_OUTPUT);
  gpioWrite(27, 0); // Set LCD contrast

  struct LCD lcd = {
    2, 3, 4, // RS, R/W, E
    14, 15, 17, 18 // DB4 - DB7
  };
  lcdInitialize(&lcd);
  lcdPrintf(&lcd, "System started");

  time_t caltime = time(NULL);
  struct tm* loctime = localtime(&caltime);

  lcdCursorAt(&lcd, LCD_DDRAM_LINE2);
  lcdPrintf(&lcd, "%d/%d %02d:%02d:%02d",
      loctime->tm_mon + 1,
      loctime->tm_mday,
      loctime->tm_hour,
      loctime->tm_min,
      loctime->tm_sec);

  gpioTerminate();
}
