/**
 * @file Adafruit_NeoPixel.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
/*--------------------------------------------------------------------
  This file is part of the Adafruit NeoPixel library.

  NeoPixel is free software: you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation, either version 3 of
  the License, or (at your option) any later version.

  NeoPixel is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with NeoPixel.  If not, see
  <http://www.gnu.org/licenses/>.

 - Ported v1.1.6 for Nol.A-SDK by Jongsoo Jeong (CoXlab Inc.)

  --------------------------------------------------------------------*/

#ifndef ADAFRUIT_NEOPIXEL_HPP
#define ADAFRUIT_NEOPIXEL_HPP

#include <cox.h>

// The order of primary colors in the NeoPixel data stream can vary
// among device types, manufacturers and even different revisions of
// the same item.  The third parameter to the Adafruit_NeoPixel
// constructor encodes the per-pixel byte offsets of the red, green
// and blue primaries (plus white, if present) in the data stream --
// the following #defines provide an easier-to-use named version for
// each permutation.  e.g. NEO_GRB indicates a NeoPixel-compatible
// device expecting three bytes per pixel, with the first byte
// containing the green value, second containing red and third
// containing blue.  The in-memory representation of a chain of
// NeoPixels is the same as the data-stream order; no re-ordering of
// bytes is required when issuing data to the chain.

// Bits 5,4 of this value are the offset (0-3) from the first byte of
// a pixel to the location of the red color byte.  Bits 3,2 are the
// green offset and 1,0 are the blue offset.  If it is an RGBW-type
// device (supporting a white primary in addition to R,G,B), bits 7,6
// are the offset to the white byte...otherwise, bits 7,6 are set to
// the same value as 5,4 (red) to indicate an RGB (not RGBW) device.
// i.e. binary representation:
// 0bWWRRGGBB for RGBW devices
// 0bRRRRGGBB for RGB

// RGB NeoPixel permutations; white and red offsets are always same
// Offset:         W          R          G          B
/** @brief NeoPixel byte order: RGB. Combine with a NEO_KHZ* signaling rate. */
#define NEO_RGB  ((0 << 6) | (0 << 4) | (1 << 2) | (2))
/** @brief NeoPixel byte order: RBG. Combine with a NEO_KHZ* signaling rate. */
#define NEO_RBG  ((0 << 6) | (0 << 4) | (2 << 2) | (1))
/** @brief NeoPixel byte order: GRB. Combine with a NEO_KHZ* signaling rate. */
#define NEO_GRB  ((1 << 6) | (1 << 4) | (0 << 2) | (2))
/** @brief NeoPixel byte order: GBR. Combine with a NEO_KHZ* signaling rate. */
#define NEO_GBR  ((2 << 6) | (2 << 4) | (0 << 2) | (1))
/** @brief NeoPixel byte order: BRG. Combine with a NEO_KHZ* signaling rate. */
#define NEO_BRG  ((1 << 6) | (1 << 4) | (2 << 2) | (0))
/** @brief NeoPixel byte order: BGR. Combine with a NEO_KHZ* signaling rate. */
#define NEO_BGR  ((2 << 6) | (2 << 4) | (1 << 2) | (0))

// RGBW NeoPixel permutations; all 4 offsets are distinct
// Offset:         W          R          G          B
/** @brief NeoPixel byte order: WRGB. Combine with a NEO_KHZ* signaling rate. */
#define NEO_WRGB ((0 << 6) | (1 << 4) | (2 << 2) | (3))
/** @brief NeoPixel byte order: WRBG. Combine with a NEO_KHZ* signaling rate. */
#define NEO_WRBG ((0 << 6) | (1 << 4) | (3 << 2) | (2))
/** @brief NeoPixel byte order: WGRB. Combine with a NEO_KHZ* signaling rate. */
#define NEO_WGRB ((0 << 6) | (2 << 4) | (1 << 2) | (3))
/** @brief NeoPixel byte order: WGBR. Combine with a NEO_KHZ* signaling rate. */
#define NEO_WGBR ((0 << 6) | (3 << 4) | (1 << 2) | (2))
/** @brief NeoPixel byte order: WBRG. Combine with a NEO_KHZ* signaling rate. */
#define NEO_WBRG ((0 << 6) | (2 << 4) | (3 << 2) | (1))
/** @brief NeoPixel byte order: WBGR. Combine with a NEO_KHZ* signaling rate. */
#define NEO_WBGR ((0 << 6) | (3 << 4) | (2 << 2) | (1))

