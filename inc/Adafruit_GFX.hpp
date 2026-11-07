/**
 * @file Adafruit_GFX.hpp
 * @brief Public driver declarations and configuration constants.
 */
#ifndef _ADAFRUIT_GFX_H
#define _ADAFRUIT_GFX_H

#include <cox.h>
#include "gfxfont.hpp"
#include <Print.hpp>

/**
 * @brief Device-independent drawing and text interface derived from Print.
 */
class Adafruit_GFX : public Print {

 public:

  /**
   * @brief Construct a graphics surface with native width w and height h in pixels.
   * Coordinates use pixels from the upper-left corner; colors are interpreted by
   * the concrete display. Subclasses must implement drawPixel().
   */
  Adafruit_GFX(int16_t w, int16_t h); // Constructor

  // This MUST be defined by the subclass:
  /**
   * @brief Draw one pixel at x/y using the device-specific color value. Canvas overrides update RAM.
   */
  virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;

  // TRANSACTION API / CORE DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  /**
   * @brief Write a vertical line of h pixels at x/y using color.
   */
  virtual void writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
  /**
   * @brief Write a horizontal line of w pixels at x/y using color.
   */
  virtual void writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
  /**
   * @brief Write a line between the inclusive endpoints (x0,y0) and (x1,y1).
   */
  virtual void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);


  // CONTROL API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  /**
   * @brief Select orientation using the low two bits of r (0, 90, 180, or 270 degrees).
   */
  virtual void setRotation(uint8_t r);
  /**
   * @brief Request device inversion when i is true; the base implementation is a no-op.
   */
  virtual void invertDisplay(boolean i);

  // BASIC DRAW API
  // These MAY be overridden by the subclass to provide device-specific
  // optimized code.  Otherwise 'generic' versions are used.
  virtual void
    // It's good to implement those, even if using transaction API
    /**
     * @brief Draw a vertical line of h pixels at x/y with the current display orientation.
     */
    drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
    /**
     * @brief Draw a horizontal line of w pixels at x/y with the current display orientation.
     */
    drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
    /**
     * @brief Fill a rectangle at x/y of width w and height h in pixels.
     */
    fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
    /**
     * @brief Fill the entire graphics surface with color.
     */
    fillScreen(uint16_t color),
    // Optional and probably not necessary to change
    /**
     * @brief Draw a line between the inclusive endpoints (x0,y0) and (x1,y1).
     */
    drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color),
    /**
     * @brief Draw the outline of a rectangle at x/y of width w and height h in pixels.
     */
    drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);

  /**
   * @brief Render one character at the text cursor, honoring font, scale, wrapping, and colors.
   * @return One byte consumed; inherited Print helpers can write strings and numbers.
   */
  virtual size_t write(uint8_t);
  
  // These exist only with Adafruit_GFX (no subclass overrides)
  void
    /**
     * @brief Draw a circle outline centered at x0/y0 with radius r in pixels.
     */
    drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    /**
     * @brief Draw selected circle quadrants; cornername is the four-bit quadrant mask.
     */
    drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      uint16_t color),
    /**
     * @brief Fill a circle centered at x0/y0 with radius r in pixels.
     */
    fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color),
    /**
     * @brief Fill selected circle sides; cornername selects sides and delta extends vertical spans.
     */
    fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,
      int16_t delta, uint16_t color),
    /**
     * @brief Draw a triangle outline through the three supplied pixel-coordinate vertices.
     */
    drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    /**
     * @brief Fill a triangle through the three supplied pixel-coordinate vertices.
     */
    fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1,
      int16_t x2, int16_t y2, uint16_t color),
    /**
     * @brief Draw a rectangle outline at x0/y0 with pixel dimensions w/h and corner radius.
     */
    drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    /**
     * @brief Fill a rectangle at x0/y0 with pixel dimensions w/h and corner radius.
     */
    fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h,
      int16_t radius, uint16_t color),
    /**
     * @brief Draw a row-padded, MSB-first 1-bit bitmap of w by h pixels at x/y.
     * Set bits use color; clear bits are transparent unless the bg overload is used.
     */
    drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
      int16_t w, int16_t h, uint16_t color),
    /**
     * @brief Draw a row-padded, MSB-first 1-bit bitmap of w by h pixels at x/y.
     * Set bits use color; clear bits are transparent unless the bg overload is used.
     */
    drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
      int16_t w, int16_t h, uint16_t color, uint16_t bg),
    /**
     * @brief Draw a row-padded, MSB-first 1-bit bitmap of w by h pixels at x/y.
     * Set bits use color; clear bits are transparent unless the bg overload is used.
     */
    drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color),
    /**
     * @brief Draw a row-padded, MSB-first 1-bit bitmap of w by h pixels at x/y.
     * Set bits use color; clear bits are transparent unless the bg overload is used.
     */
    drawBitmap(int16_t x, int16_t y, uint8_t *bitmap,
      int16_t w, int16_t h, uint16_t color, uint16_t bg),
    /**
     * @brief Draw an LSB-first XBM image of w by h pixels; zero bits are transparent.
     */
    drawXBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
      int16_t w, int16_t h, uint16_t color),
    /**
     * @brief Draw w by h bytes of grayscale pixels at x/y.
     * Mask overloads use an MSB-first 1-bit mask; zero mask bits are transparent.
     */
    drawGrayscaleBitmap(int16_t x, int16_t y, const uint8_t bitmap[],
      int16_t w, int16_t h),
    /**
     * @brief Draw w by h bytes of grayscale pixels at x/y.
     * Mask overloads use an MSB-first 1-bit mask; zero mask bits are transparent.
     */
    drawGrayscaleBitmap(int16_t x, int16_t y, uint8_t *bitmap,
      int16_t w, int16_t h),
    /**
     * @brief Draw w by h bytes of grayscale pixels at x/y.
     * Mask overloads use an MSB-first 1-bit mask; zero mask bits are transparent.
     */
    drawGrayscaleBitmap(int16_t x, int16_t y,
      const uint8_t bitmap[], const uint8_t mask[],
      int16_t w, int16_t h),
    /**
     * @brief Draw w by h bytes of grayscale pixels at x/y.
     * Mask overloads use an MSB-first 1-bit mask; zero mask bits are transparent.
     */
    drawGrayscaleBitmap(int16_t x, int16_t y,
      uint8_t *bitmap, uint8_t *mask, int16_t w, int16_t h),
    /**
     * @brief Draw w by h RGB565 pixels at x/y.
     * Mask overloads use an MSB-first 1-bit mask; zero mask bits are transparent.
     */
    drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[],
      int16_t w, int16_t h),
    /**
     * @brief Draw w by h RGB565 pixels at x/y.
     * Mask overloads use an MSB-first 1-bit mask; zero mask bits are transparent.
     */
    drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap,
      int16_t w, int16_t h),
    /**
     * @brief Draw w by h RGB565 pixels at x/y.
     * Mask overloads use an MSB-first 1-bit mask; zero mask bits are transparent.
     */
    drawRGBBitmap(int16_t x, int16_t y,
      const uint16_t bitmap[], const uint8_t mask[],
      int16_t w, int16_t h),
    /**
     * @brief Draw w by h RGB565 pixels at x/y.
     * Mask overloads use an MSB-first 1-bit mask; zero mask bits are transparent.
     */
    drawRGBBitmap(int16_t x, int16_t y,
      uint16_t *bitmap, uint8_t *mask, int16_t w, int16_t h),
    /**
     * @brief Draw character c at x/y with foreground/background colors and integer pixel scale.
     */
    drawChar(int16_t x, int16_t y, unsigned char c, uint16_t color,
      uint16_t bg, uint8_t size),
    /**
     * @brief Set the text cursor in pixels; custom-font y coordinates refer to the baseline.
     */
    setCursor(int16_t x, int16_t y),
    /**
     * @brief Set text foreground color; the two-argument overload also sets background.
     * The one-argument overload uses transparent background for the classic font.
     */
    setTextColor(uint16_t c),
    /**
     * @brief Set text foreground color; the two-argument overload also sets background.
     * The one-argument overload uses transparent background for the classic font.
     */
    setTextColor(uint16_t c, uint16_t bg),
    /**
     * @brief Set integer text magnification; zero is treated as one.
     */
    setTextSize(uint8_t s),
    /**
     * @brief Enable or disable automatic text wrapping at the right edge.
     */
    setTextWrap(boolean w),
    /**
     * @brief Enable corrected CP437 character mapping; false retains the legacy mapping.
     */
    cp437(boolean x=true),
    /**
     * @brief Select a borrowed GFXfont; nullptr restores the built-in fixed-width font.
     */
    setFont(const GFXfont *f = NULL);

  /**
   * @brief Return the surface height in pixels after rotation.
   */
  int16_t height(void) const;
  /**
   * @brief Return the surface width in pixels after rotation.
   */
  int16_t width(void) const;
  /**
   * @brief Return the current rotation index, 0 through 3.
   */
  uint8_t getRotation(void) const;

  // get current cursor position (get rotation safe maximum values, using: width() for x, height() for y)
  /**
   * @brief Return the text cursor X coordinate in pixels.
   */
  int16_t getCursorX(void) const;
  /**
   * @brief Return the text cursor Y coordinate in pixels.
   */
  int16_t getCursorY(void) const;

 protected:
  void
    charBounds(char c, int16_t *x, int16_t *y,
      int16_t *minx, int16_t *miny, int16_t *maxx, int16_t *maxy);
  const int16_t
    WIDTH, HEIGHT;   // This is the 'raw' display w/h - never changes
  int16_t
    _width, _height, // Display w/h as modified by current rotation
    cursor_x, cursor_y;
  uint16_t
    textcolor, textbgcolor;
  uint8_t
    textsize,
    rotation;
    boolean
    wrap,   // If set, 'wrap' text at right edge of display
    _cp437; // If set, use correct CP437 charset (default is off)
    const GFXfont
    *gfxFont;
};

