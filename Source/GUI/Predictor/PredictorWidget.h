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
  void filterUnwanted(bool filterUnwanted);

private:
  const QBrush greenBrush = QBrush(QColor("#32CD32"));
  const QBrush redBrush = QBrush(QColor("#B22222"));

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
