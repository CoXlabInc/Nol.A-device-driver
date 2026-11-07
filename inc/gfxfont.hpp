/**
 * @file gfxfont.hpp
 * @brief Glyph and font storage formats used by Adafruit_GFX.
 */
// Font structures for newer Adafruit_GFX (1.1 and later).
// Example fonts are included in 'Fonts' directory.
// To use a font in your Arduino sketch, #include the corresponding .h
// file and pass address of GFXfont struct to setFont().  Pass NULL to
// revert to 'classic' fixed-space bitmap font.
#include "cox.h"

#ifndef _GFXFONT_H_
#define _GFXFONT_H_

/** @brief Metrics and bitmap offset for one GFX font glyph. */
typedef struct { // Data stored PER GLYPH
	uint16_t bitmapOffset;     ///< Byte offset into GFXfont::bitmap
  /** @brief Bitmap dimensions in pixels. */
	uint8_t  width, height;
	uint8_t  xAdvance;         ///< Distance to advance cursor (x axis)
  /** @brief Pixel offsets from cursor to the glyph upper-left corner. */
	int8_t   xOffset, yOffset;
} GFXglyph;

/** @brief Borrowed bitmap/glyph tables and character range for Adafruit_GFX::setFont(). */
typedef struct { // Data stored for FONT AS A WHOLE:
	uint8_t  *bitmap;      ///< Glyph bitmaps, concatenated
	GFXglyph *glyph;       ///< Glyph array
  /** @brief Inclusive character-code range covered by the glyph table. */
	uint8_t   first, last;
	uint8_t   yAdvance;    ///< Newline distance (y axis)
} GFXfont;

#endif // _GFXFONT_H_
