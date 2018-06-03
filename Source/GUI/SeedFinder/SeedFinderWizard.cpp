#include "SeedFinderWizard.h"

#include <QAbstractButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QtConcurrent>

#include "../SPokemonRNG.h"

int SeedFinderWizard::numberPass = 1;

SeedFinderWizard::SeedFinderWizard(QWidget* parent, GUICommon::gameSelection game)
    : QWizard(parent), m_game(game)
{
  numberPass = 1;
  m_cancelSeedFinderPass = false;
  m_seedFinderFuture = QFuture<void>();
  setPage(pageID::Start, new StartPage(this, game));
  setPage(pageID::SeedFinderPass, getSeedFinderPassPageForGame());
  setPage(pageID::End, new EndPage(this));
  setStartId(pageID::Start);

  setOptions(QWizard::HaveCustomButton1);
  setButtonText(QWizard::CustomButton1, "&Next Pass >");
  connect(this, &SeedFinderWizard::customButtonClicked, this, [=](int which) {
    if (which == QWizard::CustomButton1)
      nextSeedFinderPass();
  });
  connect(this, &SeedFinderWizard::onUpdateSeedFinderProgress, this, [=](int nbrSeedsSimulated) {
    static_cast<SeedFinderPassPage*>(currentPage())->setSeedFinderProgress(nbrSeedsSimulated);
  });
  connect(this, &SeedFinderWizard::onSeedFinderPassDone, this,
          &SeedFinderWizard::seedFinderPassDone);

  connect(button(QWizard::NextButton), &QAbstractButton::clicked, this,
          &SeedFinderWizard::pageChanged);

  setWindowTitle(tr("Seed Finder Wizard"));

  QList<QWizard::WizardButton> layout;
  layout << QWizard::Stretch << QWizard::CancelButton << QWizard::NextButton;
  setButtonLayout(layout);
}

SeedFinderPassPage* SeedFinderWizard::getSeedFinderPassPageForGame()
{
  switch (m_game)
  {
  case GUICommon::gameSelection::Colosseum:
    return new SeedFinderPassColosseum(this);
  case GUICommon::gameSelection::XD:
    return new SeedFinderPassXD(this);
  default:
    return nullptr;
  }
}

void SeedFinderWizard::nextSeedFinderPass()
{
  SeedFinderPassPage* page = static_cast<SeedFinderPassPage*>(currentPage());

  m_seedFinderFuture = QtConcurrent::run([=] {
    page->showSeedFinderProgress(true);
    SPokemonRNG::getInstance()->getSystem()->seedFinder(
        page->obtainCriteria(), seeds, false, 5, true,
        [=](int nbrSeedsSimulated) { emit onUpdateSeedFinderProgress(nbrSeedsSimulated); },
        [=] { return m_cancelSeedFinderPass; });
    page->showSeedFinderProgress(false);
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
    QWizard::next();
  }
  else
  {
    numberPass++;
    setPage(numberPass, getSeedFinderPassPageForGame());
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

  QVBoxLayout* layout = new QVBoxLayout;
  layout->addWidget(label);
  setLayout(layout);
}

int StartPage::nextId() const
{
  return SeedFinderWizard::pageID::SeedFinderPass;
}

EndPage::EndPage(QWidget* parent) : QWizardPage(parent)
{
  setTitle(tr("End"));
}

int EndPage::nextId() const
{
  return -1;
}
