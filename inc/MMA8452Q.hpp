/**
 * @file MMA8452Q.hpp
 * @brief Public driver declarations and configuration constants.
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

#ifndef MMA8452Q_HPP
#define MMA8452Q_HPP

#include <stdint.h>
#include <TwoWire.hpp>

/**
 * @brief MMA8452Q accelerometer register driver with optional interrupt GPIOs.
 * Initialize the I2C bus before begin(); communication errors are not reported.
 */
class MMA8452Q {
public:
  /**
   * @brief Bind the bus/address and attach falling-edge interrupt handlers.
   * @param wire Initialized I2C bus that outlives the driver.
   * @param addr 7-bit sensor address.
   * @param pinInt1 Optional INT1 GPIO; -1 disables attachment.
   * @param pinInt2 Optional INT2 GPIO; -1 disables attachment.
   */
  void begin(TwoWire &wire, uint8_t addr, int8_t pinInt1 = -1, int8_t pinInt2 = -1);

  /**
   * @brief Return the WHO_AM_I register byte.
   */
  uint8_t readSensorId();
  /**
   * @brief Read and assemble signed axis register values without conversion to g.
   * @param x Optional X output; nullptr skips storing it.
   * @param y Optional Y output; nullptr skips storing it.
   * @param z Optional Z output; nullptr skips storing it.
   * The current implementation combines each high byte shifted by four with the
   * unshifted low byte, then sign-extends bit 11; it does not normalize the low byte.
   */
  void readXYZ(int16_t *x, int16_t *y, int16_t *z);

  /**
   * @brief Return the ACTIVE bit from the control register.
   */
  bool isActive();
  /**
   * @brief Set the ACTIVE bit to enable measurements.
   */
  void setActive();
  /**
   * @brief Clear the ACTIVE bit to enter standby.
   */
  void setStandby();

  /** @brief Output data rate selections in hertz. */
  typedef enum {
    ODR_800Hz = 0,
    ODR_400Hz = 1,
    ODR_200Hz = 2,
    ODR_100Hz = 3,
    ODR_50Hz = 4,
    ODR_12p5Hz = 5,
    ODR_6p25Hz = 6,
    ODR_1p56Hz = 7,
  } ODR_t;
  /**
   * @brief Return the configured output data rate.
   */
  ODR_t getODR();
  /**
   * @brief Set the output data rate, temporarily entering standby if needed.
   * @param val Output data rate selection.
   */
  void setODR(ODR_t val);

  /**
   * @brief Register the transient interrupt callback; nullptr disables notification.
   * @param func Receives this driver when a transient interrupt is handled.
   */
  void onDetectTransient(void (*func)(MMA8452Q &));
  /**
   * @brief Configure transient detection and restore the previous active state.
   * @param enableX Requested X-axis enable; currently not applied independently (X follows enableZ).
   * @param enableY Enable Y-axis transient detection.
   * @param enableZ Enable Z-axis detection; also controls X in the current implementation.
   * @param thresholdMilliG Threshold in milli-g, clamped to 8000 and quantized in 63 milli-g steps.
   * @param durationMicros Duration in microseconds, clamped and quantized for the current mode/rate.
   */
  void setTransientDetection(bool enableX,
                             bool enableY,
                             bool enableZ,
                             uint16_t thresholdMilliG,
                             uint32_t durationMicros);

  /** @brief Sampling power/noise mode selections. */
  typedef enum {
    MODE_NORMAL = 0,
    MODE_LOW_NOISE_LOW_POWER = 1,
    MODE_HIGH_RESOLUTION = 2,
    MODE_LOW_POWER = 3,
  } Mode_t;
  /**
   * @brief Write the sampling power/noise mode; put the sensor in standby first.
   */
  void setMode(Mode_t);
  /**
   * @brief Return the configured sampling power/noise mode.
   */
  Mode_t getMode();

private:
  TwoWire *wire;
  uint8_t addr;

  void (*handlerTransientDetection)(MMA8452Q &);

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

  enum {
    INT_EN_ASLP    = (1 << 7),
    INT_EN_TRANS   = (1 << 5),
    INT_EN_LNDPRT  = (1 << 4),
    INT_EN_PULSE   = (1 << 3),
    INT_EN_FF_MT   = (1 << 2),
    INT_EN_DRDY    = (1 << 0),
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

  static void EventInterrupt1(void *);
  static void EventInterrupt2(void *);
};

#endif //MMA8452Q_HPP
