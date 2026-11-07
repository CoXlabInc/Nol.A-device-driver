/**
 * @file HTU2xD.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
// -*- mode:c++; indent-tabs-mode:nil; -*-
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
   \date 2022. 8. 23.
*/

#pragma once

#include <stdint.h>
#include <TwoWire.hpp>

/**
 * @brief Synchronous HTU2xD I2C temperature and humidity driver at address 0x40.
 * Call begin() first. Reads do not validate CRC or return communication errors.
 */
class HTU2xD {
public:
  /**
   * @brief Bind the I2C bus, which must outlive the driver.
   */
  HTU2xD(TwoWire &w) : wire(w) {}
  
  /**
   * @brief Initialize the I2C bus and send a sensor reset command.
   */
  void begin();

  /**
   * @brief Send the soft-reset command without waiting for reset completion.
   */
  void reset();
  
  /**
   * @brief Perform a hold-master measurement and return degrees Celsius.
   */
  float readTemperature();

  /**
   * @brief Perform a hold-master measurement and return percent relative humidity.
   */
  float readHumidity();

private:
  TwoWire &wire;
};

/** @} */
