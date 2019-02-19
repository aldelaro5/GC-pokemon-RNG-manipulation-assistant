#include "MainWindow.h"

#include <QFileInfo>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QRegularExpression>
#include <QVBoxLayout>
#include <QtConcurrent>

#include <sstream>
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
  m_dlgProgressPrecalc = nullptr;

  connect(this, &MainWindow::onPrecalcDone, this, &MainWindow::precalcDone);
  connect(this, &MainWindow::onUpdatePrecalcProgress, this, [=](long value) {
    if (value == m_dlgProgressPrecalc->maximum())
      m_dlgProgressPrecalc->setLabelText(tr("Writting to disk..."));
    else
      m_dlgProgressPrecalc->setValue(value);
  });

  if (SConfig::getInstance().getRestorePreviousWindowGeometry())
    restoreGeometry(SConfig::getInstance().getPreviousWindowGeometry());
}

MainWindow::~MainWindow()
{
  SPokemonRNG::deleteSystem();
  delete m_dlgProgressPrecalc;
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  SConfig::getInstance().setPreviousWindowGeometry(saveGeometry());
}

void MainWindow::initialiseWidgets()
{
  m_cmbGame = new QComboBox;
  m_cmbGame->addItems(GUICommon::gamesStr);
  m_cmbGame->addItem(tr("--Select your game--"));
  m_cmbGame->setCurrentIndex(static_cast<int>(GUICommon::gameSelection::Unselected));
  connect(m_cmbGame, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &MainWindow::gameChanged);

  m_btnSettings = new QPushButton(tr("&Settings"));
  connect(m_btnSettings, &QPushButton::clicked, this, &MainWindow::openSettings);

  m_btnStartSeedFinder = new QPushButton(tr("&Find your seed"));
  connect(m_btnStartSeedFinder, &QPushButton::clicked, this, &MainWindow::startSeedFinder);
  m_btnStartSeedFinder->setEnabled(false);

  m_btnReset = new QPushButton(tr("&Reset"));
  connect(m_btnReset, &QPushButton::clicked, this, &MainWindow::resetPredictor);
  m_btnReset->setEnabled(false);

  m_chkFilterUnwantedPredictions = new QCheckBox(tr("Hide unwanted predictions"));
  m_chkFilterUnwantedPredictions->setChecked(false);
  connect(m_chkFilterUnwantedPredictions, &QCheckBox::stateChanged, this,
          [=](int state) { m_predictorWidget->filterUnwanted(state == Qt::Checked); });

  m_edtManualSeed = new QLineEdit();
  m_edtManualSeed->setEnabled(false);
  m_btnSetSeedManually = new QPushButton("Set See&d");
  connect(m_btnSetSeedManually, &QPushButton::clicked, this, &MainWindow::setSeedManually);
  m_btnSetSeedManually->setEnabled(false);

  m_lblCurrentSeed = new QLabel("  ????  ");
  m_lblStoredSeed = new QLabel("  None  ");

  m_btnStoreSeed = new QPushButton("St&ore Seed");
  connect(m_btnStoreSeed, &QPushButton::clicked, this, &MainWindow::storeSeed);
  m_btnStoreSeed->setEnabled(false);

  m_btnRestoreSeed = new QPushButton("Res&tore Seed");
  connect(m_btnRestoreSeed, &QPushButton::clicked, this, &MainWindow::restoreSeed);
  m_btnRestoreSeed->setEnabled(false);

  m_btnRerollPrediciton = new QPushButton(tr("R&eroll\n(requires an additional team generation)"));
  connect(m_btnRerollPrediciton, &QPushButton::clicked, this, &MainWindow::singleRerollPredictor);
  m_btnRerollPrediciton->setEnabled(false);

  m_btnAutoRerollPrediciton =
      new QPushButton(tr("&Auto Reroll\n(rerolls until the next desired prediction)"));
  connect(m_btnAutoRerollPrediciton, &QPushButton::clicked, this, &MainWindow::autoRerollPredictor);
  m_btnAutoRerollPrediciton->setEnabled(false);

  m_lblRerollCount = new QLabel(QString::number(m_rerollCount), this);

  m_predictorWidget = new PredictorWidget(this);
  m_statsReporterWidget = new StatsReporterWidget(this);
  m_statsReporterWidget->setDisabled(true);
  connect(m_predictorWidget, &PredictorWidget::selectedPredictionChanged, this,
          [=](BaseRNGSystem::StartersPrediction prediction) {
            m_statsReporterWidget->startersPredictionChanged(prediction);
            m_statsReporterWidget->setEnabled(true);
          });
}

