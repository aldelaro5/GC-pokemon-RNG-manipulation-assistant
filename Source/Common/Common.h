#pragma once

#include "CommonTypes.h"

namespace Common
{
static const u64 nbrPossibleSeeds = 0x100000000;

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
