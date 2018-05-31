#pragma once

#include "../Common/Common.h"
#include "../Common/CommonTypes.h"

#include <vector>

// This class manages all the backend and the implementation details of the RNG of a GameCube
// Pokemon game, it consists of a seed finder with precalculation to improve performances and a stat
// predictor based on the frame of confirming a preset name on the naming screen.
class BaseRNGSystem
{
public:
  struct starterGen
  {
    std::string name = "";
    int hpIV = 0;
    int atkIV = 0;
    int defIV = 0;
    int spAtkIV = 0;
    int spDefIV = 0;
    int speedIV = 0;
    int genderIndex = 0;
    int natureIndex = 0;
    bool isShiny = false;
  };

  struct startersPrediction
  {
    std::vector<starterGen> starters;
    u32 startingSeed = 0;
    int frameNumber = 0;
  };

  virtual std::string getPrecalcFilenameForSettings(bool useWii, int rtcErrorMarginSeconds);
  size_t getPracalcFileSize(bool useWii, int rtcErrorMarginSeconds);
  // Does the precalculation which consist of outputing to a file the number of RNG calls done
  // before getting to the battle menu, this improves performance significantly thanks to the LCGn
  // function and this file can be reused in subsequent seed finding.
  void precalculateNbrRollsBeforeTeamGeneration(bool useWii, int rtcErrorMarginSeconds);
  // Seed finding algorithm, this does only one pass with parellelism
  void seedFinder(std::vector<int> criteria, std::vector<u32>& seeds, bool useWii,
                  int rtcErrorMarginSeconds, bool usePrecalc, bool firstPass);
  std::vector<startersPrediction> predictStartersForNbrSeconds(u32 seed, int nbrSeconds);

protected:
  struct seedRange
  {
    s64 min = 0;
    s64 max = 0;
  };

  BaseRNGSystem::seedRange getRangeForSettings(bool useWii, int rtcErrorMarginSeconds);
  // By TASing on Dolphin to get the fastest time to set the clock and have the game init its seed,
  // we arrive at 235 frames for the GameCube
  static const u32 minRTCTicksToBootGC = (Common::ticksPerSecondGC / 60) * 235;
  // By TASing on Dolphin to get the fastest time to set the clock and have the game init its seed,
  // we arrive at 860 frames for the Wii
  static const u32 minRTCTicksToBootWii = (Common::ticksPerSecondWii / 60) * 860;
  // The number of time the game polls the input per second on the naming screen
  static const int pollingRateNamingScreenPerSec = 60;
  virtual u32 rollRNGToBattleMenu(u32 seed, u16* counter = nullptr) = 0;
  // Does one battle team generation RNG calls, returns whether or not the criteria sent matches the
  // outcome got
  virtual bool generateBattleTeam(u32& seed, std::vector<int> criteria) = 0;
  virtual int getMinFramesAmountNamingScreen() = 0;
  virtual int getNbrStartersPrediction() = 0;
  // Do all the RNG calls to get to before the first possible frame of confirming the name on the
  // naming screen using a preset name
  virtual u32 rollRNGNamingScreenInit(u32 seed) = 0;
  // Do a single naming screen render call to get the next frame's seed, must use a seed that was
  // the output of rollRNGNamingScreenInit
  virtual u32 rollRNGNamingScreenNext(u32 seed) = 0;
  // Generates the starters with a given seed, the seed must have passed the naming screen
  virtual startersPrediction generateStarterPokemons(u32 seed) = 0;

  // The LCG used in both Pokemon games
  u32 inline LCG(u32& seed, u16* counter = nullptr)
  {
    seed = seed * 0x343fd + 0x269EC3;
    if (counter != nullptr)
      (*counter)++;
    return seed;
  }

  // Apply the LCG n times in O(log n) complexity
  u32 inline LCGn(u32 seed, u32 n, u16* counter = nullptr)
  {
    u32 ex = n - 1;
    u32 q = 0x343fd;
    u32 factor = 1;
    u32 sum = 0;
    while (ex > 0)
    {
      if (!(ex & 1))
      {
        sum = sum + (factor * Common::modpow32(q, ex));
        ex--;
      }
      factor *= (1 + q);
      q *= q;
      ex /= 2;
    }
    seed = (seed * Common::modpow32(0x343fd, n)) + (sum + factor) * 0x269EC3;
    if (counter != nullptr)
      *counter += n;
    return seed;
  }

  bool inline isPidShiny(u16 TID, u16 SID, u32 PID)
  {
    return ((TID ^ SID ^ (PID & 0xFFFF) ^ (PID >> 16)) < 8);
  }

  int inline getPidGender(u8 genderRatio, u32 pid)
  {
    return genderRatio > (pid & 0xff) ? 1 : 0;
  }
};
