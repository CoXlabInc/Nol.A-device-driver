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
    s.begin(9600);
    s.onReceive([](void *ctx) {
                  ((MaxBotix_HRXL_MaxSonar_WR *) ctx)->onReceive();
                }, this);
    timeout.onFired([](void *ctx) {
                      ((MaxBotix_HRXL_MaxSonar_WR *) ctx)->onTimeout();
                    }, this);
    timeout.startOneShot(timeoutMsec);
  };

  virtual void turnOff() {
    timeout.stop();
    s.stopListening();
  };

protected:
  SerialPort &s;

private:
  void onReceive();
  void onTimeout();

  void (*onGotValue)(MaxBotix_HRXL_MaxSonar_WR &, int16_t mm) = nullptr;
  uint8_t bufIndex = 5;
  char buf[5];
  Timer timeout;
  uint16_t timeoutMsec;
};
