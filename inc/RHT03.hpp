/**
 * @file RHT03.hpp
 * @brief Public driver declarations and configuration constants.
 */
/*
  The MIT License (MIT)

  Copyright (c) 2016 SparkFun Electronics

  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

/*
	Jim Lindblom <jim@sparkfun.com>
	August 31, 2015
    
    Ported to Nol.A SDK by Jongsoo Jeong <jsjeong@coxlab.kr>
    Jan 24, 2017
*/

#include "cox.h"

#ifndef RHT03_HPP
#define RHT03_HPP

/** @brief Suggested RHT03 sampling interval in milliseconds; not enforced by update(). */
#define RHT_READ_INTERVAL_MS 1000

/**
 * @brief Blocking RHT03 temperature/humidity reader. Call begin(), then update()
 * before accessing cached readings; the caller controls the sampling interval.
 */
class RHT03 {
public:
    /**
     * @brief Construct a reader without configuring a GPIO.
     */
    RHT03();
    /**
     * @brief Bind and configure the data GPIO as INPUT_PULLUP.
     * @param dataPin Sensor data pin.
     */
    void begin(int dataPin);
    /**
     * @brief Read and validate a sensor frame synchronously.
     * @return 1 on success; zero or negative values indicate timing or checksum errors.
     * Only a successful read updates the cached temperature and humidity.
     */
    int update();
    /**
     * @brief Return the cached temperature in degrees Celsius.
     */
    float tempC();
    /**
     * @brief Return the cached temperature in degrees Fahrenheit.
     */
    float tempF();
    /**
     * @brief Return the cached relative humidity in percent.
     */
    float humidity();

private:
    int _dataPin;
    int _humidity;
    int _temperature;
    
    enum dataByteLocations { HUMIDITY_H, HUMIDITY_L, TEMP_H, TEMP_L, CHECKSUM };
    
    bool checksum(uint8_t check, uint8_t *data, unsigned int datalen);
    int errorExit(int code);
    bool waitForRHT(int pinState, unsigned int timeout = 1000);
};

#endif //RHT03_HPP
