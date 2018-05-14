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

#include "cox.h"

#ifndef PMS3003_HPP
#define PMS3003_HPP

class PMS3003 {
public:
  PMS3003(SerialPort &, int pinSet, int pinReset);
  void begin();

  void onReadDone(void (*func)(int32_t pm1_0_CF1,
                               int32_t pm2_5_CF1,
                               int32_t pm10_0_CF1,
                               int32_t pm1_0_Atmosphere,
                               int32_t pm2_5_Atmosphere,
                               int32_t pm10_0_Atmosphere));

private:
  void (*callback)(int32_t pm1_0_CF1,
                   int32_t pm2_5_CF1,
                   int32_t pm10_0_CF1,
                   int32_t pm1_0_Atmosphere,
                   int32_t pm2_5_Atmosphere,
                   int32_t pm10_0_Atmosphere) = NULL;

  SerialPort &p;
  const int pinSet, pinReset;
  uint8_t index = 0, high = 0;
  int32_t measured[6];

  void eventSensorDataReceived();
};

#endif //PMS3003_HPP
