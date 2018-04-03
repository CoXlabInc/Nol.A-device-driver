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

#include <Adafruit_VS1053.hpp>

#ifndef _BV
  #define _BV(x) (1<<(x))
#endif

#define VS1053_CONTROL_SPI_SETTING  250000,  SPI::MSBFIRST, SPI::MODE0
#define VS1053_DATA_SPI_SETTING     8000000, SPI::MSBFIRST, SPI::MODE0

Adafruit_VS1053_FilePlayer::Adafruit_VS1053_FilePlayer(SPI &spi, int8_t rst, int8_t cs, int8_t dcs, int8_t dreq) 
  : Adafruit_VS1053(spi, rst, cs, dcs, dreq), playingMusic(false) {
}

Adafruit_VS1053_FilePlayer::Adafruit_VS1053_FilePlayer(SPI &spi, int8_t cs, int8_t dcs, int8_t dreq) 
  : Adafruit_VS1053(spi, -1, cs, dcs, dreq), playingMusic(false) {
}


boolean Adafruit_VS1053_FilePlayer::begin(void) {
  // Set the card to be disabled while we get the VS1053 up
  uint8_t v  = Adafruit_VS1053::begin();   

  //dumpRegs();
  //Serial.print("Version = "); Serial.println(v);
  return (v == 4);
}


boolean Adafruit_VS1053_FilePlayer::playFullFile(const char *trackname) {
  if (! startPlayingFile(trackname)) return false;

  while (playingMusic) {
    // twiddle thumbs
    feedBuffer();
    delay(5);           // give IRQs a chance
  }
  // music file finished!
  return true;
}

void Adafruit_VS1053_FilePlayer::stopPlaying(void) {
  // cancel all playback
  sciWrite(REG_MODE, MODE_SM_LINE1 | MODE_SM_SDINEW | MODE_SM_CANCEL);
  
  // wrap it up!
  playingMusic = false;
  fclose(currentTrack);
  currentTrack = NULL;
}

void Adafruit_VS1053_FilePlayer::pausePlaying(boolean pause) {
  if (pause) 
    playingMusic = false;
  else {
    playingMusic = true;
    feedBuffer();
  }
}

boolean Adafruit_VS1053_FilePlayer::paused(void) {
  return (!playingMusic && currentTrack);
}

boolean Adafruit_VS1053_FilePlayer::stopped(void) {
  return (!playingMusic && !currentTrack);
}


boolean Adafruit_VS1053_FilePlayer::startPlayingFile(const char *trackname) {
  // reset playback
  sciWrite(REG_MODE, MODE_SM_LINE1 | MODE_SM_SDINEW);
  // resync
  sciWrite(REG_WRAMADDR, 0x1e29);
  sciWrite(REG_WRAM, 0);

  currentTrack = fopen(trackname, "r");
  if (!currentTrack) {
    return false;
  }

  // don't let the IRQ get triggered by accident here
  noInterrupts();

  // As explained in datasheet, set twice 0 in REG_DECODETIME to set time back to 0
  sciWrite(REG_DECODETIME, 0x00);
  sciWrite(REG_DECODETIME, 0x00);

  playingMusic = true;

  // wait till its ready for data
  while (! readyForData() ) {
#if defined(ESP8266)
	yield();
#endif
  }

  // fill it up!
  while (playingMusic && readyForData()) {
    feedBuffer();
  }
  
  // ok going forward, we can use the IRQ
  interrupts();

  return true;
}

void Adafruit_VS1053_FilePlayer::feedBuffer(void) {
  noInterrupts();
  // dont run twice in case interrupts collided
  // This isn't a perfect lock as it may lose one feedBuffer request if
  // an interrupt occurs before feedBufferLock is reset to false. This
  // may cause a glitch in the audio but at least it will not corrupt
  // state.
  if (feedBufferLock) {
    interrupts();
    return;
  }
  feedBufferLock = true;
  interrupts();

  feedBuffer_noLock();

  feedBufferLock = false;
}

