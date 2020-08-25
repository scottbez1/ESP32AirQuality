#include <Arduino.h>
#include <TFT_eSPI.h>

#include "display_task.h"
#include "pms_task.h"

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);

PmsTask pms = PmsTask(0);
DisplayTask display_task = DisplayTask(pms, 1);

/**
 * Pinout:
 *  LCD
 *    GND=GND
 *    VCC=3.3V
 *    CLK=18
 *    MOSI=19
 *    RES=23
 *    DC=16
 *    BLK=4
 * 
 *  PMS5003
 *    1=5V
 *    2=GND
 *    3=3.3v pullup
 *    4=21
 *    5=17
 *    6=3.3v pullup
 *    7=NC
 *    8=NC
 */

void setup() {
  Serial.begin(921600);

  pms.begin();
  display_task.begin();

  // Delete the default Arduino loopTask to free up Core 1
  vTaskDelete(NULL);
}


void loop() {
  assert(false);
}