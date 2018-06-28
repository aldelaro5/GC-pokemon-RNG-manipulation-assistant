#include "SeedFinderWizard.h"

#include <QAbstractButton>
#include <QMessageBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtConcurrent>

#include "../SPokemonRNG.h"
#include "../Settings/SConfig.h"

int SeedFinderWizard::numberPass = 1;

SeedFinderWizard::SeedFinderWizard(QWidget* parent, const GUICommon::gameSelection game,
                                   const int rtcErrorMarginSeconds, const bool useWii)
    : QWizard(parent), m_game(game), m_rtcErrorMarginSeconds(rtcErrorMarginSeconds),
      m_useWii(useWii)
{
  numberPass = 1;
  m_cancelSeedFinderPass = false;
  m_seedFinderFuture = QFuture<void>();
  m_precalcFuture = QFuture<void>();
  setPage(pageID::Start, new StartPage(this, game));
  setPage(pageID::Instructions, new InstructionsPage(this, game, useWii));
  setPage(pageID::SeedFinderPass, getSeedFinderPassPageForGame());
  setStartId(pageID::Start);

  setOptions(QWizard::HaveCustomButton1);
  setButtonText(QWizard::CustomButton1, "&Next Pass >");
  connect(this, &SeedFinderWizard::customButtonClicked, this, [=](int which) {
    if (which == QWizard::CustomButton1)
      nextSeedFinderPass();
  });
  connect(this, &SeedFinderWizard::onUpdateSeedFinderProgress, this, [=](int nbrSeedsSimulated) {
    if (!m_cancelSeedFinderPass)
      static_cast<SeedFinderPassPage*>(currentPage())->setSeedFinderProgress(nbrSeedsSimulated);
  });
  connect(this, &SeedFinderWizard::onSeedFinderPassDone, this,
          &SeedFinderWizard::seedFinderPassDone);
  connect(this, &SeedFinderWizard::onPrecalcDone, this, &SeedFinderWizard::precalcDone);

  connect(button(QWizard::NextButton), &QAbstractButton::clicked, this,
          &SeedFinderWizard::pageChanged);

  setWindowTitle(tr("Seed Finder Wizard"));
  setWizardStyle(QWizard::ModernStyle);

  QList<QWizard::WizardButton> btnlayout;
  btnlayout << QWizard::Stretch << QWizard::CancelButton << QWizard::NextButton;
  setButtonLayout(btnlayout);
  setFixedWidth(650);

  connect(this, &SeedFinderWizard::onUpdatePrecalcProgress, this,
          [=](long value) { m_dlgProgressPrecalc->setValue(value); });
}

std::vector<u32> SeedFinderWizard::getSeeds() const
{
  return m_seeds;
}

SeedFinderPassPage* SeedFinderWizard::getSeedFinderPassPageForGame()
{
  SeedFinderPassPage* page;
  switch (m_game)
  {
  case GUICommon::gameSelection::Colosseum:
    page = new SeedFinderPassColosseum(this, static_cast<int>(m_seeds.size()),
                                       m_rtcErrorMarginSeconds, m_useWii, m_usePrecalc);
    break;
  case GUICommon::gameSelection::XD:
    page = new SeedFinderPassXD(this, static_cast<int>(m_seeds.size()), m_rtcErrorMarginSeconds,
                                m_useWii, m_usePrecalc);
    break;
  default:
    return nullptr;
  }
  QString strResultStatus("No passes done");
  if (m_seeds.size() > 1)
    strResultStatus = QString::number(m_seeds.size()) + QString(" results");
  page->setTitle("Seed Finder Pass #" + QString::number(numberPass) + " (" + strResultStatus + ")");
  adjustSize();
  return page;
}

void SeedFinderWizard::nextSeedFinderPass()
{
  SeedFinderPassPage* page = static_cast<SeedFinderPassPage*>(currentPage());

  button(QWizard::CustomButton1)->setEnabled(false);

  if (numberPass == 1)
    page->setNewUsePrecalc(m_usePrecalc);

  page->showSeedFinderProgress(true);
  m_seedFinderFuture = QtConcurrent::run([=] {
    SPokemonRNG::getCurrentSystem()->seedFinder(
        page->obtainCriteria(), m_seeds, m_useWii, m_rtcErrorMarginSeconds, m_usePrecalc,
        [=](long int nbrSeedsSimulated) { emit onUpdateSeedFinderProgress(nbrSeedsSimulated); },
        [=] { return m_cancelSeedFinderPass; });
    if (!m_cancelSeedFinderPass)
      emit onSeedFinderPassDone();
  });
}

