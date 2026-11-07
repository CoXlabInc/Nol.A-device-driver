#include "VL53L5CX.hpp"

VL53L5CX::VL53L5CX(TwoWire &wire, uint8_t address) : wire(wire) {
  config.platform.wire = &wire;
  config.platform.address = uint16_t(address) << 1;
}
uint8_t VL53L5CX::configurable() const {
  return initialized && !ranging ? VL53L5CX_STATUS_OK : VL53L5CX_STATUS_ERROR;
}
uint8_t VL53L5CX::begin() {
  if (ranging || config.platform.address < 0x10 || config.platform.address > 0xee)
    return VL53L5CX_STATUS_INVALID_PARAM;
  initialized = false;
  wire.begin();
  uint8_t alive = 0;
  uint8_t status = vl53l5cx_is_alive(&config, &alive);
  if (status) return status;
  if (!alive) return VL53L5CX_STATUS_ERROR;
  status = vl53l5cx_init(&config);
  if (!status) status = vl53l5cx_set_ranging_mode(&config, VL53L5CX_RANGING_MODE_CONTINUOUS);
  initialized = !status;
  return status;
}
uint8_t VL53L5CX::setAddress(uint8_t address) {
  if (address < 0x08 || address > 0x77) return VL53L5CX_STATUS_INVALID_PARAM;
  if (configurable()) return configurable();
  // Update the host address only after the device accepts the new address.
  uint8_t status = VL53L5CX_WrByte(&config.platform, 0x7fff, 0);
  if (status) return status;
  status = VL53L5CX_WrByte(&config.platform, 4, address);
  if (!status) config.platform.address = uint16_t(address) << 1;
  uint8_t restore = VL53L5CX_WrByte(&config.platform, 0x7fff, 2);
  return status ? status : restore;
}
uint8_t VL53L5CX::setResolution(uint8_t value) {
  return configurable() ? configurable() : vl53l5cx_set_resolution(&config, value);
}
uint8_t VL53L5CX::setRangingFrequencyHz(uint8_t value) {
  return configurable() ? configurable() : vl53l5cx_set_ranging_frequency_hz(&config, value);
}
uint8_t VL53L5CX::setRangingMode(RangingMode mode) {
  if (configurable()) return configurable();
  if (mode != RangingMode::Continuous && mode != RangingMode::Autonomous)
    return VL53L5CX_STATUS_INVALID_PARAM;
  return vl53l5cx_set_ranging_mode(&config, mode == RangingMode::Continuous ?
      VL53L5CX_RANGING_MODE_CONTINUOUS : VL53L5CX_RANGING_MODE_AUTONOMOUS);
}
uint8_t VL53L5CX::getRangingMode(RangingMode &mode) {
  if (!initialized) return VL53L5CX_STATUS_ERROR;
  uint8_t value = 0, status = vl53l5cx_get_ranging_mode(&config, &value);
  if (!status) mode = value == VL53L5CX_RANGING_MODE_CONTINUOUS ?
      RangingMode::Continuous : RangingMode::Autonomous;
  return status;
}
uint8_t VL53L5CX::setIntegrationTimeMs(uint32_t ms) {
  return configurable() ? configurable() : vl53l5cx_set_integration_time_ms(&config, ms);
}
uint8_t VL53L5CX::getIntegrationTimeMs(uint32_t &ms) {
  return initialized ? vl53l5cx_get_integration_time_ms(&config, &ms) : VL53L5CX_STATUS_ERROR;
}
uint8_t VL53L5CX::startRanging() {
  if (configurable()) return configurable();
  uint8_t status = vl53l5cx_start_ranging(&config);
  if (!status) ranging = true;
  return status;
}
uint8_t VL53L5CX::stopRanging() {
  if (!initialized) return VL53L5CX_STATUS_ERROR;
  uint8_t status = vl53l5cx_stop_ranging(&config);
  if (!status) ranging = false;
  return status;
}
uint8_t VL53L5CX::checkDataReady(bool &ready) {
  ready = false;
  if (!initialized || !ranging) return VL53L5CX_STATUS_ERROR;
  uint8_t value = 0, status = vl53l5cx_check_data_ready(&config, &value);
  if (!status) ready = value != 0;
  return status;
}
uint8_t VL53L5CX::getRangingData(VL53L5CX_ResultsData &results) {
  return initialized && ranging ? vl53l5cx_get_ranging_data(&config, &results) : VL53L5CX_STATUS_ERROR;
}
