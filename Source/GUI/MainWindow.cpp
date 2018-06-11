#include "MainWindow.h"

#include <QHBoxLayout>
#include <QSettings>
#include <QVBoxLayout>

#include "GUICommon.h"
#include "SPokemonRNG.h"
#include "SeedFinder/SeedFinderWizard.h"
#include "Settings/DlgSettings.h"

MainWindow::MainWindow()
{
  initialiseWidgets();
  makeLayouts();
}

MainWindow::~MainWindow()
{
  delete m_system;
}

void MainWindow::initialiseWidgets()
{
  m_cmbGame = new QComboBox;
  m_cmbGame->addItems(GUICommon::gamesStr);
  m_cmbGame->addItem(tr("--Select your game--"));
  m_cmbGame->setCurrentIndex(static_cast<int>(GUICommon::gameSelection::Unselected));
  connect(m_cmbGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::gameChanged);

  m_btnSettings = new QPushButton(tr("Settings"));
  connect(m_btnSettings, &QPushButton::clicked, this, &MainWindow::openSettings);

  m_btnStartSeedFinder = new QPushButton(tr("Find your seed"));
  connect(m_btnStartSeedFinder, &QPushButton::clicked, this, &MainWindow::startSeedFinder);
  m_btnStartSeedFinder->setEnabled(false);

  m_predictorWidget = new PredictorWidget(this);
}

void MainWindow::makeLayouts()
{
  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(m_btnStartSeedFinder);
  buttonsLayout->addWidget(m_btnSettings);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_cmbGame);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->addWidget(m_predictorWidget);

  QWidget* mainWidget = new QWidget;
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);
}

void MainWindow::gameChanged()
{
  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  if (selection == GUICommon::gameSelection::Colosseum)
    SPokemonRNG::getInstance()->switchGame(SPokemonRNG::GCPokemonGame::Colosseum);
  else if (selection = GUICommon::gameSelection::XD)
    SPokemonRNG::getInstance()->switchGame(SPokemonRNG::GCPokemonGame::XD);

  if (m_cmbGame->count() == static_cast<int>(GUICommon::gameSelection::Unselected) + 1)
  {
    m_cmbGame->removeItem(static_cast<int>(GUICommon::gameSelection::Unselected));
    m_btnStartSeedFinder->setEnabled(true);
  }
}

void MainWindow::startSeedFinder()
{
  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  QSettings settings("settings.ini", QSettings::IniFormat);
  int rtcMarginError = settings.value("generalSettings/finder/rtcMarginError", 5).toInt();
  bool useWii =
      settings
          .value("generalSettings/finder/platform", static_cast<int>(GUICommon::platform::GameCube))
          .toInt() == static_cast<int>(GUICommon::platform::Wii);
  SeedFinderWizard* wizard = new SeedFinderWizard(this, selection, rtcMarginError, useWii);
  if (wizard->exec() == QDialog::Accepted)
  {
    std::vector<BaseRNGSystem::StartersPrediction> predictions =
        SPokemonRNG::getInstance()->getSystem()->predictStartersForNbrSeconds(
            wizard->getSeeds()[0], settings.value("generalSettings/predictor/time", 10).toInt());
    m_predictorWidget->setStartersPrediction(predictions, selection);
  }
}

void MainWindow::openSettings()
{
  DlgSettings* dlg = new DlgSettings(this);
  dlg->exec();
}
