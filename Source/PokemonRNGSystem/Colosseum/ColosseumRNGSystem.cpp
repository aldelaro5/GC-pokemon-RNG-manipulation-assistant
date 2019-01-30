#include "ColosseumRNGSystem.h"

#include <array>
#include <string>

// The natures wanted for all Pokemon of all ultimate teams
static const std::array<std::array<u8, 6>, 8> s_natureTeamsData = {
    {{{0x16, 0x15, 0x0f, 0x13, 0x04, 0x04}},
     {{0x0b, 0x08, 0x01, 0x10, 0x10, 0x0C}},
     {{0x02, 0x10, 0x0f, 0x12, 0x0f, 0x03}},
     {{0x10, 0x13, 0x03, 0x16, 0x03, 0x18}},
     {{0x11, 0x10, 0x0f, 0x13, 0x05, 0x04}},
     {{0x0f, 0x11, 0x01, 0x03, 0x13, 0x03}},
     {{0x01, 0x08, 0x03, 0x01, 0x03, 0x03}},
     {{0x03, 0x0a, 0x0f, 0x03, 0x0f, 0x03}}}};

// The genders wanted for all Pokemon of all ultimate teams
static const std::array<std::array<u8, 6>, 8> s_genderTeamsData = {{{{0, 1, 1, 0, 0, 1}},
                                                                    {{2, 1, 0, 0, 1, 0}},
                                                                    {{0, 1, 0, 1, 0, 1}},
                                                                    {{2, 1, 1, 1, 0, 0}},
                                                                    {{0, 0, 0, 0, 0, 1}},
                                                                    {{2, 1, 2, 0, 2, 1}},
                                                                    {{2, 0, 0, 1, 1, 0}},
                                                                    {{1, 0, 1, 0, 1, 0}}}};

// The genders wanted for dummy Pokemon of dummy teams
static const std::array<std::array<u8, 6>, 4> s_genderDummyTeamsData = {
    {{{1, 0, 1, 0, 1, 0}}, {{1, 0, 1, 0, 1, 0}}, {{1, 0, 1, 0, 1, 0}}, {{1, 0, 1, 0, 1, 0}}}};

// The gender ratios of all Pokemon of all ultimate teams
static const std::array<std::array<u8, 6>, 8> s_genderRatioTeamsData = {
    {{{0x1f, 0x7f, 0x7f, 0x7f, 0xbf, 0x7f}},
     {{0xff, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f}},
     {{0x1f, 0x3f, 0x7f, 0x7f, 0x7f, 0x7f}},
     {{0xff, 0xbf, 0x7f, 0x7f, 0x1f, 0x7f}},
     {{0x1f, 0x1f, 0x1f, 0x1f, 0x1f, 0x7f}},
     {{0xff, 0x7f, 0xff, 0x7f, 0xff, 0x7f}},
     {{0xff, 0x1f, 0x3f, 0x7f, 0x7f, 0x3f}},
     {{0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f}}}};

// The gender ratios of all Pokemon of all dummy teams
static const std::array<std::array<u8, 6>, 4> s_genderRatioDummyTeamsData = {
    {{{0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x1f}},
     {{0x1f, 0x7f, 0x1f, 0x7f, 0x7f, 0x7f}},
     {{0xbf, 0x1f, 0x7f, 0x7f, 0x7f, 0x7f}},
     {{0x7f, 0x7f, 0xff, 0x7f, 0x7f, 0x7f}}}};

// Espeon first because it matters the most for speedruns
static const std::array<std::string, 2> s_startersName = {{"Espeon", "Umbreon"}};
static const std::array<std::string, 3> s_secondariesName = {{"Quilava", "Croconaw", "Bayleef"}};

// The gender ratio of the starters.
static const u8 s_genderRatioStarters = 0x1f;
// Order of generation so Umbreon then Espeon
static const std::array<int, 2> s_startersHpBaseStats = {{95, 65}};
static const std::array<int, 2> s_startersStartingLevel = {{26, 25}};

