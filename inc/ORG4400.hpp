/**
 * @file ORG4400.hpp
 * @brief Public driver declarations and configuration constants.
 */
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

#ifndef ORG4400_HPP
#define ORG4400_HPP

#include <GPS.hpp>
#include <SerialPort.hpp>

/**
 * @brief ORG4400 GPS receiver with on/off pulse and wakeup-status GPIOs.
 * Call begin() and register the device-specific callback before enabling reception.
 */
class ORG4400 : public Gps {
public:
  /**
   * @brief Bind UART, on/off output pin, and wakeup-status input pin.
   */
  ORG4400(SerialPort &uart, int8_t pinOnOff, int8_t pinWakeup);

  /**
   * @brief Configure 4800-baud reception and GPIO; listen if the receiver is already on.
   */
  void begin();

  /**
   * @brief Register the legacy position callback; nullptr disables notification.
   * @param func Receives encoded latitude, longitude, altitude, and satellite count.
   * The legacy parser combines integer and fractional fields with bitwise OR
   * (coordinate integer parts are multiplied by 10000, altitude by 10) and ignores
   * hemisphere indicators. Values must not be treated as signed decimal degrees.
   */
  void onReadDone(void (*func)(int32_t latitude,
                               int32_t longitude,
                               int32_t altitude,
                               uint8_t numSatellites));

  /**
   * @brief If off, enable reception and pulse the hardware on/off pin.
   */
  void turnOn();

  /**
   * @brief If on, pulse the hardware on/off pin and stop UART input/listening.
   */
  void turnOff();

  /**
   * @brief Return true when the receiver wakeup-status pin is high.
   */
  bool isOn();

private:
  SerialPort &Uart;
  const int8_t PinOnOff;
  const int8_t PinWakeup;

  char buf[255];

  void (*callbackRead)(int32_t, int32_t, int32_t, uint8_t);

  static void NMEAReceived(void *);
};

#endif //ORG4400_HPP
