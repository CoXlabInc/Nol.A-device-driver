// -*- mode:c++; indent-tabs-mode:nil; -*-
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 CoXlab Inc.
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
   \author Jongsoo Jeong (CoXlab)
   \date 2016. 6. 13.
*/

#include <HTU2xD.hpp>
#include <stdio.h>

void HTU2xD::begin() {
  this->wire.begin();

  this->reset();
}

void HTU2xD::reset() {
  this->wire.beginTransmission(0x40);
  this->wire.write(0xFE);
  this->wire.endTransmission();
}

float HTU2xD::readTemperature() {
  this->wire.beginTransmission(0x40);
  this->wire.write(0xE3); //Trigger temperature measurement with holding master.
  this->wire.endTransmission();

  this->wire.requestFrom(0x40, 3);

  uint16_t sTemp = this->wire.read() << 8;
  sTemp |= this->wire.read() & 0b11111100;
  this->wire.read(); //checksum (skip)

  return ((float) sTemp / 65536.0  * 175.72 - 46.85);
}

float HTU2xD::readHumidity() {
  this->wire.beginTransmission(0x40);
  this->wire.write(0xE5); //Trigger humidity measurement with holding master.
  this->wire.endTransmission();

  this->wire.requestFrom(0x40, 3);

  uint16_t sRH = this->wire.read() << 8;
  sRH |= this->wire.read() & 0b11111100;
  this->wire.read(); //checksum (skip)

  return ((float) sRH / 65536.0 * 125.0 - 6.0);
}
