#pragma once

#include "../BaseRNGSystem.h"

class GaleDarknessRNGSystem final : public BaseRNGSystem
{
public:
  std::string getPrecalcFilenameForSettings(bool useWii, int rtcErrorMarginSeconds) final override;

private:
  enum class WantedShininess
  {
    notShiny,
    shiny,
    any
  };

  u32 rollRNGToBattleMenu(u32 seed, u16* counter = nullptr) final override;
  bool generateBattleTeam(u32& seed, std::vector<int> criteria) final override;
  int getMinFramesAmountNamingScreen() final override;
  int getNbrStartersPrediction() final override;
  u32 rollRNGNamingScreenInit(u32 seed) final override;
  u32 rollRNGNamingScreenNext(u32 seed) final override;
  startersPrediction generateStarterPokemons(u32 seed) final override;
  u32 generatePokemonPID(u32& seed, u32 hTrainerId, u32 lTrainerId, u32 dummyId,
                                u16* counter = nullptr,
                                WantedShininess shininess = WantedShininess::any,
                                s8 wantedGender = -1, u32 genderRatio = 257, s8 wantedNature = -1);
  std::array<u8, 6> generateEVs(u32& seed, bool allowUnfilledEV, bool endPrematurely,
                                       u16* counter = nullptr);
};