void Adafruit_VS1053_FilePlayer::feedBuffer_noLock(void) {
  if ((! playingMusic) // paused or stopped
      || (! currentTrack) 
      || (! readyForData())) {
    return; // paused or stopped
  }

  // Feed the hungry buffer! :)
  while (readyForData()) {
    // Read some audio data from the SD card file
    int bytesread = fread(mp3buffer, DATABUFFERLEN, DATABUFFERLEN, currentTrack);
    
    if (bytesread == 0) {
      // must be at the end of the file, wrap it up!
      playingMusic = false;
      fclose(currentTrack);
      currentTrack = NULL;
      break;
    }

    playData(mp3buffer, bytesread);
  }
}


/***************************************************************/

Adafruit_VS1053::Adafruit_VS1053(SPI &s, int8_t rst, int8_t cs, int8_t dcs, int8_t dreq) : _dreq(dreq), _spi(s), _reset(rst), _cs(cs), _dcs(dcs) {
}


void Adafruit_VS1053::applyPatch(const uint16_t *patch, uint16_t patchsize) {
  uint16_t i = 0;

 // Serial.print("Patch size: "); Serial.println(patchsize);
  while ( i < patchsize ) {
    uint16_t addr, n, val;

    addr = *patch; patch++; //pgm_read_word(patch++);
    n = *patch; patch++; //pgm_read_word(patch++);
    i += 2;

    //Serial.println(addr, HEX);
    if (n & 0x8000U) { // RLE run, replicate n samples 
      n &= 0x7FFF;
      val = *patch; patch++; //pgm_read_word(patch++);
      i++;
      while (n--) {
	sciWrite(addr, val);
      }      
    } else {           // Copy run, copy n samples 
      while (n--) {
	val = *patch; patch++; //pgm_read_word(patch++);
	i++;
	sciWrite(addr, val);
      }
    }
  }
}


uint16_t Adafruit_VS1053::loadPlugin(char *plugname) {

  FILE *plugin = fopen(plugname, "r");
  if (!plugin) {
    Serial.printf("Couldn't open the plugin file '%s'.\n", plugname);
    return 0xFFFF;
  }

  char c;
  if ((fread(&c, 1, 1, plugin) != 1 || c != 'P') ||
      (fread(&c, 1, 1, plugin) != 1 || c != '&') ||
      (fread(&c, 1, 1, plugin) != 1 || c != 'H')) {
    fclose(plugin);
    return 0xFFFF;
  }

  uint16_t type;

 // Serial.print("Patch size: "); Serial.println(patchsize);
  while (fread(&c, 1, 1, plugin) == 1 && (type = c) >= 0) {
    uint16_t offsets[] = {0x8000UL, 0x0, 0x4000UL};
    uint16_t addr, len;

    //Serial.print("type: "); Serial.println(type, HEX);

    if (type >= 4) {
        fclose(plugin);
	return 0xFFFF;
    }

    fread(&c, 1, 1, plugin); len = ((uint16_t) c << 8);
    fread(&c, 1, 1, plugin); len |= c & ~1;
    fread(&c, 1, 1, plugin); addr = ((uint16_t) c << 8);
    fread(&c, 1, 1, plugin); addr |= c;
    //Serial.print("len: "); Serial.print(len); 
    //Serial.print(" addr: $"); Serial.println(addr, HEX);

    if (type == 3) {
      // execute rec!
      fclose(plugin);
      return addr;
    }

    // set address
    sciWrite(REG_WRAMADDR, addr + offsets[type]);
    // write data
    do {
      uint16_t data;
      fread(&c, 1, 1, plugin); data = c; data <<= 8;
      fread(&c, 1, 1, plugin); data |= c;
      sciWrite(REG_WRAM, data);
    } while ((len -=2));
  }

  fclose(plugin);
  return 0xFFFF;
}




boolean Adafruit_VS1053::readyForData(void) {
  return digitalRead(_dreq);
}

void Adafruit_VS1053::playData(uint8_t *buffer, uint8_t buffsiz) {
  this->_spi.begin(VS1053_DATA_SPI_SETTING);
  digitalWrite(_dcs, LOW);
  
  spiwrite(buffer, buffsiz);

  digitalWrite(_dcs, HIGH);
  this->_spi.end();
}

void Adafruit_VS1053::setVolume(uint8_t left, uint8_t right) {
  uint16_t v;
  v = left;
  v <<= 8;
  v |= right;

  noInterrupts(); //cli();
  sciWrite(REG_VOLUME, v);
  interrupts();  //sei();
}

uint16_t Adafruit_VS1053::decodeTime() {
  noInterrupts(); //cli();
  uint16_t t = sciRead(REG_DECODETIME);
  interrupts(); //sei();
  return t;
}


