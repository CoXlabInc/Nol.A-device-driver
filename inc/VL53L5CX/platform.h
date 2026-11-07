/* Platform adaptation for ST VL53L5CX ULD. See LICENSE.md in this directory. */
#pragma once
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
class TwoWire;
extern "C" {
#endif

typedef struct {
  uint16_t address; /* ST 8-bit address; TwoWire receives address >> 1. */
  void *wire;
  uint32_t readRetries;
  uint32_t firmwareWriteRetries;
  uint8_t page;
} VL53L5CX_Platform;

#ifndef VL53L5CX_NB_TARGET_PER_ZONE
#define VL53L5CX_NB_TARGET_PER_ZONE 1U
#endif
#define PROCESSOR_LITTLE_ENDIAN
#define SWAP_UINT16(x) (x)
#define SWAP_UINT32(x) (x)

uint8_t VL53L5CX_RdByte(VL53L5CX_Platform *, uint16_t, uint8_t *);
uint8_t VL53L5CX_WrByte(VL53L5CX_Platform *, uint16_t, uint8_t);
uint8_t VL53L5CX_RdMulti(VL53L5CX_Platform *, uint16_t, uint8_t *, uint32_t);
uint8_t VL53L5CX_WrMulti(VL53L5CX_Platform *, uint16_t, uint8_t *, uint32_t);
void VL53L5CX_SwapBuffer(uint8_t *, uint16_t);
uint8_t VL53L5CX_WaitMs(VL53L5CX_Platform *, uint32_t);
#ifdef __cplusplus
}
#endif
