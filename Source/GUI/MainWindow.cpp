#include "MainWindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>

#include "../PokemonRNGSystem/Colosseum/ColosseumRNGSystem.h"
#include "../PokemonRNGSystem/XD/GaleDarknessRNGSystem.h"
#include "GUICommon.h"

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
}

void MainWindow::makeLayouts()
{
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_cmbGame);
  mainLayout->addWidget(m_btnStartSeedFinder);

  QWidget* mainWidget = new QWidget;
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);
}

void MainWindow::gameChanged()
{
  gameSelection selection = static_cast<gameSelection>(m_cmbGame->currentIndex());
  delete m_system;
  if (selection == gameSelection::Colosseum)
    m_system = new ColosseumRNGSystem();
  else if (selection = gameSelection::XD)
    m_system = new GaleDarknessRNGSystem();

  if (m_cmbGame->count() == static_cast<int>(gameSelection::Unselected) + 1)
    m_cmbGame->removeItem(static_cast<int>(gameSelection::Unselected));
}

void MainWindow::startSeedFinder()
{
}