void Adafruit_VS1053::softReset(void) {
  sciWrite(REG_MODE, MODE_SM_SDINEW | MODE_SM_RESET);
  delay(100);
}

void Adafruit_VS1053::reset() {
  // TODO: http://www.vlsi.fi/player_vs1011_1002_1003/modularplayer/vs10xx_8c.html#a3
  // hardware reset
  if (_reset >= 0) {
    digitalWrite(_reset, LOW);
    delay(100);
    digitalWrite(_reset, HIGH);
  }
  digitalWrite(_cs, HIGH);
  digitalWrite(_dcs, HIGH);
  delay(100);
  softReset();
  delay(100);

  sciWrite(REG_CLOCKF, 0x6000);

  setVolume(40, 40);
}

uint8_t Adafruit_VS1053::begin(void) {
  if (_reset >= 0) {
    pinMode(_reset, OUTPUT);
    digitalWrite(_reset, LOW);
  }

  pinMode(_cs, OUTPUT);
  digitalWrite(_cs, HIGH);
  pinMode(_dcs, OUTPUT);
  digitalWrite(_dcs, HIGH);
  pinMode(_dreq, INPUT);

  reset();

  return (sciRead(REG_STATUS) >> 4) & 0x0F;
}

void Adafruit_VS1053::dumpRegs(void) {
  Serial.print("Mode = 0x"); Serial.println(sciRead(REG_MODE), HEX);
  Serial.print("Stat = 0x"); Serial.println(sciRead(REG_STATUS), HEX);
  Serial.print("ClkF = 0x"); Serial.println(sciRead(REG_CLOCKF), HEX);
  Serial.print("Vol. = 0x"); Serial.println(sciRead(REG_VOLUME), HEX);
}


uint16_t Adafruit_VS1053::recordedWordsWaiting(void) {
  return sciRead(REG_HDAT1);
}

uint16_t Adafruit_VS1053::recordedReadWord(void) {
  return sciRead(REG_HDAT0);
}


boolean Adafruit_VS1053::prepareRecordOgg(char *plugname) {
  sciWrite(REG_CLOCKF, 0xC000);  // set max clock
  delay(1);    while (! readyForData() );

  sciWrite(REG_BASS, 0);  // clear Bass
  
  softReset();
  delay(1);    while (! readyForData() );

  sciWrite(SCI_AIADDR, 0);
  // disable all interrupts except SCI
  sciWrite(REG_WRAMADDR, INT_ENABLE);
  sciWrite(REG_WRAM, 0x02);

  int pluginStartAddr = loadPlugin(plugname);
  if (pluginStartAddr == 0xFFFF) return false;
  Serial.print("Plugin at $"); Serial.println(pluginStartAddr, HEX);
  if (pluginStartAddr != 0x34) return false;

  return true;
}

void Adafruit_VS1053::stopRecordOgg(void) {
  sciWrite(SCI_AICTRL3, 1);
}

void Adafruit_VS1053::startRecordOgg(boolean mic) {
  /* Set VS1053 mode bits as instructed in the VS1053b Ogg Vorbis Encoder
     manual. Note: for microphone input, leave SMF_LINE1 unset! */
  if (mic) {
    sciWrite(REG_MODE, MODE_SM_ADPCM | MODE_SM_SDINEW);
  } else {
    sciWrite(REG_MODE, MODE_SM_LINE1 | 
	     MODE_SM_ADPCM | MODE_SM_SDINEW);
  }
  sciWrite(SCI_AICTRL0, 1024);
  /* Rec level: 1024 = 1. If 0, use AGC */
  sciWrite(SCI_AICTRL1, 1024);
  /* Maximum AGC level: 1024 = 1. Only used if SCI_AICTRL1 is set to 0. */
  sciWrite(SCI_AICTRL2, 0);
  /* Miscellaneous bits that also must be set before recording. */
  sciWrite(SCI_AICTRL3, 0);
  
  sciWrite(SCI_AIADDR, 0x34);
  delay(1);    while (! readyForData() );
}

