/**
 * @file T9602_I2C.hpp
 * @brief Public driver declarations and configuration constants.
 */
/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2022 CoXlab Inc.
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
   \date 2022. 7. 19.
*/

#pragma once

#include <stdint.h>
#include <TwoWire.hpp>

/**
 * @brief Synchronous T9602 humidity and temperature reader.
 * Call begin() first. Communication errors and sensor status are not reported.
 */
class T9602_I2C {
public:
  /**
   * @brief Bind the I2C bus and 7-bit sensor address. Both are used by subsequent reads.
   */
  T9602_I2C(TwoWire &w, uint8_t address) : wire(w), address(address) {}
  
  /**
   * @brief Initialize the bound I2C bus.
   */
  void begin();

  /**
   * @brief Read the temperature register and return degrees Celsius.
   */
  float readTemperature();

  /**
   * @brief Read the humidity register and return percent relative humidity.
   */
  float readHumidity();

  /**
   * @brief Read humidity and temperature in one four-byte transaction.
   * @param h Output in percent relative humidity; nullptr skips storing humidity.
   * @param t Output in degrees Celsius; nullptr skips storing temperature.
   */
  void readHumidityAndTemperature(float *h, float *t);

private:
  TwoWire &wire;
  const uint8_t address;
};
