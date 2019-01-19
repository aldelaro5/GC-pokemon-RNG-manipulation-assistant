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
  void reset();

private:
  void onSecondaryChanged(int newIndex);
  void onStatsGenderChanged();
  void updateStatsSelection();
  void updateStartersLayout();
  QPixmap pixmapForPokemon(QString name);
  void selectedPossibilityChanged();
  void resetStatsSelection();

  const int c_maxResultsShown = 100;

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
