#pragma once

#include "../PokemonRNGSystem/BaseRNGSystem.h"

// Singleton wrapper arround BaseRNGSystem, to use from UI
class SPokemonRNG
{
public:
  enum class GCPokemonGame
  {
    Colosseum,
    XD,
    None
  };

  static BaseRNGSystem* getInstance();
  static void switchGame(GCPokemonGame game);

private:
  SPokemonRNG();
  static BaseRNGSystem* m_instance;
  static GCPokemonGame m_currentGame;
};