static const int s_nbrPossibleTeams = 8;
static const int s_nbrPossibleTrainerName = 3;

// The minimum possible amount of naming screen animation render calls obtained by TASing the input
// of the naming screen when using the WES preset name.
static const int minNamingScreenAnimRenderCalls = 689;

// The minimum possible amount of frames spending between confirming a new game and confirming the
// preset name WES on the naming screen, obtained by TASing.
static const int minNamingScreenFrames = 107;

std::string ColosseumRNGSystem::getPrecalcFilename()
{
  return "colo.precalc";
}

u32 inline ColosseumRNGSystem::generatePokemonPID(u32& seed, const u32 hTrainerId,
                                                  const u32 lTrainerId, const u32 dummyId,
                                                  u16* counter, const s8 wantedGender,
                                                  const u32 genderRatio, const s8 wantedNature)
{
  u32 id = 0;
  bool goodId = false;
  while (!goodId)
  {
    // A personality ID is generated as candidate, high then low 16 bits
    u32 hId = LCG(seed, counter) >> 16;
    u32 lId = LCG(seed, counter) >> 16;
    id = (hId << 16) | (lId);

    // If we want a gender AND the gender of the pokemon is uncertain
    if (wantedGender != -1 && !(genderRatio == 0xff || genderRatio == 0xfe || genderRatio == 0x00))
    {
      if (wantedGender == 2)
      {
        u8 pokemonIdGender = getPidGender(genderRatio, dummyId);
        u8 idGender = getPidGender(genderRatio, id);
        if (pokemonIdGender != idGender)
          continue;
      }
      else
      {
        u8 idGender = getPidGender(genderRatio, id);
        if (idGender != wantedGender)
          continue;
      }
    }

    // Reroll until we have the correct nature in the perosnality ID
    if (wantedNature != -1 && id % 25 != wantedNature)
      continue;
    // This is apparently preventing a shiny personality ID...in the most convoluted way ever!
    if (!isPidShiny(lTrainerId, hTrainerId, id))
      goodId = true;
  }
  return id;
}

u32 inline ColosseumRNGSystem::rollRNGToPokemonCompanyLogo(u32 seed, u16* counter)
{
  // The game generates 500 numbers of 32 bit in a row, this is 2 LCG call per number which makes
  // 1000 calls
  seed = LCGn(seed, 1000, counter);
  // A personality ID is generated as candidate, low then high 16 bits
  u32 lTrainerId = LCG(seed, counter) >> 16;
  u32 hTrainerId = LCG(seed, counter) >> 16;

  for (int i = 0; i < 2; i++)
  {
    // A personality ID is generated as candidate, high then low 16 bits
    u32 hDummyId = LCG(seed, counter) >> 16;
    u32 lDummyId = LCG(seed, counter) >> 16;
    u32 dummyId = (hDummyId << 16) | (lDummyId);

    // These calls seems to generate some IV and a 50/50, they don't actually matter for the rest of
    // the calls
    LCG(seed, counter);
    LCG(seed, counter);
    LCG(seed, counter);
    generatePokemonPID(seed, hTrainerId, lTrainerId, dummyId, counter, 0, 0x1f);
  }

  // These calls don't matter
  LCG(seed, counter);
  LCG(seed, counter);

  return seed;
}

