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
#include <SerialPort.hpp>

class MT3339 : public Gps {
public:
  MT3339(SerialPort &uart);

  virtual void begin();

  virtual void turnOn();

  virtual void turnOff();

  virtual bool isOn();

  const char *getLatestRMC() { return _latestRMC; }
  const char *getLatestGGA() { return _latestGGA; }
  const char *getLatestGSA() { return _latestGSA; }
  const char *getLatestGSV(uint8_t seq) { return (seq < 3) ? _latestGSV[seq] : nullptr; }
  const char *getLatestVTG() { return _latestVTG; }

private:
  SerialPort &Uart;

  char buf[255];

  const char *_latestRMC = nullptr;
  const char *_latestGGA = nullptr;
  const char *_latestGSA = nullptr;
  const char *_latestGSV[3] = { nullptr, };
  const char *_latestVTG = nullptr;

  void parseNMEA();
};

#endif //MT3339_HPP
