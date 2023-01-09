// -*- mode:c++; indent-tabs-mode:nil; -*-

#pragma once

#include "cox.h"

class MaxBotix_HRXL_MaxSonar_WR {
public:
  MaxBotix_HRXL_MaxSonar_WR(SerialPort &);

  void begin(void (*func)(MaxBotix_HRXL_MaxSonar_WR &,
                          int16_t mm),
             uint16_t timeoutMsec);

  virtual void turnOn() {
    this->s.begin(9600);
    this->s.listen();
    this->timeout.startOneShot(this->timeoutMsec);
  };

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
