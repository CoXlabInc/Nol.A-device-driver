/*************************************************** 
  This is a library for the Adafruit VS1053 Codec Breakout

  Designed specifically to work with the Adafruit VS1053 Codec Breakout 
  ----> https://www.adafruit.com/products/1381

  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution

 - Ported v1.0.4 for Nol.A-SDK by Jongsoo Jeong (CoXlab Inc.)
 ****************************************************/
#ifndef ADAFRUIT_VS1053_HPP
#define ADAFRUIT_VS1053_HPP

#include <cox.h>
#include <SPI.hpp>

class Adafruit_VS1053 {
 public:
  Adafruit_VS1053(SPI &s, int8_t rst, int8_t cs, int8_t dcs, int8_t dreq);
  uint8_t begin(void);
  void reset(void);
  void softReset(void);
  uint16_t sciRead(uint8_t addr);
  void sciWrite(uint8_t addr, uint16_t data);
  void sineTest(uint8_t n, uint16_t ms);
  void spiwrite(uint8_t d);
  void spiwrite(uint8_t *c, uint16_t num); 
  uint8_t spiread(void);

  uint16_t decodeTime(void);
  void setVolume(uint8_t left, uint8_t right);
  void dumpRegs(void);

  void playData(uint8_t *buffer, uint8_t buffsiz);
  boolean readyForData(void);
  void applyPatch(const uint16_t *patch, uint16_t patchsize);
  uint16_t loadPlugin(char *fn);

  void GPIO_digitalWrite(uint8_t i, uint8_t val);
  void GPIO_digitalWrite(uint8_t i);
  uint16_t GPIO_digitalRead(void);
  boolean GPIO_digitalRead(uint8_t i);
  void GPIO_pinMode(uint8_t i, uint8_t dir);
 
  boolean prepareRecordOgg(char *plugin);
  void startRecordOgg(boolean mic);
  void stopRecordOgg(void);
  uint16_t recordedWordsWaiting(void);
  uint16_t recordedReadWord(void);

  enum {
    DATABUFFERLEN = 32,

    SCI_READ = 0x03,
    SCI_WRITE = 0x02,

    REG_MODE = 0x00,
    REG_STATUS = 0x01,
    REG_BASS = 0x02,
    REG_CLOCKF = 0x03,
    REG_DECODETIME = 0x04,
    REG_AUDATA = 0x05,
    REG_WRAM = 0x06,
    REG_WRAMADDR = 0x07,
    REG_HDAT0 = 0x08,
    REG_HDAT1 = 0x09,
    REG_VOLUME = 0x0B,

    GPIO_DDR = 0xC017,
    GPIO_IDATA = 0xC018,
    GPIO_ODATA = 0xC019,

    INT_ENABLE = 0xC01A,

    MODE_SM_DIFF = 0x0001,
    MODE_SM_LAYER12 = 0x0002,
    MODE_SM_RESET = 0x0004,
    MODE_SM_CANCEL = 0x0008,
    MODE_SM_EARSPKLO = 0x0010,
    MODE_SM_TESTS = 0x0020,
    MODE_SM_STREAM = 0x0040,
    MODE_SM_SDINEW = 0x0800,
    MODE_SM_ADPCM = 0x1000,
    MODE_SM_LINE1 = 0x4000,
    MODE_SM_CLKRANGE = 0x8000,

    SCI_AIADDR = 0x0A,
    SCI_AICTRL0 = 0x0C,
    SCI_AICTRL1 = 0x0D,
    SCI_AICTRL2 = 0x0E,
    SCI_AICTRL3 = 0x0F,
  };

  uint8_t mp3buffer[DATABUFFERLEN];

 protected:
  uint8_t  _dreq;
 private:
  SPI &_spi;
  const int8_t _reset, _cs, _dcs;
};


class Adafruit_VS1053_FilePlayer : public Adafruit_VS1053 {
 public:
  Adafruit_VS1053_FilePlayer (SPI &spi, int8_t rst, int8_t cs, int8_t dcs, int8_t dreq);
  Adafruit_VS1053_FilePlayer (SPI &spi, int8_t cs, int8_t dcs, int8_t dreq);

  boolean begin(void);
  FILE *currentTrack = NULL;
  volatile boolean playingMusic;
  void feedBuffer(void);
  boolean startPlayingFile(const char *trackname);
  boolean playFullFile(const char *trackname);
  void stopPlaying(void);
  boolean paused(void);
  boolean stopped(void);
  void pausePlaying(boolean pause);

 private:
  void feedBuffer_noLock(void);
  volatile boolean feedBufferLock = false;
};

#endif // ADAFRUIT_VS1053_HPP
