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
   \author Jongsoo Jeong (CoXlab)
   \date 2017. 6. 14.
*/

#ifndef MT3339_HPP
#define MT3339_HPP

#include <GPS.hpp>

class MT3339 : public Gps {
public:
  MT3339(SerialPort &uart);

  void begin();

  void turnOn();

  void turnOff();

  bool isOn();

  void useGSA();

  char getGsaOpMode();

  uint8_t getGsaNavMode();

  float getGsaPDoP();

  float getGsaHDoP();

  float getGsaVDoP();

  void useRMC();

  float getRmcSpeedOverGround();

  float getRmcCourseOverGround();

private:
  SerialPort &Uart;

  char buf[255];

  char gsaOpMode;
  uint8_t gsaNavMode;
  float gsaPDoP;
  float gsaHDoP;
  float gsaVDoP;
  float rmcSpd;
  float rmcCog;

  bool (*gsaParser)(
    const char *msg,
    char *opMode,
    uint8_t *navMode,
    float *pdop,
    float *hdop,
    float *vdop
  );

  bool (*rmcParser)(
    const char *msg,
    float *spd,
    float *cog
  );

  static void NMEAReceived(void *);
};

#endif //MT3339_HPP
