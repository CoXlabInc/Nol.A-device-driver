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
 * @date 2016. 7. 8.
 * @brief Nol.A library for digital weight scale based on HX711
 *
 * @note This code is implemented based on Weihong Guan's Arduino library
 * (https://github.com/aguegu/Arduino).
 */

/* Arduino library for digital weight scale of hx711
 *
 * hardware design: syyyd
 * available at http://syyyd.taobao.com
 *
 * library design: Weihong Guan (@aguegu)
 * http://aguegu.net
 *
 * library host on
 * https://github.com/aguegu/Arduino
 *
 *  Created on: Oct 31, 2012
 */

#ifndef HX711_HPP
#define HX711_HPP

#include "cox.h"

class Hx711 {
public:
  Hx711(uint8_t pin_din, uint8_t pin_slk);
  void begin();
  int32_t getMilligram();
  int32_t getAveragedMilligram(uint8_t times = 32);
  void setOffset(int32_t offset);
  int32_t getOffset();
  void setScale(int32_t scale = 742);
  int32_t getScale();
  void adjustZero();

private:
  const uint8_t _pin_dout;
  const uint8_t _pin_slk;
  int32_t _offset;
  int32_t _scale;

  int32_t getRaw();
  int32_t getAverage(uint8_t times = 32);
};

#endif /* HX711_HPP */
