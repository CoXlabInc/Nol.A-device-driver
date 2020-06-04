/*
  Imported from https://github.com/BornaBiro/ST7920_GFX_Library
*/
#pragma once

#include <SPI.hpp>
#include <Adafruit_GFX.hpp>

class ST7920 : public Adafruit_GFX {
public:
  ST7920(uint16_t width, uint16_t height, SPI &, int8_t CS);

  bool begin(void);
  void clearDisplay(void);
  void invertDisplay(void);
  bool isInverted();
  virtual void invalidate();

  void drawPixel(int16_t x, int16_t y, uint16_t color);

  void backlight();
  void noBacklight();
  void cursor();
  void blink();

protected:
  SPI &spi;
  int8_t cs;
  uint16_t *buff = nullptr;
  void ST7920Data(uint8_t data);
  void ST7920Command(uint8_t data);

private:
  bool _isInverted = false;
};
