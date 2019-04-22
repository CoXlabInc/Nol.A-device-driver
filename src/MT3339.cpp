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

#include "MT3339.hpp"
#include <cox.h>

MT3339::MT3339(SerialPort &uart) : Uart(uart) {
}

void MT3339::begin() {
  Uart.begin(9600);
  Uart.onReceive([](void *ctx) {
                   ((MT3339 *) ctx)->parseNMEA();
                 }, this);
  Uart.input(this->buf, 254, '\r');
  Uart.stopListening();
}

void MT3339::parseNMEA() {
  char *received = this->buf;

  while(*received != '$' && received < this->buf + sizeof(this->buf)) {
    received++;
  }

  if (memcmp(received, "$GPGGA", 6) == 0) {
    if (this->_latestGGA) {
      delete [] this->_latestGGA;
      this->_latestGGA = nullptr;
    }

    uint8_t len = strlen(received);
    char *gga = new char[len + 3];
    if (gga) {
      sprintf(gga, "%s\r\n", received);
      this->_latestGGA = gga;
    }

    /* Legacy method */
    if (this->callbackRead) {
      uint8_t fixQuality, hour = 0xFF, minute = 0xFF, sec = 0xFF, numSatellites = 0;
      uint16_t subsec = 0xFFFF;
      int32_t latitude, longitude, altitude;

      fixQuality = ParseGGA( received,
                             &hour, &minute, &sec, &subsec,
                             &latitude, &longitude,
                             &numSatellites, NULL, &altitude,
                             NULL, NULL, NULL);
      this->callbackRead(fixQuality, hour, minute, sec, subsec,
                         latitude, longitude, altitude, numSatellites);
    }
  } else if (memcmp(received, "$GPGSA", 6) == 0) {
    if (this->_latestGSA) {
      delete [] this->_latestGSA;
      this->_latestGSA = nullptr;
    }

    uint8_t len = strlen(received);
    char *gsa = new char[len + 3];
    if (gsa) {
      sprintf(gsa, "%s\r\n", received);
      this->_latestGSA = gsa;
    }
  } else if (memcmp(received, "$GPRMC", 6) == 0) {
    if (this->_latestRMC) {
      delete [] this->_latestRMC;
      this->_latestRMC = nullptr;
    }

    uint8_t len = strlen(received);
    char *rmc = new char[len + 3];
    if (rmc) {
      sprintf(rmc, "%s\r\n", received);
      this->_latestRMC = rmc;
    }
  } else if (memcmp(received, "$GPGSV,", 7) == 0
             && (received[7] >= '1' || received[7] <= '3')
             && received[8] == ','
             && (received[9] >= '1' || received[9] <= '3')) {
    uint8_t seq = received[7] - (uint8_t) '1';
    if (this->_latestGSV[seq]) {
      delete [] this->_latestGSV[seq];
      this->_latestGSV[seq] = nullptr;
    }

    uint8_t len = strlen(received);
    char *gsv = new char[len + 3];
    if (gsv) {
      sprintf(gsv, "%s\r\n", received);
      this->_latestGSV[seq] = gsv;
    }
  } else if (memcmp(received, "$GPVTG", 6) == 0) {
    if (this->_latestVTG) {
      delete [] this->_latestVTG;
      this->_latestVTG = nullptr;
    }

    uint8_t len = strlen(received);
    char *vtg = new char[len + 3];
    if (vtg) {
      sprintf(vtg, "%s\r\n", received);
      this->_latestVTG = vtg;
    }

    if (!this->callbackRead && this->onNMEAReceived) {
      onNMEAReceived(*this);
    }

    if (this->_latestRMC) {
      delete [] this->_latestRMC;
      this->_latestRMC = nullptr;
    }

    if (this->_latestGGA) {
      delete [] this->_latestGGA;
      this->_latestGGA = nullptr;
    }

    if (this->_latestGSA) {
      delete [] this->_latestGSA;
      this->_latestGSA = nullptr;
    }

    for (int i = 0; i < 3; i++) {
      if (this->_latestGSV[i]) {
        delete [] this->_latestGSV[i];
        this->_latestGSV[i] = nullptr;
      }
    }
  }

  this->Uart.input(this->buf, 254, '\r');
}

void MT3339::turnOn() {
  Uart.listen();
}

void MT3339::turnOff() {
  Uart.stopListening();
}

bool MT3339::isOn() {
  return Uart.isListening();
}
