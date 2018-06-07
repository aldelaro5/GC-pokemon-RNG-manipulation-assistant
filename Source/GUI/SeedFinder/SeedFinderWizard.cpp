#include "SeedFinderWizard.h"

#include <QAbstractButton>
#include <QMessageBox>
#include <QProgressDialog>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtConcurrent>

#include "../SPokemonRNG.h"

int SeedFinderWizard::numberPass = 1;

SeedFinderWizard::SeedFinderWizard(QWidget* parent, GUICommon::gameSelection game,
                                   int rtcErrorMarginSeconds, bool useWii)
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

  QList<QWizard::WizardButton> btnlayout;
  btnlayout << QWizard::Stretch << QWizard::CancelButton << QWizard::NextButton;
  setButtonLayout(btnlayout);

  QSize size(600, 750);
  setFixedSize(size);
}

std::vector<u32> SeedFinderWizard::getSeeds() const
{
  return seeds;
}

SeedFinderPassPage* SeedFinderWizard::getSeedFinderPassPageForGame()
{
  SeedFinderPassPage* page;
  switch (m_game)
  {
  case GUICommon::gameSelection::Colosseum:
    page = new SeedFinderPassColosseum(this, seeds.size(), m_rtcErrorMarginSeconds, m_useWii,
                                       m_usePrecalc);
    break;
  case GUICommon::gameSelection::XD:
    page =
        new SeedFinderPassXD(this, seeds.size(), m_rtcErrorMarginSeconds, m_useWii, m_usePrecalc);
    break;
  default:
    return nullptr;
  }
  QString strResultStatus("No passes done");
  if (seeds.size() > 1)
    strResultStatus = QString::number(seeds.size()) + QString(" results");
  page->setTitle("Seed Finder Pass #" + QString::number(numberPass) + " (" + strResultStatus + ")");
  return page;
}

void SeedFinderWizard::nextSeedFinderPass()
{
  SeedFinderPassPage* page = static_cast<SeedFinderPassPage*>(currentPage());

  button(QWizard::CustomButton1)->setEnabled(false);

  m_seedFinderFuture = QtConcurrent::run([=] {
    page->showSeedFinderProgress(true);
    SPokemonRNG::getInstance()->getSystem()->seedFinder(
        page->obtainCriteria(), seeds, m_useWii, m_rtcErrorMarginSeconds, m_usePrecalc,
        [=](int nbrSeedsSimulated) { emit onUpdateSeedFinderProgress(nbrSeedsSimulated); },
        [=] { return m_cancelSeedFinderPass; });
    if (!m_cancelSeedFinderPass)
      emit onSeedFinderPassDone();
  });
}

void SeedFinderWizard::seedFinderPassDone()
{
  if (seeds.size() <= 1)
  {
    SeedFinderPassPage* page = static_cast<SeedFinderPassPage*>(currentPage());
    page->setSeedFinderDone(true);
    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch << QWizard::FinishButton;
    setButtonLayout(layout);
    m_seedFinderDone = true;
    if (seeds.size() == 1)
      setPage(pageID::End, new EndPage(this, true, seeds[0]));
    else
      setPage(pageID::End, new EndPage(this, false));
    QWizard::next();
  }
  else
  {
    numberPass++;
    setPage(numberPass, getSeedFinderPassPageForGame());
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
    SeedFinderPassPage* page = static_cast<SeedFinderPassPage*>(currentPage());
    page->setNewUsePrecalc(true);
  }
}

void SeedFinderWizard::pageChanged()
{
  if (currentId() == pageID::SeedFinderPass)
  {
    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch << QWizard::CancelButton << QWizard::CustomButton1;
    setButtonLayout(layout);

    QFileInfo info(QString::fromStdString(
        SPokemonRNG::getInstance()->getSystem()->getPrecalcFilenameForSettings(
            m_useWii, m_rtcErrorMarginSeconds)));
    m_usePrecalc = (info.exists() && info.isFile());
    if (!(info.exists() && info.isFile()))
    {
      size_t fileSize = SPokemonRNG::getInstance()->getSystem()->getPracalcFileSize(
          m_useWii, m_rtcErrorMarginSeconds);
      QMessageBox* msg = new QMessageBox(
          QMessageBox::Question, "Precalculation file",
          "Do you want to generate a precalculation file? This file may take a while to generate "
          "and "
          "be rather large, but will significantly speed up performance of the seed finder with "
          "the "
          "given game and the following settings:\n\nClock margin of error(seeconds): " +
              QString::number(m_rtcErrorMarginSeconds) +
              "\nPlatform: " + (m_useWii ? QString("Nintendo Wii") : QString("Nintendo GameCube")) +
              "\n\nEstimated file size: " + QString::number(fileSize / 1024 / 1024) +
              "MB, do you want to create it?",
          QMessageBox::No | QMessageBox::Yes, this);
      msg->exec();
      if (msg->result() == QMessageBox::Yes)
      {
        BaseRNGSystem::seedRange range =
            SPokemonRNG::getInstance()->getSystem()->getRangeForSettings(m_useWii,
                                                                         m_rtcErrorMarginSeconds);
        QProgressDialog* dlg = new QProgressDialog(
            "Precalculating " + QString::number(range.max - range.min) + " seeds...", "&Cancel", 0,
            range.max - range.min, this);
        connect(dlg, &QProgressDialog::canceled, this, [=]() { m_cancelPrecalc = true; });

        QtConcurrent::run([=]() {
          SPokemonRNG::getInstance()->getSystem()->precalculateNbrRollsBeforeTeamGeneration(
              m_useWii, m_rtcErrorMarginSeconds, [=](int value) { dlg->setValue(value); },
              [=]() { return m_cancelPrecalc; });
          emit onPrecalcDone();
        });

        dlg->exec();
      }
    }
  }
}

