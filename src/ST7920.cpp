#include <stdlib.h>

#include "Adafruit_GFX.hpp"
#include "ST7920.hpp"

#include <stdio.h>

#define ST7920_HEIGHT 	64		//64 pixels tall display
#define ST7920_WIDTH	128		//128 pixels wide display

#define BLACK 1					//Defines color - Black color -> Bit in buffer is set to one
#define WHITE 0					//Defines color - White color -> Bit in buffer is set to zero

uint16_t buff[ST7920_HEIGHT][ST7920_WIDTH / 16];

void ST7920::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if(x<0 || x>=ST7920_WIDTH || y<0 || y>=ST7920_HEIGHT) return;
  if (color == BLACK) {
    bitSet(buff[y][x / 16], 15 - (x % 16));
  } else {
    bitClear(buff[y][x / 16], 15 - (x % 16));
  }
}

ST7920::ST7920(SPI &s, int8_t CS) : Adafruit_GFX(ST7920_WIDTH, ST7920_HEIGHT), spi(s), cs(CS) {
}

void ST7920::begin(void) {
  pinMode(cs, OUTPUT);
  digitalWrite(cs, LOW);

  this->spi.begin(8000000ul, SPI::MSBFIRST, SPI::MODE3);
  digitalWrite(cs, HIGH);

  ST7920Command(0b00001100);

  digitalWrite(cs, LOW);
  this->spi.end();
}

void ST7920::clearDisplay() {
  long* p = (long*)&buff;
  for (int i = 0; i < 256; i++) {
    p[i] = 0;
  }
}

void ST7920::display() {
  int x = 0, y = 0, n = 0;

  this->spi.begin(8000000ul, SPI::MSBFIRST, SPI::MODE3);
  digitalWrite(cs, HIGH);

  ST7920Command(0b00100100); //EXTENDED INSTRUCTION SET
  ST7920Command(0b00100110); //EXTENDED INSTRUCTION SET

  for (y = 0; y < ST7920_HEIGHT / 2; y++) {
    ST7920Command(0x80 | y);
    ST7920Command(0x80 | 0);
    for (x = 0; x < 8; x++) {
      uint16_t d = buff[y][x];
      ST7920Data(d >> 8);
      ST7920Data(d & 0xFF);
    }

    for (x = 0; x < 8; x++) {
      uint16_t d = buff[y + 32][x];
      ST7920Data(d >> 8);
      ST7920Data(d & 0xFF);
    }
  }
  digitalWrite(cs, LOW);
  this->spi.end();
}

void ST7920::invertDisplay() {
  long* p = (long*)&buff;
  for(int i = 0; i<256; i++) {
    p[i] = ~p[i];
  }
}

void ST7920::ST7920Data(uint8_t data) { //RS = 1 RW = 0
  this->spi.transfer(0b11111010);
  this->spi.transfer((data & 0b11110000));
  this->spi.transfer((data & 0b00001111) << 4);
  delayMicroseconds(38);
}

void ST7920::ST7920Command(uint8_t data) { //RS = 0 RW = 0
  this->spi.transfer(0b11111000);
  this->spi.transfer((data & 0b11110000));
  this->spi.transfer((data & 0b00001111) << 4);
  delayMicroseconds(38);
}

void ST7920::backlight() {
}

void ST7920::noBacklight() {
}

void ST7920::cursor() {
}

void ST7920::blink() {
}
