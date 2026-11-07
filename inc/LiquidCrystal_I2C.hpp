/**
 * @file LiquidCrystal_I2C.hpp
 * @brief Public driver declarations and configuration constants.
 */
/*
  Original source: https://github.com/johnrickman/LiquidCrystal_I2C
  Ported by jsjeong (CoXlab Inc.)
*/
#pragma once

#include <inttypes.h>
#include <Print.hpp>
#include <TwoWire.hpp>

// commands
/** @brief HD44780 cleardisplay command or bit-field value. */
#define LCD_CLEARDISPLAY 0x01
/** @brief HD44780 returnhome command or bit-field value. */
#define LCD_RETURNHOME 0x02
/** @brief HD44780 entrymodeset command or bit-field value. */
#define LCD_ENTRYMODESET 0x04
/** @brief HD44780 displaycontrol command or bit-field value. */
#define LCD_DISPLAYCONTROL 0x08
/** @brief HD44780 cursorshift command or bit-field value. */
#define LCD_CURSORSHIFT 0x10
/** @brief HD44780 functionset command or bit-field value. */
#define LCD_FUNCTIONSET 0x20
/** @brief HD44780 setcgramaddr command or bit-field value. */
#define LCD_SETCGRAMADDR 0x40
/** @brief HD44780 setddramaddr command or bit-field value. */
#define LCD_SETDDRAMADDR 0x80

// flags for display entry mode
/** @brief HD44780 entryright command or bit-field value. */
#define LCD_ENTRYRIGHT 0x00
/** @brief HD44780 entryleft command or bit-field value. */
#define LCD_ENTRYLEFT 0x02
/** @brief HD44780 entryshiftincrement command or bit-field value. */
#define LCD_ENTRYSHIFTINCREMENT 0x01
/** @brief HD44780 entryshiftdecrement command or bit-field value. */
#define LCD_ENTRYSHIFTDECREMENT 0x00

// flags for display on/off control
/** @brief HD44780 displayon command or bit-field value. */
#define LCD_DISPLAYON 0x04
/** @brief HD44780 displayoff command or bit-field value. */
#define LCD_DISPLAYOFF 0x00
/** @brief HD44780 cursoron command or bit-field value. */
#define LCD_CURSORON 0x02
/** @brief HD44780 cursoroff command or bit-field value. */
#define LCD_CURSOROFF 0x00
/** @brief HD44780 blinkon command or bit-field value. */
#define LCD_BLINKON 0x01
/** @brief HD44780 blinkoff command or bit-field value. */
#define LCD_BLINKOFF 0x00

// flags for display/cursor shift
/** @brief HD44780 displaymove command or bit-field value. */
#define LCD_DISPLAYMOVE 0x08
/** @brief HD44780 cursormove command or bit-field value. */
#define LCD_CURSORMOVE 0x00
/** @brief HD44780 moveright command or bit-field value. */
#define LCD_MOVERIGHT 0x04
/** @brief HD44780 moveleft command or bit-field value. */
#define LCD_MOVELEFT 0x00

// flags for function set
/** @brief HD44780 8bitmode command or bit-field value. */
#define LCD_8BITMODE 0x10
/** @brief HD44780 4bitmode command or bit-field value. */
#define LCD_4BITMODE 0x00
/** @brief HD44780 2line command or bit-field value. */
#define LCD_2LINE 0x08
/** @brief HD44780 1line command or bit-field value. */
#define LCD_1LINE 0x00
/** @brief HD44780 5x10dots command or bit-field value. */
#define LCD_5x10DOTS 0x04
/** @brief HD44780 5x8dots command or bit-field value. */
#define LCD_5x8DOTS 0x00

// flags for backlight control
/** @brief HD44780 backlight command or bit-field value. */
#define LCD_BACKLIGHT 0x08
/** @brief HD44780 nobacklight command or bit-field value. */
#define LCD_NOBACKLIGHT 0x00

#define En 0b00000100  // Enable bit
#define Rw 0b00000010  // Read/Write bit
#define Rs 0b00000001  // Register select bit

/**
 * @brief HD44780-compatible character display through an I2C backpack.
 * Inherits Print for formatted text. The I2C bus must outlive the display.
 */
