#pragma once

#include <QComboBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QVector>
#include <QWidget>

#include "../../Common/CommonTypes.h"
#include "../../PokemonRNGSystem/BaseRNGSystem.h"
#include "../GUICommon.h"
#include "PokemonPropertiesFrame.h"

class StatsReporterWidget : public QWidget
{
public:
  StatsReporterWidget(QWidget* parent = nullptr);
  void gameChanged(const GUICommon::gameSelection game);
  void startersPredictionChanged(const BaseRNGSystem::StartersPrediction starters);
  void reset();

private:
  void onSecondaryChanged(const int newIndex);
  void onStatsGenderChanged();
  void updateStatsSelection();
  void updateStartersLayout();
  QPixmap pixmapForPokemon(const QString name);
  void selectedPossibilityChanged();
  void resetStatsSelection();

  const int c_maxResultsShown = 100;

  u32 m_startingSeed = 0;

  QComboBox* m_cmbSecondaryPokemon;

  QComboBox* m_cmbGender;
  QComboBox* m_cmbHpStat;
  QComboBox* m_cmbAtkStat;
  QComboBox* m_cmbDefStat;
  QComboBox* m_cmbSpAtkStat;
  QComboBox* m_cmbSpDefStat;
  QComboBox* m_cmbSpeedStat;

  QStringList m_possibilitiesHeaderLabels;
  QTableWidget* m_tblSecondaryPossibilities;
  QLabel* m_lblResultsCount;

  QVBoxLayout* m_startersLayout;

  QVector<PokemonPropertiesFrame*> m_starterFrames;
  std::vector<BaseRNGSystem::SecondaryCandidate> m_filteredCandidates;
  PokemonPropertiesFrame* m_secondaryFrame;
};