void MainWindow::makeLayouts()
{
  QHBoxLayout* buttonsLayout = new QHBoxLayout;
  buttonsLayout->addWidget(m_btnSettings);
  buttonsLayout->addWidget(m_btnStartSeedFinder);
  buttonsLayout->addWidget(m_btnReset);

  QHBoxLayout* setSeedLayout = new QHBoxLayout;
  setSeedLayout->addWidget(new QLabel("Set the seed manually:"));
  setSeedLayout->addWidget(m_edtManualSeed);
  setSeedLayout->addWidget(m_btnSetSeedManually);

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

  QHBoxLayout* seedInfoLayout = new QHBoxLayout;
  seedInfoLayout->addStretch();
  seedInfoLayout->addWidget(new QLabel("Stored Seed:"));
  seedInfoLayout->addWidget(m_lblStoredSeed);
  seedInfoLayout->addSpacing(20);
  seedInfoLayout->addWidget(new QLabel("Current Seed:"));
  seedInfoLayout->addWidget(m_lblCurrentSeed);
  seedInfoLayout->addStretch();

  QHBoxLayout* seedStoreRestoreLayout = new QHBoxLayout;
  seedStoreRestoreLayout->addWidget(m_btnStoreSeed);
  seedStoreRestoreLayout->addWidget(m_btnRestoreSeed);

  QHBoxLayout* rerollButtonsLayout = new QHBoxLayout;
  rerollButtonsLayout->addWidget(m_btnRerollPrediciton);
  rerollButtonsLayout->addWidget(m_btnAutoRerollPrediciton);

  QVBoxLayout* predictorLayout = new QVBoxLayout;
  predictorLayout->addWidget(m_cmbGame);
  predictorLayout->addLayout(buttonsLayout);
  predictorLayout->addLayout(setSeedLayout);
  predictorLayout->addLayout(filterUnwantedLayout);
  predictorLayout->addWidget(m_predictorWidget);
  predictorLayout->addLayout(rerollButtonsLayout);
  predictorLayout->addLayout(rerollCountLayout);
  predictorLayout->addLayout(seedStoreRestoreLayout);
  predictorLayout->addLayout(seedInfoLayout);

  QFrame* separatorLine = new QFrame();
  separatorLine->setFrameShape(QFrame::VLine);

  QHBoxLayout* mainLayout = new QHBoxLayout;
  mainLayout->addLayout(predictorLayout);
  mainLayout->addWidget(separatorLine);
  mainLayout->addWidget(m_statsReporterWidget);

  QWidget* mainWidget = new QWidget;
  mainWidget->setLayout(mainLayout);
  setCentralWidget(mainWidget);

  setWindowTitle("GameCube Pokémon RNG assistant");
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
        "Version 1.0.2\n\n" +
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
  {
    SPokemonRNG::setCurrentSystem(new ColosseumRNGSystem());
  }
  else if (selection = GUICommon::gameSelection::XD)
  {
    SPokemonRNG::setCurrentSystem(new GaleDarknessRNGSystem());
    GaleDarknessRNGSystem::setPalEnabled(SConfig::getInstance().getXDPalVersionEnabled());
  }

  if (m_cmbGame->count() == static_cast<int>(GUICommon::gameSelection::Unselected) + 1)
  {
    m_cmbGame->removeItem(static_cast<int>(GUICommon::gameSelection::Unselected));
    m_btnStartSeedFinder->setEnabled(true);
    m_actGenerationPrecalcFile->setEnabled(true);
  }
  m_predictorWidget->switchGame(selection);
  m_btnReset->setEnabled(false);
  m_btnRerollPrediciton->setEnabled(false);
  m_btnAutoRerollPrediciton->setEnabled(false);
  m_rerollCount = 0;
  m_lblRerollCount->setText(QString::number(m_rerollCount));
  m_edtManualSeed->setEnabled(true);
  m_btnSetSeedManually->setEnabled(true);
  m_lblCurrentSeed->setText("  ????  ");
  m_seedSet = false;
  m_lblStoredSeed->setText("  None  ");

  m_statsReporterWidget->gameChanged(selection);
  m_statsReporterWidget->setDisabled(true);
}

void MainWindow::setCurrentSeed(u32 seed, int rerollCount)
{
  m_currentSeed = seed;

  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());

  m_lblCurrentSeed->setText(QString::number(m_currentSeed, 16).toUpper());
  std::vector<BaseRNGSystem::StartersPrediction> predictions =
      SPokemonRNG::getCurrentSystem()->predictStartersForNbrSeconds(
          m_currentSeed, SConfig::getInstance().getPredictionTime());
  m_predictorWidget->setStartersPrediction(predictions);
  m_predictorWidget->updateGUI(selection);
  m_predictorWidget->filterUnwanted(m_chkFilterUnwantedPredictions->isChecked());
  m_btnReset->setEnabled(true);
  m_btnRerollPrediciton->setEnabled(true);
  m_btnAutoRerollPrediciton->setEnabled(true);
  m_rerollCount = rerollCount;
  m_lblRerollCount->setText(QString::number(m_rerollCount));
  m_btnStoreSeed->setEnabled(true);
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
    setCurrentSeed(wizard->getSeeds()[0], 0);
    m_seedSet = true;
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
  m_btnAutoRerollPrediciton->setEnabled(false);
  m_rerollCount = 0;
  m_lblRerollCount->setText(QString::number(m_rerollCount));
  m_btnStoreSeed->setEnabled(false);
  m_btnRestoreSeed->setEnabled(false);
  m_lblCurrentSeed->setText("  ????  ");
  m_seedSet = false;
  m_lblStoredSeed->setText("  None  ");
  m_statsReporterWidget->reset();
  m_statsReporterWidget->setDisabled(true);
}