/** @brief NeoPixel byte order: RWGB. Combine with a NEO_KHZ* signaling rate. */
#define NEO_RWGB ((1 << 6) | (0 << 4) | (2 << 2) | (3))
/** @brief NeoPixel byte order: RWBG. Combine with a NEO_KHZ* signaling rate. */
#define NEO_RWBG ((1 << 6) | (0 << 4) | (3 << 2) | (2))
/** @brief NeoPixel byte order: RGWB. Combine with a NEO_KHZ* signaling rate. */
#define NEO_RGWB ((2 << 6) | (0 << 4) | (1 << 2) | (3))
/** @brief NeoPixel byte order: RGBW. Combine with a NEO_KHZ* signaling rate. */
#define NEO_RGBW ((3 << 6) | (0 << 4) | (1 << 2) | (2))
/** @brief NeoPixel byte order: RBWG. Combine with a NEO_KHZ* signaling rate. */
#define NEO_RBWG ((2 << 6) | (0 << 4) | (3 << 2) | (1))
/** @brief NeoPixel byte order: RBGW. Combine with a NEO_KHZ* signaling rate. */
#define NEO_RBGW ((3 << 6) | (0 << 4) | (2 << 2) | (1))

/** @brief NeoPixel byte order: GWRB. Combine with a NEO_KHZ* signaling rate. */
#define NEO_GWRB ((1 << 6) | (2 << 4) | (0 << 2) | (3))
/** @brief NeoPixel byte order: GWBR. Combine with a NEO_KHZ* signaling rate. */
#define NEO_GWBR ((1 << 6) | (3 << 4) | (0 << 2) | (2))
/** @brief NeoPixel byte order: GRWB. Combine with a NEO_KHZ* signaling rate. */
#define NEO_GRWB ((2 << 6) | (1 << 4) | (0 << 2) | (3))
/** @brief NeoPixel byte order: GRBW. Combine with a NEO_KHZ* signaling rate. */
#define NEO_GRBW ((3 << 6) | (1 << 4) | (0 << 2) | (2))
/** @brief NeoPixel byte order: GBWR. Combine with a NEO_KHZ* signaling rate. */
#define NEO_GBWR ((2 << 6) | (3 << 4) | (0 << 2) | (1))
/** @brief NeoPixel byte order: GBRW. Combine with a NEO_KHZ* signaling rate. */
#define NEO_GBRW ((3 << 6) | (2 << 4) | (0 << 2) | (1))

/** @brief NeoPixel byte order: BWRG. Combine with a NEO_KHZ* signaling rate. */
#define NEO_BWRG ((1 << 6) | (2 << 4) | (3 << 2) | (0))
/** @brief NeoPixel byte order: BWGR. Combine with a NEO_KHZ* signaling rate. */
#define NEO_BWGR ((1 << 6) | (3 << 4) | (2 << 2) | (0))
/** @brief NeoPixel byte order: BRWG. Combine with a NEO_KHZ* signaling rate. */
#define NEO_BRWG ((2 << 6) | (1 << 4) | (3 << 2) | (0))
/** @brief NeoPixel byte order: BRGW. Combine with a NEO_KHZ* signaling rate. */
#define NEO_BRGW ((3 << 6) | (1 << 4) | (2 << 2) | (0))
/** @brief NeoPixel byte order: BGWR. Combine with a NEO_KHZ* signaling rate. */
#define NEO_BGWR ((2 << 6) | (3 << 4) | (1 << 2) | (0))
/** @brief NeoPixel byte order: BGRW. Combine with a NEO_KHZ* signaling rate. */
#define NEO_BGRW ((3 << 6) | (2 << 4) | (1 << 2) | (0))

/** @brief Select an 800 kHz NeoPixel data stream. */
#define NEO_KHZ800 0x0000 // 800 KHz datastream
/** @brief Select a 400 kHz NeoPixel data stream. */
#define NEO_KHZ400 0x0100 // 400 KHz datastream

/**
 * @brief Encoded pixel byte order and signaling rate, combining NEO_* and NEO_KHZ* constants.
 */
typedef uint16_t neoPixelType;

/**
 * @brief Buffered RGB/RGBW LED strip driver. Keep the object alive while using its buffer.
 */
class Adafruit_NeoPixel {

 public:

