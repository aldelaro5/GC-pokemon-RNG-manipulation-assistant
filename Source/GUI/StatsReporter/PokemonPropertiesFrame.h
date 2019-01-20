#pragma once

#include <QFrame>
#include <QLabel>
#include <QPixmap>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"

// This frame is only for presenting the different properties of the starters and seocndaries
// Pokémon; the caller is responsible for providing the data.
class PokemonPropertiesFrame : public QFrame
{
public:
  PokemonPropertiesFrame(const QString pokemonName, const QPixmap pokemonIcon,
                         QWidget* parent = nullptr);
  void setPokemonName(const QString name);
  void setPokemonIcon(const QPixmap icon);
  void setPokemonProperties(const BaseRNGSystem::PokemonProperties properties);
  void reset();

private:
  QLabel* m_lblHpIv;
  QLabel* m_lblAtkIv;
  QLabel* m_lblDefIv;
  QLabel* m_lblSpAtkIv;
  QLabel* m_lblSpDefIv;
  QLabel* m_lblSpeedIv;

  QLabel* m_lblIcon;
  QLabel* m_lblName;
  QLabel* m_lblGender;
  QLabel* m_lblNature;
  QLabel* m_lblHiddenPower;
};
