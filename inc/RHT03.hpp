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

#define RHT_READ_INTERVAL_MS 1000

class RHT03 {
public:
    RHT03();
    void begin(int dataPin);
    int update();
    float tempC();
    float tempF();
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
