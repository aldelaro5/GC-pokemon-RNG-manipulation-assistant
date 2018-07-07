#include "BaseRNGSystem.h"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

void BaseRNGSystem::generatePrecalculationFile(unsigned int threadCount,
                                               std::function<void(long)> progressUpdate,
                                               std::function<bool()> shouldCancelNow)
{
  int progressUpdateCurrentBlock = 0;
  int nbrTotalSeedsProcessedCurrentBlock = 0;
  bool hasCancelled = false;
  // Apparently, std::set are so slow compared to periodically std::sort + std::unique that it's
  // much easier to use a vector to maintain RAM comapcted (std::set takes much more RAM for some
  // reaosns)
  std::map<u8, std::vector<u32>> seedsMap;
  int numBlocks = 32;
  int seedPerBlock = static_cast<int>(Common::nbrPossibleSeeds / numBlocks);
  for (int blockId = 0; blockId < numBlocks; blockId++)
  {
    if (shouldCancelNow())
    {
      hasCancelled = true;
      break;
    }

#pragma omp parallel for num_threads(threadCount)
    for (s64 i = seedPerBlock * blockId; i < seedPerBlock + seedPerBlock * blockId; i++)
    {
      if (shouldCancelNow())
      {
        hasCancelled = true;
        continue;
      }
      u32 seed = 0;
      seed = rollRNGToBattleMenu(static_cast<u32>(i));
      std::vector<int> criteria = obtainTeamGenerationCritera(seed);
      u8 fileIndex = firstTwoCriteriaToIndex(criteria);
#pragma omp critical(storeSeed)
      seedsMap[fileIndex].push_back(seed);
#pragma omp critical(progressUpdate)
      {
        nbrTotalSeedsProcessedCurrentBlock++;
        if (nbrTotalSeedsProcessedCurrentBlock == 65536)
        {
          progressUpdateCurrentBlock++;
          nbrTotalSeedsProcessedCurrentBlock = 0;
          progressUpdate(progressUpdateCurrentBlock);
        }
      }
    }
    if (hasCancelled)
      break;

    for (u8 i = 0; i < getNbrCombinationsFirstTwoCriteria(); i++)
    {
      std::sort(seedsMap[i].begin(), seedsMap[i].end());
      auto last = std::unique(seedsMap[i].begin(), seedsMap[i].end());
      seedsMap[i].erase(last, seedsMap[i].end());
    }
  }
  if (!hasCancelled)
  {
    std::string filename = getPrecalcFilename();
    std::ofstream precalcFile(filename, std::ios::binary | std::ios::out);

    for (u8 i = 0; i < getNbrCombinationsFirstTwoCriteria(); i++)
    {
      u32 size = static_cast<u32>(seedsMap[i].size());
      precalcFile.write(reinterpret_cast<const char*>(&size), sizeof(u32));
    }

    for (u8 i = 0; i < getNbrCombinationsFirstTwoCriteria(); i++)
    {
      precalcFile.write(reinterpret_cast<const char*>(seedsMap[i].data()),
                        sizeof(u32) * seedsMap[i].size());
    }

    precalcFile.close();
  }
}

void BaseRNGSystem::seedFinderPass(unsigned int threadCount, const std::vector<int> criteria,
                                   std::vector<u32>& seeds,
                                   std::function<void(long)> progressUpdate,
                                   std::function<bool()> shouldCancelNow)
{
  std::vector<u32> newSeeds;
  if (seeds.size() == 0)
  {
    std::ifstream precalcFile(getPrecalcFilename(), std::ios::binary | std::ios::in);
    bool actuallyUsePrecalc = precalcFile.good();
    if (actuallyUsePrecalc)
    {
      u32* seedsArraySizes = new u32[getNbrCombinationsFirstTwoCriteria()];
      precalcFile.read(reinterpret_cast<char*>(seedsArraySizes),
                       sizeof(u32) * getNbrCombinationsFirstTwoCriteria());

      int seedArrayFileIndex = firstTwoCriteriaToIndex(criteria);
      u32 offsetSeeds = 0;
      for (u8 i = 0; i < seedArrayFileIndex; i++)
        offsetSeeds += seedsArraySizes[i];

      precalcFile.seekg(offsetSeeds * sizeof(u32), std::ios_base::cur);
      seeds.resize(seedsArraySizes[seedArrayFileIndex]);
      precalcFile.read(reinterpret_cast<char*>(seeds.data()),
                       sizeof(u32) * seedsArraySizes[seedArrayFileIndex]);
      delete[] seedsArraySizes;
    }
    return;
  }
  long nbrSeedsSimulatedTotal = 0;
  int seedsSimulatedCurrentBlock = 0;
#pragma omp parallel for num_threads(threadCount)
  for (int i = 0; i < seeds.size(); i++)
  {
    // This is probably the most awkward way to do this, but it can't be done properly with OpenMP
    // because it requires to set an environement variable which cannot be set after the program is
    // started
    if (shouldCancelNow())
      continue;

    u32 seed = 0;
    bool goodSeed = false;
    seed = seeds[i];
    goodSeed = generateBattleTeam(seed, criteria);

    if (goodSeed)
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
