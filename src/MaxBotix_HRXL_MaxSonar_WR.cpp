// -*- mode:c++; indent-tabs-mode:nil; -*-

#include "MaxBotix_HRXL_MaxSonar_WR.hpp"

MaxBotix_HRXL_MaxSonar_WR::MaxBotix_HRXL_MaxSonar_WR(SerialPort &s)
  : s(s) {
}

void MaxBotix_HRXL_MaxSonar_WR::begin(void (*func)(MaxBotix_HRXL_MaxSonar_WR &,
                                                   int16_t mm),
                                      uint16_t timeoutMsec) {
  this->onGotValue = func;
  this->timeoutMsec = timeoutMsec;
  this->s.onReceive([](void *ctx) {
                      ((MaxBotix_HRXL_MaxSonar_WR *) ctx)->onReceive();
                    }, this);
  this->timeout.onFired([](void *ctx) {
                          ((MaxBotix_HRXL_MaxSonar_WR *) ctx)->onTimeout();
                        }, this);
}

void MaxBotix_HRXL_MaxSonar_WR::onReceive() {
  while (s.available() > 0) {
    char c = (char) s.read();
    if (c == 'R') {
      buf[0] = 'R';
      bufIndex = 1;
    } else if (c == '\r' && bufIndex == 1 + 4) {
      buf[bufIndex] = '\0';
      uint16_t val = strtoul(&buf[1], nullptr, 10);

      if (this->onGotValue) {
        this->onGotValue(*this, val);
      }
      bufIndex = 0;
    } else if (c >= '0' && c <= '9' && bufIndex < sizeof(buf)) {
      buf[bufIndex++] = c;
    } else {
      bufIndex = 0;
    }
  }
}

void MaxBotix_HRXL_MaxSonar_WR::onTimeout() {
  if (this->onGotValue) {
    this->onGotValue(*this, -1);
  }
}
