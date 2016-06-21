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

#ifndef MMA8452Q_HPP
#define MMA8452Q_HPP

#include <cox.h>

class MMA8452Q {
public:
  void begin(TwoWire &, uint8_t addr);

  uint8_t readSensorId();
  void readXYZ(int16_t *x, int16_t *y, int16_t *z);

  bool isActive();
  void setActive();
  void setStandby();


private:
  TwoWire *wire;
  uint8_t addr;

  enum {
    REG_STATUS           = 0x00,
    REG_OUT_X_MSB        = 0x01,
    REG_OUT_X_LSB        = 0x02,
    REG_OUT_Y_MSB        = 0x03,
    REG_OUT_Y_LSB        = 0x04,
    REG_OUT_Z_MSB        = 0x05,
    REG_OUT_Z_LSB        = 0x06,
    REG_SYSMOD           = 0x0B,
    REG_INT_SOURCE       = 0x0C,
    REG_WHO_AM_I         = 0x0D,
    REG_XYZ_DATA_CFG     = 0x0E,
    REG_HP_FILTER_CUTOFF = 0x0F,
    REG_PL_STATUS        = 0x10,
    REG_PL_CFG           = 0x11,
    REG_PL_COUNT         = 0x12,
    REG_PL_BF_ZCOMP      = 0x13,
    REG_P_L_THS_REG      = 0x14,
    REG_FF_MT_CFG        = 0x15,
    REG_FF_MT_SRC        = 0x16,
    REG_FF_MT_THS        = 0x17,
    REG_FF_MT_COUNT      = 0x18,
    REG_TRANSIENT_CFG    = 0x1D,
    REG_TRANSIENT_SRC    = 0x1E,
    REG_TRANSIENT_THS    = 0x1F,
    REG_TRANSIENT_COUNT  = 0x20,
    REG_PULSE_CFG        = 0x21,
    REG_PULSE_SRC        = 0x22,
    REG_PULSE_THSX       = 0x23,
    REG_PULSE_THSY       = 0x24,
    REG_PULSE_THSZ       = 0x25,
    REG_PULSE_TMLT       = 0x26,
    REG_PULSE_LTCY       = 0x27,
    REG_PULSE_WIND       = 0x28,
    REG_ASLP_COUNT       = 0x29,
    REG_CTRL1            = 0x2A,
    REG_CTRL2            = 0x2B,
    REG_CTRL3            = 0x2C,
    REG_CTRL4            = 0x2D,
    REG_CTRL5            = 0x2E,
    REG_OFF_X            = 0x2F,
    REG_OFF_Y            = 0x30,
    REG_OFF_Z            = 0x31,
  };

  uint8_t read(uint8_t reg) {
    uint8_t val;
    read(reg, 1, &val);
    return val;
  }

  void read(uint8_t reg, uint8_t len, uint8_t *dst);

  void write(uint8_t reg, uint8_t val) {
    write(reg, 1, &val);
  }

  void write(uint8_t reg, uint8_t len, const uint8_t *val);
};

#endif //MMA8452Q_HPP