u32 inline ColosseumRNGSystem::rollRNGEnteringBattleMenu(u32 seed, u16* counter)
{
  seed = LCGn(seed, 120, counter);
  for (int i = 0; i < 4; i++)
  {
    // A trainer ID is generated, low then high 16 bits
    u32 lTrainerId = LCG(seed, counter) >> 16;
    u32 hTrainerId = LCG(seed, counter) >> 16;
    for (int j = 0; j < 7; j++)
    {
      // For some reasons, the last call of all the 24 call to the perosnality ID generation is
      // different
      if (j == 6 && i != 3)
        continue;
      // A personality ID is generated as candidate, high then low 16 bits
      u32 hDummyId = LCG(seed, counter) >> 16;
      u32 lDummyId = LCG(seed, counter) >> 16;
      u32 dummyId = (hDummyId << 16) | (lDummyId);

      // These calls generate the IV and the ability, they don't actually matter for the rest
      // of the calls
      LCG(seed, counter);
      LCG(seed, counter);
      LCG(seed, counter);
      if (j == 6)
      {
        generatePokemonPID(seed, hTrainerId, lTrainerId, dummyId, counter);
      }
      else
      {
        generatePokemonPID(seed, hTrainerId, lTrainerId, dummyId, counter,
                           s_genderDummyTeamsData[i][j], s_genderRatioDummyTeamsData[i][j]);
      }
    }
  }
  return seed;
}

u32 ColosseumRNGSystem::rollRNGToBattleMenu(u32 seed, u16* counter)
{
  seed = rollRNGToPokemonCompanyLogo(seed, counter);
  return rollRNGEnteringBattleMenu(seed, counter);
}

bool ColosseumRNGSystem::generateBattleTeam(u32& seed, const std::vector<int> criteria)
{
  int enemyTeamIndex = (LCG(seed) >> 16) & 7;
  int playerTeamIndex = -1;
  // The game generates a player team index as long as it is not the same as the enemy one
  do
  {
    playerTeamIndex = (LCG(seed) >> 16) & 7;
  } while (enemyTeamIndex == playerTeamIndex);

  if (playerTeamIndex != criteria[0] && criteria[0] != -1)
    return false;

  // The enemy trainer ID is generated as candidate, low then high 16 bits
  u32 lTrainerId = LCG(seed) >> 16;
  u32 hTrainerId = LCG(seed) >> 16;
  // For each enemy pokemon
  for (int i = 0; i < 6; i++)
  {
    // A dummy perosnality ID is generated, high then low 16 bits
    u32 hDummyId = LCG(seed) >> 16;
    u32 lDummyId = LCG(seed) >> 16;
    u32 dummyId = (hDummyId << 16) | (lDummyId);

    // These calls generate the IV and the ability, they don't actually matter for the rest
    // of the calls
    LCG(seed);
    LCG(seed);
    LCG(seed);
    generatePokemonPID(
        seed, hTrainerId, lTrainerId, dummyId, nullptr, s_genderTeamsData[enemyTeamIndex][i],
        s_genderRatioTeamsData[enemyTeamIndex][i], s_natureTeamsData[enemyTeamIndex][i]);
  }

  int playerNameIndex = (LCG(seed) >> 16) % 3;
  if (playerNameIndex != criteria[1] && criteria[1] != -1)
    return false;

#pragma omp critical
  {
    m_lastObtainedCriterias.clear();
    m_lastObtainedCriterias.push_back(playerTeamIndex);
    m_lastObtainedCriterias.push_back(playerNameIndex);
  }

  // The player trainer ID is generated, low then high 16 bits
  lTrainerId = LCG(seed) >> 16;
  hTrainerId = LCG(seed) >> 16;
  // For each player pokemon
  for (int i = 0; i < 6; i++)
  {
    // A dummy personality ID is generated, high then low 16 bits
    u32 hDummyId = LCG(seed) >> 16;
    u32 lDummyId = LCG(seed) >> 16;
    u32 dummyId = (hDummyId << 16) | (lDummyId);

    // These calls generate the IV and the ability, they don't actually matter for the rest
    // of the calls
    LCG(seed);
    LCG(seed);
    LCG(seed);
    generatePokemonPID(
        seed, hTrainerId, lTrainerId, dummyId, nullptr, s_genderTeamsData[playerTeamIndex][i],
        s_genderRatioTeamsData[playerTeamIndex][i], s_natureTeamsData[playerTeamIndex][i]);
  }
  return true;
}

