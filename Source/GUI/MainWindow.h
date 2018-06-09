#pragma once

#include <QComboBox>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

#include "../PokemonRNGSystem/BaseRNGSystem.h"
#include "Predictor/PredictorWidget.h"

class MainWindow : public QMainWindow
{
public:
  MainWindow();
  ~MainWindow();

  void gameChanged();
  void startSeedFinder();
  void openSettings();

private:
  void initialiseWidgets();
  void makeLayouts();

  BaseRNGSystem* m_system = nullptr;

  QComboBox* m_cmbGame;
  QPushButton* m_btnStartSeedFinder;
  QPushButton* m_btnSettings;
  PredictorWidget* m_predictorWidget;
};