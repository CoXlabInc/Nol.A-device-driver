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

#include "EVAM8M.hpp"
#include <cox.h>

EVAM8M::EVAM8M(SerialPort &uart) :
Uart(uart),
gsaOpMode('\0'),
gsaNavMode(0),
gsaPDoP(99.99),
gsaHDoP(99.99),
gsaVDoP(99.99),
rmcSpd(0),
rmcCog(0),
gsaParser(NULL),
rmcParser(NULL) {
}

void EVAM8M::begin() {
  Uart.begin(9600);
  Uart.onReceive(NMEAReceived, this);
  Uart.input(this->buf, 254, '\r');
  Uart.stopListening();
}

void EVAM8M::NMEAReceived(void *ctx) {
  EVAM8M *gps = (EVAM8M *) ctx;

  char *received = gps->buf;

  while(*received != '$' && received < gps->buf + sizeof(gps->buf)) {
    received++;
  }

  if (memcmp(received, "$GNGGA", 6) == 0) {
    uint8_t fixQuality, hour = 0xFF, minute = 0xFF, sec = 0xFF, numSatellites = 0;
    uint16_t subsec = 0xFFFF;
    int32_t latitude, longitude, altitude;

    fixQuality = ParseGGA( received,
                  &hour, &minute, &sec, &subsec,
                  &latitude, &longitude,
                  &numSatellites, NULL, &altitude,
                  NULL, NULL, NULL);
    if (gps->callbackRead) {
      gps->callbackRead(fixQuality, hour, minute, sec, subsec,
                        latitude, longitude, altitude, numSatellites);
    }
  } else if (gps->gsaParser != NULL && memcmp(received, "$GNGSA", 6) == 0) {
    char opMode;
    uint8_t navMode;
    float pdop;
    float hdop;
    float vdop;

    if (gps->gsaParser(received, &opMode, &navMode, &pdop, &hdop, &vdop)) {
      gps->gsaOpMode = opMode;
      gps->gsaNavMode = navMode;
      gps->gsaPDoP = pdop;
      gps->gsaHDoP = hdop;
      gps->gsaVDoP = vdop;
    }
  } else if (gps->rmcParser != NULL && memcmp(received, "$GNRMC", 6) == 0) {
    float spd, cog;

    if (gps->rmcParser(received, &spd, &cog)) {
      gps->rmcSpd = spd;
      gps->rmcCog = cog;
    }
  }

  gps->Uart.input(gps->buf, 254, '\r');
}

void EVAM8M::turnOn() {
  Uart.listen();
}

void EVAM8M::turnOff() {
  Uart.stopListening();
}

bool EVAM8M::isOn() {
  return Uart.isListening();
}

void EVAM8M::useGSA() {
  this->gsaParser = ParseGSA;
}

char EVAM8M::getGsaOpMode() {
  return this->gsaOpMode;
}

uint8_t EVAM8M::getGsaNavMode() {
  return this->gsaNavMode;
}

float EVAM8M::getGsaPDoP() {
  return this->gsaPDoP;
}

float EVAM8M::getGsaHDoP() {
  return this->gsaHDoP;
}

float EVAM8M::getGsaVDoP() {
  return this->gsaVDoP;
}

void EVAM8M::useRMC() {
  this->rmcParser = ParseRMC;
}

float EVAM8M::getRmcSpeedOverGround() {
  return this->rmcSpd;
}

float EVAM8M::getRmcCourseOverGround() {
  return this->rmcCog;
}