  // Constructor: number of LEDs, pin number, LED type
  /**
   * @brief Construct a strip with n LEDs, output pin p, and type t (color order plus bit rate).
   * The no-argument constructor requires updateLength(), setPin(), and updateType()
   * before use. Call begin() before show(); buffer edits are sent only by show().
   */
  Adafruit_NeoPixel(uint16_t n, uint8_t p=6, neoPixelType t=NEO_GRB + NEO_KHZ800);
  /**
   * @brief Construct a strip with n LEDs, output pin p, and type t (color order plus bit rate).
   * The no-argument constructor requires updateLength(), setPin(), and updateType()
   * before use. Call begin() before show(); buffer edits are sent only by show().
   */
  Adafruit_NeoPixel(void);
  /**
   * @brief Release the pixel buffer and return the configured pin to input mode.
   */
  ~Adafruit_NeoPixel();

  void
    /**
     * @brief Configure the strip output pin for use.
     */
    begin(void),
    /**
     * @brief Transmit the current pixel buffer synchronously, observing the latch delay.
     */
    show(void),
    /**
     * @brief Change the output pin, releasing the previous pin when initialized.
     */
    setPin(uint8_t p),
    /**
     * @brief Set zero-based pixel n in the buffer; out-of-range indices are ignored.
     * Byte overloads use 0-255 R/G/B and optional white; packed colors use 0xWWRRGGBB.
     * Call show() to transmit the change.
     */
    setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b),
    /**
     * @brief Set zero-based pixel n in the buffer; out-of-range indices are ignored.
     * Byte overloads use 0-255 R/G/B and optional white; packed colors use 0xWWRRGGBB.
     * Call show() to transmit the change.
     */
    setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b, uint8_t w),
    /**
     * @brief Set zero-based pixel n in the buffer; out-of-range indices are ignored.
     * Byte overloads use 0-255 R/G/B and optional white; packed colors use 0xWWRRGGBB.
     * Call show() to transmit the change.
     */
    setPixelColor(uint16_t n, uint32_t c),
    /**
     * @brief Set overall brightness from 0 (off) to 255 (maximum).
     * Rescales stored bytes with precision loss; repeatedly changing it is lossy.
     */
    setBrightness(uint8_t),
    /**
     * @brief Zero the pixel buffer without transmitting it.
     */
    clear(),
    /**
     * @brief Reallocate the strip buffer for n LEDs; contents are cleared.
     * Allocation failure leaves an empty strip.
     */
    updateLength(uint16_t n),
    /**
     * @brief Set color order and signaling rate; changing RGB/RGBW size reallocates storage.
     */
    updateType(neoPixelType t);
  uint8_t
   /**
    * @brief Return the borrowed raw byte buffer in the configured device color order.
    * Direct writes bypass brightness scaling; do not free the buffer.
    */
   *getPixels(void) const,
    /**
     * @brief Return the configured overall brightness, 0 through 255.
     */
    getBrightness(void) const;
  int8_t
    /**
     * @brief Return the configured output GPIO number.
     */
    getPin(void) { return pin; };
  uint16_t
    /**
     * @brief Return the number of LEDs currently allocated.
     */
    numPixels(void) const;
  static uint32_t
    /**
     * @brief Pack 0-255 R/G/B and optional white components into 0xWWRRGGBB.
     */
    Color(uint8_t r, uint8_t g, uint8_t b),
    /**
     * @brief Pack 0-255 R/G/B and optional white components into 0xWWRRGGBB.
     */
    Color(uint8_t r, uint8_t g, uint8_t b, uint8_t w);
  uint32_t
    /**
     * @brief Return the packed color at zero-based pixel n, or zero if out of range.
     * Brightness rescaling may prevent exact recovery of the original components.
     */
    getPixelColor(uint16_t n) const;
  inline bool
    /**
     * @brief Return whether at least 300 microseconds have elapsed since the previous transmission.
     */
    canShow(void) { return (micros() - endTime) >= 300L; }

 protected:

  boolean
    is800KHz,      // ...true if 800 KHz pixels
    begun;         // true if begin() previously called
  uint16_t
    numLEDs,       // Number of RGB LEDs in strip
    numBytes;      // Size of 'pixels' buffer below (3 or 4 bytes/pixel)
  int8_t
    pin;           // Output pin number (-1 if not yet set)
  uint8_t
    brightness,
   *pixels,        // Holds LED color values (3 or 4 bytes each)
    rOffset,       // Index of red byte within each 3- or 4-byte pixel
    gOffset,       // Index of green byte
    bOffset,       // Index of blue byte
    wOffset;       // Index of white byte (same as rOffset if no white)
  uint32_t
    endTime;       // Latch timing reference
};

#endif // ADAFRUIT_NEOPIXEL_HPP

/** @} */
