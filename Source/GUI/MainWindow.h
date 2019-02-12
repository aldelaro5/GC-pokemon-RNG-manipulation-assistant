#pragma once

#include <QCheckBox>
#include <QComboBox>
#include <QFuture>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QProgressDialog>
#include <QPushButton>

#include "../Common/CommonTypes.h"
#include "Predictor/PredictorWidget.h"
#include "StatsReporter/StatsReporterWidget.h"

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  MainWindow();
  ~MainWindow();

signals:
  void onUpdatePrecalcProgress(const long value);
  void onPrecalcDone();

private:
  void closeEvent(QCloseEvent* event) override;

  void initialiseWidgets();
  void makeLayouts();
  void makeMenus();

  void gameChanged();
  void startSeedFinder();
  void resetPredictor();
  void storeSeed();
  void restoreSeed();
  void setSeedManually();
  void singleRerollPredictor();
  void autoRerollPredictor();
  void openSettings();
  void generatePrecalc();
  void precalcDone();

  void setCurrentSeed(u32 seed, int rerollCount);
  // Returns whether we have a desired prediction after the reroll
  bool rerollPredictor(bool withGuiUpdates);

  QMenu* m_menuFile;
  QMenu* m_menuEdit;
  QMenu* m_menuHelp;

  QAction* m_actGenerationPrecalcFile;
  QComboBox* m_cmbGame;
  QPushButton* m_btnStartSeedFinder;
  QPushButton* m_btnSettings;
  QPushButton* m_btnReset;
  QLineEdit* m_edtManualSeed;
  QPushButton* m_btnSetSeedManually;
  QLabel* m_lblCurrentSeed;
  QLabel* m_lblStoredSeed;
  QPushButton* m_btnStoreSeed;
  QPushButton* m_btnRestoreSeed;
  QPushButton* m_btnRerollPrediciton;
  QPushButton* m_btnAutoRerollPrediciton;

  QLabel* m_lblRerollCount;
  QCheckBox* m_chkFilterUnwantedPredictions;
  PredictorWidget* m_predictorWidget;
  StatsReporterWidget* m_statsReporterWidget;
  bool m_cancelPrecalc = false;
  QFuture<void> m_precalcFuture;
  QProgressDialog* m_dlgProgressPrecalc;
  u32 m_currentSeed = 0;
  bool m_seedSet = false;
  u32 m_storedSeed = 0;
  int m_rerollCount = 0;
  int m_storedRerollCount = 0;
};
