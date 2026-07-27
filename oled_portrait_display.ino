#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "portrait2_bmp.h"   // ← changed from portrait_bmp.h

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    for (;;);
  }

  display.clearDisplay();
  display.drawBitmap(0, 0, portrait2_bmp, PORTRAIT2_WIDTH, PORTRAIT2_HEIGHT, SSD1306_WHITE);
  display.display();
}

void loop() {
}