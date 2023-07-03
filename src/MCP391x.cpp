#include <MCP391x.hpp>
#include <stdio.h>

enum {
	MCP3914_MAX_SAMPLE_SIZE = 32,
};

enum { MCP3914_CONFIG0_PRE_MASK = (3ul << 16),
       MCP3914_CONFIG0_PRE1 = (0ul << 16),
       MCP3914_CONFIG0_PRE2 = (1ul << 16),
       MCP3914_CONFIG0_PRE4 = (2ul << 16),
       MCP3914_CONFIG0_PRE8 = (3ul << 16),
       MCP3914_CONFIG0_OSC_32 = (0ul << 13),
       MCP3914_CONFIG0_OSC_64 = (1ul << 13),
       MCP3914_CONFIG0_OSC_128 = (2ul << 13),
       MCP3914_CONFIG0_OSC_256 = (3ul << 13),
       MCP3914_CONFIG0_OSC_512 = (4ul << 13),
       MCP3914_CONFIG0_OSC_1024 = (5ul << 13),
       MCP3914_CONFIG0_OSC_2048 = (6ul << 13),
       MCP3914_CONFIG0_OSC_4096 = (7ul << 13),
       MCP3914_CONFIG0_OSC_MASK = (7ul << 13),

       MCP3914_CONFIG1_VREF_MASK = (1ul << 7),
       MCP3914_CONFIG1_VREF_EXT = (1ul << 7),
       MCP3914_CONFIG1_VREF_INT = (0ul << 7),
       MCP3914_CONFIG1_CLK_MASK = (1ul << 6),
       MCP3914_CONFIG1_CLK_EXT = (1ul << 6),
       MCP3914_CONFIG1_CLK_INT = (0ul << 6),

       MCP3914_STATUSCOM_WIDTH_DATA_SHIFT = 16,
       MCP3914_STATUSCOM_WIDTH_DATA_MASK = (3ul << MCP3914_STATUSCOM_WIDTH_DATA_SHIFT),
       MCP3914_STATUSCOM_WIDTH_DATA_32S  = (3ul << MCP3914_STATUSCOM_WIDTH_DATA_SHIFT),
       MCP3914_STATUSCOM_WIDTH_DATA_32Z  = (2ul << MCP3914_STATUSCOM_WIDTH_DATA_SHIFT),
       MCP3914_STATUSCOM_WIDTH_DATA_24   = (1ul << MCP3914_STATUSCOM_WIDTH_DATA_SHIFT),
       MCP3914_STATUSCOM_WIDTH_DATA_16   = (0ul << MCP3914_STATUSCOM_WIDTH_DATA_SHIFT),
       MCP3914_STATUSCOM_WIDTH_DATA_DEFAULT = MCP3914_STATUSCOM_WIDTH_DATA_24,
       MCP3914_STATUSCOM_READ_SHIFT = 22,
       MCP3914_STATUSCOM_READ_MASK  = (3ul << MCP3914_STATUSCOM_READ_SHIFT),
       MCP3914_STATUSCOM_READ_ALL   = (3ul << MCP3914_STATUSCOM_READ_SHIFT),
       MCP3914_STATUSCOM_READ_TYPES = (2ul << MCP3914_STATUSCOM_READ_SHIFT),
       MCP3914_STATUSCOM_READ_GROUP = (1ul << MCP3914_STATUSCOM_READ_SHIFT),
       MCP3914_STATUSCOM_READ_ONE   = (0ul << MCP3914_STATUSCOM_READ_SHIFT),
       MCP3914_STATUSCOM_READ_DEFAULT = MCP3914_STATUSCOM_READ_TYPES,
};

enum {
	MCP3914_CMD_ADDRESS = 1,
	MCP3914_CMD_READ_MASK = 1,
	MCP3914_CMD_WRITE_MASK = 0
};

enum MCP3914_STREAM
{
	MCP3914_STREAM_SINGLE = 0,
	MCP3914_STREAM_GROUP,
	MCP3914_STREAM_ALL
};

void MCP391x::begin(bool useInternalClock) {
  pinMode(this->PIN_CS, OUTPUT);
  digitalWrite(this->PIN_CS, HIGH);

  pinMode(this->PIN_DR, INPUT_PULLUP);

  if (this->PIN_RESET >= 0) {
    pinMode(this->PIN_RESET, OUTPUT);
    digitalWrite(this->PIN_RESET, HIGH);
    delay(1);
    digitalWrite(this->PIN_RESET, LOW);
    delay(1);
    digitalWrite(this->PIN_RESET, HIGH);
    delay(1);
  }

  uint32_t v = this->read_reg(REG_SECURITY);
  printf("[MCP391x] security:0x%lX", v);

  if (v == 0xa50000) {
    uint32_t config1 = this->read_reg(REG_CONFIG1);
    uint32_t config1Modified = config1;
    if (useInternalClock) {
      config1Modified = (config1 & ~MCP3914_CONFIG1_CLK_MASK) | MCP3914_CONFIG1_CLK_INT;
      this->write_reg(REG_CONFIG1, config1Modified);
    }

    uint32_t statuscom = this->read_reg(REG_STATUSCOM);
    update_statuscom(statuscom);

    uint32_t config0 = this->read_reg(REG_CONFIG0);
    uint32_t config0Modified = (((config0 & ~MCP3914_CONFIG0_PRE_MASK) | MCP3914_CONFIG0_PRE1) |
                                ((config0 & ~MCP3914_CONFIG0_OSC_MASK) | MCP3914_CONFIG0_OSC_4096));
    this->write_reg(REG_CONFIG0, config0Modified);

    printf("config1:0x%lx->%lx,statuscom:0x%lx(channelWidth:%u,repeat:%u,dataSize:%u),config0:0x%lx->%lx",
           config1, config1Modified,
           statuscom, this->channelWidth, this->repeat, this->dataSize,
           config0, config0Modified);
  }
  printf("\n");
}

