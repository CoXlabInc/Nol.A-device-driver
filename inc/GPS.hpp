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

#include <cox.h>

class Gps {
public:
  void onReadDone(void (*func)( uint8_t fixQuality,
                                uint8_t hour,
                                uint8_t minute,
                                uint8_t sec,
                                uint16_t subsec,
                                int32_t latitude,
                                int32_t longitude,
                                int32_t altitude,
                                uint8_t numSatellites));

  virtual void turnOn() = 0;

  virtual void turnOff() = 0;

  virtual bool isOn() = 0;

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
  static uint8_t ParseGGA(const char *msg,
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
                          int32_t *diffStation);

  static bool ParseGSA(const char *msg,
                       char *opMode,
                       uint8_t *navMode,
                       float *pdop,
                       float *hdop,
                       float *vdop);

  void (*callbackRead)(uint8_t fixQuality,
                       uint8_t hour,
                       uint8_t minute,
                       uint8_t sec,
                       uint16_t subsec,
                       int32_t latitude,
                       int32_t longitude,
                       int32_t altitude,
                       uint8_t numSatellites);

private:
  static uint8_t CopyStringUntil(char *dst,
                                 const char *src,
                                 char terminator,
                                 uint8_t maxLength);
};

#endif //GPS_HPP