/**
 * @brief Drawable button geometry with application-supplied press state.
 */
class Adafruit_GFX_Button {

 public:
  /**
   * @brief Construct an unconfigured button; call initButton() or initButtonUL() before drawing.
   */
  Adafruit_GFX_Button(void);

  // "Classic" initButton() uses center & size
  /**
   * @brief Configure a button using center x/y and width/height in pixels.
   * The graphics surface must outlive the button; at most nine label characters are stored.
   */
  void initButton(Adafruit_GFX *gfx, int16_t x, int16_t y,
   uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
   uint16_t textcolor, char *label, uint8_t textsize);
  // New/alt initButton() uses upper-left corner & size
  /**
   * @brief Configure a button using upper-left x1/y1 and width/height in pixels.
   * The graphics surface must outlive the button; at most nine label characters are stored.
   */
  void initButtonUL(Adafruit_GFX *gfx, int16_t x1, int16_t y1,
   uint16_t w, uint16_t h, uint16_t outline, uint16_t fill,
   uint16_t textcolor, char *label, uint8_t textsize);
  /**
   * @brief Render the button, exchanging fill/text colors when inverted is true.
   */
  void drawButton(boolean inverted = false);
  /**
   * @brief Store the current pressed state and retain the previous state for edge queries.
   */
  void press(boolean p);

