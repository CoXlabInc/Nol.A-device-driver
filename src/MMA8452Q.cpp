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
 * @date 2016. 6. 13.
 */

#include "MMA8452Q.hpp"

void MMA8452Q::begin(TwoWire &w, uint8_t addr) {
  this->wire = &w;
  this->addr = addr;
}

uint8_t MMA8452Q::readSensorId() {
  return read(REG_WHO_AM_I);
}

void MMA8452Q::readXYZ(int16_t *x, int16_t *y, int16_t *z) {
  wire->beginTransmission(addr);
  wire->write(REG_OUT_X_MSB);
  wire->endTransmission(false);

  wire->requestFrom(addr, 6);

  uint16_t valX = (wire->read() << 4);
  valX |= wire->read();
  uint16_t valY = (wire->read() << 4);
  valY |= wire->read();
  uint16_t valZ = (wire->read() << 4);
  valZ |= wire->read();

  if (x) {
    if (bitRead(valX, 11) == 1) {
      valX |= 0xF000;
    }
    *x = (int16_t) valX;
  }

  if (y) {
    if (bitRead(valY, 11) == 1) {
      valY |= 0xF000;
    }
    *y = (int16_t) valY;
  }
  if (z) {
    if (bitRead(valZ, 11) == 1) {
      valZ |= 0xF000;
    }
    *z = (int16_t) valZ;
  }
}

bool MMA8452Q::isActive() {
  uint8_t ctrl1 = read(REG_CTRL1);

  return bitRead(ctrl1, 0);
}

void MMA8452Q::setActive() {
  uint8_t ctrl1 = read(REG_CTRL1);
  if (bitRead(ctrl1, 0) == 1) {
    return; //already active
  }

  bitSet(ctrl1, 0);
  write(REG_CTRL1, ctrl1);
}

void MMA8452Q::setStandby() {
  uint8_t ctrl1 = read(REG_CTRL1);
  if (bitRead(ctrl1, 0) == 0) {
    return; //already standby
  }

  bitClear(ctrl1, 0);
  write(REG_CTRL1, ctrl1);
}

void MMA8452Q::read(uint8_t reg, uint8_t len, uint8_t *dst) {
  if (dst == NULL)
    return;

  wire->beginTransmission(addr);
  wire->write(reg);
  wire->endTransmission(false);

  wire->requestFrom(addr, len);

  uint8_t i;
  for (i = 0; i < len; i++) {
    dst[i] = wire->read();
  }
}

void MMA8452Q::write(uint8_t reg, uint8_t len, const uint8_t *val) {
  wire->beginTransmission(addr);
  wire->write(reg);
  uint8_t i;
  for (i = 0; i < len; i++) {
    wire->write(val[i]);
  }
  wire->endTransmission(true);
}
