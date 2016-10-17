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

#include "M25P32.hpp"

//#define DEBUG
#ifdef DEBUG
#warning "Debugging prints enabled!"
#define debugPrint(m)      printf("[M25P32] %s\n",m)
#define debugPrintD(m,v)   printf("[M25P32] %s: %ld\n", m, v)
#define debugPrintU(m,v)   printf("[M25P32] %s: %lu\n", m, v)
#define debugPrintX(m,v)   printf("[M25P32] %s: 0x%lx\n", m, v)
#define debugPrintP(m,v)   printf("[M25P32] %s: p%p\n", m, v)
#else
#define debugPrint(...)
#define debugPrintD(...)
#define debugPrintU(...)
#define debugPrintX(...)
#define debugPrintP(...)
#endif //DEBUG

#define SPI_CONF  10000000ul, SPI::MSBFIRST, SPI::MODE0

M25P32::M25P32(uint8_t pinChipSelect, SPI &spi) {
  this->pinCS = pinChipSelect;
  this->spi = &spi;
}

void M25P32::beginTransaction() {
  this->spi->begin(SPI_CONF);
  digitalWrite(this->pinCS, LOW);
}

void M25P32::endTransaction() {
  digitalWrite(this->pinCS, HIGH);
  this->spi->end();
}

void M25P32::begin() {
  uint8_t buf[20];

  wakeup();

  pinMode(this->pinCS, OUTPUT);
  digitalWrite(this->pinCS, HIGH);

  beginTransaction();

  this->spi->transfer(CMD_READ_IDENTIFICATION);
  for (uint8_t i = 0; i < sizeof(buf); i++) {
    buf[i] = this->spi->transfer(0);
  }

  endTransaction();

  beginTransaction();

  this->spi->transfer(CMD_READ_STATUS);
  uint8_t st = this->spi->transfer(0);

  endTransaction();

  printf("[M25P32] ID:");
  for (uint8_t i = 0; i < sizeof(buf); i++) {
    printf(" %02X", buf[i]);
  }
  printf("\n");
  printf("[M25P32] st:0x%02X\n", st);

  enableWrite(false);

  sleep();
}

uint32_t M25P32::length() {
  return ADDR_END + 1;
}

uint8_t M25P32::read(int addr) {
  uint8_t data;

  if (read(&data, addr, 1) == ERROR_SUCCESS) {
    return data;
  } else {
    return 0xFF;
  }
}

error_t M25P32::readInternal(void *dst, int addr, uint32_t len) {
  uint32_t i;

  beginTransaction();
  spi->transfer(CMD_READ_DATA);
  spi->transfer((addr >> 16) & 0xff);
  spi->transfer((addr >> 8) & 0xff);
  spi->transfer((addr >> 0) & 0xff);

  //printf("addr: 0x%lX~0x%lX (up to 0x%lX)\n", addr, addr + len, ADDR_END + 0x10000);
  for (i = 0; i < len && addr + i <= ADDR_END + 0x10000; i++) {
    ((uint8_t *) dst)[i] = spi->transfer(0);
    //printf(" %02X", ((uint8_t *) dst)[i]);
  }
  endTransaction();

  //printf("\n");

  return ERROR_SUCCESS;
}

error_t M25P32::read(void *dst, int addr, uint32_t len) {
  if (addr > ADDR_END || len == 0)
    return ERROR_INVALID_ARGS;

  wakeup();
  error_t err = readInternal(dst, addr, len);
  sleep();

  return err;
}

error_t M25P32::write(int addr, uint8_t value) {
  return write(&value, addr, 1);
}

