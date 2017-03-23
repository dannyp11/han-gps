#include "lcd.h"
#include "LCD_ll.h"
#include "chprintf.h"

#define BUFF_LEN LCD_LINE_LEN + 1

void lcd_demo(void) {
  static char buffer[BUFF_LEN];

  LCDSendCommand(CURSOROFF);
  LCDSendCommand(SHOWFIRMWARE);

  LCDSetCursor(2, 0);
  LCDPrint("LCD:D.Pham,OS:R.Dong");
  LCDSetCursor(3, 0);
  chsnprintf(buffer, BUFF_LEN, "Version: %s", __DATE__);
  LCDPrint(buffer);

  char i;
  for (i = 0; i < 5; ++i) {
    LCDSetCursor(4, 0);
    chsnprintf(buffer, BUFF_LEN, "Demo in %d seconds...", 5 - i);
    LCDPrint(buffer);
    chThdSleepMilliseconds(1000);
  }
  LCDClear();

  /*
	 * brightness
	 */
  LCDPrint("Changing brightness");
  for (i = 1; i <= 8; ++i) {
    LCDSetCursor(3, 0);
    chsnprintf(buffer, BUFF_LEN, "Level: %d", (int) i);
    LCDPrint(buffer);
    LCDSetBrightness(i);
    chThdSleepMilliseconds(500);
  }
  LCDClear();

  /*
	 * contrast
	 */
  LCDPrint("Changing contrast");
  for (i = 1; i <= 50; ++i) {
    LCDSetCursor(3, 0);
    chsnprintf(buffer, BUFF_LEN, "Level: %d", (int) i);
    LCDPrint(buffer);
    LCDSetContrast(i);
    chThdSleepMilliseconds(50);
  }
  LCDClear();

  /*
	 * Turn on and off
	 */
  char *blinking_msg = "..Blinking Srceen...";
  LCDPrint(blinking_msg);
  LCDSetCursor(2, 0);
  LCDPrint(blinking_msg);
  LCDSetCursor(3, 0);
  LCDPrint(blinking_msg);
  LCDSetCursor(4, 0);
  LCDPrint(blinking_msg);
  for (i = 1; i <= 5; ++i) {
    LCDSendCommand(LCDOFF);
    chThdSleepMilliseconds(500);
    LCDSendCommand(LCDON);
    chThdSleepMilliseconds(500);
  }
  LCDClear();

  /*
	 * Move cursor
	 */
  LCDPrint("Move Cursor");
  chThdSleepMilliseconds(2000);
  LCDSendCommand(CLEARSCREEN);
  LCDSendCommand(CURSORON);
  char j;
  for (i = 1; i <= 4; ++i) {
    for (j = 0; j < 20; ++j) {
      LCDSetCursor(i, j);
      chThdSleepMilliseconds(100);
    }
  }
  LCDClear();

  LCDSetCursor(2, 5);
  LCDPrint("Done demo");
}

THD_WORKING_AREA(waTdLCD, LCD_WA_SIZE);
THD_FUNCTION(tdLCD, arg) {
  (void)arg;

  LCDInit();
  while (true) {
    lcd_demo();
    chThdSleepSeconds(1);
  }
}