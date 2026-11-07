/*!
 * @file Adafruit_VL53L0X.hpp
 * @ingroup device_drivers

  This is a library for the Adafruit VL53L0X Sensor Breakout

  Designed specifically to work with the VL53L0X sensor from Adafruit
  ----> https://www.adafruit.com/products/3317

  These sensors use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  Updated by Andrew DeVries for Digital Example to include methods needed for
  Interrupt triggering.
  BSD license, all text above must be included in any
  redistribution
 ****************************************************/

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */

#pragma once

#include <cox.h>
#include <TwoWire.hpp>
#include "Adafruit_VL53L0X/vl53l0x_api.h"

#define VL53L0X_I2C_ADDR 0x29 ///< Default sensor I2C address

/**************************************************************************/
/*!
    @brief  Class that stores state and functions for interacting with VL53L0X
   time-of-flight sensor chips
*/
/**************************************************************************/
class Adafruit_VL53L0X {
public:
  /**
   * @brief Bind the I2C bus and initial 7-bit address. Call begin() before ranging.
   */
  Adafruit_VL53L0X(TwoWire &, uint8_t i2cAddr = VL53L0X_I2C_ADDR);

  /** Sensor configurations */
  typedef enum {
    VL53L0X_SENSE_DEFAULT = 0,
    VL53L0X_SENSE_LONG_RANGE,
    VL53L0X_SENSE_HIGH_SPEED,
    VL53L0X_SENSE_HIGH_ACCURACY
  } VL53L0X_Sense_config_t;

  /**
   * @brief Initialize the bus and sensor with a ranging preset.
   * @param vl_config Default, long-range, high-speed, or high-accuracy preset.
   * @param debug Enable diagnostic printing.
   * @return True on successful initialization; false on API failure.
   */
  boolean begin(VL53L0X_Sense_config_t vl_config = VL53L0X_SENSE_DEFAULT,
                boolean debug = false);
  /**
   * @brief Change the sensor I2C address; return true on success.
   */
  boolean setAddress(uint8_t newAddr);

  // uint8_t getAddress(void); // not currently implemented

  /**************************************************************************/
  /*!
      @brief  get a ranging measurement from the device
      @param  pRangingMeasurementData the pointer to the struct the data will be
     stored in
      @param debug Optional debug flag. If true debug information will print via
     Serial.print during execution. Defaults to false.
      @returns ST API status: VL53L0X_ERROR_NONE on success.
  */
  /**************************************************************************/
  VL53L0X_Error
  rangingTest(VL53L0X_RangingMeasurementData_t *pRangingMeasurementData,
              boolean debug = false) {
    return getSingleRangingMeasurement(pRangingMeasurementData, debug);
  };

  /**
   * @brief Perform a blocking single measurement into pRangingMeasurementData.
   * @return ST status (VL53L0X_ERROR_NONE on success); inspect the result RangeStatus too.
   */
  VL53L0X_Error getSingleRangingMeasurement(
      VL53L0X_RangingMeasurementData_t *pRangingMeasurementData,
      boolean debug = false);
  void
  /**
   * @brief Print a human-readable interpretation of the supplied measurement RangeStatus.
   */
  printRangeStatus(VL53L0X_RangingMeasurementData_t *pRangingMeasurementData);

  /**
   * @brief Read a measurement into caller storage; return ST API status.
   */
  VL53L0X_Error getRangingMeasurement(
      VL53L0X_RangingMeasurementData_t *pRangingMeasurementData,
      boolean debug = false);
  /**
   * @brief Start measurement using the configured device mode; return ST status.
   */
  VL53L0X_Error startMeasurement(boolean debug = false);
  /**
   * @brief Stop measurement using the ST API; return ST status.
   */
  VL53L0X_Error stopMeasurement(boolean debug = false);
  /**
   * @brief Read the selected limit-check result into a 16.16 fixed-point output; return ST status.
   */
  VL53L0X_Error getLimitCheckCurrent(uint8_t LimitCheckId,
                                     FixPoint1616_t *pLimitCheckCurrent,
                                     boolean debug = false);
  /**
   * @brief Read the device mode into pDeviceMode; return ST status.
   */
  VL53L0X_Error getDeviceMode(VL53L0X_DeviceModes *pDeviceMode,
                              boolean debug = false);
  /**
   * @brief Set the ST device mode; return ST status.
   */
  VL53L0X_Error setDeviceMode(VL53L0X_DeviceModes DeviceMode,
                              boolean debug = false);

  /**
   * @brief Set low/high interrupt thresholds in ST 16.16 distance format; return ST status.
   */
  VL53L0X_Error setInterruptThresholds(FixPoint1616_t ThresholdLow,
                                       FixPoint1616_t ThresholdHigh,
                                       boolean debug = false);
  /**
   * @brief Read low/high ST 16.16 interrupt thresholds into caller storage; return ST status.
   */
  VL53L0X_Error getInterruptThresholds(FixPoint1616_t *pThresholdLow,
                                       FixPoint1616_t *pThresholdHigh,
                                       boolean debug = false);
  /**
   * @brief Clear the sensor interrupt mask; return ST status.
   */
  VL53L0X_Error clearInterruptMask(boolean debug = false);

