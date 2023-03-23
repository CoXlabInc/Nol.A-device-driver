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
   \date 2022. 7. 19.
*/

#include "T9602_I2C.hpp"

void T9602_I2C::begin() {
  this->wire.begin();
}

float T9602_I2C::readTemperature() {
  this->wire.beginTransmission(this->address);
  this->wire.write(0x02);
  this->wire.endTransmission();

  uint8_t val[2];
  this->wire.requestFrom(this->address, 2);
  for (uint8_t i = 0; i < sizeof(val); i++) {
    val[i] = this->wire.read();
  }

  return (float) (((uint16_t) val[0] << 6) + (val[1] >> 2)) / 16384.0 * 165.0 - 40.0;
}

float T9602_I2C::readHumidity() {
  this->wire.beginTransmission(this->address);
  this->wire.write(0x00);
  this->wire.endTransmission();

  uint8_t val[2];
  this->wire.requestFrom(this->address, 2);
  for (uint8_t i = 0; i < sizeof(val); i++) {
    val[i] = this->wire.read();
  }

  return (float) ((((uint16_t) val[0] & 0x3f) << 8) + val[1]) / 163.84;
}

void T9602_I2C::readHumidityAndTemperature(float *h, float *t) {
  this->wire.beginTransmission(this->address);
  this->wire.write(0x00);
  this->wire.endTransmission();

  uint8_t val[4];
  this->wire.requestFrom(this->address, 4);
  for (uint8_t i = 0; i < sizeof(val); i++) {
    val[i] = this->wire.read();
  }

  if (h) {
    *h = (float) ((((uint16_t) val[0] & 0x3f) << 8) + val[1]) / 163.84;
  }

  if (t) {
    *t = (float) (((uint16_t) val[2] << 6) + (val[3] >> 2)) / 16384.0 * 165.0 - 40.0;
  }  
}