void Adafruit_VS1053::GPIO_pinMode(uint8_t i, uint8_t dir) {
  if (i > 7) return;

  sciWrite(REG_WRAMADDR, GPIO_DDR);
  uint16_t ddr = sciRead(REG_WRAM);

  if (dir == INPUT)
    ddr &= ~_BV(i);
  if (dir == OUTPUT)
    ddr |= _BV(i);

  sciWrite(REG_WRAMADDR, GPIO_DDR);
  sciWrite(REG_WRAM, ddr);
}


void Adafruit_VS1053::GPIO_digitalWrite(uint8_t val) {
  sciWrite(REG_WRAMADDR, GPIO_ODATA);
  sciWrite(REG_WRAM, val);
}

void Adafruit_VS1053::GPIO_digitalWrite(uint8_t i, uint8_t val) {
  if (i > 7) return;

  sciWrite(REG_WRAMADDR, GPIO_ODATA);
  uint16_t pins = sciRead(REG_WRAM);

  if (val == LOW)
    pins &= ~_BV(i);
  if (val == HIGH)
    pins |= _BV(i);

  sciWrite(REG_WRAMADDR, GPIO_ODATA);
  sciWrite(REG_WRAM, pins);
}

uint16_t Adafruit_VS1053::GPIO_digitalRead(void) {
  sciWrite(REG_WRAMADDR, GPIO_IDATA);
  return sciRead(REG_WRAM) & 0xFF;
}

boolean Adafruit_VS1053::GPIO_digitalRead(uint8_t i) {
  if (i > 7) return 0;

  sciWrite(REG_WRAMADDR, GPIO_IDATA);
  uint16_t val = sciRead(REG_WRAM);
  if (val & _BV(i)) return true;
  return false;
}

uint16_t Adafruit_VS1053::sciRead(uint8_t addr) {
  uint16_t data;

  this->_spi.begin(VS1053_CONTROL_SPI_SETTING);
  digitalWrite(_cs, LOW);  
  spiwrite(SCI_READ);
  spiwrite(addr);
  delayMicroseconds(10);
  data = spiread();
  data <<= 8;
  data |= spiread();
  digitalWrite(_cs, HIGH);
  this->_spi.end();

  return data;
}


void Adafruit_VS1053::sciWrite(uint8_t addr, uint16_t data) {
  this->_spi.begin(VS1053_CONTROL_SPI_SETTING);
  digitalWrite(_cs, LOW);  
  spiwrite(SCI_WRITE);
  spiwrite(addr);
  spiwrite(data >> 8);
  spiwrite(data & 0xFF);
  digitalWrite(_cs, HIGH);
  this->_spi.end();
}



uint8_t Adafruit_VS1053::spiread(void)
{
  int8_t i, x;
  x = 0;

  // MSB first, clock low when inactive (CPOL 0), data valid on leading edge (CPHA 0)
  // Make sure clock starts low

  x = this->_spi.transfer(0x00);
  return x;
}

void Adafruit_VS1053::spiwrite(uint8_t c)
{

  uint8_t x __attribute__ ((aligned (32))) = c;
  spiwrite(&x, 1);
}


void Adafruit_VS1053::spiwrite(uint8_t *c, uint16_t num)
{
  // MSB first, clock low when inactive (CPOL 0), data valid on leading edge (CPHA 0)
  // Make sure clock starts low

  //#if defined(ESP32)  // optimized
  //  this->_spi.writeBytes(c, num);
  //  return;
  //#endif

  while (num--) {
    this->_spi.transfer(c[0]);
    c++;
  }
}



void Adafruit_VS1053::sineTest(uint8_t n, uint16_t ms) {
  reset();
  
  uint16_t mode = sciRead(REG_MODE);
  mode |= 0x0020;
  sciWrite(REG_MODE, mode);

  while (!digitalRead(_dreq));
	 //  delay(10);

  this->_spi.begin(VS1053_DATA_SPI_SETTING);
  digitalWrite(_dcs, LOW);  
  spiwrite(0x53);
  spiwrite(0xEF);
  spiwrite(0x6E);
  spiwrite(n);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  digitalWrite(_dcs, HIGH);  
  this->_spi.end();
  
  delay(ms);

  this->_spi.begin(VS1053_DATA_SPI_SETTING);
  digitalWrite(_dcs, LOW);  
  spiwrite(0x45);
  spiwrite(0x78);
  spiwrite(0x69);
  spiwrite(0x74);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  spiwrite(0x00);
  digitalWrite(_dcs, HIGH);  
  this->_spi.end();
}
