/**
 * @file Adafruit_VS1053.hpp
 * @brief Public driver declarations and configuration constants.
 */
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

#ifdef NOLA_FILE_SUPPORTED

/**
 * @brief VS1053 SPI codec driver, available only when NOLA_FILE_SUPPORTED is defined.
 */
class Adafruit_VS1053 {
 public:
  /**
   * @brief Bind the SPI bus and reset, control-select, data-select, and data-ready pins.
   * The bus must outlive the driver; reset pin -1 omits the hardware reset pin.
   */
  Adafruit_VS1053(SPI &s, int8_t rst, int8_t cs, int8_t dcs, int8_t dreq);
  /**
   * @brief Initialize codec GPIO/SPI and reset it.
   * The codec base returns the version nibble; FilePlayer returns true only for
   * version 4 and successful attachment of the DREQ change interrupt.
   */
  uint8_t begin(void);
  /**
   * @brief Perform hardware reset when available, then software reset and codec setup.
   */
  void reset(void);
  /**
   * @brief Set the software-reset bit and wait for codec startup.
   */
  void softReset(void);
  /**
   * @brief Read a 16-bit SCI register at addr; the caller must observe codec readiness.
   */
  uint16_t sciRead(uint8_t addr);
  /**
   * @brief Write a 16-bit SCI register at addr; the caller must observe codec readiness.
   */
  void sciWrite(uint8_t addr, uint16_t data);
  /**
   * @brief Run the encoded sine-test tone n for ms milliseconds.
   */
  void sineTest(uint8_t n, uint16_t ms);
  /**
   * @brief Write one byte or num bytes over the codec SPI data connection.
   */
  void spiwrite(uint8_t d);
  /**
   * @brief Write one byte or num bytes over the codec SPI data connection.
   */
  void spiwrite(uint8_t *c, uint16_t num); 
  /**
   * @brief Transfer a dummy byte and return the received SPI byte.
   */
  uint8_t spiread(void);

  /**
   * @brief Return the codec decoded playback time in seconds.
   */
  uint16_t decodeTime(void);
  /**
   * @brief Set left/right attenuation bytes in half-decibel steps (0 is loudest).
   */
  void setVolume(uint8_t left, uint8_t right);
  /**
   * @brief Print diagnostic SCI register values.
   */
  void dumpRegs(void);

  /**
   * @brief Send buffsiz encoded audio bytes from buffer to the codec; use readyForData() first.
   */
  void playData(uint8_t *buffer, uint8_t buffsiz);
  /**
   * @brief Return the current data-ready GPIO level.
   */
  boolean readyForData(void);
  /**
   * @brief Apply a codec patch array of patchsize 16-bit words.
   */
  void applyPatch(const uint16_t *patch, uint16_t patchsize);
  /**
   * @brief Load a plugin file named fn; return its entry address, or 0xFFFF on failure.
   */
  uint16_t loadPlugin(char *fn);

  /**
   * @brief Write a codec GPIO pin level, or the entire output byte with the one-argument overload.
   */
  void GPIO_digitalWrite(uint8_t i, uint8_t val);
  /**
   * @brief Write a codec GPIO pin level, or the entire output byte with the one-argument overload.
   */
  void GPIO_digitalWrite(uint8_t i);
  /**
   * @brief Return the codec GPIO input byte or, in the pin overload, one pin level.
   */
  uint16_t GPIO_digitalRead(void);
  /**
   * @brief Return the codec GPIO input byte or, in the pin overload, one pin level.
   */
  boolean GPIO_digitalRead(uint8_t i);
  /**
   * @brief Set codec GPIO pin i (0 through 7) to INPUT or OUTPUT.
   */
  void GPIO_pinMode(uint8_t i, uint8_t dir);
 
  /**
   * @brief Load an Ogg recording plugin; return true only when its entry address is 0x34.
   */
  boolean prepareRecordOgg(char *plugin);
  /**
   * @brief Start Ogg recording; mic selects microphone input when true, line input otherwise.
   */
  void startRecordOgg(boolean mic);
  /**
   * @brief Request recording stop through the codec control register.
   */
  void stopRecordOgg(void);
  /**
   * @brief Return the number of recorded 16-bit words waiting in the codec.
   */
  uint16_t recordedWordsWaiting(void);
  /**
   * @brief Read one recorded 16-bit word from the codec.
   */
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

  /**
   * @brief Audio transfer scratch buffer owned by the codec.
   */
  uint8_t mp3buffer[DATABUFFERLEN];

 protected:
  uint8_t  _dreq;
 private:
  SPI &_spi;
  const int8_t _reset, _cs, _dcs;
};


/**
 * @brief File-backed VS1053 playback with data-ready interrupt feeding.
 * Requires NOLA_FILE_SUPPORTED, an interrupt-capable DREQ GPIO, and task dispatch.
 */
class Adafruit_VS1053_FilePlayer : public Adafruit_VS1053 {
 public:
  /**
   * @brief Bind the SPI bus and codec pins; the overload without rst uses no reset GPIO.
   */
  Adafruit_VS1053_FilePlayer (SPI &spi, int8_t rst, int8_t cs, int8_t dcs, int8_t dreq);
  /**
   * @brief Bind the SPI bus and codec pins; the overload without rst uses no reset GPIO.
   */
  Adafruit_VS1053_FilePlayer (SPI &spi, int8_t cs, int8_t dcs, int8_t dreq);

  /**
   * @brief Initialize codec GPIO/SPI and reset it.
   * The codec base returns the version nibble; FilePlayer returns true only for
   * version 4 and successful attachment of the DREQ change interrupt.
   */
  boolean begin(void);
  /**
   * @brief Current owned playback file handle; nullptr when stopped.
   */
  FILE *currentTrack = NULL;
  /**
   * @brief Playback feeding flag; use paused()/stopped() to distinguish inactive states.
   */
  volatile boolean playingMusic;
  /**
   * @brief Feed available file bytes to the codec while playing; also used by the DREQ interrupt handler.
   */
  void feedBuffer(void);
  /**
   * @brief Open trackname and begin background feeding.
   * @param trackname File path passed to fopen().
   * @param func Optional completion callback posted when playback reaches the end.
   * @return False if the file cannot be opened, true after playback is started.
   */
  boolean startPlayingFile(const char *trackname, void (*func)() = NULL);
  /**
   * @brief Start trackname and block until playback finishes; currently returns true even if opening fails.
   */
  boolean playFullFile(const char *trackname);
  /**
   * @brief Stop playback and close the current file.
   */
  void stopPlaying(void);
  /**
   * @brief Return true when a file is open but playingMusic is false.
   */
  boolean paused(void);
  /**
   * @brief Return true when no file is open and playingMusic is false.
   */
  boolean stopped(void);
  /**
   * @brief Pause feeding when pause is true; resume feeding when false.
   */
  void pausePlaying(boolean pause);

 private:
  void feedBuffer_noLock(void);
  volatile boolean feedBufferLock = false;
  void (*callbackPlayDone)() = NULL;
  static void TaskFeeder(void *);
  static void TaskCallbackPlayDone(void *);
};

#endif // NOLA_FILE_SUPPORTED
#endif // ADAFRUIT_VS1053_HPP
