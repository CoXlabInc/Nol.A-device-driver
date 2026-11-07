/*!
 * @file Adafruit_SSD1306.hpp
 * @ingroup device_drivers
 *
 * This is part of for Adafruit's SSD1306 library for monochrome
 * OLED displays: http://www.adafruit.com/category/63_98
 *
 * These displays use I2C or SPI to communicate. I2C requires 2 pins
 * (SCL+SDA) and optionally a RESET pin. SPI requires 4 pins (MOSI, SCK,
 * select, data/command) and optionally a reset pin. Hardware SPI or
 * 'bitbang' software SPI are both supported.
 *
 * Adafruit invests time and resources providing this open source code,
 * please support Adafruit and open-source hardware by purchasing
 * products from Adafruit!
 *
 * Written by Limor Fried/Ladyada for Adafruit Industries, with
 * contributions from the open source community.
 *
 * BSD license, all text above, and the splash screen header file,
 * must be included in any redistribution.
 *
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */

#pragma once

// ONE of the following three lines must be #defined:
//#define SSD1306_128_64 ///< DEPRECTAED: old way to specify 128x64 screen
#define SSD1306_128_32   ///< DEPRECATED: old way to specify 128x32 screen
//#define SSD1306_96_16  ///< DEPRECATED: old way to specify 96x16 screen
// This establishes the screen dimensions in old Adafruit_SSD1306 sketches
// (NEW CODE SHOULD IGNORE THIS, USE THE CONSTRUCTORS THAT ACCEPT WIDTH
// AND HEIGHT ARGUMENTS).

#if defined(ARDUINO_STM32_FEATHER)
  typedef class HardwareSPI SPIClass;
#endif

#include <TwoWire.hpp>
#include <SPI.hpp>
#include <Adafruit_GFX.hpp>

#if defined(__AVR__)
  typedef volatile uint8_t  PortReg;
  typedef uint8_t           PortMask;
  #define HAVE_PORTREG
#elif defined(__SAM3X8E__)
  typedef volatile RwReg    PortReg;
  typedef uint32_t          PortMask;
  #define HAVE_PORTREG
#elif defined(__arm__) || defined(ARDUINO_FEATHER52)
  typedef volatile uint32_t PortReg;
  typedef uint32_t          PortMask;
  #define HAVE_PORTREG
#endif

typedef class SPI SPIClass;
#undef HAVE_PORTREG

#define BLACK                          0 ///< Draw 'off' pixels
#define WHITE                          1 ///< Draw 'on' pixels
#define INVERSE                        2 ///< Invert pixels

#define SSD1306_MEMORYMODE          0x20 ///< See datasheet
#define SSD1306_COLUMNADDR          0x21 ///< See datasheet
#define SSD1306_PAGEADDR            0x22 ///< See datasheet
#define SSD1306_SETCONTRAST         0x81 ///< See datasheet
#define SSD1306_CHARGEPUMP          0x8D ///< See datasheet
#define SSD1306_SEGREMAP            0xA0 ///< See datasheet
#define SSD1306_DISPLAYALLON_RESUME 0xA4 ///< See datasheet
#define SSD1306_DISPLAYALLON        0xA5 ///< Not currently used
#define SSD1306_NORMALDISPLAY       0xA6 ///< See datasheet
#define SSD1306_INVERTDISPLAY       0xA7 ///< See datasheet
#define SSD1306_SETMULTIPLEX        0xA8 ///< See datasheet
#define SSD1306_DISPLAYOFF          0xAE ///< See datasheet
#define SSD1306_DISPLAYON           0xAF ///< See datasheet
#define SSD1306_COMSCANINC          0xC0 ///< Not currently used
#define SSD1306_COMSCANDEC          0xC8 ///< See datasheet
#define SSD1306_SETDISPLAYOFFSET    0xD3 ///< See datasheet
#define SSD1306_SETDISPLAYCLOCKDIV  0xD5 ///< See datasheet
#define SSD1306_SETPRECHARGE        0xD9 ///< See datasheet
#define SSD1306_SETCOMPINS          0xDA ///< See datasheet
#define SSD1306_SETVCOMDETECT       0xDB ///< See datasheet

#define SSD1306_SETLOWCOLUMN        0x00 ///< Not currently used
#define SSD1306_SETHIGHCOLUMN       0x10 ///< Not currently used
#define SSD1306_SETSTARTLINE        0x40 ///< See datasheet

#define SSD1306_EXTERNALVCC         0x01 ///< External display voltage source
#define SSD1306_SWITCHCAPVCC        0x02 ///< Gen. display voltage from 3.3V

