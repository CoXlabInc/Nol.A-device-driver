#include <stdlib.h>
#include <algorithm>
#include "KXM12864M_3.hpp"

#define BLACK 1					//Defines color - Black color -> Bit in buffer is set to one
#define WHITE 0					//Defines color - White color -> Bit in buffer is set to zero

KXM12864M_3::KXM12864M_3(SPI &s, int8_t cs)
  : ST7920(128, 64, s, cs) {
}

void KXM12864M_3::invalidate() {
  if (!this->buff) {
    return;
  }
  
  this->spi.begin(8000000ul, SPI::MSBFIRST, SPI::MODE3);
  digitalWrite(cs, HIGH);

  ST7920Command(0b00100100); //EXTENDED INSTRUCTION SET
  ST7920Command(0b00100110); //EXTENDED INSTRUCTION SET

  /*
    +--------+
    | Upper  |            +--------+--------+
    | Lower  |       =>   | Upper  | Lower  |
    +--------+            +--------+--------+
    Actual Display        GDRAM address
   */
  for (int16_t y = 0; y < this->HEIGHT / 2; y++) {
    ST7920Command(0x80 | y);
    ST7920Command(0x80 | 0);

    for (int16_t x = 0; x < this->WIDTH / 16; x++) {
      uint16_t d = this->buff[y * (this->WIDTH / 16) + x];
      ST7920Data(d >> 8);
      ST7920Data(d & 0xFF);
    }

    /* For lower half block */
    for (int16_t x = 0; x < this->WIDTH / 16; x++) {
      uint16_t d = this->buff[(y + this->HEIGHT / 2) * (this->WIDTH / 16) + x];
      ST7920Data(d >> 8);
      ST7920Data(d & 0xFF);
    }
  }
  digitalWrite(cs, LOW);
  this->spi.end();
}
