/**
 * @file PCF8574.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
#pragma once
//
//    FILE: PCF8574.h
//  AUTHOR: Rob Tillaart
//    DATE: 02-febr-2013
// VERSION: 0.4.2
// PURPOSE: Arduino library for PCF8574 - 8 channel I2C IO expander
//     URL: https://github.com/RobTillaart/PCF8574
//          http://forum.arduino.cc/index.php?topic=184800


#include "TwoWire.hpp"


/** @brief Upstream PCF8574 library version string. */
#define PCF8574_LIB_VERSION         (F("0.4.2"))

#ifndef PCF8574_INITIAL_VALUE
/** @brief Default output latch byte, with all pins released high. */
#define PCF8574_INITIAL_VALUE       0xFF
#endif

/** @brief No stored PCF8574 error. */
#define PCF8574_OK                  0x00
/** @brief Invalid PCF8574 pin index. */
#define PCF8574_PIN_ERROR           0x81
/** @brief PCF8574 I2C read failure. */
#define PCF8574_I2C_ERROR           0x82


/**
 * @brief PCF8574 eight-pin quasi-bidirectional I2C expander.
 * Call begin() first. Release input pins by writing 1; check lastError() after I/O.
 */
class PCF8574
{
public:
  /**
   * @brief Bind the bus and 7-bit device address (default 0x20).
   */
  PCF8574(TwoWire &wire, const uint8_t deviceAddress = 0x20);

  /**
   * @brief Probe the address and set the output latch to value. Initialize the I2C bus first.
   * @return True if the device acknowledges its address.
   */
  bool    begin(uint8_t value = PCF8574_INITIAL_VALUE);
  /**
   * @brief Probe the device address and return whether it acknowledges.
   */
  bool    isConnected();


  //  note: setting the address corrupt internal buffer values
  //  a read8() / write8() call updates them.
  /**
   * @brief Change the host-side 7-bit address and probe it; does not reprogram hardware.
   * Refresh cached inputs/outputs with read8()/write8() after changing address.
   */
  bool    setAddress(const uint8_t deviceAddress);
  /**
   * @brief Return the configured host-side 7-bit address.
   */
  uint8_t getAddress() const { return _address; }

  /**
   * @brief Read all eight input levels and cache them; inspect lastError() for I2C failure.
   */
  uint8_t read8();
  /**
   * @brief Read pin 0 through 7 as 0/1; invalid pins set PCF8574_PIN_ERROR.
   */
  uint8_t read(const uint8_t pin);
  /**
   * @brief Return the last cached input byte without bus access.
   */
  uint8_t value() const { return _dataIn; };


  /**
   * @brief Write and cache all eight output latch bits.
   */
  void    write8(const uint8_t value);
  /**
   * @brief Update one output latch bit for pin 0 through 7.
   */
  void    write(const uint8_t pin, const uint8_t value);
  /**
   * @brief Return the cached output latch byte without bus access.
   */
  uint8_t valueOut() const { return _dataOut; }


  //  added 0.1.07/08 Septillion
  /**
   * @brief Read button inputs using the configured button mask.
   */
  uint8_t readButton8() { return PCF8574::readButton8(_buttonMask); }
  /**
   * @brief Temporarily release the pins in mask, read inputs, and restore the output latch.
   */
  uint8_t readButton8(const uint8_t mask);
  /**
   * @brief Release and read one button pin, then restore its output latch state.
   */
  uint8_t readButton(const uint8_t pin);
  /**
   * @brief Cache the mask of pins released by the no-argument readButton8().
   */
  void    setButtonMask(const uint8_t mask) { _buttonMask = mask; };
  /**
   * @brief Return the configured button mask.
   */
  uint8_t getButtonMask() const { return _buttonMask; };


  //  rotate, shift, toggle, reverse expect all lines are output
  /**
   * @brief Invert one cached output bit and write the latch; pin must be 0 through 7.
   */
  void    toggle(const uint8_t pin);
  //      default 0xFF ==> invertAll()
  /**
   * @brief Invert output latch bits selected by mask; default inverts all eight.
   */
  void    toggleMask(const uint8_t mask = 0xFF);
  /**
   * @brief Shift the output latch right by n bits, filling with zeros.
   */
  void    shiftRight(const uint8_t n = 1);
  /**
   * @brief Shift the output latch left by n bits, filling with zeros.
   */
  void    shiftLeft(const uint8_t n = 1);
  /**
   * @brief Rotate the eight output bits right by n positions.
   */
  void    rotateRight(const uint8_t n = 1);
  /**
   * @brief Rotate the eight output bits left by n positions.
   */
  void    rotateLeft(const uint8_t n = 1);
  /**
   * @brief Reverse the order of the eight output bits and write the latch.
   */
  void    reverse();


  /**
   * @brief Set only the selected output pin high; pin must be 0 through 7.
   */
  void    select(const uint8_t pin);
  /**
   * @brief Write the mask (2 << pin) - 1 for pin 0 through 7; other values write 0xFF.
   */
  void    selectN(const uint8_t pin);
  /**
   * @brief Drive all eight output latch bits low.
   */
  void    selectNone() { write8(0x00); };
  /**
   * @brief Release all eight pins by writing all output latch bits high.
   */
  void    selectAll()  { write8(0xFF); };


  /**
   * @brief Return and clear the stored error (PCF8574_OK means no error).
   */
  int     lastError();


private:
  int     _error {PCF8574_OK};
  uint8_t _address;
  uint8_t _dataIn {0};
  uint8_t _dataOut {0xFF};
  uint8_t _buttonMask {0xFF};


  TwoWire*  _wire;
};


//  -- END OF FILE --


/** @} */
