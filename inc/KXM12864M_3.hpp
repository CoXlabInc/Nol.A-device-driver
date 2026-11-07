/**
 * @file KXM12864M_3.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
#pragma once

#include "ST7920.hpp"

/**
 * @brief 128x64 ST7920 panel with the KXM12864M-3 split GDRAM layout.
 * Initialize and draw through ST7920, then call invalidate().
 */
class KXM12864M_3 : public ST7920 {
public:
  /**
   * @brief Bind the SPI bus and active-high chip-select GPIO.
   */
  KXM12864M_3(SPI &, int8_t CS);
  /**
   * @brief Send the framebuffer with upper/lower halves mapped to panel GDRAM.
   */
  void invalidate() override;
};

/** @} */
