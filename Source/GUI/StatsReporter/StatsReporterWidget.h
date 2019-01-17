#pragma once

#include <QComboBox>
#include <QVBoxLayout>
#include <QTableWidget>
#include <QVector>
#include <QWidget>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"
#include "../GUICommon.h"
#include "PokemonPropertiesFrame.h"

class StatsReporterWidget : public QWidget
{
public:
  StatsReporterWidget(QWidget* parent = nullptr);
  void gameChanged(GUICommon::gameSelection game);
  void startersPredictionChanged(BaseRNGSystem::StartersPrediction starters);

private:
  void updateStartersLayout();
  QPixmap pixmapForPokemon(QString name);

  QComboBox* m_cmbSecondaryPokemon;

  QComboBox* m_cmbGender;
  QComboBox* m_cmbHpStat;
  QComboBox* m_cmbAtkStat;
  QComboBox* m_cmbDefStat;
  QComboBox* m_cmbSpAtkStat;
  QComboBox* m_cmbSpDefStat;
  QComboBox* m_cmbSpeedStat;

  QTableWidget* m_tblSecondaryPossibilities;

  QVBoxLayout* m_startersLayout;

  QVector<PokemonPropertiesFrame*> m_starterFrames;
  PokemonPropertiesFrame* m_secondaryFrame;
};
