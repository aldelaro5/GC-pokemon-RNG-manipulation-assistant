#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QVector>
#include <QWidget>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"

class StartersPredictionWidget : public QWidget
{
public:
  StartersPredictionWidget(QWidget* parent = nullptr);
  void setStartersPrediction(BaseRNGSystem::startersPrediction startersPrediction);

private:
  void initialiseWidgets();
  void makeLayouts();

  BaseRNGSystem::startersPrediction m_startersPrediction;
  QHBoxLayout* m_startersNamesLayout;
  QVector<QLabel*> m_lblStartersNames;
  QTableWidget* m_tblStartersPrediction;
};
