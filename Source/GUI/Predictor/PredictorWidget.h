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
  void setStartersPrediction(BaseRNGSystem::startersPrediction startersPrediction);

private:
  void initialiseWidgets();
  void makeLayouts();
  void switchGame();

  BaseRNGSystem::startersPrediction m_startersPrediction;
  QHBoxLayout* m_startersNamesLayout;
  QVector<QLabel*> m_lblStartersNames;
  QStringList m_tblHeaderLabels;
  QTableWidget* m_tblStartersPrediction;
};
