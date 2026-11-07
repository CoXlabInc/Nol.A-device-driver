/**
 * @file EVAM8M.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
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
 * @date 2016. 9. 7.
 */

#ifndef EVAM8M_HPP
#define EVAM8M_HPP

#include <GPS.hpp>
#include <SerialPort.hpp>

/**
 * @brief EVA-M8M NMEA receiver. Call begin(), register callbacks, then turnOn().
 * The UART must outlive the driver; turnOn()/turnOff() control listening only.
 */
class EVAM8M : public Gps {
public:
  /**
   * @brief Bind the receiver UART without starting reception.
   */
  EVAM8M(SerialPort &uart);

  /**
   * @brief Configure 9600-baud NMEA reception; listening remains disabled.
   */
  void begin();

  /**
   * @brief Start listening for NMEA sentences.
   */
  void turnOn();

  /**
   * @brief Stop listening without switching off receiver power.
   */
  void turnOff();

  /**
   * @brief Return whether the UART is currently listening.
   */
  bool isOn();

  /**
   * @brief Enable parsing and caching of subsequent GNGSA sentences.
   */
  void useGSA();

  /**
   * @brief Return cached GSA operating mode (A/M), or NUL before a valid GSA.
   */
  char getGsaOpMode();

  /**
   * @brief Return cached GSA navigation mode (1=no fix, 2=2D, 3=3D), initially 0.
   */
  uint8_t getGsaNavMode();

  /**
   * @brief Return cached dimensionless position dilution of precision, initially 99.99.
   */
  float getGsaPDoP();

  /**
   * @brief Return cached dimensionless horizontal dilution of precision, initially 99.99.
   */
  float getGsaHDoP();

  /**
   * @brief Return cached dimensionless vertical dilution of precision, initially 99.99.
   */
  float getGsaVDoP();

  /**
   * @brief Enable parsing and caching of subsequent GNRMC speed and course fields.
   */
  void useRMC();

  /**
   * @brief Return cached RMC speed over ground in knots; initially zero.
   */
  float getRmcSpeedOverGround();

  /**
   * @brief Return cached RMC course over ground in degrees; initially zero.
   */
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

#endif //EVAM8M_HPP

/** @} */
