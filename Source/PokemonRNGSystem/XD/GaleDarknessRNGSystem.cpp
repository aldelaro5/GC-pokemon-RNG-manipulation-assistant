#include "GaleDarknessRNGSystem.h"

#include <array>

// HP stat of the quick battle team pokemon without IV and EV at level 100
static const std::array<std::array<int, 2>, 10> s_quickBattleTeamMaxBaseHPStat = {{{{322, 340}},
                                                                                   {{310, 290}},
                                                                                   {{210, 620}},
                                                                                   {{320, 230}},
                                                                                   {{310, 310}},
                                                                                   {{290, 310}},
                                                                                   {{290, 270}},
                                                                                   {{290, 250}},
                                                                                   {{320, 270}},
                                                                                   {{270, 230}}}};
static const int eeveeGenderRatio = 0x1f;
static const int eeveeBaseHpStat = 55;
static const int eeveeStartingLevel = 10;

static const int s_nbrPossiblePlayerTeams = 5;
static const int s_nbrPossibleEnemyTeams = 5;

// The minimum possible amount of naming screen animation render calls obtained by TASing the input
// of the naming screen when using a custom name.
static const int minNamingScreenAnimRenderCalls = 725;

// The minimum possible amount of frames spending between confirming a new game and confirming
// a custom name on the naming screen, obtained by TASing.
static const int minNamingScreenFrames = 116;

std::string GaleDarknessRNGSystem::getPrecalcFilename()
{
  return "xd.precalc";
}

u32 inline GaleDarknessRNGSystem::generatePokemonPID(u32& seed, const u32 hTrainerId,
                                                     const u32 lTrainerId, const u32 dummyId,
                                                     u16* counter, const WantedShininess shininess,
                                                     const s8 wantedGender, const u32 genderRatio,
                                                     const s8 wantedNature)
{
  u32 pid = 0;
  bool goodId = false;
  int nbrPidAttemptLeft = 3145728;
  while (nbrPidAttemptLeft > 0)
  {
    // A personality ID is generated as candidate, high then low 16 bits
    u32 hId = LCG(seed, counter) >> 16;
    u32 lId = LCG(seed, counter) >> 16;
    pid = (hId << 16) | (lId);
    nbrPidAttemptLeft--;

    // If we want a gender AND the gender of the pokemon is uncertain
    if (wantedGender != -1 && !(genderRatio == 0xff || genderRatio == 0xfe || genderRatio == 0x00))
    {
      if (wantedGender == 2)
      {
        u8 pokemonIdGender = genderRatio > (dummyId & 0xff) ? 1 : 0;
        u8 idGender = genderRatio > (pid & 0xff) ? 1 : 0;
        if (pokemonIdGender != idGender)
          continue;
      }
      else
      {
        u8 idGender = genderRatio > (pid & 0xff) ? 1 : 0;
        if (idGender != wantedGender)
          continue;
      }
    }

    // Reroll until we have the correct nature in the perosnality ID
    if (wantedNature != -1 && pid % 25 != wantedNature)
      continue;

    if (shininess != WantedShininess::any)
    {
      if (shininess == WantedShininess::notShiny)
      {
        if (!isPidShiny(lTrainerId, hTrainerId, pid))
          return pid;
      }
      else if (shininess == WantedShininess::shiny)
      {
        if (isPidShiny(lTrainerId, hTrainerId, pid))
          return pid;
      }
    }
    else
    {
      return pid;
    }
  }
  return pid;
}

std::array<u8, 6> inline GaleDarknessRNGSystem::generateEVs(u32& seed, const bool allowUnfilledEV,
                                                            const bool endPrematurely, u16* counter)
{
  std::array<u8, 6> EVs = {0};
  int sumEV = 0;
  for (int j = 0; j < 101; j++)
  {
    for (int k = 0; k < 6; k++)
    {
      EVs[k] += (LCG(seed, counter) >> 16) & 0xFF;
      sumEV += EVs[k];
    }
    if (allowUnfilledEV)
    {
      if (sumEV <= 510)
      {
        return EVs;
      }
      else
      {
        if (j >= 100)
          continue;
        EVs.fill(0);
        sumEV = 0;
        continue;
      }
    }
    else if (sumEV == 510)
    {
      return EVs;
    }
    else if (sumEV <= 490)
    {
      sumEV = 0;
      continue;
    }
    else if (sumEV >= 530)
    {
      if (j >= 100)
        continue;
      EVs.fill(0);
      sumEV = 0;
      continue;
    }
    else
    {
      break;
    }
  }
  bool goodSum = false;
  while (!goodSum && !endPrematurely)
  {
    for (int k = 0; k < 6; k++)
    {
      if (sumEV < 510)
      {
        if (EVs[k] < 255)
        {
          EVs[k]++;
          sumEV++;
        }
      }
      else if (sumEV > 510)
      {
        if (EVs[k] != 0)
        {
          EVs[k]--;
          sumEV--;
        }
      }

      if (sumEV == 510)
        goodSum = true;
    }
  }
  return EVs;
}

