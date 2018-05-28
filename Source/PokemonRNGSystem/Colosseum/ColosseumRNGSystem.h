#pragma once

#include "../BaseRNGSystem.h"

class ColosseumRNGSystem final : public BaseRNGSystem
{
public:
  std::string getPrecalcFilenameForSettings(bool useWii, int rtcErrorMarginSeconds) final override;

private:
  u32 rollRNGToBattleMenu(u32 seed, u16* counter = nullptr) final override;
  bool generateBattleTeam(u32& seed, std::vector<int> criteria) final override;
  int getMinFramesAmountNamingScreen() final override;
  u32 rollRNGNamingScreenInit(u32 seed) final override;
  u32 rollRNGNamingScreenNext(u32 seed) final override;
  startersPrediction generateStarterPokemons(u32 seed) final override;
  u32 generatePokemonPID(u32& seed, u32 hTrainerId, u32 lTrainerId, u32 dummyId,
                                u16* counter = nullptr, s8 wantedGender = -1, u32 genderRatio = 257,
                                s8 wantedNature = -1);
  u32 rollRNGToPokemonCompanyLogo(u32 seed, u16* counter = nullptr);
  u32 rollRNGEnteringBattleMenu(u32 seed, u16* counter = nullptr);
};