  /**
   * @brief Return whether the pixel coordinate x/y lies inside the button rectangle.
   */
  boolean contains(int16_t x, int16_t y);
  /**
   * @brief Return the last state supplied to press().
   */
  boolean isPressed();
  /**
   * @brief Return true for a released-to-pressed transition in the last press() update.
   */
  boolean justPressed();
  /**
   * @brief Return true for a pressed-to-released transition in the last press() update.
   */
  boolean justReleased();

 private:
  Adafruit_GFX *_gfx;
  int16_t       _x1, _y1; // Coordinates of top-left corner
  uint16_t      _w, _h;
  uint8_t       _textsize;
  uint16_t      _outlinecolor, _fillcolor, _textcolor;
  char          _label[10];
  boolean currstate, laststate;
};

/**
 * @brief Owned off-screen monochrome drawing surface.
 */
class GFXcanvas1 : public Adafruit_GFX {
 public:
  /**
   * @brief Allocate a w by h pixel monochrome canvas; getBuffer() is nullptr on allocation failure.
   */
  GFXcanvas1(uint16_t w, uint16_t h);
  /**
   * @brief Release the owned monochrome framebuffer.
   */
  ~GFXcanvas1(void);
  /**
   * @brief Draw one pixel at x/y using the device-specific color value. Canvas overrides update RAM.
   */
  void     drawPixel(int16_t x, int16_t y, uint16_t color),
           /**
            * @brief Fill the entire graphics surface with color.
            */
           fillScreen(uint16_t color);
  /**
   * @brief Return the borrowed canvas buffer, or nullptr on allocation failure.
   * Storage is owned by the canvas: 1-bit rows are byte-padded and MSB-first;
   * 8-bit and RGB565 canvases store one element per pixel in row-major order.
   */
  uint8_t *getBuffer(void);
 private:
  uint8_t *buffer;
};

