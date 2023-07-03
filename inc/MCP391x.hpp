// -*- mode:c++; indent-tabs-mode:nil; -*-

#pragma once

#include <SPI.hpp>
#include <nola-common.h>

class MCP391x {
public:
  /* TWI not supported yet */
  typedef enum { MCP3918, // 1 channel (SPI/TWI)
                 MCP3910, // 2 channels (SPI/TWI)
                 MCP3919, // 3 channels (SPI/TWI)
                 MCP3912, // 4 channels (SPI)
                 MCP3913, // 6 channels (SPI)
                 MCP3914, // 8 channels (SPI)
  } Chip_t;
  
  MCP391x(Chip_t type, SPI &spi, int8_t cs, int8_t dr, int8_t reset = -1)
    : spi(spi), PIN_RESET(reset), PIN_CS(cs), PIN_DR(dr),
      NumChannels((type == MCP3918) ? 1 :
                  (type == MCP3910) ? 2 :
                  (type == MCP3919) ? 3 :
                  (type == MCP3912) ? 4 :
                  (type == MCP3913) ? 6 :
                  (type == MCP3914) ? 8 : 0) {
  }

  void begin(bool useInternalClock = false);

private:
  SPI &spi;
  const int8_t PIN_RESET, PIN_CS, PIN_DR;
  const uint8_t NumChannels;
  unsigned channelWidth;
  unsigned repeat;
  unsigned dataSize;
  unsigned dataOffset;
  
  virtual void select() {
    this->spi.begin(20000000ul, SPI::MSBFIRST, SPI::MODE0);
    digitalWrite(this->PIN_CS, LOW);
  };
  
  virtual void deselect() {
    digitalWrite(this->PIN_CS, HIGH);
    this->spi.end();
  };
  
  void parse_statuscom(uint32_t statuscom, unsigned *channel_width, unsigned *repeat);
  void update_statuscom(uint32_t val);

  typedef enum { REG_CHANNEL_BASE = 0,
                 REG_MOD = 8,
                 REG_PHASE0 = 9,
                 REG_PHASE1 = 10,
                 REG_GAIN = 11,
                 REG_STATUSCOM = 12,
                 REG_CONFIG0 = 13,
                 REG_CONFIG1 = 14,
                 REG_OFFCAL_CH0 = 15, // every channel takes 2 register
                 REG_GAINCAL_CH0 = 16, // every channel takes 2 register
                 REG_SECURITY = 31,
                 REG_NUM = 32,
  } Reg_t;

  uint8_t reg_width(Reg_t reg);
  void write_reg(Reg_t, uint32_t val);
  uint32_t read_reg(Reg_t);
  uint32_t read_channel(unsigned channel, unsigned len);
  void stream_start(unsigned reg);
  void stream_end();
};