  /**
   * @brief Read device mode, GPIO function, and polarity into caller storage; return ST status.
   */
  VL53L0X_Error getGpioConfig(VL53L0X_DeviceModes *pDeviceMode,
                              VL53L0X_GpioFunctionality *pFunctionality,
                              VL53L0X_InterruptPolarity *pPolarity,
                              boolean debug = false);
  /**
   * @brief Configure the interrupt GPIO function/polarity for DeviceMode; return ST status.
   */
  VL53L0X_Error setGpioConfig(VL53L0X_DeviceModes DeviceMode,
                              VL53L0X_GpioFunctionality Functionality,
                              VL53L0X_InterruptPolarity Polarity,
                              boolean debug = false);

  VL53L0X_Error Status =
      VL53L0X_ERROR_NONE; ///< indicates whether or not the sensor has
                          ///< encountered an error
  // Add similar methods as Adafruit_VL6180X class adapted to range of device
  /**
   * @brief Perform a single-shot measurement; return millimeters or 0xFFFF on API error.
   * Check readRangeStatus() before using the distance.
   */
  uint16_t readRange(void);
  // float readLux(uint8_t gain);
  /**
   * @brief Return the last cached measurement RangeStatus (zero indicates a valid range).
   */
  uint8_t readRangeStatus(void);

  /**
   * @brief Start a nonblocking single range; return true on API success.
   */
  boolean startRange(void);
  /**
   * @brief Return true when data is ready or an API error occurred; inspect Status.
   */
  boolean isRangeComplete(void);
  /**
   * @brief Wait for measurement readiness; return false on API error.
   */
  boolean waitRangeComplete(void);
  /**
   * @brief Read completed range in millimeters, or 0xFFFF on API error; check readRangeStatus().
   */
  uint16_t readRangeResult(void);

  /**
   * @brief Start continuous ranging with period_ms between measurements; return true on success.
   */
  boolean startRangeContinuous(uint16_t period_ms = 50);
  /**
   * @brief Stop continuous ranging and update Status.
   */
  void stopRangeContinuous(void);

  //  void setTimeout(uint16_t timeout) { io_timeout = timeout; }
  // uint16_t getTimeout(void) { return io_timeout; }
  /**************************************************************************/
  /*!
      @brief  timeout status
      @returns Always false; timeout tracking is not implemented.
  */
  /**************************************************************************/
  boolean timeoutOccurred(void) { return false; }

  /**
   * @brief Apply a ranging preset; return true on API success.
   */
  boolean configSensor(VL53L0X_Sense_config_t vl_config);

  // Export some wrappers to internal setting functions
  // that are used by the above helper function to allow
  // more complete control.
  /**
   * @brief Set the measurement timing budget in microseconds; return true on success.
   */
  boolean setMeasurementTimingBudgetMicroSeconds(uint32_t budget_us);
  /**
   * @brief Return the measurement timing budget in microseconds; inspect Status for API errors.
   */
  uint32_t getMeasurementTimingBudgetMicroSeconds(void);

  /**
   * @brief Set the selected VCSEL pulse period in PCLK units; return true on success.
   */
  boolean setVcselPulsePeriod(VL53L0X_VcselPeriod VcselPeriodType,
                              uint8_t VCSELPulsePeriod);

  /**
   * @brief Return the selected VCSEL pulse period in PCLK units; inspect Status for errors.
   */
  uint8_t getVcselPulsePeriod(VL53L0X_VcselPeriod VcselPeriodType);

  /**
   * @brief Enable/disable the specified ST limit check; return true on success.
   */
  boolean setLimitCheckEnable(uint16_t LimitCheckId, uint8_t LimitCheckEnable);
  /**
   * @brief Return the selected limit-check enable value; inspect Status for API errors.
   */
  uint8_t getLimitCheckEnable(uint16_t LimitCheckId);
  /**
   * @brief Set the selected ST limit threshold in 16.16 fixed-point format; return true on success.
   */
  boolean setLimitCheckValue(uint16_t LimitCheckId,
                             FixPoint1616_t LimitCheckValue);
  /**
   * @brief Return the selected ST limit threshold in 16.16 format; inspect Status for errors.
   */
  FixPoint1616_t getLimitCheckValue(uint16_t LimitCheckId);

private:
  TwoWire &wire;
  const uint8_t I2CAddr;
  
  VL53L0X_Dev_t MyDevice;
  VL53L0X_DeviceInfo_t DeviceInfo;

  uint8_t _rangeStatus;
};

// Keep the vendor API topic hierarchy beneath the device driver topic.
/**
 * @addtogroup VL53L0X_platform_group
 * @ingroup device_drivers
 */
/**
 * @addtogroup VL53L0X_globaldefine_group
 * @ingroup device_drivers
 */
/**
 * @addtogroup VL53L0X_cut11_group
 * @ingroup device_drivers
 */
/**
 * @addtogroup VL53L0X_DevSpecDefines_group
 * @ingroup device_drivers
 */
/**
 * @addtogroup calc_sigma_estimate
 * @ingroup device_drivers
 */

/** @} */