/**
 * @brief Owned off-screen 8-bit drawing surface.
 */
class GFXcanvas8 : public Adafruit_GFX {
 public:
  /**
   * @brief Allocate a w by h pixel 8-bit canvas; getBuffer() is nullptr on allocation failure.
   */
  GFXcanvas8(uint16_t w, uint16_t h);
  /**
   * @brief Release the owned 8-bit framebuffer.
   */
  ~GFXcanvas8(void);
  /**
   * @brief Draw one pixel at x/y using the device-specific color value. Canvas overrides update RAM.
   */
  void     drawPixel(int16_t x, int16_t y, uint16_t color),
           /**
            * @brief Fill the entire graphics surface with color.
            */
           fillScreen(uint16_t color),
           /**
            * @brief Write a horizontal line of w pixels at x/y using color.
            */
           writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

  /**
   * @brief Return the borrowed canvas buffer, or nullptr on allocation failure.
   * Storage is owned by the canvas: 1-bit rows are byte-padded and MSB-first;
   * 8-bit and RGB565 canvases store one element per pixel in row-major order.
   */
  uint8_t *getBuffer(void);
 private:
  uint8_t *buffer;
};

/**
 * @brief Owned off-screen RGB565 drawing surface.
 */
class GFXcanvas16 : public Adafruit_GFX {
 public:
  /**
   * @brief Allocate a w by h pixel RGB565 canvas; getBuffer() is nullptr on allocation failure.
   */
  GFXcanvas16(uint16_t w, uint16_t h);
  /**
   * @brief Release the owned RGB565 framebuffer.
   */
  ~GFXcanvas16(void);
  /**
   * @brief Draw one pixel at x/y using the device-specific color value. Canvas overrides update RAM.
   */
  void      drawPixel(int16_t x, int16_t y, uint16_t color),
            /**
             * @brief Fill the entire graphics surface with color.
             */
            fillScreen(uint16_t color);
  /**
   * @brief Return the borrowed canvas buffer, or nullptr on allocation failure.
   * Storage is owned by the canvas: 1-bit rows are byte-padded and MSB-first;
   * 8-bit and RGB565 canvases store one element per pixel in row-major order.
   */
  uint16_t *getBuffer(void);
 private:
  uint16_t *buffer;
};

#endif // _ADAFRUIT_GFX_H
