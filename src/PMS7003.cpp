/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2018 CoXlab Inc.
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
 * @date 2018. 8. 16.
 */

#include "PMS7003.hpp"

PMS7003::PMS7003(SerialPort &port, int pinSet, int pinReset)
  : PMS3003(port, pinSet, pinReset) {
}

void PMS7003::eventSensorDataReceived() {
  while (this->p.available() > 0) {
    uint8_t c = this->p.read();

    if ((this->index == 0 && c != 0x42) ||
        (this->index == 1 && c != 0x4d)) {
      this->index = 0;
      this->checkCode = 0;
      continue;
    } else if (this->index % 2 == 0) {
      /* High bytes */
      this->high = c;
    } else if (this->index == 3) {
      uint16_t frameLength = (this->high << 8) | c;

      if (frameLength != 28) {
        this->index = 0;
        this->checkCode = 0;
        continue;
      }
    } else if (this->index == 5) {
      this->sensingValue.pm1_CF1 = (this->high << 8) | c;
    } else if (this->index == 7) {
      this->sensingValue.pm2_5_CF1 = (this->high << 8) | c;
    } else if (this->index == 9) {
      this->sensingValue.pm10_CF1 = (this->high << 8) | c;
    } else if (this->index == 11) {
      this->sensingValue.pm1_Atmosphere = (this->high << 8) | c;
    } else if (this->index == 13) {
      this->sensingValue.pm2_5_Atmosphere = (this->high << 8) | c;
    } else if (this->index == 15) {
      this->sensingValue.pm10_Atmosphere = (this->high << 8) | c;
    } else if (this->index == 17) {
      this->sensingValue.nParticlesBeyond0_3um_in100mL = (this->high << 8) | c;
    } else if (this->index == 19) {
      this->sensingValue.nParticlesBeyond0_5um_in100mL = (this->high << 8) | c;
    } else if (this->index == 21) {
      this->sensingValue.nParticlesBeyond1um_in100mL = (this->high << 8) | c;
    } else if (this->index == 23) {
      this->sensingValue.nParticlesBeyond2_5um_in100mL = (this->high << 8) | c;
    } else if (this->index == 25) {
      this->sensingValue.nParticlesBeyond5um_in100mL = (this->high << 8) | c;
    } else if (this->index == 27) {
      this->sensingValue.nParticlesBeyond10um_in100mL = (this->high << 8) | c;
    }

    if (this->index == 31) {
      /* The last byte */
      uint16_t checkCodeExpected = (this->high << 8) | c;

      if (this->callback) {
        if (checkCodeExpected != this->checkCode) {
          this->sensingValue.pm1_CF1 = -1;
          this->sensingValue.pm2_5_CF1 = -1;
          this->sensingValue.pm10_CF1 = -1;
          this->sensingValue.pm1_Atmosphere = -1;
          this->sensingValue.pm2_5_Atmosphere = -1;
          this->sensingValue.pm10_Atmosphere = -1;
          this->sensingValue.nParticlesBeyond0_3um_in100mL = -1;
          this->sensingValue.nParticlesBeyond0_5um_in100mL = -1;
          this->sensingValue.nParticlesBeyond1um_in100mL = -1;
          this->sensingValue.nParticlesBeyond2_5um_in100mL = -1;
          this->sensingValue.nParticlesBeyond5um_in100mL = -1;
          this->sensingValue.nParticlesBeyond10um_in100mL = -1;
        }
        this->callback(this->sensingValue);
      }

      this->index = 0;
      this->checkCode = 0;
    } else {
      this->index++;

      if (this->index < 30) {
        this->checkCode += c;
      }
    }
  }
}

void PMS7003::onReadDone(void (*func)(value_t &)) {
  this->callback = func;
}
