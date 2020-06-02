/*
  Imported from https://github.com/BornaBiro/ST7920_GFX_Library
*/
#pragma once

#include <SPI.hpp>
#include <Adafruit_GFX.hpp>

class ST7920 : public Adafruit_GFX {
public:
  ST7920(SPI &, int8_t CS);

  void begin(void);
  void clearDisplay(void);
  void invertDisplay(void);
  void display();

  void drawPixel(int16_t x, int16_t y, uint16_t color);

  void backlight();
  void noBacklight();
  void cursor();
  void blink();

private:
  SPI &spi;
  int8_t cs;
  void ST7920Data(uint8_t data);
  void ST7920Command(uint8_t data);
};
