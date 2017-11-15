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

#include "HTU20D.hpp"

void HTU20D::begin(TwoWire &w) {
  this->wire = &w;
  timerDelay.stop();
  state = IDLE;
  timerDelay.onFired(DelayDone, this);
}

void HTU20D::onTemperatureReadDone(void (*func)(uint16_t val)) {
  callbackTemperature = func;
}

void HTU20D::readTemperature() {
  timerDelay.stop();

  wire->beginTransmission(0x40);
  wire->write(0xF3); //Trigger temperature measurement without holding master.
  wire->endTransmission();

  state = READ_TEMPERATURE;
  timerDelay.startOneShot(50);
}

void HTU20D::onHumidityReadDone(void (*func)(uint16_t val)) {
  callbackHumidity = func;
}

void HTU20D::readHumidity() {
  timerDelay.stop();

  wire->beginTransmission(0x40);
  wire->write(0xF5); //Trigger humidity measurement without holding master.
  wire->endTransmission();

  state = READ_HUMIDITY;
  timerDelay.startOneShot(50);
}

void HTU20D::DelayDone(void *ctx) {
  HTU20D *sensor = (HTU20D *) ctx;

  if (sensor->state != IDLE) {
    if (sensor->wire->requestFrom(0x40, 3) == 0) {
      sensor->timerDelay.startOneShot(10);
      return;
    }
  }

  if (sensor->state == READ_TEMPERATURE) {
    uint16_t sTemp = sensor->wire->read() << 8;
    sTemp |= sensor->wire->read();
    sensor->wire->read(); //checksum
    sensor->state = IDLE;

    if (sensor->callbackTemperature) {
      sensor->callbackTemperature(17572ul * sTemp / 65536ul - 4685);
    }

  } else if (sensor->state == READ_HUMIDITY) {
    uint16_t sRH = sensor->wire->read() << 8;
    sRH |= sensor->wire->read();
    sensor->wire->read(); //checksum
    sensor->state = IDLE;

    if (sensor->callbackHumidity) {
      sensor->callbackHumidity(12500ul * sRH / 65536ul - 6);
    }

  }
}
