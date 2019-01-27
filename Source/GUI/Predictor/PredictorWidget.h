#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QVector>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"
#include "../GUICommon.h"

class PredictorWidget : public QWidget
{
  Q_OBJECT

public:
  PredictorWidget(QWidget* parent = nullptr);
  std::vector<BaseRNGSystem::StartersPrediction> getStartersPrediction();
  // Returns whether or not a desired starter has been found
  void
  setStartersPrediction(const std::vector<BaseRNGSystem::StartersPrediction> startersPrediction);
  bool desiredPredictionFound(const GUICommon::gameSelection game);
  void updateGUI(const GUICommon::gameSelection game);
  void resetPredictor(const GUICommon::gameSelection currentGame);
  void filterUnwanted(const bool filterUnwanted);
  void switchGame(const GUICommon::gameSelection game);

signals:
  void selectedPredictionChanged(BaseRNGSystem::StartersPrediction prediction);

private:
  const QBrush greenBrush = QBrush(QColor("#32CD32"));
  const QBrush redBrush = QBrush(QColor("#DC143C"));

  void clearLabels();
  void initialiseWidgets();
  void makeLayouts();
  void scrollToSelectedItem();
  void onSelectedPredictionChanged();

  std::vector<BaseRNGSystem::StartersPrediction> m_startersPrediction;
  QHBoxLayout* m_startersNamesLayout;
  QVector<QLabel*> m_lblStartersNames;
  QStringList m_tblHeaderLabels;
  QTableWidget* m_tblStartersPrediction;
};
