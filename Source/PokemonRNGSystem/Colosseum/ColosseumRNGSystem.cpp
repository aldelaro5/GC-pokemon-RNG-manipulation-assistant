#include "ColosseumRNGSystem.h"

#include "../../Common/Common.h"

/*
0 WES
1 SETH
2 THOMAS

0 BLAZIKEN
1 ENTEI
2 SWAMPERT
3 RAIKOU
4 MEGANIUM
5 SUICUNE
6 METAGROSS
7 HERACROSS
*/

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

// The gender ratio of the starters.
static const std::array<u8, 2> s_genderRatioStarters = {{0x1f, 0x1f}};

static const std::array<std::string, 2> s_startersName = {{"Umbreon", "Espeon"}};

// The minimum possible amount of naming screen animation render calls obtained by TASing the input
// of the naming screen when using the WES preset name.
static const int minNamingScreenAnimRenderCalls = 689;

// The minimum possible amount of frames spending between confirming a new game and confirming the
// preset name WES on the naming screen, obtained by TASing.
static const int minNamingScreenFrames = 107;

std::string ColosseumRNGSystem::getPrecalcFilenameForSettings(bool useWii,
                                                              int rtcErrorMarginSeconds)
{
  return BaseRNGSystem::getPrecalcFilenameForSettings(useWii, rtcErrorMarginSeconds) + ".colo";
}

u32 inline ColosseumRNGSystem::generatePokemonPID(u32& seed, u32 hTrainerId, u32 lTrainerId,
                                                  u32 dummyId, u16* counter, s8 wantedGender,
                                                  u32 genderRatio, s8 wantedNature)
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

u32 inline ColosseumRNGSystem::rollRNGToBattleMenu(u32 seed, u16* counter)
{
  seed = rollRNGToPokemonCompanyLogo(seed, counter);
  return rollRNGEnteringBattleMenu(seed, counter);
}

bool inline ColosseumRNGSystem::generateBattleTeam(u32& seed, std::vector<int> criteria)
{
  int enemyTeamIndex = (LCG(seed) >> 16) & 7;
  int playerTeamIndex = -1;
  // The game generates a player team index as long as it is not the same as the enemy one
  do
  {
    playerTeamIndex = (LCG(seed) >> 16) & 7;
  } while (enemyTeamIndex == playerTeamIndex);

  if (playerTeamIndex != criteria[0])
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

  if ((LCG(seed) >> 16) % 3 != criteria[1])
    return false;

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

int ColosseumRNGSystem::getMinFramesAmountNamingScreen()
{
  return minNamingScreenFrames;
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

BaseRNGSystem::startersPrediction ColosseumRNGSystem::generateStarterPokemons(u32 seed)
{
  startersPrediction result;
  std::vector<starterGen> startersProperties;
  starterGen starter;

  // 500 numbers of 32 bits are generated, but they don't seem to influence anything.
  seed = LCGn(seed, 1000);

  // Every RNG call from now on influence the starters.
  result.startingSeed = seed;

  // The trainer ID is generated, low then high 16 bits
  u32 lBaseId = LCG(seed) >> 16;
  u32 hBaseId = LCG(seed) >> 16;
  // For each starter pokemon
  for (int i = 0; i < 2; i++)
  {
    // A dummy personality ID is generated, high then low 16 bits
    u32 hDummyId = LCG(seed) >> 16;
    u32 lDummyId = LCG(seed) >> 16;
    u32 dummyId = (hDummyId << 16) | (lDummyId);

    // HP, ATK, DEF IV
    LCG(seed);
    starter.hpIV = (seed >> 16) & 31;
    starter.atkIV = (seed >> 21) & 31;
    starter.defIV = (seed >> 26) & 31;
    // SPEED, SPATK, SPDEF IV
    LCG(seed);
    starter.speedIV = (seed >> 16) & 31;
    starter.spAtkIV = (seed >> 21) & 31;
    starter.spDefIV = (seed >> 26) & 31;
    // Ability, doesn't matter
    LCG(seed);
    // Generates the true perosnality ID with any nature, but the gender has to be male.
    u32 personalityID = generatePokemonPID(seed, hBaseId, lBaseId, dummyId, nullptr, 0,
                                           s_genderRatioStarters[i], -1);
    starter.isShiny = false;
    starter.genderIndex = 0;
    starter.natureIndex = personalityID % 25;
    starter.name = s_startersName[i];
    startersProperties.push_back(starter);
  }
  result.starters = startersProperties;
  return result;
}
