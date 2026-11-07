#include <TwoWire.hpp>
#include <cox.h>
#include "VL53L5CX/vl53l5cx_api.h"

static bool valid(VL53L5CX_Platform *platform, uint16_t reg,
                  uint8_t *data, uint32_t size) {
  return platform && platform->wire && (data || !size) &&
         size <= 0x10000UL - reg;
}

static uint8_t writeChunk(TwoWire &wire, uint8_t address, uint16_t reg,
                          const uint8_t *data, uint8_t count, bool stop = true) {
  wire.beginTransmission(address);
  bool written = wire.write(uint8_t(reg >> 8)) == 1;
  written &= wire.write(uint8_t(reg)) == 1;
  written &= wire.write(data, count) == count;
  return written ? wire.endTransmission(stop) : TwoWire::DATA_TOO_LONG;
}

uint8_t VL53L5CX_WrMulti(VL53L5CX_Platform *platform, uint16_t reg,
                        uint8_t *data, uint32_t size) {
  if (!valid(platform, reg, data, size)) return VL53L5CX_STATUS_INVALID_PARAM;
  TwoWire &wire = *static_cast<TwoWire *>(platform->wire);
  // Only firmware RAM writes can be replayed. Runtime commands are never retried.
  bool firmware = reg == 0 && size >= 0x5000 && platform->page >= 9 && platform->page <= 11;
  if (reg == 0x7fff && size == 1) {
    uint8_t error = writeChunk(wire, platform->address >> 1, reg, data, 1);
    if (error) return VL53L5CX_MCU_ERROR;
    platform->page = data[0];
    return VL53L5CX_STATUS_OK;
  }
  while (size) {
    uint8_t count = size > 30 ? 30 : size;
    uint8_t error = 0;
    for (uint8_t attempt = 0; attempt < (firmware ? 3 : 1); ++attempt) {
      if (attempt) {
        ++platform->firmwareWriteRetries;
        System.feedWatchdog();
        // A failed final chunk may have reached the page selector: restore the bank.
        if (writeChunk(wire, platform->address >> 1, 0x7fff, &platform->page, 1))
          return VL53L5CX_MCU_ERROR;
      }
      error = writeChunk(wire, platform->address >> 1, reg, data, count);
      if (!error) break;
    }
    if (error) return VL53L5CX_MCU_ERROR;
    reg += count;
    data += count;
    size -= count;
    if (firmware) System.feedWatchdog();
  }
  return VL53L5CX_STATUS_OK;
}

uint8_t VL53L5CX_RdMulti(VL53L5CX_Platform *platform, uint16_t reg,
                        uint8_t *data, uint32_t size) {
  if (!valid(platform, reg, data, size)) return VL53L5CX_STATUS_INVALID_PARAM;
  TwoWire &wire = *static_cast<TwoWire *>(platform->wire);
  while (size) {
    uint8_t count = size > 32 ? 32 : size;
    bool received = false;
    for (uint8_t attempt = 0; attempt < 3; ++attempt) {
      if (attempt) { ++platform->readRetries; System.feedWatchdog(); }
      // Re-establish the exact register pointer on every attempt; discard partial data.
      if (writeChunk(wire, platform->address >> 1, reg, nullptr, 0, false)) continue;
      if (wire.requestFrom(platform->address >> 1, count) != count) continue;
      received = true;
      for (uint8_t i = 0; i < count; ++i) {
        int value = wire.read();
        if (value < 0) { received = false; break; }
        data[i] = uint8_t(value);
      }
      if (received) break;
    }
    if (!received) return VL53L5CX_MCU_ERROR;
    reg += count;
    data += count;
    size -= count;
  }
  return VL53L5CX_STATUS_OK;
}

uint8_t VL53L5CX_RdByte(VL53L5CX_Platform *p, uint16_t reg, uint8_t *data) {
  return VL53L5CX_RdMulti(p, reg, data, 1);
}
uint8_t VL53L5CX_WrByte(VL53L5CX_Platform *p, uint16_t reg, uint8_t data) {
  return VL53L5CX_WrMulti(p, reg, &data, 1);
}
void VL53L5CX_SwapBuffer(uint8_t *data, uint16_t size) {
  for (uint16_t i = 0; i + 3 < size; i += 4) {
    uint8_t a = data[i], b = data[i + 1];
    data[i] = data[i + 3]; data[i + 1] = data[i + 2];
    data[i + 2] = b; data[i + 3] = a;
  }
}
uint8_t VL53L5CX_WaitMs(VL53L5CX_Platform *, uint32_t ms) {
  while (ms) {
    uint16_t part = ms > 1000 ? 1000 : ms;
    delay(part);
    ms -= part;
    System.feedWatchdog();
  }
  return VL53L5CX_STATUS_OK;
}