void SeedFinderWizard::accept()
{
  QDialog::accept();
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

StartPage::StartPage(QWidget* parent, GUICommon::gameSelection game) : QWizardPage(parent)
{
  setTitle(tr("Introduction"));

  QLabel* label = new QLabel("This wizard will guide you through finding your seed in " +
                             GUICommon::gamesStr[game] + ".\n\nPress \"Next\" to continue.");
  label->setWordWrap(true);

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addWidget(label);
  setLayout(mainlayout);
}

int StartPage::nextId() const
{
  return SeedFinderWizard::pageID::Instructions;
}

InstructionsPage::InstructionsPage(QWidget* parent, GUICommon::gameSelection game, bool useWii)
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
        "After a hardware reset or boot, insert your disc then go to Wii settings -> calendar and "
        "set the date to 01/01/2000. Then, set the time to 00:00, AS SOON AS you confirm the new "
        "time, naviguate to the disc channel and boot the game as fast as possible. Depending on "
        "your margin of error in your settings, this step may be more lenient in how fast you need "
        "to be because the margin of error corespond to the time you can WASTE considering frame "
        "perfect input. Note: when going to the disc channel, make sure you see the entire disc "
        "spinning animation. To ensure you will see it, insert your disc before going to Wii "
        "settings and wait you see the GameCube logo thumbnail on the disc channel."));
  }
  else
  {
    m_lblConsoleInstructions = new QLabel(tr(
        "After a hardware reset or boot, insert your disc then go to the GameCube main menu by "
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
        "Upon reaching the main menu in the game, naviguate to Battle Now -> Single Battle -> "
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
        "Upon reaching the main menu in the game, naviguate to VS Mode -> Quick Battle -> "
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

  QLabel* lblNext =
      new QLabel(tr("Press \"Next\" once you acknowledged the above instructions to start the seed "
                    "finding procedure."));
  lblNext->setWordWrap(true);
  m_lblConsoleInstructions->setWordWrap(true);
  m_lblGameInstructions->setWordWrap(true);

  QVBoxLayout* instructionsLayout = new QVBoxLayout;
  instructionsLayout->addWidget(lblSummary);
  instructionsLayout->addSpacing(30);
  instructionsLayout->addWidget(m_lblConsoleInstructions);
  instructionsLayout->addSpacing(30);
  instructionsLayout->addWidget(m_lblGameInstructions);
  instructionsLayout->addSpacing(30);
  instructionsLayout->addWidget(lblNext);
  instructionsLayout->addStretch();

  QWidget* instructionsWidget = new QWidget(this);
  instructionsWidget->setLayout(instructionsLayout);
  instructionsWidget->setMinimumHeight(1500);
  instructionsWidget->setMaximumWidth(525);

  QScrollArea* mainWidget = new QScrollArea(this);
  mainWidget->setWidget(instructionsWidget);
  mainWidget->setMaximumWidth(600);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(mainWidget);

  setLayout(mainLayout);
}

int InstructionsPage::nextId() const
{
  return SeedFinderWizard::pageID::SeedFinderPass;
}

EndPage::EndPage(QWidget* parent, bool sucess, u32 seed) : QWizardPage(parent)
{
  setTitle(tr("End"));

  if (sucess)
  {
    m_lblResult = new QLabel(
        "The seed finding procedure completed sucessfully.\n\n" + QString("Your current seed is ") +
            QString::number(seed, 16).toUpper() +
            QString("\n\nClick \"Finish\" to see your prediction in the previous window."),
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
  setLayout(mainlayout);
}

int EndPage::nextId() const
{
  return -1;
}
