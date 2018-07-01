#pragma once

#include "../BaseRNGSystem.h"

class GaleDarknessRNGSystem final : public BaseRNGSystem
{
public:
  enum BattleNowTeamLeaderPlayer
  {
    Mewtwo = 0,
    Mew,
    Deoxys,
    Rayquaza,
    Jirachi
  };

  enum BattleNowTeamLeaderEnemy
  {
    Articuno = 0,
    Zapidos,
    Moltres,
    Kangaskhan,
    Latias
  };

  std::string getPrecalcFilenameForSettings(const bool useWii,
                                            const int rtcErrorMarginSeconds) final override;

private:
  enum class WantedShininess
  {
    notShiny,
    shiny,
    any
  };

  u32 rollRNGToBattleMenu(u32 seed, u16* counter = nullptr) final override;
  bool generateBattleTeam(u32& seed, const std::vector<int> criteria) final override;
  std::vector<int> obtainTeamGenerationCritera(u32 seed) final override;
  int getMinFramesAmountNamingScreen() final override;
  int getNbrStartersPrediction() final override;
  std::vector<std::string> getStartersName() final override;
  u32 rollRNGNamingScreenInit(u32 seed) final override;
  u32 rollRNGNamingScreenNext(u32 seed) final override;
  StartersPrediction generateStarterPokemons(const u32 seed) final override;
  u32 generatePokemonPID(u32& seed, const u32 hTrainerId, const u32 lTrainerId, const u32 dummyId,
                         u16* counter = nullptr,
                         const WantedShininess shininess = WantedShininess::any,
                         const s8 wantedGender = -1, const u32 genderRatio = 257,
                         const s8 wantedNature = -1);
  std::array<u8, 6> generateEVs(u32& seed, const bool allowUnfilledEV, const bool endPrematurely,
                                u16* counter = nullptr);
};
