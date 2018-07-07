#include "MainWindow.h"

#include <QFileInfo>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QtConcurrent>

#include <thread>

#include "../PokemonRNGSystem/Colosseum/ColosseumRNGSystem.h"
#include "../PokemonRNGSystem/XD/GaleDarknessRNGSystem.h"
#include "GUICommon.h"
#include "SPokemonRNG.h"
#include "SeedFinder/SeedFinderWizard.h"
#include "Settings/DlgSettings.h"
#include "Settings/SConfig.h"

MainWindow::MainWindow()
{
  initialiseWidgets();
  makeLayouts();
  makeMenus();

  m_precalcFuture = QFuture<void>();

  connect(this, &MainWindow::onPrecalcDone, this, &MainWindow::precalcDone);
  connect(this, &MainWindow::onUpdatePrecalcProgress, this, [=](long value) {
    if (value == m_dlgProgressPrecalc->maximum())
      m_dlgProgressPrecalc->setLabelText(tr("Writting to disk..."));
    else
      m_dlgProgressPrecalc->setValue(value);
  });
}

MainWindow::~MainWindow()
{
  SPokemonRNG::deleteSystem();
  delete m_dlgProgressPrecalc;
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

  m_btnReset = new QPushButton(tr("Reset"));
  connect(m_btnReset, &QPushButton::clicked, this, &MainWindow::resetPredictor);
  m_btnReset->setEnabled(false);

  m_chkFilterUnwantedPredictions = new QCheckBox(tr("Hide unwanted predictions"));
  m_chkFilterUnwantedPredictions->setChecked(false);
  connect(m_chkFilterUnwantedPredictions, &QCheckBox::stateChanged, this,
          [=](int state) { m_predictorWidget->filterUnwanted(state == Qt::Checked); });

  m_btnRerollPrediciton = new QPushButton(tr("Reroll\n(requires an additional team generation)"));
  connect(m_btnRerollPrediciton, &QPushButton::clicked, this, &MainWindow::rerollPredictor);
  m_btnRerollPrediciton->setEnabled(false);

  m_lblRerollCount = new QLabel(QString::number(m_rerollCount), this);

  m_predictorWidget = new PredictorWidget(this);
}

void MainWindow::makeLayouts()
{
  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(m_btnSettings);
  buttonsLayout->addWidget(m_btnStartSeedFinder);
  buttonsLayout->addWidget(m_btnReset);

  QHBoxLayout* filterUnwantedLayout = new QHBoxLayout;
  filterUnwantedLayout->addStretch();
  filterUnwantedLayout->addWidget(m_chkFilterUnwantedPredictions);
  filterUnwantedLayout->addStretch();

  QLabel* lblReroll = new QLabel(tr("Reroll count: "), this);

  QHBoxLayout* rerollCountLayout = new QHBoxLayout;
  rerollCountLayout->addStretch();
  rerollCountLayout->addWidget(lblReroll);
  rerollCountLayout->addWidget(m_lblRerollCount);
  rerollCountLayout->addStretch();

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_cmbGame);
  mainLayout->addLayout(buttonsLayout);
  mainLayout->addLayout(filterUnwantedLayout);
  mainLayout->addWidget(m_predictorWidget);
  mainLayout->addWidget(m_btnRerollPrediciton);
  mainLayout->addLayout(rerollCountLayout);

  QWidget* mainWidget = new QWidget;
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);

  setWindowTitle("GameCube Pokémon RNG assistant");
  setMinimumWidth(600);
}

void MainWindow::makeMenus()
{
  m_menuFile = menuBar()->addMenu(tr("&File"));
  m_actGenerationPrecalcFile =
      m_menuFile->addAction(tr("Generate the precalculation file for the chosen game"), this,
                            [=]() { generatePrecalc(); });
  m_actGenerationPrecalcFile->setEnabled(false);
  m_menuFile->addAction(tr("&Quit"), this, [=]() { close(); });

  m_menuEdit = menuBar()->addMenu(tr("&Edit"));
  m_menuEdit->addAction(tr("&Settings"), this, [=]() { openSettings(); });

  m_menuHelp = menuBar()->addMenu(tr("&Help"));
  m_menuHelp->addAction(tr("&About"), this, [=]() {
    QString title = tr("About GameCube Pokémon RNG assistant");
    QString text =
        "Beta version 0.2.0\n\n" +
        tr("A program to allow the manipulation of the starters RNG in Pokémon Colosseum and "
           "Pokémon XD: Gale of darkness.\n\nThis program is licensed under the MIT license. "
           "You should have received a copy of the MIT license along with this program");
    QMessageBox::about(this, title, text);
  });
}

void MainWindow::gameChanged()
{
  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  if (selection == GUICommon::gameSelection::Colosseum)
    SPokemonRNG::setCurrentSystem(new ColosseumRNGSystem());
  else if (selection = GUICommon::gameSelection::XD)
    SPokemonRNG::setCurrentSystem(new GaleDarknessRNGSystem());

  if (m_cmbGame->count() == static_cast<int>(GUICommon::gameSelection::Unselected) + 1)
  {
    m_cmbGame->removeItem(static_cast<int>(GUICommon::gameSelection::Unselected));
    m_btnStartSeedFinder->setEnabled(true);
    m_actGenerationPrecalcFile->setEnabled(true);
  }
  m_predictorWidget->switchGame(selection);
  m_btnReset->setEnabled(false);
  m_btnRerollPrediciton->setEnabled(false);
  m_rerollCount = 0;
  m_lblRerollCount->setText(QString::number(m_rerollCount));
}

