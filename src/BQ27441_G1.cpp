/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 CoXlab Inc.
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
   \date 2017. 6. 15.
*/

#include "BQ27441_G1.hpp"

BQ27441_G1::BQ27441_G1(TwoWire &wire, uint8_t addr, int8_t pinGPOut)
  : wire(wire),
    Addr(addr),
    PinGPOut(pinGPOut) {
}

void BQ27441_G1::begin() {
  //TODO initialize interrupt.
}

uint16_t BQ27441_G1::readDeviceType() {
  this->wire.beginTransmission(this->Addr);
  this->wire.write(0x00);
  this->wire.write(0x01);
  this->wire.write(0x00);
  this->wire.endTransmission();

  this->wire.beginTransmission(this->Addr);
  this->wire.write(0x00);
  this->wire.endTransmission();
  this->wire.requestFrom(this->Addr, 2);
  uint16_t val;
  val = this->wire.read();
  val |= (this->wire.read() << 8);
  return val;
}