std::string ColosseumRNGSystem::getLastObtainedCriteriasString()
{
  std::string criteriasString = "Name: ";
  switch (m_lastObtainedCriterias[1])
  {
  case Wes:
    criteriasString += "WES";
    break;
  case Seth:
    criteriasString += "SETH";
    break;
  case Thomas:
    criteriasString += "THOMAS";
    break;
  }

  criteriasString += "\nPlayer team leader: ";
  switch (m_lastObtainedCriterias[0])
  {
  case Blaziken:
    criteriasString += "BLAZIKEN";
    break;
  case Entei:
    criteriasString += "ENTEI";
    break;
  case Swampert:
    criteriasString += "SWAMPERT";
    break;
  case Raikou:
    criteriasString += "RAIKOU";
    break;
  case Meganium:
    criteriasString += "MEGANIUM";
    break;
  case Suicune:
    criteriasString += "SUICUNE";
    break;
  case Metagross:
    criteriasString += "METAGROSS";
    break;
  case Heracross:
    criteriasString += "HERACROSS";
    break;
  }

  return criteriasString;
}

std::vector<int> ColosseumRNGSystem::obtainTeamGenerationCritera(u32& seed)
{
  std::vector<int> criteria;

  int enemyTeamIndex = (LCG(seed) >> 16) & 7;
  int playerTeamIndex = -1;
  // The game generates a player team index as long as it is not the same as the enemy one
  do
  {
    playerTeamIndex = (LCG(seed) >> 16) & 7;
  } while (enemyTeamIndex == playerTeamIndex);

  criteria.push_back(playerTeamIndex);
  // The enemy trainer ID is generated as candidate, low then high 16 bits
  u32 lTrainerId = LCG(seed) >> 16;
  u32 hTrainerId = LCG(seed) >> 16;
  // For each enemy pokemon
  for (int i = 0; i < 6; i++)
  {
    // A dummy perosnality ID is generated, high then low 16 bits
    u32 hDummyId = LCG(seed) >> 16;
    u32 lDummyId = LCG(seed) >> 16;
    u32 dummyId = (hDummyId << 16) | (lDummyId);

    // These calls generate the IV and the ability, they don't actually matter for the rest
    // of the calls
    LCG(seed);
    LCG(seed);
    LCG(seed);
    generatePokemonPID(
        seed, hTrainerId, lTrainerId, dummyId, nullptr, s_genderTeamsData[enemyTeamIndex][i],
        s_genderRatioTeamsData[enemyTeamIndex][i], s_natureTeamsData[enemyTeamIndex][i]);
  }

  criteria.push_back((LCG(seed) >> 16) % 3);

  // The player trainer ID is generated, low then high 16 bits
  lTrainerId = LCG(seed) >> 16;
  hTrainerId = LCG(seed) >> 16;
  // For each player pokemon
  for (int i = 0; i < 6; i++)
  {
    // A dummy personality ID is generated, high then low 16 bits
    u32 hDummyId = LCG(seed) >> 16;
    u32 lDummyId = LCG(seed) >> 16;
    u32 dummyId = (hDummyId << 16) | (lDummyId);

    // These calls generate the IV and the ability, they don't actually matter for the rest
    // of the calls
    LCG(seed);
    LCG(seed);
    LCG(seed);
    generatePokemonPID(
        seed, hTrainerId, lTrainerId, dummyId, nullptr, s_genderTeamsData[playerTeamIndex][i],
        s_genderRatioTeamsData[playerTeamIndex][i], s_natureTeamsData[playerTeamIndex][i]);
  }

  return criteria;
}

int ColosseumRNGSystem::getNbrCombinationsFirstTwoCriteria()
{
  return s_nbrPossibleTrainerName * s_nbrPossibleTeams;
}

int ColosseumRNGSystem::firstTwoCriteriaToIndex(const std::vector<int> criteria)
{
  return criteria[0] + s_nbrPossibleTeams * criteria[1];
}