class LiquidCrystal_I2C : public Print {
public:
  /**
   * @brief Bind the I2C bus, 7-bit backpack address, column count, and row count.
   * Call init() (or oled_init() for OLED mode) before using the display.
   */
  LiquidCrystal_I2C(TwoWire &, uint8_t lcd_Addr,uint8_t lcd_cols,uint8_t lcd_rows);
  /**
   * @brief Configure the display geometry and character cell size; use LCD_5x8DOTS by default.
   */
  void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS );
  /**
   * @brief Clear the display and return the hardware cursor home; waits for command completion.
   */
  void clear();
  /**
   * @brief Return the cursor and display shift to the home position.
   */
  void home();
  /**
   * @brief Turn off visible output while preserving display memory.
   */
  void noDisplay();
  /**
   * @brief Turn on visible output.
   */
  void display();
  /**
   * @brief Disable cursor blinking.
   */
  void noBlink();
  /**
   * @brief Enable cursor blinking.
   */
  void blink();
  /**
   * @brief Hide the underline cursor.
   */
  void noCursor();
  /**
   * @brief Show the underline cursor.
   */
  void cursor();
  /**
   * @brief Shift the displayed contents one position left.
   */
  void scrollDisplayLeft();
  /**
   * @brief Shift the displayed contents one position right.
   */
  void scrollDisplayRight();
  /**
   * @brief Legacy declaration without an implementation in this port; do not call.
   */
  void printLeft();
  /**
   * @brief Legacy declaration without an implementation in this port; do not call.
   */
  void printRight();
  /**
   * @brief Advance the cursor left-to-right after each character.
   */
  void leftToRight();
  /**
   * @brief Advance the cursor right-to-left after each character.
   */
  void rightToLeft();
  /**
   * @brief Legacy declaration without an implementation in this port; do not call.
   */
  void shiftIncrement();
  /**
   * @brief Legacy declaration without an implementation in this port; do not call.
   */
  void shiftDecrement();
  /**
   * @brief Switch the backpack backlight off.
   */
  void noBacklight();
  /**
   * @brief Switch the backpack backlight on.
   */
  void backlight();
  /**
   * @brief Enable automatic display shifting as characters are written.
   */
  void autoscroll();
  /**
   * @brief Disable automatic display shifting as characters are written.
   */
  void noAutoscroll();
  /**
   * @brief Upload an eight-row custom glyph to slot 0 through 7.
   * The array overload reads RAM; the const char pointer overload reads program memory.
   */
  void createChar(uint8_t, uint8_t[]);
  /**
   * @brief Upload an eight-row custom glyph to slot 0 through 7.
   * The array overload reads RAM; the const char pointer overload reads program memory.
   */
  void createChar(uint8_t location, const char *charmap);
  // Example: 	const char bell[8] PROGMEM = {B00100,B01110,B01110,B01110,B11111,B00000,B00100,B00000};

  /**
   * @brief Set the zero-based column and row using the two arguments, respectively.
   */
  void setCursor(uint8_t, uint8_t);
  /**
   * @brief Send one character byte through Print and return 1.
   */
  virtual size_t write(uint8_t);
  /**
   * @brief Send one raw HD44780 command byte.
   */
  void command(uint8_t);
  /**
   * @brief Initialize the I2C bus and the configured LCD geometry.
   */
  void init();
  /**
   * @brief Select OLED timing, initialize the bus, and configure display geometry.
   */
  void oled_init();

////compatibility API function aliases
/**
 * @brief Compatibility alias for blink().
 */
void blink_on();						// alias for blink()
/**
 * @brief Compatibility alias for noBlink().
 */
void blink_off();       					// alias for noBlink()
/**
 * @brief Compatibility alias for cursor().
 */
void cursor_on();      	 					// alias for cursor()
/**
 * @brief Compatibility alias for noCursor().
 */
void cursor_off();      					// alias for noCursor()
/**
 * @brief Enable backlight for nonzero new_val; disable it for zero.
 */
void setBacklight(uint8_t new_val);				// alias for backlight() and nobacklight()
/**
 * @brief Upload eight glyph rows to custom-character slot char_num (0 through 7).
 */
void load_custom_character(uint8_t char_num, uint8_t *rows);	// alias for createChar()
/**
 * @brief Print a NUL-terminated character string.
 */
void printstr(const char[]);

////Unsupported API functions (not implemented in this library)
/**
 * @brief Unsupported compatibility stub; always returns zero.
 */
uint8_t status();
/**
 * @brief Unsupported compatibility stub; does nothing.
 */
void setContrast(uint8_t new_val);
/**
 * @brief Unsupported compatibility stub; always returns zero.
 */
uint8_t keypad();
/**
 * @brief Unsupported compatibility stub; does nothing.
 */
void setDelay(int,int);
/**
 * @brief Unsupported compatibility stub; does nothing.
 */
void on();
/**
 * @brief Unsupported compatibility stub; does nothing.
 */
void off();
/**
 * @brief Unsupported compatibility stub; always returns zero.
 */
uint8_t init_bargraph(uint8_t graphtype);
/**
 * @brief Unsupported compatibility stub; does nothing.
 */
void draw_horizontal_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);
/**
 * @brief Unsupported compatibility stub; does nothing.
 */
void draw_vertical_graph(uint8_t row, uint8_t column, uint8_t len,  uint8_t pixel_col_end);


private:
  void init_priv();
  void send(uint8_t, uint8_t);
  void write4bits(uint8_t);
  void expanderWrite(uint8_t);
  void pulseEnable(uint8_t);
  TwoWire &Wire;
  uint8_t _Addr;
  uint8_t _displayfunction;
  uint8_t _displaycontrol;
  uint8_t _displaymode;
  uint8_t _numlines;
  bool _oled = false;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _backlightval;
};
