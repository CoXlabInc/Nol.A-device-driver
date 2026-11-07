/**
 * @file TM1637Display.hpp
 * @brief Public driver declarations and configuration constants.
 */
#ifndef __TM1637DISPLAY__
#define __TM1637DISPLAY__

#include <inttypes.h>

/**
 * @brief GPIO-driven TM1637 segment display. Call setBrightness() before the first update.
 * Buffered brightness is applied
 * on the next setSegments() or number display operation.
 */
class TM1637Display {
public:
  /**
   * @brief Cache display brightness and enable state.
   * @param brightness Brightness level 0 through 7 (only low three bits are used).
   * @param on True enables the display; false switches it off on the next update.
   */
  void setBrightness(uint8_t brightness, bool on = true);
  /**
   * @brief Send raw segment bytes to the display.
   * @param segments Segment bitmap array, one byte per digit.
   * @param length Number of bytes to read from segments.
   * @param pos Starting digit position; caller must keep the range within the display.
   */
  void setSegments(const uint8_t segments[], uint8_t length = 4, uint8_t pos = 0);
  /**
   * @brief Display a signed decimal number.
   * @param num Number to format.
   * @param leading_zero Pad with zeros instead of blanks.
   * @param length Digit field width, 1 through 4.
   * @param pos Starting digit position within the four-digit display.
   */
  void showNumberDec(int num, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);
  /**
   * @brief Display a signed decimal number with decimal-point/colon bits.
   * @param num Number to format.
   * @param dots Dot mask; bits 7 through 4 correspond to the four displayed digits.
   * @param leading_zero Pad with zeros instead of blanks.
   * @param length Digit field width, 1 through 4.
   * @param pos Starting digit position; keep pos + length at most 4.
   */
  void showNumberDecEx(int num, uint8_t dots = 0, bool leading_zero = false, uint8_t length = 4, uint8_t pos = 0);

  /**
   * @brief Return the seven-segment bitmap for the low nibble of digit (hexadecimal 0-F).
   */
  uint8_t encodeDigit(uint8_t digit);

  /**
   * @brief Bind clock/data pins, configure released inputs, and clear output latches.
   * @param pinClk Clock GPIO.
   * @param pinDIO Bidirectional data GPIO.
   */
  TM1637Display(uint8_t pinClk, uint8_t pinDIO);

protected:
  void bitDelay();
  void start();
  void stop();

  bool writeByte(uint8_t b);

private:
  uint8_t m_pinClk;
  uint8_t m_pinDIO;
  uint8_t m_brightness;
};

#endif