void MainWindow::storeSeed()
{
  m_storedSeed = m_currentSeed;
  m_storedRerollCount = m_rerollCount;
  m_lblStoredSeed->setText(QString::number(m_storedSeed, 16).toUpper());
  m_btnRestoreSeed->setEnabled(true);
}

void MainWindow::restoreSeed()
{
  setCurrentSeed(m_storedSeed, m_storedRerollCount);
}

void MainWindow::setSeedManually()
{
  QRegularExpression hexMatcher("^[0-9A-F]{1,8}$", QRegularExpression::CaseInsensitiveOption);
  QRegularExpressionMatch match = hexMatcher.match(m_edtManualSeed->text());
  if (match.hasMatch())
  {
    std::stringstream ss(m_edtManualSeed->text().toStdString());
    ss >> std::hex;
    u32 seed = 0;
    ss >> seed;
    setCurrentSeed(seed, 0);
    m_seedSet = true;
  }
  else
  {
    QMessageBox* msg = new QMessageBox(QMessageBox::Critical, "Invalid seed",
                                       "The seed you have entered is not a valid seed. Pleaser "
                                       "enter a valid 32 bit hexadecimal number.",
                                       QMessageBox::Ok);
    msg->exec();
    delete msg;
  }
}

void MainWindow::singleRerollPredictor()
{
  rerollPredictor(true);
  m_lblCurrentSeed->setText(QString::number(m_currentSeed, 16).toUpper());
}

bool MainWindow::rerollPredictor(bool withGuiUpdates)
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
  m_rerollCount++;

  m_predictorWidget->setStartersPrediction(predictions);
  bool desiredStarterFound = m_predictorWidget->desiredPredictionFound(selection);

  if (withGuiUpdates)
  {
    m_predictorWidget->updateGUI(selection);
    m_predictorWidget->filterUnwanted(m_chkFilterUnwantedPredictions->isChecked());
    m_lblRerollCount->setText(QString::number(m_rerollCount));
    m_statsReporterWidget->setDisabled(true);
  }
  return desiredStarterFound;
}

void MainWindow::autoRerollPredictor()
{
  QDialog* autoRerollDlg = new QDialog;
  autoRerollDlg->setModal(true);
  autoRerollDlg->setWindowTitle("Auto rerolling");
  QLabel* lblAutoRerolling = new QLabel(autoRerollDlg);
  lblAutoRerolling->setText("Auto rerolling, please wait a moment...");
  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(lblAutoRerolling);
  autoRerollDlg->setLayout(mainLayout);
  autoRerollDlg->show();

  bool desiredPredictionFound = false;
  int nbrRerolls = 0;
  for (nbrRerolls; nbrRerolls < SConfig::getInstance().getMaxAutoReroll(); nbrRerolls++)
  {
    if (rerollPredictor(false))
    {
      desiredPredictionFound = true;
      break;
    }
    // No idea why, but this is required for the dialog to keep rendering which is odd because one
    // call to this before the loop should have been enough, but apparently, it isn't
    QCoreApplication::processEvents();
  }
  autoRerollDlg->close();
  delete autoRerollDlg;

  if (desiredPredictionFound)
  {
    QString lastBattleConfirmationStr =
        QString::fromStdString(SPokemonRNG::getCurrentSystem()->getLastObtainedCriteriasString());
    QMessageBox* msg =
        new QMessageBox(QMessageBox::Information, "Desired prediction found",
                        "After " + QString::number(nbrRerolls + 1) +
                            " rerolls, a desired prediction has been found; the predictions list "
                            "will be updated.\n\nOn the last battle confirmation screen, the "
                            "following information should be displayed:\n" +
                            lastBattleConfirmationStr,
                        QMessageBox::Ok);
    msg->exec();
    delete msg;
  }
  else
  {
    QMessageBox* msg = new QMessageBox(
        QMessageBox::Critical, "No desired prediction has been found",
        "The predictor has reached the limit of " + QString::number(nbrRerolls) +
            " rerolls, but no desired prediction has been found; the predictions list will be "
            "updated. You may try to auto reroll again.",
        QMessageBox::Ok);
    msg->exec();
    delete msg;
  }
  GUICommon::gameSelection selection =
      static_cast<GUICommon::gameSelection>(m_cmbGame->currentIndex());
  m_predictorWidget->updateGUI(selection);
  m_predictorWidget->filterUnwanted(m_chkFilterUnwantedPredictions->isChecked());
  m_lblRerollCount->setText(QString::number(m_rerollCount));
  m_lblCurrentSeed->setText(QString::number(m_currentSeed, 16).toUpper());
  m_statsReporterWidget->setDisabled(true);
}

void MainWindow::openSettings()
{
  DlgSettings* dlg = new DlgSettings(this);
  int dlgResult = dlg->exec();
  delete dlg;
  if (dlgResult == QDialog::Accepted && m_seedSet)
  {
    // Refresh the predictor
    setCurrentSeed(m_currentSeed, m_rerollCount);
  }
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
