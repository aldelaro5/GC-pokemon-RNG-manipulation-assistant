#pragma once

#include "../Common/Common.h"
#include "../Common/CommonTypes.h"

#include <functional>
#include <vector>

// This class manages all the backend and the implementation details of the RNG of a GameCube
// Pokemon game, it consists of a seed finder with precalculation to improve performances and a stat
// predictor based on the frame of confirming a name on the naming screen
class BaseRNGSystem
{
public:
  struct StarterGen
  {
    int hpIV = 0;
    int atkIV = 0;
    int defIV = 0;
    int spAtkIV = 0;
    int spDefIV = 0;
    int speedIV = 0;
    int hpStartingStat = 0;
    int hiddenPowerTypeIndex = 0;
    int hiddenPowerPower = 0;
    int genderIndex = 0;
    int natureIndex = 0;
    bool isShiny = false;
  };

  struct StartersPrediction
  {
    std::vector<StarterGen> starters;
    u32 startingSeed = 0;
    int frameNumber = 0;
    int trainerId = 0;
  };

  virtual std::string getPrecalcFilename() = 0;
  virtual int getNbrStartersPrediction() = 0;
  virtual std::vector<std::string> getStartersName() = 0;
  virtual std::vector<int> obtainTeamGenerationCritera(u32& seed) = 0;
  virtual int getNbrCombinationsFirstTwoCriteria() = 0;
  // Does the precalculation which consist of outputing to a file the remaining unique seeds after a
  // complete first pass, the order of seeds will allow to tell the corespondance with the first two
  // criteria
  void generatePrecalculationFile(unsigned int threadCount,
                                  std::function<void(long)> progressUpdate,
                                  std::function<bool()> shouldCancelNow);
  virtual int firstTwoCriteriaToIndex(const std::vector<int> criteria) = 0;
  // Seed finding algorithm, this does only one pass with parellelism
  void seedFinderPass(unsigned int threadCount, const std::vector<int> criteria,
                      std::vector<u32>& seeds, std::function<void(long)> progressUpdate,
                      std::function<bool()> shouldCancelNow);
  std::vector<StartersPrediction> predictStartersForNbrSeconds(u32 seed, const int nbrSeconds);
  // Does one battle team generation RNG calls, returns whether or not the criteria sent matches the
  // outcome got
  virtual bool generateBattleTeam(u32& seed, const std::vector<int> criteria) = 0;

protected:
  // The number of time the game polls the input per second on the naming screen
  static const int pollingRateNamingScreenPerSec = 60;
  virtual u32 rollRNGToBattleMenu(const u32 seed, u16* counter = nullptr) = 0;
  virtual int getMinFramesAmountNamingScreen() = 0;
  // Do all the RNG calls to get to before the first possible frame of confirming the name on the
  // naming screen using a preset name
  virtual u32 rollRNGNamingScreenInit(const u32 seed) = 0;
  // Do a single naming screen render call to get the next frame's seed, must use a seed that was
  // the output of rollRNGNamingScreenInit
  virtual u32 rollRNGNamingScreenNext(const u32 seed) = 0;
  // Generates the starters with a given seed, the seed must have passed the naming screen
  virtual StartersPrediction generateStarterPokemons(const u32 seed) = 0;

  // The LCG used in both Pokemon games
  u32 inline LCG(u32& seed, u16* counter = nullptr)
  {
    seed = seed * 0x343fd + 0x269EC3;
    if (counter != nullptr)
      (*counter)++;
    return seed;
  }

  // Apply the LCG n times in O(log n) complexity
  u32 inline LCGn(u32 seed, const u32 n, u16* counter = nullptr)
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

  bool inline isPidShiny(const u16 TID, const u16 SID, const u32 PID)
  {
    return ((TID ^ SID ^ (PID & 0xFFFF) ^ (PID >> 16)) < 8);
  }

  void inline fillStarterGenHiddenPowerInfo(StarterGen& starter)
  {
    int typeSum = (starter.hpIV & 1) + 2 * (starter.atkIV & 1) + 4 * (starter.defIV & 1) +
                  8 * (starter.speedIV & 1) + 16 * (starter.spAtkIV & 1) +
                  32 * (starter.spDefIV & 1);
    starter.hiddenPowerTypeIndex = typeSum * 15 / 63;
    int powerSum = ((starter.hpIV & 2) >> 1) + 2 * ((starter.atkIV & 2) >> 1) +
                   4 * ((starter.defIV & 2) >> 1) + 8 * ((starter.speedIV & 2) >> 1) +
                   16 * ((starter.spAtkIV & 2) >> 1) + 32 * ((starter.spDefIV & 2) >> 1);
    starter.hiddenPowerPower = (powerSum * 40 / 63) + 30;
  }

  int inline getPidGender(const u8 genderRatio, const u32 pid)
  {
    return genderRatio > (pid & 0xff) ? 1 : 0;
  }
};
