#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QVector>
#include <QWidget>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"
#include "../GUICommon.h"

class PredictorWidget : public QWidget
{
public:
  PredictorWidget(QWidget* parent = nullptr);
  void setStartersPrediction(std::vector<BaseRNGSystem::StartersPrediction> startersPrediction,
                             GUICommon::gameSelection game);
  void resetPredictor(GUICommon::gameSelection currentGame);

private:
  void clearLabels();
  void initialiseWidgets();
  void makeLayouts();
  void switchGame(GUICommon::gameSelection game);

  BaseRNGSystem::StartersPrediction m_startersPrediction;
  QHBoxLayout* m_startersNamesLayout;
  QVector<QLabel*> m_lblStartersNames;
  QStringList m_tblHeaderLabels;
  QTableWidget* m_tblStartersPrediction;
};