void MainWindow::startSeedFinder()
{
  QFileInfo info(QString::fromStdString(SPokemonRNG::getCurrentSystem()->getPrecalcFilename()));
  if (!(info.exists() && info.isFile()))
  {
    QMessageBox* msg = new QMessageBox(
        QMessageBox::Critical, "Precalculation file missing",
        "The precalculation file " + info.fileName() +
            " specific to this game is missing from the program's directory and it is required for "
            "the seed finding procedure. Please either download it (it should have been "
            "distributed along the release of the program), or generate it from the File menu. It "
            "is highly recommended to download it as the generation takes a few hours.",
        QMessageBox::Ok);
    msg->exec();
    delete msg;
    return;
  }

  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  SeedFinderWizard* wizard = new SeedFinderWizard(this, selection);
  if (wizard->exec() == QDialog::Accepted)
  {
    m_currentSeed = wizard->getSeeds()[0];
    std::vector<BaseRNGSystem::StartersPrediction> predictions =
        SPokemonRNG::getCurrentSystem()->predictStartersForNbrSeconds(
            m_currentSeed, SConfig::getInstance().getPredictionTime());
    m_predictorWidget->setStartersPrediction(predictions, selection);
    m_predictorWidget->filterUnwanted(m_chkFilterUnwantedPredictions->isChecked());
    m_btnReset->setEnabled(true);
    m_btnRerollPrediciton->setEnabled(true);
    m_rerollCount = 0;
    m_lblRerollCount->setText(QString::number(m_rerollCount));
  }
  delete wizard;
}

void MainWindow::resetPredictor()
{
  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  m_predictorWidget->resetPredictor(selection);
  m_btnReset->setEnabled(false);
  m_btnRerollPrediciton->setEnabled(false);
  m_rerollCount = 0;
  m_lblRerollCount->setText(QString::number(m_rerollCount));
}

void MainWindow::rerollPredictor()
{
  std::vector<int> dummyCriteria;
  for (int i = 0; i < 6; i++)
    dummyCriteria.push_back(-1);

  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  SPokemonRNG::getCurrentSystem()->generateBattleTeam(m_currentSeed, dummyCriteria);
  std::vector<BaseRNGSystem::StartersPrediction> predictions =
      SPokemonRNG::getCurrentSystem()->predictStartersForNbrSeconds(
          m_currentSeed, SConfig::getInstance().getPredictionTime());
  m_predictorWidget->setStartersPrediction(predictions, selection);
  m_predictorWidget->filterUnwanted(m_chkFilterUnwantedPredictions->isChecked());
  m_rerollCount++;
  m_lblRerollCount->setText(QString::number(m_rerollCount));
}

void MainWindow::openSettings()
{
  DlgSettings* dlg = new DlgSettings(this);
  dlg->exec();
  delete dlg;
}

void MainWindow::generatePrecalc()
{
  QMessageBox* msg = new QMessageBox(
      QMessageBox::Warning, "Precalculation file",
      "You are about to start the generation of a precalculation file for the chosen game. This "
      "generation will take a few hours to complete depending on your CPU and thread count. "
      "Consider downloading the file instead which should have been distributed along the release "
      "of this program.\n\nDo you really want to start the file generation process?",
      QMessageBox::No | QMessageBox::Yes, this);
  msg->exec();
  if (msg->result() == QMessageBox::Yes)
  {
    unsigned int threadCount = SConfig::getInstance().getThreadLimit();
    if (threadCount == 0)
      threadCount = std::thread::hardware_concurrency();

    delete m_dlgProgressPrecalc;
    m_dlgProgressPrecalc = new QProgressDialog(this);
    m_dlgProgressPrecalc->setWindowModality(Qt::WindowModal);
    m_dlgProgressPrecalc->setWindowTitle(tr("Precalculation file generation"));
    m_dlgProgressPrecalc->setCancelButtonText(tr("&Cancel"));
    m_dlgProgressPrecalc->setMinimum(0);
    m_dlgProgressPrecalc->setLabelText("Precalculating " +
                                       QString::number(Common::nbrPossibleSeeds) + " seeds with " +
                                       QString::number(threadCount) + " threads...");
    m_dlgProgressPrecalc->setMaximum(65536);
    connect(m_dlgProgressPrecalc, &QProgressDialog::canceled, this, [=]() {
      m_cancelPrecalc = true;
      m_precalcFuture.waitForFinished();
    });
    m_dlgProgressPrecalc->setValue(0);

    QtConcurrent::run([=] {
      SPokemonRNG::getCurrentSystem()->generatePrecalculationFile(
          threadCount, [=](long value) { emit onUpdatePrecalcProgress(value); },
          [=]() { return m_cancelPrecalc; });
      if (!m_cancelPrecalc)
        emit onPrecalcDone();
    });
    m_precalcFuture.waitForFinished();
    m_cancelPrecalc = false;
  }
  delete msg;
}

void MainWindow::precalcDone()
{
  m_dlgProgressPrecalc->setValue(m_dlgProgressPrecalc->maximum());
  QMessageBox* msg =
      new QMessageBox(QMessageBox::Information, "Precalculation success",
                      "The precalculation file was created successfully.", QMessageBox::Ok);
  msg->exec();
  delete msg;
}
