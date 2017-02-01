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
 * @author Jongsoo Jeong (CoXlab)
 * @date 2017. 1. 31.
 */

#include "PMS3003.hpp"

PMS3003::PMS3003(SerialPort &port, int pinSet, int pinReset) {
  this->p = &port;
  this->pinSet = pinSet;
  this->pinReset = pinReset;
  this->index = 0;
  this->high = 0;

  for (int i = 0; i < 6; i++) {
    this->measured[i] = -1;
  }

  this->callback = NULL;
}

void PMS3003::begin() {
  pinMode(this->pinSet, OUTPUT);
  digitalWrite(this->pinSet, LOW);

  pinMode(this->pinReset, OUTPUT);
  digitalWrite(this->pinReset, LOW);
  delay(1000);
  digitalWrite(this->pinReset, HIGH);

  digitalWrite(this->pinSet, HIGH);

  this->p->begin(9600);
  this->p->onReceive(SerialDataReceived, this);
  this->p->listen();
}

void PMS3003::eventSensorDataReceived() {
  while (this->p->available() > 0) {
    uint8_t c = this->p->read();

    if ((this->index == 0 && c != 0x42) ||
        (this->index == 1 && c != 0x4d)) {
      this->index = 0;
      continue;

    } else if (this->index == 2 ||
               this->index == 4 ||
               this->index == 6 ||
               this->index == 8 ||
               this->index == 10 ||
               this->index == 12 ||
               this->index == 14) {
      this->high = c;
    } else if (this->index == 3) {
      uint16_t frameLength = (this->high << 8) | c;

      if (frameLength != 20) {
        this->index = 0;
        continue;
      }
    } else if (this->index == 5) {
      this->measured[0] = (this->high << 8) | c;
    } else if (this->index == 7) {
      this->measured[1] = (this->high << 8) | c;
    } else if (this->index == 9) {
      this->measured[2] = (this->high << 8) | c;
    } else if (this->index == 11) {
      this->measured[3] = (this->high << 8) | c;
    } else if (this->index == 13) {
      this->measured[4] = (this->high << 8) | c;
    } else if (this->index == 15) {
      this->measured[5] = (this->high << 8) | c;
    } else if (this->index == 23) {
      this->index = 0;

      if (this->callback) {
        this->callback(this->measured[0],
                       this->measured[1],
                       this->measured[2],
                       this->measured[3],
                       this->measured[4],
                       this->measured[5]);
      }
      continue;
    }
    this->index++;
  }
}

void PMS3003::SerialDataReceived(void *ctx) {
  PMS3003 *pms3003 = (PMS3003 *) ctx;
  pms3003->eventSensorDataReceived();
}

void PMS3003::onReadDone(void (*func)(int32_t pm1_0_CF1,
                                      int32_t pm2_5_CF1,
                                      int32_t pm10_0_CF1,
                                      int32_t pm1_0_Atmosphere,
                                      int32_t pm2_5_Atmosphere,
                                      int32_t pm10_0_Atmosphere)) {
  this->callback = func;
}
