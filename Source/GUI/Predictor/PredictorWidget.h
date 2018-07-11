#pragma once

#include <QHBoxLayout>
#include <QLabel>
#include <QTableWidget>
#include <QVector>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"
#include "../GUICommon.h"

class PredictorWidget : public QWidget
{
public:
  PredictorWidget(QWidget* parent = nullptr);
  void
  setStartersPrediction(const std::vector<BaseRNGSystem::StartersPrediction> startersPrediction,
                        const GUICommon::gameSelection game);
  void resetPredictor(const GUICommon::gameSelection currentGame);
  bool filterUnwanted(const bool filterUnwanted);
  void switchGame(const GUICommon::gameSelection game);

private:
  const QBrush greenBrush = QBrush(QColor("#32CD32"));
  const QBrush redBrush = QBrush(QColor("#DC143C"));

  void clearLabels();
  void initialiseWidgets();
  void makeLayouts();

  BaseRNGSystem::StartersPrediction m_startersPrediction;
  QHBoxLayout* m_startersNamesLayout;
  QVector<QLabel*> m_lblStartersNames;
  QStringList m_tblHeaderLabels;
  QTableWidget* m_tblStartersPrediction;
};
