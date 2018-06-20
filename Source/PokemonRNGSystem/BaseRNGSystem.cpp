#include "BaseRNGSystem.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <sstream>

std::string BaseRNGSystem::getPrecalcFilenameForSettings(const bool useWii,
                                                         const int rtcErrorMarginSeconds)
{
  std::stringstream ss;
  ss << (useWii ? "Wii" : "GC");
  ss << '-';
  ss << rtcErrorMarginSeconds;
  return ss.str();
}

BaseRNGSystem::seedRange BaseRNGSystem::getRangeForSettings(const bool useWii,
                                                            const int rtcErrorMarginSeconds)
{
  seedRange range;
  int ticksPerSecond = useWii ? Common::ticksPerSecondWii : Common::ticksPerSecondGC;
  if (rtcErrorMarginSeconds == 0)
  {
    range.min = 0;
    range.max = 0x100000000;
  }
  else
  {
    range.min = useWii ? minRTCTicksToBootWii : minRTCTicksToBootGC;
    range.max = range.min + ticksPerSecond * rtcErrorMarginSeconds;
  }
  return range;
}

size_t BaseRNGSystem::getPracalcFileSize(const bool useWii, const int rtcErrorMarginSeconds)
{
  seedRange range = getRangeForSettings(useWii, rtcErrorMarginSeconds);
  return (range.max - range.min) * sizeof(u16);
}

void BaseRNGSystem::precalculateNbrRollsBeforeTeamGeneration(
    const bool useWii, const int rtcErrorMarginSeconds, std::function<void(int)> progressUpdate,
    std::function<bool()> shouldCancelNow)
{
  std::string filename = getPrecalcFilenameForSettings(useWii, rtcErrorMarginSeconds);
  std::ofstream precalcFile(filename, std::ios::binary | std::ios::out);
  seedRange range = getRangeForSettings(useWii, rtcErrorMarginSeconds);
  int nbrSeedsPrecalculatedTotal = 0;
  int seedsPrecalculatedCurrentBlock = 0;
  bool hasCancelled = false;
  for (s64 i = range.min; i < range.max; i++)
  {
    if (shouldCancelNow())
    {
      hasCancelled = true;
      break;
    }

    u32 seed = 0;
    u16 counter = 0;
    seed = rollRNGToBattleMenu(static_cast<u32>(i), &counter);
    u16* ptrCounter = &counter;
    precalcFile.write(reinterpret_cast<const char*>(ptrCounter), sizeof(u16));
    nbrSeedsPrecalculatedTotal++;
    seedsPrecalculatedCurrentBlock++;
    if (seedsPrecalculatedCurrentBlock >= 10000)
    {
      progressUpdate(nbrSeedsPrecalculatedTotal);
      seedsPrecalculatedCurrentBlock = 0;
    }
  }
  precalcFile.close();
  if (hasCancelled)
    std::remove(filename.c_str());
}

void BaseRNGSystem::seedFinder(const std::vector<int> criteria, std::vector<u32>& seeds,
                               const bool useWii, const int rtcErrorMarginSeconds,
                               const bool usePrecalc, std::function<void(int)> progressUpdate,
                               std::function<bool()> shouldCancelNow)
{
  std::vector<u32> newSeeds;
  seedRange range;
  range.max = seeds.size();
  if (seeds.size() == 0)
    range = getRangeForSettings(useWii, rtcErrorMarginSeconds);
  std::ifstream precalcFile(getPrecalcFilenameForSettings(useWii, rtcErrorMarginSeconds),
                            std::ios::binary | std::ios::in);
  bool actuallyUsePrecalc = usePrecalc && precalcFile.good();
  u16* precalc = nullptr;
  if (actuallyUsePrecalc)
  {
    precalc = new u16[range.max - range.min];
    precalcFile.read(reinterpret_cast<char*>(precalc), (range.max - range.min) * sizeof(u16));
  }
  int nbrSeedsSimulatedTotal = 0;
  int seedsSimulatedCurrentBlock = 0;
  s64 n = 1;
#pragma omp parallel for
  for (s64 i = range.min; i < range.max; i += n)
  {
    // This is probably the most awkward way to do this, but it can't be done properly with OpenMP
    // because it requires to set an environement variable which cannot be set after the program is
    // started
    if (shouldCancelNow())
    {
      n = range.max;
      continue;
    }

    u32 seed = 0;
    if (seeds.size() == 0)
    {
      if (actuallyUsePrecalc)
      {
        u16 nbrRngCalls = 0;
        std::memcpy(&nbrRngCalls, precalc + (i - range.min), sizeof(u16));
        seed = LCGn(static_cast<u32>(i), nbrRngCalls);
      }
      else
      {
        seed = rollRNGToBattleMenu(static_cast<u32>(i));
      }
    }
    else
    {
      seed = seeds[i];
    }
    if (generateBattleTeam(seed, criteria))
#pragma omp critical(addSeed)
      newSeeds.push_back(seed);
#pragma omp critical(progress)
    {
      nbrSeedsSimulatedTotal++;
      seedsSimulatedCurrentBlock++;
      if (seedsSimulatedCurrentBlock >= 10000)
      {
        progressUpdate(nbrSeedsSimulatedTotal);
        seedsSimulatedCurrentBlock = 0;
      }
    }
  }
  std::swap(newSeeds, seeds);
  // As the number of calls differs depending on the starting seed, it may happen that some seeds
  // may end up being the same as another one, this gets rid of the duplicates so the program can
  // only have one unique result at the end
  std::sort(seeds.begin(), seeds.end());
  auto last = std::unique(seeds.begin(), seeds.end());
  seeds.erase(last, seeds.end());
  delete[] precalc;
}

std::vector<BaseRNGSystem::StartersPrediction>
BaseRNGSystem::predictStartersForNbrSeconds(u32 seed, const int nbrSeconds)
{
  std::vector<StartersPrediction> predictionsResult;
  seed = rollRNGNamingScreenInit(seed);

  for (int i = getMinFramesAmountNamingScreen();
       i < getMinFramesAmountNamingScreen() + nbrSeconds * pollingRateNamingScreenPerSec; i++)
  {
    seed = rollRNGNamingScreenNext(seed);
    BaseRNGSystem::StartersPrediction prediction = generateStarterPokemons(seed);
    prediction.frameNumber = i;
    predictionsResult.push_back(prediction);
  }
  return predictionsResult;
}
