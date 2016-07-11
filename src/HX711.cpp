/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 CoXlab Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @author Jongsoo Jeong (CoXlab)
 * @date 2016. 7. 8.
 * @brief Nol.A library for digital weight scale based on HX711
 *
 * @note This code is implemented based on Weihong Guan's Arduino library
 * (https://github.com/aguegu/Arduino).
 */

/*
 * Hx711.cpp
 *
 *  Created on: Oct 31, 2012
 *      Author: agu
 */

#include "HX711.hpp"

Hx711::Hx711(uint8_t pin_dout, uint8_t pin_slk) :
  _pin_dout(pin_dout), _pin_slk(pin_slk) {
}

void Hx711::begin() {
  pinMode(_pin_slk, OUTPUT);
  pinMode(_pin_dout, INPUT);

  digitalWrite(_pin_slk, HIGH);
  delayMicroseconds(100);
  digitalWrite(_pin_slk, LOW);

  this->setOffset(getAverage());
  this->setScale();
}

int32_t Hx711::getAveragedMilligram(uint8_t times) {
  return (getAverage(times) - _offset) * 1000 / _scale;
}

int32_t Hx711::getMilligram() {
  return (getRaw() - _offset) * 1000 / _scale;
}

int32_t Hx711::getAverage(uint8_t times) {
  int32_t sum = 0;
  for (uint8_t i = 0; i < times; i++) {
    sum += getRaw();
  }

  return sum / times;
}

int32_t Hx711::getRaw() {
  uint8_t data[3];

  while (digitalRead(_pin_dout)) {
  }

  for (uint8_t j = 3; j--;) {
    for (uint8_t i = 8; i--;) {
      digitalWrite(_pin_slk, HIGH);
      bitWrite(data[j], i, digitalRead(_pin_dout));
      digitalWrite(_pin_slk, LOW);
    }
  }

  digitalWrite(_pin_slk, HIGH);
  digitalWrite(_pin_slk, LOW);

  data[2] ^= 0x80;

  return (((int32_t) data[2] << 16) |
          ((int32_t) data[1] << 8) |
          ((int32_t) data[0] << 0));
}

void Hx711::setOffset(int32_t offset) {
  _offset = offset;
}

void Hx711::setScale(int32_t scale) {
  _scale = scale;
}
