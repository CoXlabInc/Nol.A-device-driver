// -*- mode:c++; indent-tabs-mode:nil; -*-

#include "MaxBotix_HRXL_MaxSonar_WR.hpp"

MaxBotix_HRXL_MaxSonar_WR::MaxBotix_HRXL_MaxSonar_WR(SerialPort &s)
  : s(s) {
}

void MaxBotix_HRXL_MaxSonar_WR::begin(void (*func)(MaxBotix_HRXL_MaxSonar_WR &,
                                                   int16_t mm), uint16_t timeoutMsec) {
  this->onGotValue = func;
}

void MaxBotix_HRXL_MaxSonar_WR::onReceive() {
  while (s.available() > 0) {
    char c = (char) s.read();
    if (c == 'R') {
      bufIndex = 0;
    } else if (c == '\r') {
      if (bufIndex == 4) {
        s.stopListening();
        buf[4] = '\0';
        uint16_t val = strtoul(buf, nullptr, 10);

        if (this->onGotValue) {
          onGotValue(*this, val);
        }
      } else {
        bufIndex = 5;
      }
    } else if (c >= '0' && c <= '9') {
      buf[bufIndex++] = c;
    } else {
      bufIndex = 5;
    }
  }
}

void MaxBotix_HRXL_MaxSonar_WR::onTimeout() {
  if (this->onGotValue) {
    this->onGotValue(*this, -1);
  }
}