#define SSD1306_RIGHT_HORIZONTAL_SCROLL              0x26 ///< Init rt scroll
#define SSD1306_LEFT_HORIZONTAL_SCROLL               0x27 ///< Init left scroll
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29 ///< Init diag scroll
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL  0x2A ///< Init diag scroll
#define SSD1306_DEACTIVATE_SCROLL                    0x2E ///< Stop scroll
#define SSD1306_ACTIVATE_SCROLL                      0x2F ///< Start scroll
#define SSD1306_SET_VERTICAL_SCROLL_AREA             0xA3 ///< Set scroll range

// Deprecated size stuff for backwards compatibility with old sketches
#if defined SSD1306_128_64
 #define SSD1306_LCDWIDTH  128 ///< DEPRECATED: width w/SSD1306_128_64 defined
 #define SSD1306_LCDHEIGHT  64 ///< DEPRECATED: height w/SSD1306_128_64 defined
#endif
#if defined SSD1306_128_32
 #define SSD1306_LCDWIDTH  128 ///< DEPRECATED: width w/SSD1306_128_32 defined
 #define SSD1306_LCDHEIGHT  32 ///< DEPRECATED: height w/SSD1306_128_32 defined
#endif
#if defined SSD1306_96_16
 #define SSD1306_LCDWIDTH   96 ///< DEPRECATED: width w/SSD1306_96_16 defined
 #define SSD1306_LCDHEIGHT  16 ///< DEPRECATED: height w/SSD1306_96_16 defined
#endif

/*! 
    @brief  Class that stores state and functions for interacting with
            SSD1306 OLED displays.
*/
class Adafruit_SSD1306 : public Adafruit_GFX {
 public:
  // NEW CONSTRUCTORS -- recommended for new projects
  /**
   * @brief Bind an SSD1306 display without allocating its framebuffer.
   * Width/height are pixels; constructors select I2C, software SPI, or hardware SPI.
   * Bus pointers are borrowed. Reset pin -1 omits GPIO reset; I2C clocks and SPI
   * bitrate are in hertz. Call begin() before drawing, then display() to send changes.
   * Legacy constructors without dimensions are declared but not implemented in this port.
   */
  Adafruit_SSD1306(uint8_t w, uint8_t h, TwoWire *twi, int8_t rst_pin=-1,
    uint32_t clkDuring=400000UL, uint32_t clkAfter=100000UL);
  /**
   * @brief Bind an SSD1306 display without allocating its framebuffer.
   * Width/height are pixels; constructors select I2C, software SPI, or hardware SPI.
   * Bus pointers are borrowed. Reset pin -1 omits GPIO reset; I2C clocks and SPI
   * bitrate are in hertz. Call begin() before drawing, then display() to send changes.
   * Legacy constructors without dimensions are declared but not implemented in this port.
   */
  Adafruit_SSD1306(uint8_t w, uint8_t h, int8_t mosi_pin, int8_t sclk_pin,
    int8_t dc_pin, int8_t rst_pin, int8_t cs_pin);
  /**
   * @brief Bind an SSD1306 display without allocating its framebuffer.
   * Width/height are pixels; constructors select I2C, software SPI, or hardware SPI.
   * Bus pointers are borrowed. Reset pin -1 omits GPIO reset; I2C clocks and SPI
   * bitrate are in hertz. Call begin() before drawing, then display() to send changes.
   * Legacy constructors without dimensions are declared but not implemented in this port.
   */
  Adafruit_SSD1306(uint8_t w, uint8_t h, SPIClass *spi,
    int8_t dc_pin, int8_t rst_pin, int8_t cs_pin, uint32_t bitrate=8000000UL);

  // DEPRECATED CONSTRUCTORS - for back compatibility, avoid in new projects
  /**
   * @brief Bind an SSD1306 display without allocating its framebuffer.
   * Width/height are pixels; constructors select I2C, software SPI, or hardware SPI.
   * Bus pointers are borrowed. Reset pin -1 omits GPIO reset; I2C clocks and SPI
   * bitrate are in hertz. Call begin() before drawing, then display() to send changes.
   * Legacy constructors without dimensions are declared but not implemented in this port.
   */
  Adafruit_SSD1306(int8_t mosi_pin, int8_t sclk_pin, int8_t dc_pin,
    int8_t rst_pin, int8_t cs_pin);
  /**
   * @brief Bind an SSD1306 display without allocating its framebuffer.
   * Width/height are pixels; constructors select I2C, software SPI, or hardware SPI.
   * Bus pointers are borrowed. Reset pin -1 omits GPIO reset; I2C clocks and SPI
   * bitrate are in hertz. Call begin() before drawing, then display() to send changes.
   * Legacy constructors without dimensions are declared but not implemented in this port.
   */
  Adafruit_SSD1306(int8_t dc_pin, int8_t rst_pin, int8_t cs_pin);
  /**
   * @brief Bind an SSD1306 display without allocating its framebuffer.
   * Width/height are pixels; constructors select I2C, software SPI, or hardware SPI.
   * Bus pointers are borrowed. Reset pin -1 omits GPIO reset; I2C clocks and SPI
   * bitrate are in hertz. Call begin() before drawing, then display() to send changes.
   * Legacy constructors without dimensions are declared but not implemented in this port.
   */
  Adafruit_SSD1306(int8_t rst_pin = -1);

