#include "SPokemonRNG.h"

#include "../PokemonRNGSystem/Colosseum/ColosseumRNGSystem.h"
#include "../PokemonRNGSystem/XD/GaleDarknessRNGSystem.h"

SPokemonRNG* SPokemonRNG::m_instance = nullptr;
SPokemonRNG::GCPokemonGame SPokemonRNG::m_currentGame = SPokemonRNG::GCPokemonGame::None;
BaseRNGSystem* SPokemonRNG::m_system = nullptr;

SPokemonRNG::SPokemonRNG()
{
}

SPokemonRNG* SPokemonRNG::getInstance()
{
  if (m_instance == nullptr)
    m_instance = new SPokemonRNG();
  return m_instance;
}

BaseRNGSystem* SPokemonRNG::getSystem()
{
  return m_system;
}

void SPokemonRNG::switchGame(SPokemonRNG::GCPokemonGame game)
{
  if (m_currentGame == game || game == GCPokemonGame::None)
    return;

  m_currentGame = game;
  switch (m_currentGame)
  {
  case GCPokemonGame::Colosseum:
    m_system = new ColosseumRNGSystem();
    emit onSwitchGame();
    break;
  case GCPokemonGame::XD:
    m_system = new GaleDarknessRNGSystem();
    emit onSwitchGame();
    break;
  }
}
