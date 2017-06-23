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

#include "ORG4400.hpp"
#include <cox.h>

ORG4400::ORG4400(SerialPort &uart, int8_t pinOnOff, int8_t pinWakeup)
  : Uart(uart), PinOnOff(pinOnOff), PinWakeup(pinWakeup) {
}

void ORG4400::begin() {
  Uart.begin(4800);
  Uart.onReceive(NMEAReceived, this);

  pinMode(PinOnOff, OUTPUT);
  digitalWrite(PinOnOff, LOW);

  pinMode(PinWakeup, INPUT);

  if (isOn()) {
    Uart.input(this->buf, 254, '\r');
    Uart.listen();
  }
}

void ORG4400::NMEAReceived(void *ctx) {
  ORG4400 *gps = (ORG4400 *) ctx;

  char *received = gps->buf;

  while(*received != '$' && received < gps->buf + sizeof(gps->buf)) {
    received++;
  }

  do {
    char *strTmp = strtok(received, ",");

    if (strcmp(strTmp, "$GPGGA") != 0)
      break;

    uint8_t nComma = 0;
    while (strTmp < gps->buf + sizeof(gps->buf)) {
      if (*strTmp == ',') {
        nComma++;
      }

      strTmp++;
      if (nComma == 5) {
        break;
      }
    }

    if (*strTmp != '0') {
      strtok(NULL, "."); //Time (HH:MM:SS)
      strtok(NULL, ","); //Time (subseconds)

      int32_t latitude = strtol(strtok(NULL, "."), NULL, 10) * 10000;
      latitude |= strtol(strtok(NULL, ","), NULL, 10);

      strtok(NULL, ","); //North or South

      int32_t longitude = strtol(strtok(NULL, "."), NULL, 10) * 10000;
      longitude |= strtol(strtok(NULL, ","), NULL, 10);

      strtok(NULL, ","); //East or West

      strtok(NULL, ","); //Fix Quality (0:Invalid, 1:GPS fix, 2:DGPS fix)

      uint8_t numSat = strtoul(strtok(NULL, ","), NULL, 10);

      strtok(NULL, ","); //Horizontal Dilution of Precision

      int32_t altitude = strtol(strtok(NULL, "."), NULL, 10) * 10;
      altitude |= strtol(strtok(NULL, ","), NULL, 10);

      if (gps->callbackRead) {
        gps->callbackRead(latitude, longitude, altitude, numSat);
      }
    }
  } while(0);

  gps->Uart.input(gps->buf, 254, '\r');
}

void ORG4400::onReadDone(void (*func)(int32_t, int32_t, int32_t, uint8_t)) {
  callbackRead = func;
}

void ORG4400::turnOn() {
  if (isOn())
    return;

  Uart.listen();
  Uart.input(this->buf, 254, '\r');

  digitalWrite(this->PinOnOff, HIGH);
  delayMicroseconds(110);
  digitalWrite(this->PinOnOff, LOW);
  delayMicroseconds(110);
}

void ORG4400::turnOff() {
  if (isOn() == false)
    return;

  digitalWrite(this->PinOnOff, HIGH);
  delayMicroseconds(110);
  digitalWrite(this->PinOnOff, LOW);
  delayMicroseconds(110);

  Uart.stopInput();
  Uart.stopListening();
}

bool ORG4400::isOn() {
  return (digitalRead(this->PinWakeup) == HIGH) ? true : false;
}
