/**
 * @file ST7920.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
/*
  Imported from https://github.com/BornaBiro/ST7920_GFX_Library
*/
#pragma once

#include <SPI.hpp>
#include <Adafruit_GFX.hpp>

/**
 * @brief Buffered monochrome ST7920 SPI display. Call begin(), clearDisplay(),
 * draw with Adafruit_GFX methods, then invalidate() to update the panel.
 */
class ST7920 : public Adafruit_GFX {
public:
  /**
   * @brief Allocate a framebuffer and bind SPI/chip select.
   * @param width Width in pixels; must be a multiple of 16.
   * @param height Height in pixels.
   * @param s SPI bus that must outlive the display.
   * @param CS Active-high chip-select GPIO.
     */
  ST7920(uint16_t width, uint16_t height, SPI &, int8_t CS);

  /**
   * @brief Configure SPI and GPIO.
   * @return False if framebuffer allocation failed; true otherwise.
   */
  bool begin(void);
  /**
   * @brief Clear the framebuffer without sending it to the panel.
   */
  void clearDisplay(void);
  /**
   * @brief Toggle inversion and invert the framebuffer; call invalidate() to display it.
   */
  void invertDisplay(void);
  /**
   * @brief Return the current software inversion flag.
   */
  bool isInverted();
  /**
   * @brief Send the entire framebuffer to the panel synchronously.
   */
  virtual void invalidate();

  /**
   * @brief Set a buffered pixel at x/y; color 1 is black, 0 is white.
   * Invalid coordinates and other color values are ignored.
   */
  void drawPixel(int16_t x, int16_t y, uint16_t color);

  /**
   * @brief Compatibility no-op; backlight control is not implemented.
   */
  void backlight();
  /**
   * @brief Compatibility no-op; backlight control is not implemented.
   */
  void noBacklight();
  /**
   * @brief Compatibility no-op; cursor control is not implemented.
   */
  void cursor();
  /**
   * @brief Compatibility no-op; blink control is not implemented.
   */
  void blink();

protected:
  SPI &spi;
  int8_t cs;
  uint16_t *buff = nullptr;
  void ST7920Data(uint8_t data);
  void ST7920Command(uint8_t data);

private:
  bool _isInverted = false;
};

/** @} */
