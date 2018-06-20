#include "SPokemonRNG.h"

BaseRNGSystem* SPokemonRNG::m_currentSystem = nullptr;

BaseRNGSystem* SPokemonRNG::getCurrentSystem()
{
  return m_currentSystem;
}

void SPokemonRNG::deleteSystem()
{
  delete m_currentSystem;
}

void SPokemonRNG::setCurrentSystem(BaseRNGSystem* system)
{
  m_currentSystem = system;
}
