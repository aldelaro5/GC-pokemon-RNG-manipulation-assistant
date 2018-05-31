#pragma once

#include "CommonTypes.h"

#include <array>
#include <string>

namespace Common
{
// The GameCube RTC does 40500000 ticks per second while the Wii is exactly 1.5 faster
static const int ticksPerSecondGC = 40500000;
static const int ticksPerSecondWii = static_cast<int>(ticksPerSecondGC * 1.5);

// Alogorithm that returns (base^exp) mod 2^32 in the complexity O(log n)
inline u32 modpow32(u32 base, u32 exp)
{
  u32 result = 1;
  while (exp > 0)
  {
    if (exp & 1)
      result = result * base;
    base = base * base;
    exp >>= 1;
  }
  return result;
}
}; // namespace Common
