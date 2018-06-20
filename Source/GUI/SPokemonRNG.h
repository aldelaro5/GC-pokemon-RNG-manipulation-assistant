#pragma once

#include "../PokemonRNGSystem/BaseRNGSystem.h"

class SPokemonRNG
{
public:
  static BaseRNGSystem* getCurrentSystem();
  static void setCurrentSystem(BaseRNGSystem* system);
  static void deleteSystem();

private:
  static BaseRNGSystem* m_currentSystem;
};
