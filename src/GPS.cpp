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

#include "GPS.hpp"

void Gps::onReadDone(void (*func)(uint8_t fixQuality,
                                  uint8_t hour,
                                  uint8_t minute,
                                  uint8_t sec,
                                  uint8_t subsec,
                                  int32_t latitude,
                                  int32_t longitude,
                                  int32_t altitude,
                                  uint8_t numSatellites)) {
  this->callbackRead = func;
}

uint8_t Gps::CopyStringUntil(char *dst,
                             const char *src,
                             char terminator,
                             uint8_t maxLength) {
  uint8_t len = 0;
  while (maxLength > len && src[len] != terminator) {
    dst[len] = src[len];
    len++;
  }
  dst[len] = '\0';
  return len;
}

uint8_t Gps::ParseGGA(const char *msg,
                      uint8_t *hour,
                      uint8_t *minute,
                      uint8_t *sec,
                      uint8_t *subsec,
                      int32_t *latitude,
                      int32_t *longitude,
                      uint8_t *numSat,
                      char *strHDOP,
                      int32_t *altitude,
                      int32_t *sep,
                      int32_t *diffAge,
                      int32_t *diffStation) {
  uint8_t len;
  char buf[100];
  uint32_t tmp;
  uint8_t fixQuality;

  // char TEST[] = "$GNGGA,092725.00,4717.11399,N,00833.91590,E,1,08,1.01,499.6,M,48.0,M,,*5B";
  // msg = TEST;

  if (*msg != '$') {
    return false;
  }

  /* Find the first comma. */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  msg += len + 1;
  // printf("%s() GNGGA: %s\n", __func__, msg);


  /* Time (HHMMSS) field. */
  len = CopyStringUntil(buf, msg, '.', sizeof(buf));
  if (len != 6) {
    // printf("(GGA1: fail len:%u\n", len);
    return 0;
  }
  msg += len + 1;

  tmp = strtoul(buf, NULL, 10);

  if (hour) {
    *hour = tmp / 10000;
  }

  if (minute) {
    *minute = (tmp % 10000) / 100;
  }

  if (sec) {
    *sec = tmp % 100;
  }

  // printf("(GGA1):%s\n", buf);

  /* Sub-seconds field. */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len != 2) {
    return 0;
  }
  msg += len + 1;

  if (subsec) {
    *subsec = strtoul(buf, NULL, 10);
  }
  // printf("(GGA2):%s\n", buf);

  /* Latitude */
  len = CopyStringUntil(buf, msg, '.', sizeof(buf));
  if (len == 0 || len >= sizeof(buf)) {
    return 0;
  }
  msg += len + 1;

  if (latitude) {
    *latitude = strtoul(buf, NULL, 10) * 100000;
  }
  // printf("(GGA3):%s\n", buf);

  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len == 0 || len >= sizeof(buf)) {
    return 0;
  }
  msg += len + 1;

  if (latitude) {
    *latitude += strtoul(buf, NULL, 10);
  }
  // printf("(GGA4):%s\n", buf);

  /* North or south indicator */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len != 1 || (buf[0] != 'N' && buf[0] != 'S')) {
    return 0;
  }
  msg += len + 1;

  if (latitude && buf[0] == 'S') {
    *latitude *= (-1);
  }
  // printf("(GGA5):%s\n", buf);

  /* Longitude */
  len = CopyStringUntil(buf, msg, '.', sizeof(buf));
  if (len == 0 || len >= sizeof(buf)) {
    return 0;
  }
  msg += len + 1;

  if (longitude) {
    *longitude = strtoul(buf, NULL, 10) * 100000;
  }
  // printf("(GGA6):%s\n", buf);

  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len == 0 || len >= sizeof(buf)) {
    return 0;
  }
  msg += len + 1;

  if (longitude) {
    *longitude += strtoul(buf, NULL, 10);
  }
  // printf("(GGA7):%s\n", buf);

  /* East or west indicator */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len != 1 || (buf[0] != 'E' && buf[0] != 'W')) {
    return 0;
  }
  msg += len + 1;

  if (longitude && buf[0] == 'W') {
    *longitude *= (-1);
  }
  // printf("(GGA8):%s\n", buf);

  /* Fix quality */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len != 1 || buf[0] == '0') {
    return 0;
  }
  msg += len + 1;

  fixQuality = strtoul(buf, NULL, 10);
  // printf("(GGA9):%s\n", buf);

  /* Number of satellites */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len == 0 || len > 2) {
    return 0;
  }
  msg += len + 1;

  if (numSat) {
    *numSat = strtoul(buf, NULL, 10);
  }
  // printf("(GGA10):%s\n", buf);

  /* Horizontal dilution of precision */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len == 0 || len >= sizeof(buf)) {
    return 0;
  }
  msg += len + 1;

  if (strHDOP) {
    strcpy(strHDOP, buf);
  }
  // printf("(GGA11):%s\n", buf);

  /* Altitude */
  len = CopyStringUntil(buf, msg, '.', sizeof(buf));
  if (len == 0 || len >= sizeof(buf)) {
    return 0;
  }
  msg += len + 1;

  if (altitude) {
    *altitude = strtoul(buf, NULL, 10) * 10;
  }
  // printf("(GGA12):%s\n", buf);

  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len != 1) {
    return 0;
  }
  msg += len + 1;

  if (altitude) {
    *altitude += strtoul(buf, NULL, 10);
  }
  // printf("(GGA13):%s\n", buf);

  /* Altitude unit */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len != 1 || buf[0] != 'M') {
    return 0;
  }
  msg += len + 1;
  // printf("(GGA14):%s\n", buf);

  /* Geoid separation */
  len = CopyStringUntil(buf, msg, '.', sizeof(buf));
  if (len == 0 || len >= sizeof(buf)) {
    return 0;
  }
  msg += len + 1;

  if (sep) {
    *sep = strtoul(buf, NULL, 10) * 10;
  }
  // printf("(GGA15):%s\n", buf);

  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len != 1) {
    return 0;
  }
  msg += len + 1;

  if (sep) {
    *sep += strtoul(buf, NULL, 10);
  }
  // printf("(GGA16):%s\n", buf);

  /* Separation unit */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len != 1 || buf[0] != 'M') {
    return 0;
  }
  msg += len + 1;
  // printf("(GGA17):%s\n", buf);

  /* Age of differential corrections (DGPS only) */
  len = CopyStringUntil(buf, msg, ',', sizeof(buf));
  if (len >= sizeof(buf)) {
    //printf("(GGA18: fail len:%u\n", len);
    return 0;
  }
  msg += len + 1;
  // if (len > 0) {
  //   printf("(GGA18):%s\n", buf);
  // }

  if (len > 0) {
    /* When the GPS is not DGPS, both diffAge and diffStation don't exist. */

    /* ID of station providing differential corrections (DGPS only) */
    len = CopyStringUntil(buf, msg, ',', sizeof(buf));
    if (len >= sizeof(buf)) {
      // printf("(GGA19: fail len:%u\n", len);
      return 0;
    }
    msg += len + 1;
    if (len > 0) {
      // printf("(GGA19):%s\n", buf);
    }
  }

  return fixQuality;
}