u32 GaleDarknessRNGSystem::rollRNGToBattleMenu(u32 seed, u16* counter)
{
  // Before getting to the quick battle menu, the reason this is deterministic is because the only
  // thing that is consequential is a dummy pokemon generation, but there's no criteria on the pid
  // meaning the first pid generated will always be the one used
  seed = LCGn(seed, 1139, counter);
  // Some dummy team gen going on...
  for (int i = 0; i < 4; i++)
  {
    // The player trainer ID is generated, low then high 16 bits
    u32 lTrainerId = LCG(seed, counter) >> 16;
    u32 hTrainerId = LCG(seed, counter) >> 16;
    for (int j = 0; j < 2; j++)
    {
      // Dummy personality ID (doesn't matter)
      LCG(seed, counter);
      LCG(seed, counter);
      // HP, ATK, DEF IV
      LCG(seed, counter);
      // SPEED, SPATK, SPDEF IV
      LCG(seed, counter);
      // Ability
      LCG(seed, counter);
      generatePokemonPID(seed, hTrainerId, lTrainerId, 0, counter, WantedShininess::notShiny);
      generateEVs(seed, true, true, counter);
    }
  }
  return seed;
}

bool GaleDarknessRNGSystem::generateBattleTeam(u32& seed, const std::vector<int> criteria)
{
  // Player trainer name generation
  LCG(seed);
  // Player team index
  int playerTeamIndex = (LCG(seed) >> 16) % 5;
  if (playerTeamIndex != criteria[0] && criteria[0] != -1)
    return false;
  // Enemy team index
  int enemyTeamIndex = (LCG(seed) >> 16) % 5;
  if (enemyTeamIndex != criteria[1] && criteria[1] != -1)
    return false;
  // modulo 6 ???
  LCG(seed);

  // Enemy gen

  // The player trainer ID is generated, low then high 16 bits
  u32 lTrainerId = LCG(seed) >> 16;
  u32 hTrainerId = LCG(seed) >> 16;
  // Pokemon gen
  for (int i = 0; i < 2; i++)
  {
    // Dummy personality ID (doesn't matter)
    LCG(seed);
    LCG(seed);
    // HP, ATK, DEF IV
    LCG(seed);
    u32 hpIV = (seed >> 16) & 31;
    // SPEED, SPATK, SPDEF IV
    LCG(seed);
    // Ability
    LCG(seed);
    generatePokemonPID(seed, hTrainerId, lTrainerId, 0, nullptr, WantedShininess::notShiny);
    std::array<u8, 6> EVs = generateEVs(seed, false, false, nullptr);
    // Here, we have the true stats, so let's check the HP
    if (EVs[0] / 4 + hpIV + s_quickBattleTeamMaxBaseHPStat[enemyTeamIndex + 5][i] !=
            criteria[4 + i] &&
        criteria[4 + i] != -1)
      return false;
  }

  // modulo 6 ???
  LCG(seed);

  // Player gen

  // The player trainer ID is generated, low then high 16 bits
  lTrainerId = LCG(seed) >> 16;
  hTrainerId = LCG(seed) >> 16;
  // Pokemon gen
  for (int i = 0; i < 2; i++)
  {
    // Dummy personality ID (doesn't matter)
    LCG(seed);
    LCG(seed);
    // HP, ATK, DEF IV
    LCG(seed);
    u32 hpIV = (seed >> 16) & 31;
    // SPEED, SPATK, SPDEF IV
    LCG(seed);
    // Ability
    LCG(seed);
    generatePokemonPID(seed, hTrainerId, lTrainerId, 0, nullptr, WantedShininess::notShiny);
    std::array<u8, 6> EVs = generateEVs(seed, false, false, nullptr);
    // Here, we have the true stats, so let's check the HP
    if (EVs[0] / 4 + hpIV + s_quickBattleTeamMaxBaseHPStat[playerTeamIndex][i] != criteria[2 + i] &&
        criteria[2 + i] != -1)
      return false;
  }
  return true;
}

