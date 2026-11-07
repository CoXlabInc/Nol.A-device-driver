/**
 * @file MaxBotix_HRXL_MaxSonar_WR.hpp
 * @brief Public driver declarations and configuration constants.
 */
// -*- mode:c++; indent-tabs-mode:nil; -*-

#pragma once

#include "cox.h"

/**
 * @brief UART ultrasonic range reader. Call begin() then turnOn().
 * The driver and serial port must remain alive while receiving or awaiting timeout.
 */
class MaxBotix_HRXL_MaxSonar_WR {
public:
  /**
   * @brief Bind the sensor serial port without starting it.
   */
  MaxBotix_HRXL_MaxSonar_WR(SerialPort &);

  /**
   * @brief Configure the receive callback and one-shot timeout.
   * @param func Receives this driver and distance in millimeters, or -1 on timeout;
   * nullptr disables notification. Successful reads do not cancel or restart the timer.
   * @param timeoutMsec Timeout in milliseconds, measured from turnOn().
   */
  void begin(void (*func)(MaxBotix_HRXL_MaxSonar_WR &,
                          int16_t mm),
             uint16_t timeoutMsec);

  /**
   * @brief Start 9600-baud listening and arm the one-shot timeout.
   */
  virtual void turnOn() {
    this->s.begin(9600);
    this->s.listen();
    this->timeout.startOneShot(this->timeoutMsec);
  };

  /**
   * @brief Stop the timer and UART listening.
   */
  virtual void turnOff() {
    this->timeout.stop();
    this->s.stopListening();
  };

protected:
  SerialPort &s;

private:
  void onReceive();
  void onTimeout();

  void (*onGotValue)(MaxBotix_HRXL_MaxSonar_WR &, int16_t mm) = nullptr;
  uint8_t bufIndex = 0;
  char buf[6];
  Timer timeout;
  uint16_t timeoutMsec;
};
