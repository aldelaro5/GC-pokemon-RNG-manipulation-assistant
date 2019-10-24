#include "SeedFinderWizard.h"

#include <thread>

#include <QAbstractButton>
#include <QMessageBox>
#include <QPushButton>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QtConcurrent>

#include "../../PokemonRNGSystem/XD/GaleDarknessRNGSystem.h"
#include "../SPokemonRNG.h"
#include "../Settings/SConfig.h"

int SeedFinderWizard::numberPass = 1;

SeedFinderWizard::SeedFinderWizard(QWidget* parent, const GUICommon::gameSelection game)
    : QWizard(parent), m_game(game)
{
  numberPass = 1;
  m_cancelSeedFinderPass = false;
  m_seedFinderFuture = QFuture<void>();
  setPage(pageID::Start, new StartPage(this, game));
  setPage(pageID::Instructions, new InstructionsPage(this, game));
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

  connect(button(QWizard::NextButton), &QAbstractButton::clicked, this,
          &SeedFinderWizard::pageChanged);

  setWindowTitle(tr("Seed Finder Wizard"));
  setWizardStyle(QWizard::ModernStyle);

  QList<QWizard::WizardButton> btnlayout;
  btnlayout << QWizard::Stretch << QWizard::CancelButton << QWizard::NextButton;
  setButtonLayout(btnlayout);
  setFixedWidth(650);
}

SeedFinderWizard::~SeedFinderWizard()
{
  for (auto page : m_passPages)
    delete page;
}

void SeedFinderWizard::keyPressEvent(QKeyEvent* event)
{
  if ((event->key() == Qt::Key_Enter) || (event->key() == Qt::Key_Return))
  {
    if (currentId() >= pageID::SeedFinderPass)
      static_cast<QPushButton*>(button(QWizard::CustomButton1))->click();
  }
  QWizard::keyPressEvent(event);
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
    page = new SeedFinderPassColosseum(this, static_cast<int>(m_seeds.size()));
    break;
  case GUICommon::gameSelection::XD:
    page = new SeedFinderPassXD(this, static_cast<int>(m_seeds.size()));
    break;
  default:
    return nullptr;
  }
  m_passPages.append(page);
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

  page->showSeedFinderProgress(true);
  unsigned int threadCount = SConfig::getInstance().getThreadLimit();
  if (threadCount == 0)
    threadCount = std::thread::hardware_concurrency();

  m_seedFinderFuture = QtConcurrent::run([=] {
    SPokemonRNG::getCurrentSystem()->seedFinderPass(
        threadCount, page->obtainCriteria(), m_seeds,
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

void SeedFinderWizard::pageChanged()
{
  if (currentId() == pageID::SeedFinderPass)
  {
    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch << QWizard::CancelButton << QWizard::CustomButton1;
    setButtonLayout(layout);
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
        new QMessageBox(QMessageBox::Question, "Seed Finder Cancellation",
                        "Are you sure you want to cancel the seed finding procedure?",
                        QMessageBox::No | QMessageBox::Yes, this);
    cancelPrompt->exec();
    if (cancelPrompt->result() == QMessageBox::Yes)
    {
      m_cancelSeedFinderPass = true;
      m_seedFinderFuture.waitForFinished();
      QWizard::reject();
    }
    delete cancelPrompt;
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

InstructionsPage::InstructionsPage(QWidget* parent, const GUICommon::gameSelection game)
    : QWizardPage(parent)
{
  setTitle(tr("Instructions"));
  setSubTitle(tr("Follow these detailled instructions before starting the seed finding procedure"));

  QLabel* lblSummary = new QLabel(
      "This procedure involves generating random battle teams over and over which gives more and "
      "more information about your seed so that it is found by narrowing it down to one possible "
      "one.",
      this);
  lblSummary->setWordWrap(true);

  switch (game)
  {
  case GUICommon::gameSelection::Colosseum:
    m_lblGameInstructions = new QLabel(tr(
        "\nUpon reaching the main menu in the game, naviguate to Battle Now -> Single Battle -> "
        "Ultimate. You should see a confirmation screen of a battle with a randomly generated "
        "team. The seed finder will ask you for the informations displayed on this screen so make "
        "sure you input them correctly. After entering the information and confirming everything, "
        "wait that the pass is done. Once done, back off the confirmation screen (NEVER accept it "
        "or it will invalidate the procedure) and select Single Battle -> Ultimate again; you are "
        "now ready to enter the informations of the next pass. You have to do this several times "
        "until only 1 result is left, you can see the number of results after each pass in the "
        "title of the wizard page. Note: NEVER go back to the main menu during this procedure or "
        "it will invalidate it."));
    break;
  case GUICommon::gameSelection::XD:
    m_lblGameInstructions = new QLabel(tr(
        "\nUpon reaching the main menu in the game, naviguate to VS Mode -> Quick Battle -> "
        "Single Battle -> Ultimate. You should see a confirmation screen of a battle with a "
        "randomly generated team. The seed finder will ask you for the informations displayed on "
        "this screen so make sure you input them correctly. After entering the information and "
        "confirming everything, wait that the pass is done. Once done, back off the confirmation "
        "screen (NEVER accept it or it will invalidate the procedure) and select Single Battle -> "
        "Ultimate again; you are now ready to enter the informations of the next pass. You have to "
        "do this several times until only 1 result is left, you can see the number of results "
        "after each pass in the title of the wizard page. Note: NEVER go back to the main menu "
        "during this procedure or it will invalidate it."));
    break;
  default:
    m_lblGameInstructions = new QLabel("");
    break;
  }

  QLabel* lblNext = new QLabel(
      tr("\nPress \"Next\" once you acknowledged the above instructions to start the seed "
         "finding procedure."));
  lblNext->setWordWrap(true);
  m_lblGameInstructions->setWordWrap(true);

  QVBoxLayout* instructionsLayout = new QVBoxLayout;
  instructionsLayout->addWidget(lblSummary);
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

    QString predictorInstructions =
        "Predictions of the starters will appear depending on the amount of frames "
        "between pressing A on starting a new game and pressing A on the trainer name "
        "confirmation.";
    if (game == GUICommon::gameSelection::XD && GaleDarknessRNGSystem::getPalEnabled())
      predictorInstructions =
          "There will only be a single prediction in the prediction list, this will be your "
          "starter no matter how many frames you spend on the naming screen.";

    m_lblResult = new QLabel(
        "The seed finding procedure completed sucessfully.\n\n" + QString("Your current seed is ") +
            QString("%1").arg(seed, 8, 16, QChar('0')).toUpper() +
            QString(
                "\n\n" + predictorInstructions +
                " Desired predictions will appear in green while undesired ones "
                "will appear in red (you may configure the filters in the settings). If you "
                "are unsatisfied with the predictions, generate another team and click the "
                "reroll or autoreroll button. You may only go back to the main menu of the game if "
                "you are satisfied with the predictions. You may also use the stats reporter when "
                "you select a prediction which allows you to see the possible secondaries "
                "stats. ") +
            additionalNotes +
            QString("\n\nClick \"Finish\" to see your prediction in "
                    "the previous window."),
        this);
  }
  else
  {
    m_lblResult = new QLabel(
        "The seed finding procedure completed, but your current seed hasn't been found. You have "
        "to restart this entire procedure again (this implies that you must soft reset the "
        "console).");
  }
  m_lblResult->setWordWrap(true);

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addWidget(m_lblResult);
  mainlayout->addStretch();
  setLayout(mainlayout);
  m_lblResult->adjustSize();
  setFixedHeight(m_lblResult->height() + 200);
}

int EndPage::nextId() const
{
  return -1;
}