static uint8_t READ_CMD(uint8_t address, uint8_t reg)
{
	return (address << 6) | (reg << 1) | MCP3914_CMD_READ_MASK;
}

static uint8_t WRITE_CMD(uint8_t address, uint8_t reg)
{
	return (address << 6) | (reg << 1) | MCP3914_CMD_WRITE_MASK;
}

void MCP391x::parse_statuscom(uint32_t statuscom, unsigned *channel_width, unsigned *repeat) {
	switch (statuscom & MCP3914_STATUSCOM_WIDTH_DATA_MASK)
	{
	case MCP3914_STATUSCOM_WIDTH_DATA_16:
		*channel_width = 2;
		break;
	case MCP3914_STATUSCOM_WIDTH_DATA_24:
		*channel_width = 3;
		break;
	case MCP3914_STATUSCOM_WIDTH_DATA_32S:
	case MCP3914_STATUSCOM_WIDTH_DATA_32Z:
		*channel_width = 4;
		break;
	}
	switch (statuscom & MCP3914_STATUSCOM_READ_MASK)
	{
	case MCP3914_STATUSCOM_READ_ONE:
		*repeat = 1;
		break;
	case MCP3914_STATUSCOM_READ_GROUP:
		*repeat = 2; // TODO: incorrect for all registers
		break;
	case MCP3914_STATUSCOM_READ_TYPES:
		*repeat = 8; // TODO: incorrect for all registers
		break;
	case MCP3914_STATUSCOM_READ_ALL:
		*repeat = 32;
		break;
	}
}

void MCP391x::update_statuscom(uint32_t val) {
  parse_statuscom(val, &this->channelWidth, &this->repeat);
  this->dataSize = this->channelWidth * this->repeat;
}

void MCP391x::write_reg(Reg_t reg, uint32_t val) {
  this->select();
	this->spi.transfer(WRITE_CMD(MCP3914_CMD_ADDRESS, reg));
	if (reg == REG_MOD)
		this->spi.transfer((val >> 24) & 0xff);
	this->spi.transfer((val >> 16) & 0xff);
	this->spi.transfer((val >> 8) & 0xff);
	this->spi.transfer(val & 0xff);
  this->deselect();
}

uint8_t MCP391x::reg_width(Reg_t reg) {
	if (reg >= REG_CHANNEL_BASE
			&& reg < REG_CHANNEL_BASE + this->NumChannels)
		return 2; // minimal size
	else
	if (reg == REG_MOD)
		return 4;
	else
		return 3;
}

uint32_t MCP391x::read_reg(Reg_t reg) {
	uint32_t val = 0;
	unsigned len = this->reg_width(reg);
	//assert(len >= 2 && len <= 4);
  this->select();
	this->spi.transfer(READ_CMD(MCP3914_CMD_ADDRESS, reg));
	switch (len) {
	case 4:
		val |= this->spi.transfer(0xff) << 24;
	case 3:
		val |= this->spi.transfer(0xff) << 16;
	case 2:
		val |= this->spi.transfer(0xff) << 8;
		val |= this->spi.transfer(0xff);
  default:
    break;
	}
	this->deselect();
	return val;
}

uint32_t MCP391x::read_channel(unsigned channel, unsigned len) {
	uint32_t val = 0;
	//assert(len >= 2 && len <= 4);
  this->select();
	this->spi.transfer(READ_CMD(MCP3914_CMD_ADDRESS, REG_CHANNEL_BASE + channel));
	switch (len) {
	case 4:
		val |= this->spi.transfer(0xff) << 24;
	case 3:
		val |= this->spi.transfer(0xff) << 16;
	case 2:
		val |= this->spi.transfer(0xff) << 8;
		val |= this->spi.transfer(0xff);
  default:
    break;
	}
  this->deselect();
	return val;
}

void MCP391x::stream_start(unsigned reg) {
  this->select();
	this->spi.transfer(READ_CMD(MCP3914_CMD_ADDRESS, reg));
}

void MCP391x::stream_end() {
  this->deselect();
}