void SeedFinderWizard::seedFinderPassDone()
{
  if (m_seeds.size() <= 1)
  {
    SeedFinderPassPage* page = static_cast<SeedFinderPassPage*>(currentPage());
    page->setSeedFinderDone(true);
    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch << QWizard::FinishButton;
    setButtonLayout(layout);
    m_seedFinderDone = true;
    if (m_seeds.size() == 1)
      setPage(pageID::End, new EndPage(this, true, m_game, m_seeds[0]));
    else
      setPage(pageID::End, new EndPage(this, false, m_game));
    QWizard::next();
  }
  else
  {
    numberPass++;
    setPage(numberPass + pageID::SeedFinderPass, getSeedFinderPassPageForGame());
    button(QWizard::CustomButton1)->setEnabled(true);
    QWizard::next();
  }
}

void SeedFinderWizard::precalcDone()
{
  if (!m_cancelPrecalc)
  {
    QMessageBox* msg =
        new QMessageBox(QMessageBox::Information, "Precalculation sucess",
                        "The precalculation file was created sucessfully, it will now be used with "
                        "any subsequent seed finding procedure with the given settings.",
                        QMessageBox::Ok);
    msg->exec();
    m_usePrecalc = true;
  }
}

void SeedFinderWizard::pageChanged()
{
  if (currentId() == pageID::SeedFinderPass)
  {
    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch << QWizard::CancelButton << QWizard::CustomButton1;
    setButtonLayout(layout);

    QFileInfo info(
        QString::fromStdString(SPokemonRNG::getCurrentSystem()->getPrecalcFilenameForSettings(
            m_useWii, m_rtcErrorMarginSeconds)));
    m_usePrecalc = (info.exists() && info.isFile());
    if (!(info.exists() && info.isFile()))
    {
      size_t fileSize =
          SPokemonRNG::getCurrentSystem()->getPracalcFileSize(m_useWii, m_rtcErrorMarginSeconds);
      QMessageBox* msg = new QMessageBox(
          QMessageBox::Question, "Precalculation file",
          "Do you want to generate a precalculation file? This file may take a while to generate "
          "and "
          "be rather large, but will significantly speed up performance of the seed finder with "
          "the "
          "given game and the following settings:\n\nClock margin of error(seconds): " +
              QString::number(m_rtcErrorMarginSeconds) +
              "\nPlatform: " + (m_useWii ? QString("Nintendo Wii") : QString("Nintendo GameCube")) +
              "\n\nEstimated file size: " + QString::number(fileSize / 1024 / 1024) +
              "MB, do you want to create it?",
          QMessageBox::No | QMessageBox::Yes, this);
      msg->exec();
      if (msg->result() == QMessageBox::Yes)
      {
        BaseRNGSystem::seedRange range =
            SPokemonRNG::getCurrentSystem()->getRangeForSettings(m_useWii, m_rtcErrorMarginSeconds);
        m_dlgProgressPrecalc = new QProgressDialog(this);
        m_dlgProgressPrecalc->setWindowTitle(tr("Precalculation file generation"));
        m_dlgProgressPrecalc->setCancelButtonText(tr("&Cancel"));
        m_dlgProgressPrecalc->setMinimum(0);
        m_dlgProgressPrecalc->setLabelText("Precalculating " +
                                           QString::number(range.max - range.min) + " seeds...");
        m_dlgProgressPrecalc->setMaximum(range.max - range.min);
        m_dlgProgressPrecalc->setFixedWidth(500);
        connect(m_dlgProgressPrecalc, &QProgressDialog::canceled, this,
                [=]() { m_cancelPrecalc = true; });
        QtConcurrent::run([=]() {
          SPokemonRNG::getCurrentSystem()->precalculateNbrRollsBeforeTeamGeneration(
              m_useWii, m_rtcErrorMarginSeconds,
              [=](long int value) { emit onUpdatePrecalcProgress(value); },
              [=]() { return m_cancelPrecalc; });
          emit onPrecalcDone();
        });

        m_dlgProgressPrecalc->exec();
        delete m_dlgProgressPrecalc;
      }
    }
  }
}

void SeedFinderWizard::accept()
{
  if (m_seeds.size() == 1)
    QDialog::accept();
  else
    QDialog::reject();
}