error_t M25P32::write(const uint8_t *buf, int addr, uint32_t len) {
  if (addr > ADDR_END || len == 0) {
    return ERROR_INVALID_ARGS;
  }

  wakeup();

  uint8_t st;
  uint32_t i, sectorAddr, pageAddr;
  uint8_t tmpBuf[256];

  for (sectorAddr = (addr / 0x10000) * 0x10000;
       sectorAddr < addr + len && sectorAddr < ADDR_END;
       sectorAddr += 0x10000) {
    debugPrintX("Target sector", sectorAddr);
    debugPrint("Erase backup sector.");

    waitUntilWriteDone();
    enableWrite(true);
    beginTransaction();
    spi->transfer(CMD_SECTOR_ERASE);
    spi->transfer(0x3F);
    spi->transfer(0x00);
    spi->transfer(0x00);
    endTransaction();

    for (pageAddr = sectorAddr;
         pageAddr < sectorAddr + 0xFFFF;
         pageAddr += 256) {

      waitUntilWriteDone();
      debugPrintX("Load a page from the sector.", pageAddr);
      readInternal(tmpBuf, pageAddr, 256);

      // for (uint32_t rp = 0; rp < 256; rp++) {
      //   printf("%02X ", tmpBuf[rp]);
      // }
      // printf("\n");

      for (uint32_t wp = pageAddr; wp < pageAddr + 256; wp++) {
        if (wp >= (uint32_t) addr && wp < (uint32_t) addr + len) {
          debugPrintX("Modified", wp - pageAddr);
          debugPrintX("refered", wp - addr);
          debugPrintX("data", buf[wp - addr]);
          tmpBuf[wp - pageAddr] = buf[wp - addr];
        }
      }

      uint32_t backupPageAddr = pageAddr - sectorAddr + 0x3F0000ul;
      debugPrintX("Save the page to the backup sector.", backupPageAddr);

      waitUntilWriteDone();
      enableWrite(true);
      beginTransaction();
      spi->transfer(CMD_PAGE_PROGRAM);
      spi->transfer((backupPageAddr >> 16) & 0xFF);
      spi->transfer((backupPageAddr >> 8) & 0xFF);
      spi->transfer((backupPageAddr >> 0) & 0xFF);

      for (uint32_t rp = 0; rp < 256; rp++) {
        // printf("%02X ", tmpBuf[rp]);
        spi->transfer(tmpBuf[rp]);
      }
      // printf("\n");
      endTransaction();
    }

    debugPrintX("Copy data from the backup sector to the target sector.",
                sectorAddr);

    waitUntilWriteDone();
    enableWrite(true);
    beginTransaction();
    spi->transfer(CMD_SECTOR_ERASE);
    spi->transfer((sectorAddr >> 16) & 0xFF);
    spi->transfer((sectorAddr >> 8) & 0xFF);
    spi->transfer((sectorAddr >> 0) & 0xFF);
    endTransaction();

    for (pageAddr = sectorAddr;
         pageAddr < sectorAddr + 0xFFFF;
         pageAddr += 256) {
      uint32_t backupPageAddr = pageAddr - sectorAddr + 0x3F0000UL;
      waitUntilWriteDone();
      debugPrintX("Load a page from the backup sector.", backupPageAddr);
      readInternal(tmpBuf, backupPageAddr, 256);

      debugPrintX("Save the page to the target sector.", pageAddr);
      enableWrite(true);
      beginTransaction();
      spi->transfer(CMD_PAGE_PROGRAM);
      spi->transfer((pageAddr >> 16) & 0xFF);
      spi->transfer((pageAddr >> 8) & 0xFF);
      spi->transfer((pageAddr >> 0) & 0xFF);

      for (uint32_t rp = 0; rp < 256; rp++) {
        // printf("%02X ", tmpBuf[rp]);
        spi->transfer(tmpBuf[rp]);
      }
      endTransaction();
      // printf("\n");
    }
  }

  sleep();

  return ERROR_FAIL;
}

void M25P32::waitUntilWriteDone() {
  uint8_t st;

  beginTransaction();
  spi->transfer(CMD_READ_STATUS);
  do {
    st = spi->transfer(0);
  } while(bitRead(st, STATUS_WIP) == 1);
  endTransaction();
}

void M25P32::wakeup() {
  beginTransaction();
  this->spi->transfer(CMD_RELEASE_FROM_DPD);
  endTransaction();
}

void M25P32::sleep() {
  uint8_t st;

  beginTransaction();
  spi->transfer(CMD_READ_STATUS);

  do {
    st = spi->transfer(0);

    if (st == 0xff)
      break; //already sleeping.
  } while(bitRead(st, STATUS_WIP) == 1);

  endTransaction();

  if (st != 0xff) {
    beginTransaction();
    spi->transfer(CMD_DEEP_POWER_DOWN);
    endTransaction();
  }
}

void M25P32::enableWrite(bool enable) {
  beginTransaction();
  this->spi->transfer((enable) ? CMD_WRITE_ENABLE : CMD_WRITE_DISABLE);
  endTransaction();
}