int ColosseumRNGSystem::getMinFramesAmountNamingScreen()
{
  return minNamingScreenFrames;
}

int ColosseumRNGSystem::getNbrStartersPrediction()
{
  return 2;
}

std::vector<std::string> ColosseumRNGSystem::getStartersName()
{
  std::vector<std::string> names;
  for (auto i : s_startersName)
    names.push_back(i);
  return names;
}

std::vector<std::string> ColosseumRNGSystem::getSecondariesName()
{
  std::vector<std::string> names;
  for (auto i : s_secondariesName)
    names.push_back(i);
  return names;
}

u32 ColosseumRNGSystem::rollRNGNamingScreenInit(u32 seed)
{
  // Exhaust all guaranteed calls before considering input and wasted frames.
  for (int i = 0; i < minNamingScreenAnimRenderCalls - 1; i++)
    seed = rollRNGNamingScreenNext(seed);
  return seed;
}

u32 ColosseumRNGSystem::rollRNGNamingScreenNext(u32 seed)
{
  seed = LCG(seed);
  u16 hiSeed = seed >> 16;
  if (static_cast<double>(hiSeed) / 65536.0 < 0.1)
    seed = LCGn(seed, 4);
  return seed;
}

BaseRNGSystem::StartersPrediction ColosseumRNGSystem::generateStarterPokemons(u32 seed)
{
  StartersPrediction result;
  std::vector<PokemonProperties> startersProperties;
  PokemonProperties starter;

  // 500 numbers of 32 bits are generated, but they don't seem to influence anything.
  seed = LCGn(seed, 1000);

  // Every RNG call from now on influence the starters.
  result.startingSeed = seed;

  // The trainer ID is generated, low then high 16 bits
  u32 lBaseId = LCG(seed) >> 16;
  u32 hBaseId = LCG(seed) >> 16;
  result.trainerId = lBaseId;

  // For each starter pokemon
  for (int i = 0; i < 2; i++)
  {
    // A dummy personality ID is generated, high then low 16 bits
    u32 hDummyId = LCG(seed) >> 16;
    u32 lDummyId = LCG(seed) >> 16;
    u32 dummyId = (hDummyId << 16) | (lDummyId);

    extractIVs(starter, seed);
    starter.hpStartingStat =
        (2 * s_startersHpBaseStats[i] + starter.hpIV) * s_startersStartingLevel[i] / 100 +
        s_startersStartingLevel[i] + 10;

    // Ability, doesn't matter
    LCG(seed);

    fillStarterGenHiddenPowerInfo(starter);

    // Generates the true perosnality ID with any nature, but the gender has to be male.
    u32 personalityID =
        generatePokemonPID(seed, hBaseId, lBaseId, dummyId, nullptr, 0, s_genderRatioStarters, -1);
    starter.isShiny = false;
    starter.genderIndex = 0;
    starter.natureIndex = personalityID % 25;
    // This will invert the order because we want Espeon first, but umbreon is generated first
    startersProperties.insert(startersProperties.begin(), starter);
  }
  result.starters = startersProperties;
  return result;
}

void ColosseumRNGSystem::generateAllSecondariesInSearchRange(const u32 postStarterSeed,
                                                             const int secondaryIndex)
{
  if (!(secondaryIndex < POKEMON_COUNT || secondaryIndex >= quilava))
    return;

  BaseRNGSystem::generateAllSecondariesInSearchRange(
      postStarterSeed, secondaryBaseStats[secondaryIndex], secondaryLevel, secondaryGenderRatio,
      secondaryRngAdvanceSearchStart, secondarySearchSeedsAmount);
}

std::array<BaseRNGSystem::StatsRange, 6>
ColosseumRNGSystem::getSecondaryStatsRange(const int secondaryIndex)
{
  switch (secondaryIndex)
  {
  case quilava:
    return quilavaStatsRange;
  case croconaw:
    return croconawStatsRange;
  case bayleef:
    return bayleefStatsRange;
  default:
    return {};
  }
}
