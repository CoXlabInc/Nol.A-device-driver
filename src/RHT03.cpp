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

#include "RHT03.hpp"

RHT03::RHT03()
{
}

void RHT03::begin(int dataPin)
{
    _dataPin = dataPin;
    pinMode(_dataPin, INPUT_PULLUP);
}

float RHT03::tempC()
{
    return (float) _temperature / 10.0;
}

float RHT03::tempF()
{
    return (tempC() * 9.0 / 5.0 + 32.0);
}

float RHT03::humidity()
{
    return (float) _humidity / 10.0;
}

int RHT03::update()
{
    unsigned long marks[41] = {0};
    unsigned long stops[40] = {0};
    unsigned int highTime, lowTime;
    uint8_t dataBytes[5] = {0};
    
    noInterrupts();
    
    // Begin state: input HIGH
    pinMode(_dataPin, INPUT_PULLUP);
    delay(100);
    // Start signal: host sets data low, waits 1 ms, then pulls back up, wait 20-40us
    pinMode(_dataPin, OUTPUT);
    digitalWrite(_dataPin, LOW);
    delay(2); // Wait 1 ms minimum
    pinMode(_dataPin, INPUT_PULLUP);
    delayMicroseconds(20);
    // Sensor should pull data pin low 80us, then pull back up
    if (!waitForRHT(LOW, 1000))
        return errorExit(0);
    if (!waitForRHT(HIGH, 1000))
        return errorExit(0);
    
    // Sensor transmits 40 bytes (16 rh, 16 temp, 8 checksum)
    // Each byte starts with a ~50us LOW then a HIGH pulse. The duration of the
    // HIGH pulse determines the value of the bit.
    // LOW: 26-28us (<LOW duration)
    // HIGH: 70us (>LOW duration)
    for (int i=0; i<40; i++)
    {
        if (! waitForRHT(LOW, 1000) )
            return errorExit(-i);
        marks[i] = micros();
        if (! waitForRHT(HIGH, 1000) )
            return errorExit(-i);
        stops[i] = micros();
    }
    if (!waitForRHT(LOW, 1000))
        return errorExit(-41);
    marks[40] = micros();
    
    interrupts();
    
    for (int i=0; i<40; i++)
    {
        lowTime = stops[i] - marks[i];
        highTime = marks[i + 1] - stops[i];
        if (highTime > 40/*lowTime*/)
        {
            dataBytes[i/8] |= (1<<(7 - i%8));
        }
    }

    if (checksum(dataBytes[CHECKSUM], dataBytes, 4))
    {
        _humidity = ((uint16_t) dataBytes[HUMIDITY_H] << 8) | dataBytes[HUMIDITY_L];
        _temperature = ((uint16_t) dataBytes[TEMP_H] << 8) | dataBytes[TEMP_L];
        return 1;
    }
    else
    {
        return -43;
    }
}

bool RHT03::checksum(uint8_t check, uint8_t *data, unsigned int datalen)
{
    uint8_t sum = 0;
    for (unsigned int i=0; i<datalen; i++)
    {
        sum = sum + data[i];
    }
    if (sum == check)
        return true;

    return false;
}

int RHT03::errorExit(int code)
{
    interrupts();
    return code;
}

bool RHT03::waitForRHT(int pinState, unsigned int timeout)
{
    unsigned int counter = 0;
    while ((digitalRead(_dataPin) != pinState) && (counter++ < timeout))
        delayMicroseconds(1);
    
    if (counter >= timeout)
        return false;
    else
        return true;
}