  /**
   * @brief Release the owned framebuffer.
   */
  ~Adafruit_SSD1306(void);

  /**
   * @brief Allocate the framebuffer and initialize the controller.
   * @param switchvcc SSD1306_SWITCHCAPVCC or SSD1306_EXTERNALVCC.
   * @param i2caddr 7-bit address; zero selects the display-height-dependent default.
   * @param reset Pulse the configured reset pin if available.
   * @param periphBegin Whether to initialize the selected peripheral.
   * @return False on framebuffer allocation failure, otherwise true; no I2C probe is performed.
   */
  boolean      begin(uint8_t switchvcc=SSD1306_SWITCHCAPVCC,
                 uint8_t i2caddr=0, boolean reset=true,
                 boolean periphBegin=true);
  /**
   * @brief Transmit the complete framebuffer to the panel synchronously.
   */
  virtual void display(void);
  /**
   * @brief Clear the framebuffer without transmitting it.
   */
  void         clearDisplay(void);
  /**
   * @brief Set hardware inversion when i is true without modifying the framebuffer.
   */
  void         invertDisplay(boolean i);
  /**
   * @brief Use the dimmed contrast setting when dim is true.
   */
  void         dim(boolean dim);
  /**
   * @brief Draw a buffered pixel at x/y using SSD1306_BLACK, SSD1306_WHITE, or SSD1306_INVERSE.
   */
  void         drawPixel(int16_t x, int16_t y, uint16_t color);
  /**
   * @brief Draw a clipped horizontal framebuffer line of w pixels using the current rotation.
   */
  virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  /**
   * @brief Draw a clipped vertical framebuffer line of h pixels using the current rotation.
   */
  virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  /**
   * @brief Start rightward hardware scrolling between inclusive page indices start and stop.
   */
  void         startscrollright(uint8_t start, uint8_t stop);
  /**
   * @brief Start leftward hardware scrolling between inclusive page indices start and stop.
   */
  void         startscrollleft(uint8_t start, uint8_t stop);
  /**
   * @brief Start diagonal rightward scrolling over inclusive page indices start and stop.
   */
  void         startscrolldiagright(uint8_t start, uint8_t stop);
  /**
   * @brief Start diagonal leftward scrolling over inclusive page indices start and stop.
   */
  void         startscrolldiagleft(uint8_t start, uint8_t stop);
  /**
   * @brief Stop hardware scrolling.
   */
  void         stopscroll(void);
  /**
   * @brief Send one raw command byte to the controller.
   */
  void         ssd1306_command(uint8_t c);
  /**
   * @brief Return the buffered pixel state at x/y; return false outside the rotated bounds.
   */
  boolean      getPixel(int16_t x, int16_t y);
  /**
   * @brief Return the borrowed page-organized framebuffer; it is owned by this object.
   */
  uint8_t     *getBuffer(void);

 private:
  inline void  SPIwrite(uint8_t d) __attribute__((always_inline));
  void         drawFastHLineInternal(int16_t x, int16_t y, int16_t w,
                 uint16_t color);
  void         drawFastVLineInternal(int16_t x, int16_t y, int16_t h,
                 uint16_t color);
  void         ssd1306_command1(uint8_t c);
  void         ssd1306_commandList(const uint8_t *c, uint8_t n);

  SPIClass    *spi;
  TwoWire     *wire;
  uint8_t     *buffer;
  int8_t       i2caddr, vccstate, page_end;
  int8_t       mosiPin    ,  clkPin    ,  dcPin    ,  csPin, rstPin;
#ifdef HAVE_PORTREG
  PortReg     *mosiPort   , *clkPort   , *dcPort   , *csPort;
  PortMask     mosiPinMask,  clkPinMask,  dcPinMask,  csPinMask;
#endif
#if defined(SPI_HAS_TRANSACTION)
  SPISettings  spiSettings;
#endif

  uint32_t     wireClk;    // Wire speed for SSD1306 transfers
  uint32_t     restoreClk; // Wire speed following SSD1306 transfers

};

/** @} */
