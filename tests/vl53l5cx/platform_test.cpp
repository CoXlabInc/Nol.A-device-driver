#include <cassert>
#include <vector>
#include <TwoWire.hpp>
#include <cox.h>
#include "VL53L5CX/vl53l5cx_api.h"

TestSystem System;
static uint32_t delayed;
void delay(uint16_t ms) { delayed += ms; }

int main() {
  for (uint32_t size : {0U, 1U, 29U, 30U, 31U, 32U, 33U, 65U, 32768U}) {
    TwoWire wire;
    VL53L5CX_Platform platform = {};
    platform.address = 0x52; platform.wire = &wire;
    std::vector<uint8_t> data(size);
    for (uint32_t i = 0; i < size; ++i) data[i] = uint8_t(i);
    assert(VL53L5CX_WrMulti(&platform, 0x1000, data.data(), size) == 0);
    uint32_t offset = 0;
    for (const auto &t : wire.transactions) {
      assert(t.address == 0x29 && t.stop && t.data.size() <= 32);
      assert(((uint32_t(t.data[0]) << 8) | t.data[1]) == 0x1000 + offset);
      for (size_t i = 2; i < t.data.size(); ++i) assert(t.data[i] == uint8_t(offset++));
    }
    assert(offset == size);
    wire.transactions.clear();
    assert(VL53L5CX_RdMulti(&platform, 0x1000, data.data(), size) == 0);
    offset = 0;
    assert(wire.reads.size() == wire.transactions.size());
    for (size_t i = 0; i < wire.reads.size(); ++i) {
      const auto &t = wire.transactions[i];
      assert(t.address == 0x29 && !t.stop && t.data.size() == 2);
      assert(((uint32_t(t.data[0]) << 8) | t.data[1]) == 0x1000 + offset);
      assert(wire.reads[i] <= 32); offset += wire.reads[i];
    }
    for (uint32_t i = 0; i < size; ++i) assert(data[i] == uint8_t(i));
    assert(offset == size);
  }
  {
    TwoWire wire;
    VL53L5CX_Platform p = {};
    p.address = 0x52; p.wire = &wire; p.page = 9;
    std::vector<uint8_t> firmware(32768, 0xa5);
    wire.nackRemaining = 1;
    assert(VL53L5CX_WrMulti(&p, 0, firmware.data(), firmware.size()) == 0);
    assert(p.firmwareWriteRetries == 1);
    assert(wire.transactions[0].data == wire.transactions[2].data);
    assert(wire.transactions[1].data == std::vector<uint8_t>({0x7f, 0xff, 9}));
    wire.transactions.clear();
    wire.nackRemaining = 1;
    uint8_t command[4] = {};
    assert(VL53L5CX_WrMulti(&p, 0x2ffc, command, 4) == VL53L5CX_MCU_ERROR);
    assert(wire.transactions.size() == 1); // Never replay a runtime command.
    wire.shortReadsRemaining = 1;
    uint8_t result[32];
    assert(VL53L5CX_RdMulti(&p, 0x100, result, sizeof(result)) == 0);
    assert(p.readRetries == 1);
    for (unsigned i = 0; i < sizeof(result); ++i) assert(result[i] == i);
  }
  uint8_t data[4] = {0, 1, 2, 255};
  TwoWire wire;
  VL53L5CX_Platform platform = {};
    platform.address = 0x52; platform.wire = &wire;
  wire.nack = true;
  assert(VL53L5CX_WrMulti(&platform, 0, data, 4) == VL53L5CX_MCU_ERROR);
  assert(VL53L5CX_RdMulti(&platform, 0, data, 4) == VL53L5CX_MCU_ERROR);
  wire.nack = false; wire.shortRead = true;
  assert(VL53L5CX_RdMulti(&platform, 0, data, 4) == VL53L5CX_MCU_ERROR);
  wire.shortRead = false; wire.failRead = true;
  assert(VL53L5CX_RdMulti(&platform, 0, data, 4) == VL53L5CX_MCU_ERROR);
  wire.failRead = false; wire.failWrite = true;
  assert(VL53L5CX_WrMulti(&platform, 0, data, 4) == VL53L5CX_MCU_ERROR);
  assert(VL53L5CX_RdMulti(&platform, 65535, data, 2) == VL53L5CX_STATUS_INVALID_PARAM);
  assert(VL53L5CX_RdMulti(nullptr, 0, data, 1) == VL53L5CX_STATUS_INVALID_PARAM);
  assert(VL53L5CX_WrMulti(&platform, 0, nullptr, 1) == VL53L5CX_STATUS_INVALID_PARAM);
  VL53L5CX_SwapBuffer(data, 4);
  assert(data[0] == 255 && data[1] == 2 && data[2] == 1 && data[3] == 0);
  assert(VL53L5CX_WaitMs(&platform, 65537) == 0 && delayed == 65537);
}
