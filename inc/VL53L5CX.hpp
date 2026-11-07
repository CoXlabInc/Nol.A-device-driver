#pragma once
#include <TwoWire.hpp>
#include "VL53L5CX/vl53l5cx_api.h"

// Keep this object and ResultsData in static storage on embedded targets.
// All operations return ST status codes (0 = success). Stop before configuring.
class VL53L5CX {
public:
  enum class RangingMode { Continuous, Autonomous };
  explicit VL53L5CX(TwoWire &wire, uint8_t address = 0x29);
  VL53L5CX(const VL53L5CX &) = delete;
  VL53L5CX &operator=(const VL53L5CX &) = delete;
  uint8_t begin();
  uint8_t setAddress(uint8_t address);
  uint8_t setResolution(uint8_t resolution);
  uint8_t setRangingFrequencyHz(uint8_t frequency);
  uint8_t setRangingMode(RangingMode mode);
  uint8_t getRangingMode(RangingMode &mode);
  uint8_t setIntegrationTimeMs(uint32_t ms);
  uint8_t getIntegrationTimeMs(uint32_t &ms);
  uint32_t getTransportRetryCount() const {
    return config.platform.readRetries + config.platform.firmwareWriteRetries;
  }
  uint8_t startRanging();
  uint8_t stopRanging();
  uint8_t checkDataReady(bool &ready);
  uint8_t getRangingData(VL53L5CX_ResultsData &results);
  // Raw ST API access; callers are responsible for its sequencing requirements.
  VL53L5CX_Configuration *configuration() { return &config; }
private:
  TwoWire &wire;
  VL53L5CX_Configuration config = {};
  bool initialized = false;
  bool ranging = false;
  uint8_t configurable() const;
};
