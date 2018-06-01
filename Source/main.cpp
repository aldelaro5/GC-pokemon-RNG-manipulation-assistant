#include "GUI/GUICommon.h"
#include "GUI/MainWindow.h"
#include "PokemonRNGSystem/Colosseum/ColosseumRNGSystem.h"
#include "PokemonRNGSystem/XD/GaleDarknessRNGSystem.h"

#include <iostream>

#include <QApplication>
#include <QMainWindow>

u32 testSeedFinder(BaseRNGSystem& system)
{
  std::vector<u32> seeds;
  bool useWii = false;
  int nbrSecondsRtcErrorMargin = 5;
  bool usePrecalc = true;
  std::vector<int> criteria;
  for (int i = 0; i < 2; i++)
  {
    int input = 0;
    std::cin >> input;
    criteria.push_back(input);
  }
  system.seedFinder(criteria, seeds, useWii, nbrSecondsRtcErrorMargin, usePrecalc, true);
  while (seeds.size() > 1)
  {
    criteria.clear();
    for (int i = 0; i < 2; i++)
    {
      int input = 0;
      std::cin >> input;
      criteria.push_back(input);
    }
    system.seedFinder(criteria, seeds, useWii, nbrSecondsRtcErrorMargin, false, false);
  }
  if (seeds.size() == 1)
  {
    std::cout << "Your seed is " << std::hex << seeds.front();
  }
  else if (seeds.size() == 0)
  {
    std::cout << "Couldn't find the seed" << std::endl;
    std::cout << "\nPress enter to exit";
    std::cin.ignore();
    return 0;
  }
  std::cout << "\nPress enter to continue";
  std::cin.ignore();
  return seeds.front();
}

void testPredictor(BaseRNGSystem& system, u32 seed)
{
  std::vector<BaseRNGSystem::StartersPrediction> predictions =
      system.predictStartersForNbrSeconds(seed, 10);

  for (auto i : predictions)
  {
    std::cout << "Frame " << std::dec << i.frameNumber << " (" << i.frameNumber / 60.0
              << " seconds): " << std::uppercase << std::hex << i.startingSeed << " " << std::dec;
    for (auto j : i.starters)
    {
      std::cout << j.hpIV << " " << j.atkIV << " " << j.defIV << " " << j.spAtkIV << " "
                << j.spDefIV << " " << j.speedIV << " "
                << GUICommon::naturesStr[j.natureIndex].toStdString() << " "
                << (j.isShiny ? "Yes" : "No") << " "
                << GUICommon::genderStr[j.genderIndex].toStdString() << "   ";
    }
    std::cout << std::endl;
  }
}

int main(int argc, char** argv)
{
  QApplication app(argc, argv);
  MainWindow window;
  window.show();
  return app.exec();
  /*ColosseumRNGSystem system;
  system.precalculateNbrRollsBeforeTeamGeneration(false, 5);
  ColosseumRNGSystem system2;
  system2.precalculateNbrRollsBeforeTeamGeneration(false, 5);
  std::cout << "done";
  u32 seed = testSeedFinder(system);
  std::cin.ignore();
  testPredictor(system, seed);
  std::cin.ignore();
  return 0;*/
}
