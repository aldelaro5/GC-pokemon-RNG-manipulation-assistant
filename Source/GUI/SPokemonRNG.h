#pragma once

#include "../PokemonRNGSystem/BaseRNGSystem.h"

#include <QObject>

// Singleton wrapper arround BaseRNGSystem, to use from UI
class SPokemonRNG : public QObject
{
  Q_OBJECT

public:
  enum class GCPokemonGame
  {
    Colosseum,
    XD,
    None
  };

  static SPokemonRNG* getInstance();

  BaseRNGSystem* getSystem();
  void switchGame(GCPokemonGame game);

signals:
  void onSwitchGame();

private:
  SPokemonRNG();
  static SPokemonRNG* m_instance;
  static BaseRNGSystem* m_system;
  static GCPokemonGame m_currentGame;
};
