/**
 * @file GPS.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @brief Sensor, display, storage, and peripheral driver APIs.
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

#ifndef GPS_HPP
#define GPS_HPP

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Common GPS callback interface and NMEA parsers for device-specific drivers.
 */
class Gps {
public:
  /**
   * @brief Register the GGA callback; nullptr disables legacy GGA notifications.
   * @param func Receives fix quality (0 means invalid), UTC hour/minute/second, the
   * unscaled fractional-second digits, encoded NMEA latitude/longitude, encoded
   * altitude, and satellite count. Coordinates are not decimal degrees: the parser
   * multiplies the degrees-and-minutes integer part by 100000 and adds the raw
   * fractional digits, with negative values for south/west. Altitude similarly
   * uses the integer part times 10 plus the fractional digits. Check fix quality
   * before using position values; fractional fields are not normalized.
   */
  void onReadDone(
    void (*func)(
      uint8_t fixQuality,
      uint8_t hour,
      uint8_t minute,
      uint8_t sec,
      uint16_t subsec,
      int32_t latitude,
      int32_t longitude,
      int32_t altitude,
      uint8_t numSatellites
    )
  );

  /**
   * @brief Enable reception or device power according to the concrete driver.
   */
  virtual void turnOn() = 0;

  /**
   * @brief Disable reception or device power according to the concrete driver.
   */
  virtual void turnOff() = 0;

  /**
   * @brief Return the concrete driver's reception or hardware power state.
   */
  virtual bool isOn() = 0;

  /**
   * @brief Optional raw-NMEA notification used by MT3339 after receiving VTG.
   * Invoked only when no onReadDone() callback is installed; read cached sentences
   * during the callback because most caches are cleared immediately afterward.
   */
  void (*onNMEAReceived)(Gps &) = nullptr;

protected:
  /**
    \return  Fix quality
              * 0: No fix / invalid
              * 1: Standard GPS (2D/3D)
              * 2: Differential GPS
              * 4: RTK fixed solution
              * 5: RTK float solution
              * 6: Estimated (DR) fix
  */
  static uint8_t ParseGGA(
    const char *msg,
    uint8_t *hour,
    uint8_t *minute,
    uint8_t *sec,
    uint16_t *subsec,
    int32_t *latitude,
    int32_t *longitude,
    uint8_t *numSat,
    char *strHDOP,
    int32_t *altitude,
    int32_t *sep,
    int32_t *diffAge,
    int32_t *diffStation
  );

  static bool ParseGSA(
    const char *msg,
    char *opMode,
    uint8_t *navMode,
    float *pdop,
    float *hdop,
    float *vdop
  );

  static bool ParseRMC(
    const char *msg,
    float *spd,
    float *cog
  );

  void (*callbackRead)(
    uint8_t fixQuality,
    uint8_t hour,
    uint8_t minute,
    uint8_t sec,
    uint16_t subsec,
    int32_t latitude,
    int32_t longitude,
    int32_t altitude,
    uint8_t numSatellites
  );

private:
  static uint8_t CopyStringUntil(
    char *dst,
    const char *src,
    char terminator,
    uint8_t maxLength
  );
};

#endif //GPS_HPP

/** @} */
