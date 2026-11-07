#pragma once
#include <cstdint>
struct TestSystem { void feedWatchdog() {} };
extern TestSystem System;
void delay(uint16_t);
