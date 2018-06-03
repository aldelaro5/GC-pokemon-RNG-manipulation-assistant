#include "SeedFinderWizard.h"

#include <QAbstractButton>
#include <QMessageBox>
#include <QVBoxLayout>

#include "../SPokemonRNG.h"

int SeedFinderWizard::numberPass = 1;

SeedFinderWizard::SeedFinderWizard(QWidget* parent, GUICommon::gameSelection game)
    : QWizard(parent), m_game(game)
{
  numberPass = 1;
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
  if (numberPass == 1)
  {
    SPokemonRNG::getInstance()->getSystem()->seedFinder(page->obtainCriteria(), seeds, false, 5,
                                                        false, true);
  }
  else
  {
    SPokemonRNG::getInstance()->getSystem()->seedFinder(page->obtainCriteria(), seeds, false, 5,
                                                        false, false);
  }
  if (seeds.size() <= 1)
  {
    page->setSeedFinderPassDone(true);
    QList<QWizard::WizardButton> layout;
    layout << QWizard::Stretch << QWizard::FinishButton;
    setButtonLayout(layout);
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
