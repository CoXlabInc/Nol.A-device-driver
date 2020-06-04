#pragma once

#include "ST7920.hpp"

class KXM12864M_3 : public ST7920 {
public:
  KXM12864M_3(SPI &, int8_t CS);
  void invalidate() override;
};
