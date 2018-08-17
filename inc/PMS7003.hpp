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
   \author Jongsoo Jeong (CoXlab)
   \date 2018. 8. 16.
*/


#ifndef PMS7003_HPP
#define PMS7003_HPP

#include "PMS3003.hpp"

class PMS7003 : public PMS3003 {
public:
  PMS7003(SerialPort &port, int pinSet, int pinReset);
  typedef struct {
    int32_t pm1_CF1;
    int32_t pm2_5_CF1;
    int32_t pm10_CF1;
    int32_t pm1_Atmosphere;
    int32_t pm2_5_Atmosphere;
    int32_t pm10_Atmosphere;
    int32_t nParticlesBeyond0_3um_in100mL;
    int32_t nParticlesBeyond0_5um_in100mL;
    int32_t nParticlesBeyond1um_in100mL;
    int32_t nParticlesBeyond2_5um_in100mL;
    int32_t nParticlesBeyond5um_in100mL;
    int32_t nParticlesBeyond10um_in100mL;
  } value_t;
  void onReadDone(void (*func)(value_t &));

protected:
  void (*callback)(value_t &) = NULL;
  void eventSensorDataReceived() override;

private:
  value_t sensingValue;
  uint16_t checkCode;
};

#endif //PMS7003_HPP