void SeedFinderWizard::reject()
{
  if (m_seedFinderDone)
  {
    accept();
  }
  else
  {
    QMessageBox* cancelPrompt =
        new QMessageBox(QMessageBox::Information, "Seed Finder Cancellation",
                        "Are you sure you want to cancel the seed finding procedure?",
                        QMessageBox::No | QMessageBox::Yes, this);
    cancelPrompt->exec();
    if (cancelPrompt->result() == QMessageBox::Yes)
    {
      m_cancelSeedFinderPass = true;
      m_seedFinderFuture.waitForFinished();
      QWizard::reject();
    }
  }
}

StartPage::StartPage(QWidget* parent, const GUICommon::gameSelection game) : QWizardPage(parent)
{
  setTitle(tr("Introduction"));

  QLabel* label = new QLabel("This wizard will guide you through finding your seed in " +
                             GUICommon::gamesStr[game] + ".\n\nPress \"Next\" to continue.");
  label->setWordWrap(true);

  m_chkSkipInstructionPage = new QCheckBox(tr("Skip the instruction page"));
  m_chkSkipInstructionPage->setChecked(SConfig::getInstance().getSkipInstructionPage());

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addWidget(label);
  mainlayout->addWidget(m_chkSkipInstructionPage);
  setLayout(mainlayout);
}

int StartPage::nextId() const
{
  SConfig::getInstance().setSkipInstructionPage(m_chkSkipInstructionPage->isChecked());
  if (m_chkSkipInstructionPage->isChecked())
    return SeedFinderWizard::pageID::SeedFinderPass;
  else
    return SeedFinderWizard::pageID::Instructions;
}

InstructionsPage::InstructionsPage(QWidget* parent, const GUICommon::gameSelection game,
                                   const bool useWii)
    : QWizardPage(parent)
{
  setTitle(tr("Instructions"));
  setSubTitle(tr("Follow these detailled instructions before starting the seed finding procedure"));

  QLabel* lblSummary = new QLabel(
      "This procedure involves first, setting your console's clock, booting the game after "
      "setting the clock as fast as possible and then generating random battle teams over and over "
      "which gives more and more information about your seed so that it is found by narrowing it "
      "down to one possible one.",
      this);
  lblSummary->setWordWrap(true);

  if (useWii)
  {
    m_lblConsoleInstructions = new QLabel(tr(
        "\nAfter a hardware reset or boot, insert your disc then go to Wii settings -> calendar "
        "and set the date to 01/01/2000. Then, set the time to 00:00, AS SOON AS you confirm the "
        "new time, naviguate to the disc channel and boot the game as fast as possible. Depending "
        "on your margin of error in your settings, this step may be more lenient in how fast you "
        "need to be because the margin of error corespond to the time you can WASTE considering "
        "frame perfect input. Note: when going to the disc channel, make sure you see the entire "
        "disc spinning animation. To ensure you will see it, insert your disc before going to Wii "
        "settings and wait you see the GameCube logo thumbnail on the disc channel."));
  }
  else
  {
    m_lblConsoleInstructions = new QLabel(tr(
        "\nAfter a hardware reset or boot, insert your disc then go to the GameCube main menu by "
        "holding A upon boot and go the the calendar settings. Set the date to 01/01/2000, then "
        "set the time to 00:00:00. AS SOON AS you press A to set the new time, naviguate to the "
        "gameplay menu and boot the game as fast as possible. Depending on your margin of error in "
        "your settings, this step may be more lenient in how fast you need to be because the "
        "margin of error corespond to the time you can WASTE considering frame perfect input. "
        "Note: you shouldn't notice any latency when turning the cube menu after setting the time, "
        "if you do notice such latency, ensure you do not buffer your stick input and just do them "
        "after letting the stick neutral for a moment. You can try again if you are unsure."));
  }

  switch (game)
  {
  case GUICommon::gameSelection::Colosseum:
    m_lblGameInstructions = new QLabel(tr(
        "\nUpon reaching the main menu in the game, naviguate to Battle Now -> Single Battle -> "
        "Ultimate. You should see a confirmation screen of a battle with a randomly generated "
        "team. The seed finder will ask you for the informations displayed on this screen so make "
        "sure you input them correctly. After entering the information and confirming everything, "
        "wait that the pass is done, the first pass especially may take a while. Once done, back "
        "off the confirmation screen (NEVER accept it or it will invalidate the procedure) and "
        "select Single Battle -> Ultimate again; you are now ready to enter the informations of "
        "the next pass. You have to do this several times until only 1 result is left, you can see "
        "the number of results after each pass in the title of the wizard page. Note: NEVER go "
        "back to the main menu during this procedure or it will invalidate it."));
    break;
  case GUICommon::gameSelection::XD:
    m_lblGameInstructions = new QLabel(tr(
        "\nUpon reaching the main menu in the game, naviguate to VS Mode -> Quick Battle -> "
        "Single Battle -> Ultimate. You should see a confirmation screen of a battle with a "
        "randomly generated team. The seed finder will ask you for the informations displayed on "
        "this screen so make sure you input them correctly. After entering the information and "
        "confirming everything, wait that the pass is done, the first pass especially may take a "
        "while. Once done, back off the confirmation screen (NEVER accept it or it will invalidate "
        "the procedure) and select Single Battle -> Ultimate again; you are now ready to enter the "
        "informations of the next pass. You have to do this several times until only 1 result is "
        "left, you can see the number of results after each pass in the title of the wizard page. "
        "Note: NEVER go back to the main menu during this procedure or it will invalidate "
        "it."));
    break;
  default:
    m_lblGameInstructions = new QLabel("");
    break;
  }

  QLabel* lblNext = new QLabel(
      tr("\nPress \"Next\" once you acknowledged the above instructions to start the seed "
         "finding procedure."));
  lblNext->setWordWrap(true);
  m_lblConsoleInstructions->setWordWrap(true);
  m_lblGameInstructions->setWordWrap(true);

  QVBoxLayout* instructionsLayout = new QVBoxLayout;
  instructionsLayout->addWidget(lblSummary);
  instructionsLayout->addWidget(m_lblConsoleInstructions);
  instructionsLayout->addWidget(m_lblGameInstructions);
  instructionsLayout->addWidget(lblNext);
  instructionsLayout->addStretch();

  QWidget* instructionsWidget = new QWidget(this);
  instructionsWidget->setLayout(instructionsLayout);

  QScrollArea* mainWidget = new QScrollArea(this);
  mainWidget->setWidget(instructionsWidget);
  mainWidget->setWidgetResizable(true);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(mainWidget);

  setLayout(mainLayout);
  adjustSize();
  setFixedHeight(400);
}

