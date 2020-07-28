#include <stdlib.h>
#include <algorithm>
#include "ST7920.hpp"

#define BLACK 1					//Defines color - Black color -> Bit in buffer is set to one
#define WHITE 0					//Defines color - White color -> Bit in buffer is set to zero

void ST7920::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if (!this->buff) return;
  
  if (x < 0 || x >= this->WIDTH ||
      y < 0 || y >= this->HEIGHT ||
      (color != BLACK && color != WHITE)) {
    return;
  }

  int16_t t;
  switch (rotation) {
  case 1:
    t = x;
    x = this->WIDTH - 1 - y;
    y = t;
    break;
  case 2:
    x = this->WIDTH - 1 - x;
    y = this->HEIGHT - 1 - y;
    break;
  case 3:
    t = x;
    x = y;
    y = this->HEIGHT - 1 - t;
    break;
  default:
    break;
  }
  
  if ((color == BLACK && !this->_isInverted) || (color == WHITE && this->_isInverted)) {
    bitSet(this->buff[y * (this->WIDTH / 16) + (x / 16)], 15 - (x % 16));
  } else {
    bitClear(this->buff[y * (this->WIDTH / 16) + (x / 16)], 15 - (x % 16));
  }
}

ST7920::ST7920(uint16_t width, uint16_t height, SPI &s, int8_t CS)
  : Adafruit_GFX((int16_t) width, (int16_t) height), spi(s), cs(CS) {
  this->buff = new uint16_t[height * (width / 16)];
}

bool ST7920::begin(void) {
  if (!this->buff) {
    return false;
  }
  
  pinMode(cs, OUTPUT);
  digitalWrite(cs, LOW);

  this->spi.begin(8000000ul, SPI::MSBFIRST, SPI::MODE3);
  digitalWrite(cs, HIGH);

  ST7920Command(0b00001100);

  digitalWrite(cs, LOW);
  this->spi.end();
  return true;
}

void ST7920::clearDisplay() {
  if (!this->buff) {
    return;
  }

  for (int16_t y = 0; y < this->HEIGHT; y++) {
    for (int16_t x = 0; x < this->WIDTH / 16; x++) {
      this->buff[y * (this->WIDTH / 16) + x] = (this->_isInverted) ? 0xFFFF : 0;
    }
  }
}

void ST7920::invalidate() {
  if (!this->buff) {
    return;
  }
  
  this->spi.begin(8000000ul, SPI::MSBFIRST, SPI::MODE3);
  digitalWrite(cs, HIGH);

  ST7920Command(0b00100100); //EXTENDED INSTRUCTION SET
  ST7920Command(0b00100110); //EXTENDED INSTRUCTION SET

  for (int16_t y = 0; y < this->HEIGHT; y++) {
    ST7920Command(0x80 | y);
    ST7920Command(0x80 | 0);
    for (int16_t x = 0; x < this->WIDTH / 16; x++) {
      uint16_t d = this->buff[y * (this->WIDTH / 16) + x];
      ST7920Data(d >> 8);
      ST7920Data(d & 0xFF);
    }
  }
  digitalWrite(cs, LOW);
  this->spi.end();
}

void ST7920::invertDisplay() {
  if (!this->buff) {
    return;
  }

  for (int16_t y = 0; y < this->HEIGHT; y++) {
    for (int16_t x = 0; x < this->WIDTH / 16; x++) {
      this->buff[y * (this->WIDTH / 16) + x] = ~this->buff[y * (this->WIDTH / 16) + x];
    }
  }
  this->_isInverted = !this->_isInverted;
}

bool ST7920::isInverted() {
  return this->_isInverted;
}

void ST7920::ST7920Data(uint8_t data) { //RS = 1 RW = 0
  this->spi.transfer(0b11111010);
  this->spi.transfer((data & 0b11110000));
  this->spi.transfer((data & 0b00001111) << 4);
  delayMicroseconds(72);
}

void ST7920::ST7920Command(uint8_t data) { //RS = 0 RW = 0
  this->spi.transfer(0b11111000);
  this->spi.transfer((data & 0b11110000));
  this->spi.transfer((data & 0b00001111) << 4);
  delayMicroseconds(72);
}

void ST7920::backlight() {
}

void ST7920::noBacklight() {
}

void ST7920::cursor() {
}

void ST7920::blink() {
}
