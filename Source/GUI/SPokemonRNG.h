#pragma once

#include <QObject>

#include "../PokemonRNGSystem/BaseRNGSystem.h"
#include "GUICommon.h"

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

  BaseRNGSystem* getSystem() const;
  void switchGame(const GCPokemonGame game);

signals:
  void onSwitchGame(const GUICommon::gameSelection game);

private:
  SPokemonRNG();
  static SPokemonRNG* m_instance;
  static BaseRNGSystem* m_system;
  static GCPokemonGame m_currentGame;
};
