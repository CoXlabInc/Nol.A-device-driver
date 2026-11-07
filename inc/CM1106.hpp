/**
 * @file CM1106.hpp
 * @ingroup device_drivers
 * @brief Public driver declarations and configuration constants.
 */

/**
 * @addtogroup device_drivers Device Drivers
 * @{
 */
#include <cox.h>

/**
 * @brief Asynchronous CM1106 UART CO2 sensor. Call begin() before measurement().
 */
class CM1106 {
public:
  /**
   * @brief Bind the sensor UART; the port must outlive this driver.
   */
  CM1106(SerialPort &);
  /**
   * @brief Start the UART at 9600 baud and install the receive handler.
   */
  void begin();
  /**
   * @brief Raw send API declared without an implementation in this driver.
   * Calling this method requires an implementation supplied elsewhere.
   */
  void sendData(char *args, uint8_t Len);
  /**
   * @brief Request a CO2 measurement asynchronously.
   * @param func Non-null callback invoked when the response frame is received.
   * Read CO2 or getCO2() in the callback. Only one callback is stored; no timeout is reported.
   */
  void measurement(void (*func)());
  /**
   * @brief Latest CO2 concentration in ppm; initially zero and updated during reception.
   */
  uint16_t CO2=0;
  /**
   * @brief Return the cached CO2 concentration in ppm without requesting a sample.
   */
  uint16_t getCO2();
private:
  SerialPort *port;
  uint8_t index;
  void eventDataReceived();
  static void SerialDataReceived(void *ctx);
  void (*gotCO2callback)() = NULL;
};

/** @} */
