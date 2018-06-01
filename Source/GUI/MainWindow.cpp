#include "MainWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "GUICommon.h"
#include "SPokemonRNG.h"

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
  m_cmbGame->setCurrentIndex(static_cast<int>(gameSelection::Unselected));
  connect(m_cmbGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::gameChanged);

  m_btnStartSeedFinder = new QPushButton(tr("Find your seed"));
  connect(m_btnStartSeedFinder, &QPushButton::clicked, this, &MainWindow::startSeedFinder);

  m_predictorWidget = new PredictorWidget(this);
}

void MainWindow::makeLayouts()
{
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_cmbGame);
  mainLayout->addWidget(m_btnStartSeedFinder);
  mainLayout->addWidget(m_predictorWidget);

  QWidget* mainWidget = new QWidget;
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);
}

void MainWindow::gameChanged()
{
  gameSelection selection = static_cast<gameSelection>(m_cmbGame->currentIndex());
  if (selection == gameSelection::Colosseum)
    SPokemonRNG::getInstance()->switchGame(SPokemonRNG::GCPokemonGame::Colosseum);
  else if (selection = gameSelection::XD)
    SPokemonRNG::getInstance()->switchGame(SPokemonRNG::GCPokemonGame::XD);

  if (m_cmbGame->count() == static_cast<int>(gameSelection::Unselected) + 1)
    m_cmbGame->removeItem(static_cast<int>(gameSelection::Unselected));
}

void MainWindow::startSeedFinder()
{
}
