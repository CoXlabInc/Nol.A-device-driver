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

void EVAM8M::begin(SerialPort &uart) {
  this->uart = &uart;

  uart.begin(9600);
  uart.onReceive(NMEAReceived, this);
  uart.input(this->buf, 254, '\r');
  uart.stopListening();
}

void EVAM8M::NMEAReceived(void *ctx) {
  EVAM8M *gps = (EVAM8M *) ctx;

  char *received = gps->buf;

  while(*received != '$' && received < gps->buf + sizeof(gps->buf)) {
    received++;
  }

  if (memcmp(received, "$GNGGA", 6) == 0) {
    uint8_t fixQuality, hour = 0xFF, minute = 0xFF, sec = 0xFF, subsec = 0xFF, numSatellites = 0;
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
  }

  gps->uart->input(gps->buf, 254, '\r');
}

void EVAM8M::turnOn() {
  uart->listen();
}

void EVAM8M::turnOff() {
  uart->stopListening();
}

bool EVAM8M::isOn() {
  return uart->isListening();
}
