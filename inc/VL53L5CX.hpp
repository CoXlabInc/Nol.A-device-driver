/**
 * @file VL53L5CX.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
#pragma once
#include <TwoWire.hpp>
#include "VL53L5CX/vl53l5cx_api.h"

// Keep this object and ResultsData in static storage on embedded targets.
// All operations return ST status codes (0 = success). Stop before configuring.
/**
 * @brief VL53L5CX multi-zone time-of-flight sensor wrapper.
 * Call begin(), configure while stopped, then startRanging(). Poll checkDataReady()
 * before getRangingData(). Status-returning methods use ST codes (0 = success).
 * Keep this object and VL53L5CX_ResultsData in static storage on embedded targets.
 */
class VL53L5CX {
public:
  /**
   * @brief Continuous or autonomous sensor ranging mode.
   */
  enum class RangingMode { Continuous, Autonomous };
  /**
   * @brief Bind the I2C bus and initial 7-bit address (default 0x29).
   */
  explicit VL53L5CX(TwoWire &wire, uint8_t address = 0x29);
  /**
   * @brief Copying is disabled because configuration contains transport state.
   */
  VL53L5CX(const VL53L5CX &) = delete;
  /**
   * @brief Copy assignment is disabled because configuration contains transport state.
   */
  VL53L5CX &operator=(const VL53L5CX &) = delete;
  /**
   * @brief Initialize the bus, probe and load sensor firmware, then select continuous mode.
   * @return ST status; an invalid address or active ranging prevents initialization.
   */
  uint8_t begin();
  /**
   * @brief Change the sensor and host address while initialized and stopped.
   * @param address New 7-bit address from 0x08 through 0x77.
   * @return ST status; the host address changes only after the sensor accepts it.
   */
  uint8_t setAddress(uint8_t address);
  /**
   * @brief Set the zone resolution while initialized and stopped.
   * @param resolution VL53L5CX_RESOLUTION_4X4 or VL53L5CX_RESOLUTION_8X8.
   * @return ST status.
   */
  uint8_t setResolution(uint8_t resolution);
  /**
   * @brief Set frame frequency in hertz while initialized and stopped.
   * @param frequency Sensor-supported frequency for the selected resolution and mode.
   * @return ST status from the underlying API.
   */
  uint8_t setRangingFrequencyHz(uint8_t frequency);
  /**
   * @brief Select continuous or autonomous operation while initialized and stopped.
   * @param mode Ranging mode.
   * @return ST status; invalid enum values are rejected.
   */
  uint8_t setRangingMode(RangingMode mode);
  /**
   * @brief Read the ranging mode after initialization.
   * @param mode Output updated on success.
   * @return ST status.
   */
  uint8_t getRangingMode(RangingMode &mode);
  /**
   * @brief Set integration time in milliseconds while initialized and stopped.
   * @param ms Sensor-supported integration time.
   * @return ST status from the underlying API.
   */
  uint8_t setIntegrationTimeMs(uint32_t ms);
  /**
   * @brief Read integration time after initialization.
   * @param ms Output integration time in milliseconds.
   * @return ST status.
   */
  uint8_t getIntegrationTimeMs(uint32_t &ms);
  /**
   * @brief Return the sum of transport read retries and firmware-write retries.
   */
  uint32_t getTransportRetryCount() const {
    return config.platform.readRetries + config.platform.firmwareWriteRetries;
  }
  /**
   * @brief Start measurements after initialization while stopped.
   * @return ST status; the local ranging flag is set only on success.
   */
  uint8_t startRanging();
  /**
   * @brief Stop measurements after initialization.
   * @return ST status; the local ranging flag is cleared only on success.
   */
  uint8_t stopRanging();
  /**
   * @brief Poll for a new frame while ranging.
   * @param ready Set false first, then updated from the sensor on success.
   * @return ST status; requires initialization and active ranging.
   */
  uint8_t checkDataReady(bool &ready);
  /**
   * @brief Read a frame while ranging; call checkDataReady() first.
   * @param results Caller-owned result storage populated by the ST API.
   * @return ST status.
   */
  uint8_t getRangingData(VL53L5CX_ResultsData &results);
  // Raw ST API access; callers are responsible for its sequencing requirements.
  /**
   * @brief Return a borrowed pointer to the raw ST configuration.
   * Callers must obey ST sequencing rules and keep wrapper state consistent;
   * changes through this pointer bypass the wrapper checks.
   */
  VL53L5CX_Configuration *configuration() { return &config; }
private:
  TwoWire &wire;
  VL53L5CX_Configuration config = {};
  bool initialized = false;
  bool ranging = false;
  uint8_t configurable() const;
};

/** @} */
