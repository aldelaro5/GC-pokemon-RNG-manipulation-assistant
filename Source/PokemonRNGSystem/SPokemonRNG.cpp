#include "SPokemonRNG.h"

#include "Colosseum/ColosseumRNGSystem.h"
#include "XD/GaleDarknessRNGSystem.h"

BaseRNGSystem* SPokemonRNG::m_instance = nullptr;
SPokemonRNG::GCPokemonGame SPokemonRNG::m_currentGame = SPokemonRNG::GCPokemonGame::None;

BaseRNGSystem* SPokemonRNG::getInstance()
{
  if (m_instance == nullptr)
    switchGame(m_currentGame);
  return m_instance;
}

void SPokemonRNG::switchGame(SPokemonRNG::GCPokemonGame game)
{
  switch (m_currentGame)
  {
  case GCPokemonGame::Colosseum:
    m_instance = new ColosseumRNGSystem();
    break;
  case GCPokemonGame::XD:
    m_instance = new GaleDarknessRNGSystem();
    break;
  }
}
