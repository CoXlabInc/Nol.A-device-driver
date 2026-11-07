/**
 * @file M25P32.hpp
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
 * @date 2016. 10. 11.
 * @brief Nol.A library for M25P32 Micron 32Mb Serial NOR Flash Memory
 */

#ifndef M25P32_HPP
#define M25P32_HPP

#include "cox.h"
#include "Storage.hpp"
#include "SPI.hpp"

/**
 * @brief Blocking SPI NOR flash Storage implementation. Call begin() first.
 * The last 64 KiB sector is reserved for write backup; operations can wait
 * indefinitely for the device to finish programming or erasing.
 */
class M25P32 : public Storage {
public:
  /**
   * @brief Bind the active-low chip-select GPIO and SPI bus.
   */
  M25P32(uint8_t pinChipSelect, SPI &);
  /**
   * @brief Initialize chip select, read device information, disable writes, and enter sleep.
   */
  void begin();
  /**
   * @brief Return the exposed capacity in bytes (0x3F0000), excluding the backup sector.
   */
  uint64_t length();
  /**
   * @brief Read one byte at the byte address.
   * @return Read value, or 0xFF on invalid address (also a valid stored value).
   */
  uint8_t read(uint64_t addr);
  /**
   * @brief Read bytes into caller-provided storage.
   * @param dst Writable buffer of at least len bytes.
   * @param addr Starting byte address; keep the entire range within length().
   * @param len Nonzero byte count.
   * @return ERROR_INVALID_ARGS for an invalid start or zero length, otherwise ERROR_SUCCESS.
   */
  error_t read(void *dst, uint64_t addr, uint32_t len);
  /**
   * @brief Write a byte using the sector backup/erase procedure.
   * @param addr Target byte address below length().
   * @param value Byte to store.
   * @return ERROR_INVALID_ARGS for an invalid address; otherwise ERROR_FAIL even
   * after programming, matching the current implementation.
   */
  error_t write(uint64_t addr, uint8_t value);
  /**
   * @brief Rewrite affected sectors via the reserved backup sector.
   * @param buf Source buffer containing at least len bytes.
   * @param addr Starting byte address; keep the entire range within length().
   * @param len Nonzero byte count.
   * @return ERROR_INVALID_ARGS for an invalid start or zero length; otherwise
   * ERROR_FAIL even after programming, matching the current implementation.
   */
  error_t write(const uint8_t *buf, uint64_t addr, uint32_t len);

private:
  SPI *spi;
  uint8_t pinCS;

  void beginTransaction();
  void endTransaction();
  void wakeup();
  void sleep();
  void enableWrite(bool enable);
  error_t readInternal(void *dst, uint32_t addr, uint32_t len);
  void waitUntilWriteDone();

  enum {
    CMD_WRITE_ENABLE = 0x06,
    CMD_WRITE_DISABLE = 0x04,
    CMD_READ_IDENTIFICATION = 0x9F,
    CMD_READ_STATUS = 0x05,
    CMD_WRITE_STATUS = 0x01,
    CMD_READ_DATA = 0x03,
    CMD_READ_DATA_HISPEED = 0x0B,
    CMD_PAGE_PROGRAM = 0x02,
    CMD_SECTOR_ERASE = 0xD8,
    CMD_BULK_ERASE = 0xC7,
    CMD_DEEP_POWER_DOWN = 0xB9,
    CMD_RELEASE_FROM_DPD = 0xAB,
  };

  enum {
    ADDR_START = 0x000000UL,
    ADDR_END = 0x3EFFFFUL,
  };

  enum {
    STATUS_SRWD = 7,
    STATUS_TOPBTM = 5,
    STATUS_BLOCK_PROTECTS = 2,
    STATUS_WEL = 1,
    STATUS_WIP = 0,
  };
};

#endif /* M25P32_HPP */
