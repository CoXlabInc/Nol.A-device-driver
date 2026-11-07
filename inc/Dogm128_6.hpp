/**
 * @file Dogm128_6.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
#include <cox.h>
#include "Adafruit_GFX.hpp"
#include "SPI.hpp"

/** @brief DOGM128-6 width in pixels. */
#define DOGM128_6_LCDWIDTH 128
/** @brief DOGM128-6 height in pixels. */
#define DOGM128_6_LCDHEIGHT 64
/** @brief Draw a black pixel in the DOGM128-6 framebuffer. */
#define BLACK 0
/** @brief Draw a white pixel in the DOGM128-6 framebuffer. */
#define WHITE 1
/** @brief Toggle the selected DOGM128-6 framebuffer pixels. */
#define INVERSE 2

/**
 * @brief Buffered DOGM128-6 128x64 SPI display. Call begin(), draw into buffer,
 * then lcdSendBuffer(buffer) to transfer the image.
 */
class Dogm128_6 : public Adafruit_GFX{
  public:
    /**
     * @brief Bind the SPI bus and power, reset, chip-select, and command/data GPIOs.
     */
    Dogm128_6(SPI &spi, int8_t lcdPwr, int8_t lcdReset, int8_t lcdPinCs, int8_t lcdMode);

    /**
     * @brief Driver-owned 1024-byte page-organized framebuffer; drawing modifies this array.
     */
    char buffer[1024] = {0x00,};
    /**
     * @brief Return the display rotation value used by this driver.
     */
    uint8_t getRotation(void) const;

    /**
     * @brief Configure display control pins and send the initialization commands.
     */
    void begin();
    /**
     * @brief Clear the local framebuffer; does not update the panel.
     */
    void lcdClear();
    /**
     * @brief Send length raw command bytes from command over SPI.
     */
    void lcdSendCommand(char *command, uint8_t length);
    /**
     * @brief Send length raw data bytes from data over SPI.
     */
    void lcdSendData(char *data, uint16_t length);
    /**
     * @brief Set the hardware write address to a column and page.
     */
    void lcdCursor(uint8_t column, uint8_t page);
    /**
     * @brief Send a complete 1024-byte page-organized framebuffer to the panel.
     */
    void lcdSendBuffer(char *buffer);
    /**
     * @brief Legacy entry point declared without an implementation; do not call.
     */
    void getCoX();
    /**
     * @brief Draw into the framebuffer; color is BLACK, WHITE, or INVERSE.
     */
    void drawPixel(int16_t x, int16_t y, uint16_t color);

    /**
     * @brief Draw a clipped vertical line into the framebuffer using the current rotation.
     */
    virtual void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
    /**
     * @brief Draw a clipped horizontal line into the framebuffer using the current rotation.
     */
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);

    /**
     * @brief Draw a clipped vertical framebuffer line in native, unrotated coordinates.
     */
    inline void drawFastVLineInternal(int16_t x, int16_t y, int16_t h, uint16_t color) __attribute__((always_inline));
    /**
     * @brief Draw a clipped horizontal framebuffer line in native, unrotated coordinates.
     */
    inline void drawFastHLineInternal(int16_t x, int16_t y, int16_t w, uint16_t color) __attribute__((always_inline));

  private:
    SPI *spi = NULL;
    uint8_t accPinCs;
    uint8_t flashPinCs;
    uint8_t lcdPwr;
    uint8_t lcdReset;
    uint8_t lcdPinCs;
    uint8_t lcdMode;
    uint8_t rotation;
};

/** @} */
