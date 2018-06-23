#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>

#include "../Common/CommonTypes.h"
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
  void makeMenus();

  QMenu* m_menuFile;
  QMenu* m_menuEdit;
  QMenu* m_menuHelp;

  QComboBox* m_cmbGame;
  QPushButton* m_btnStartSeedFinder;
  QPushButton* m_btnSettings;
  QPushButton* m_btnReset;
  QPushButton* m_btnRerollPrediciton;
  QCheckBox* m_chkFilterUnwantedPredictions;
  PredictorWidget* m_predictorWidget;
  u32 m_currentSeed = 0;
};
