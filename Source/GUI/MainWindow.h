#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

#include "../Common/CommonTypes.h"
#include "../PokemonRNGSystem/BaseRNGSystem.h"
#include "Predictor/PredictorWidget.h"

class MainWindow : public QMainWindow
{
public:
  MainWindow();
  ~MainWindow();

  void gameChanged();
  void startSeedFinder();
  void resetPredictor();
  void rerollPredictor();
  void openSettings();

private:
  void initialiseWidgets();
  void makeLayouts();

  BaseRNGSystem* m_system = nullptr;

  QComboBox* m_cmbGame;
  QPushButton* m_btnStartSeedFinder;
  QPushButton* m_btnSettings;
  QPushButton* m_btnReset;
  QPushButton* m_btnRerollPrediciton;
  QCheckBox* m_chkFilterUnwantedPredictions;
  PredictorWidget* m_predictorWidget;
  u32 m_currentSeed = 0;
};