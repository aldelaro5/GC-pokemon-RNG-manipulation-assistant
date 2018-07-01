#pragma once

#include "../BaseRNGSystem.h"

class ColosseumRNGSystem final : public BaseRNGSystem
{
public:
  enum QuickBattleTeamLeader
  {
    Blaziken = 0,
    Entei,
    Swampert,
    Raikou,
    Meganium,
    Suicune,
    Metagross,
    Heracross
  };

  enum QuickBattlePlayerName
  {
    Wes = 0,
    Seth,
    Thomas
  };

  std::string getPrecalcFilenameForSettings(const bool useWii,
                                            const int rtcErrorMarginSeconds) final override;

private:
  u32 rollRNGToBattleMenu(u32 seed, u16* counter = nullptr) final override;
  bool generateBattleTeam(u32& seed, const std::vector<int> criteria) final override;
  std::vector<int> obtainTeamGenerationCritera(u32 seed) final override;
  int getMinFramesAmountNamingScreen() final override;
  int getNbrStartersPrediction() final override;
  std::vector<std::string> getStartersName() final override;
  u32 rollRNGNamingScreenInit(u32 seed) final override;
  u32 rollRNGNamingScreenNext(u32 seed) final override;
  StartersPrediction generateStarterPokemons(u32 seed) final override;
  u32 generatePokemonPID(u32& seed, const u32 hTrainerId, const u32 lTrainerId, const u32 dummyId,
                         u16* counter = nullptr, const s8 wantedGender = -1,
                         const u32 genderRatio = 257, const s8 wantedNature = -1);
  u32 rollRNGToPokemonCompanyLogo(u32 seed, u16* counter = nullptr);
  u32 rollRNGEnteringBattleMenu(u32 seed, u16* counter = nullptr);
};
