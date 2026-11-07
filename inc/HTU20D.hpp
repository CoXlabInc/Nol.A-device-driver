/**
 * @file HTU20D.hpp
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
 * @date 2016. 6. 13.
 */

#ifndef HTU20D_HPP
#define HTU20D_HPP

#include <stdint.h>
#include <TwoWire.hpp>
#include <Timer.hpp>

/**
 * @brief Timer-driven asynchronous HTU20D temperature and humidity driver.
 * Initialize the I2C bus, call begin(), and register callbacks before requesting readings.
 * Keep the driver and bus alive until the measurement completes.
 */
class HTU20D {
public:
  /**
   * @brief Bind an initialized I2C bus and initialize the measurement timer.
   */
  void begin(TwoWire &);

  /**
   * @brief Register the temperature callback; nullptr disables notification.
   * @param func Receives the integer result 17572 * raw / 65536 - 4685, nominally
   * hundredths of a degree Celsius. Negative results wrap in the uint16_t argument.
   */
  void onTemperatureReadDone(void (*func)(uint16_t val));

  /**
   * @brief Start a temperature measurement, replacing any pending measurement.
   * Reads after 50 ms and retries every 10 ms while no bytes are available; no timeout.
   */
  void readTemperature();

  /**
   * @brief Register the humidity callback; nullptr disables notification.
   * @param func Receives the current driver formula 12500 * raw / 65536 - 6.
   * This is not directly percent RH; the offset is not scaled like the raw term.
   */
  void onHumidityReadDone(void (*func)(uint16_t val));

  /**
   * @brief Start a humidity measurement, replacing any pending measurement.
   * Reads after 50 ms and retries every 10 ms while no bytes are available; CRC is not checked.
   */
  void readHumidity();

private:
  enum {
    IDLE,
    READ_TEMPERATURE,
    READ_HUMIDITY,
  } state;

  Timer timerDelay;
  TwoWire *wire;

  void (*callbackTemperature)(uint16_t val);
  void (*callbackHumidity)(uint16_t val);

  static void DelayDone(void *);
};

#endif //HTU20D_HPP

/** @} */
