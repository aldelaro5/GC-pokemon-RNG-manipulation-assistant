#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QVector>
#include <QWidget>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"

class PredictorWidget : public QWidget
{
public:
  PredictorWidget(QWidget* parent = nullptr);
  void setStartersPrediction(std::vector<BaseRNGSystem::StartersPrediction> startersPrediction);

private:
  void initialiseWidgets();
  void makeLayouts();
  void switchGame();

  BaseRNGSystem::StartersPrediction m_startersPrediction;
  QHBoxLayout* m_startersNamesLayout;
  QVector<QLabel*> m_lblStartersNames;
  QStringList m_tblHeaderLabels;
  QTableWidget* m_tblStartersPrediction;
};
