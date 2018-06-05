#include "SeedFinderWizard.h"

#include <QAbstractButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QtConcurrent>

#include "../SPokemonRNG.h"

int SeedFinderWizard::numberPass = 1;

SeedFinderWizard::SeedFinderWizard(QWidget* parent, GUICommon::gameSelection game,
                                   int rtcErrorMarginSeconds, bool useWii, bool usePrecalc)
    : QWizard(parent), m_game(game), m_rtcErrorMarginSeconds(rtcErrorMarginSeconds),
      m_useWii(useWii), m_usePrecalc(usePrecalc)
{
  numberPass = 1;
  m_cancelSeedFinderPass = false;
  m_seedFinderFuture = QFuture<void>();
  setPage(pageID::Start, new StartPage(this, game));
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
                             GUICommon::gamesStr[game] + ".");
  label->setWordWrap(true);

  QVBoxLayout* mainlayout = new QVBoxLayout;
  mainlayout->addWidget(label);
  setLayout(mainlayout);
}

int StartPage::nextId() const
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