int InstructionsPage::nextId() const
{
  return SeedFinderWizard::pageID::SeedFinderPass;
}

EndPage::EndPage(QWidget* parent, const bool sucess, const GUICommon::gameSelection game,
                 const u32 seed)
    : QWizardPage(parent)
{
  setTitle(tr("End"));

  if (sucess)
  {
    QString additionalNotes("");
    if (game == GUICommon::gameSelection::Colosseum)
      additionalNotes = tr("You MUST use a preset name for the predictions to work.");
    if (game == GUICommon::gameSelection::XD)
      additionalNotes = tr("You MUST use a custom name for the predictions to work.");
    m_lblResult = new QLabel(
        "The seed finding procedure completed sucessfully.\n\n" + QString("Your current seed is ") +
            QString("%1").arg(seed, 8, 16, QChar('0')).toUpper() +
            QString("\n\nPredictions of the starters will appear depending on the amount of frames "
                    "between pressing A on starting a new game and pressing A on the trainer name "
                    "confirmation. Desired predictions will appear in green while undesired ones "
                    "will appear in red (you may configure the filters in the settings). If you "
                    "are unsatisfied with the predictions, generate another team and click the "
                    "reroll button. You may only go back to the main menu of the game if you are "
                    "satisfied with the predictions. ") +
            additionalNotes +
            QString("\n\nClick \"Finish\" to see your prediction in "
                    "the previous window."),
        this);
  }
  else
  {
    m_lblResult = new QLabel(
        "The seed finding procedure completed, but your current seed hasn't been found. You have "
        "to restart this entire procedure again (this implies that you must hard reset the "
        "console).\n\n" +
        QString("Tip: try to increase your clock margin of error in the settings, altough doing so "
                "will make the seed finding procedure slower, it will make booting the game after "
                "setting the clock more permissive giving you more time to boot the game"));
  }
  m_lblResult->setWordWrap(true);

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addWidget(m_lblResult);
  mainlayout->addStretch();
  setLayout(mainlayout);
  setFixedHeight(550);
}

int EndPage::nextId() const
{
  return -1;
}