std::vector<int> GaleDarknessRNGSystem::obtainTeamGenerationCritera(u32& seed)
{
  std::vector<int> criteria;
  // Player trainer name generation
  LCG(seed);
  // Player team index
  criteria.push_back((LCG(seed) >> 16) % 5);
  // Enemy team index
  criteria.push_back((LCG(seed) >> 16) % 5);
  // modulo 6 ???
  LCG(seed);

  // Enemy gen

  // The player trainer ID is generated, low then high 16 bits
  u32 lTrainerId = LCG(seed) >> 16;
  u32 hTrainerId = LCG(seed) >> 16;
  // Pokemon gen
  for (int i = 0; i < 2; i++)
  {
    // Dummy personality ID (doesn't matter)
    LCG(seed);
    LCG(seed);
    // HP, ATK, DEF IV
    LCG(seed);
    u32 hpIV = (seed >> 16) & 31;
    // SPEED, SPATK, SPDEF IV
    LCG(seed);
    // Ability
    LCG(seed);
    generatePokemonPID(seed, hTrainerId, lTrainerId, 0, nullptr, WantedShininess::notShiny);
    generateEVs(seed, false, false, nullptr);
  }

  // modulo 6 ???
  LCG(seed);

  // Player gen

  // The player trainer ID is generated, low then high 16 bits
  lTrainerId = LCG(seed) >> 16;
  hTrainerId = LCG(seed) >> 16;
  // Pokemon gen
  for (int i = 0; i < 2; i++)
  {
    // Dummy personality ID (doesn't matter)
    LCG(seed);
    LCG(seed);
    // HP, ATK, DEF IV
    LCG(seed);
    u32 hpIV = (seed >> 16) & 31;
    // SPEED, SPATK, SPDEF IV
    LCG(seed);
    // Ability
    LCG(seed);
    generatePokemonPID(seed, hTrainerId, lTrainerId, 0, nullptr, WantedShininess::notShiny);
    generateEVs(seed, false, false, nullptr);
  }
  return criteria;
}

int GaleDarknessRNGSystem::getNbrCombinationsFirstTwoCriteria()
{
  return s_nbrPossibleEnemyTeams * s_nbrPossiblePlayerTeams;
}

int GaleDarknessRNGSystem::firstTwoCriteriaToIndex(const std::vector<int> criteria)
{
  return criteria[0] * s_nbrPossiblePlayerTeams + criteria[1];
}

int GaleDarknessRNGSystem::getMinFramesAmountNamingScreen()
{
  return minNamingScreenFrames;
}

int GaleDarknessRNGSystem::getNbrStartersPrediction()
{
  return 1;
}

std::vector<std::string> GaleDarknessRNGSystem::getStartersName()
{
  std::vector<std::string> name;
  name.push_back("Eevee");
  return name;
}

u32 GaleDarknessRNGSystem::rollRNGNamingScreenInit(u32 seed)
{
  LCG(seed);
  LCG(seed);

  // Exhaust all guaranteed calls before considering input and wasted frames.
  for (int i = 0; i < minNamingScreenAnimRenderCalls - 1; i++)
    seed = rollRNGNamingScreenNext(seed);
  return seed;
}

u32 GaleDarknessRNGSystem::rollRNGNamingScreenNext(u32 seed)
{
  seed = LCG(seed);
  u16 hiSeed = seed >> 16;
  if (static_cast<double>(hiSeed) / 65536.0 < 0.1)
    seed = LCGn(seed, 4);
  return seed;
}

BaseRNGSystem::StartersPrediction GaleDarknessRNGSystem::generateStarterPokemons(u32 seed)
{
  StartersPrediction result;
  std::vector<StarterGen> startersProperties;
  StarterGen starter;

  // 500 numbers of 32 bits are generated, but they don't seem to influence anything.
  seed = LCGn(seed, 1000);

  // Every RNG call from now on influence the starters.
  result.startingSeed = seed;

  // The trainer ID is generated, low then high 16 bits
  u32 lTrainerId = LCG(seed) >> 16;
  u32 hTrainerId = LCG(seed) >> 16;
  result.trainerId = lTrainerId;

  // Dummy personality ID (doesn't matter)
  LCG(seed);
  LCG(seed);

  // HP, ATK, DEF IV
  LCG(seed);
  starter.hpIV = (seed >> 16) & 31;
  starter.hpStartingStat =
      (2 * eeveeBaseHpStat + starter.hpIV) * eeveeStartingLevel / 100 + eeveeStartingLevel + 10;
  starter.atkIV = (seed >> 21) & 31;
  starter.defIV = (seed >> 26) & 31;
  // SPEED, SPATK, SPDEF IV
  LCG(seed);
  starter.speedIV = (seed >> 16) & 31;
  starter.spAtkIV = (seed >> 21) & 31;
  starter.spDefIV = (seed >> 26) & 31;
  // Ability, doesn't matter
  LCG(seed);

  fillStarterGenHiddenPowerInfo(starter);

  // Generates the true perosnality ID with no criteria
  u32 personalityID = generatePokemonPID(seed, hTrainerId, lTrainerId, 0, nullptr);
  starter.isShiny = isPidShiny(lTrainerId, hTrainerId, personalityID);
  starter.genderIndex = getPidGender(eeveeGenderRatio, personalityID);
  starter.natureIndex = personalityID % 25;
  startersProperties.push_back(starter);

  result.starters = startersProperties;
  return result;
}
