#include "SeedFinderPassPage.h"

#include <thread>

#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"
#include "../../PokemonRNGSystem/Colosseum/ColosseumRNGSystem.h"
#include "../../PokemonRNGSystem/XD/GaleDarknessRNGSystem.h"
#include "../SPokemonRNG.h"
#include "SeedFinderWizard.h"

SeedFinderPassPage::SeedFinderPassPage(QWidget* parent, const int nbrFoundSeeds,
                                       const int rtcErrorMarginSeconds, const bool useWii,
                                       const bool usePrecalc)
    : QWizardPage(parent), m_rtcMarginErrorSeconds(rtcErrorMarginSeconds), m_useWii(useWii),
      m_usePrecalc(usePrecalc)
{
  setSubTitle("Fill in the informations you see on the battle confirmation screen");

  QLabel* label = new QLabel(QString::number(SeedFinderWizard::numberPass));
  m_pbSeedFinder = new QProgressBar(this);
  m_pbSeedFinder->setVisible(false);
  if (nbrFoundSeeds == 0)
  {
    m_pbSeedFinder->setMinimum(0);
    m_pbSeedFinder->setMaximum(0);
    m_pbSeedFinder->setValue(0);
    m_lblSeedFinderStatus =
        new QLabel("Simulating " + QString::number(Common::nbrPossibleSeeds) + " seeds using " +
                   QString::number(std::thread::hardware_concurrency()) + " thread(s) ");
  }
  else
  {
    m_pbSeedFinder->setMinimum(0);
    m_pbSeedFinder->setMaximum(nbrFoundSeeds);
    m_pbSeedFinder->setValue(0);
    m_lblSeedFinderStatus =
        new QLabel("Simulating " + QString::number(nbrFoundSeeds) + " seeds using " +
                   QString::number(std::thread::hardware_concurrency()) + " thread(s)...");
  }
  m_lblSeedFinderStatus->setVisible(false);
  m_lblSeedFinderStatus->setAlignment(Qt::AlignHCenter);
  m_lblSeedFinderStatus->setWordWrap(true);

  m_inputWidget = new QWidget(this);
  QVBoxLayout* layout = new QVBoxLayout;
  setLayout(layout);
}

SeedFinderPassPage::~SeedFinderPassPage()
{
}

void SeedFinderPassPage::setSeedFinderDone(const bool seedFinderDone)
{
  m_seedFinderDone = seedFinderDone;
}

void SeedFinderPassPage::setSeedFinderProgress(const long nbrSeedsSimulated)
{
  m_pbSeedFinder->setValue(nbrSeedsSimulated);
}

void SeedFinderPassPage::setNewUsePrecalc(const bool newUsePrecalc)
{
  m_usePrecalc = newUsePrecalc;
  m_lblSeedFinderStatus->setText(
      "Simulating " + QString::number(Common::nbrPossibleSeeds) + " seeds using " +
      QString::number(std::thread::hardware_concurrency()) + " thread(s) " +
      (m_usePrecalc ? QString("with") : QString("without")) + " precalculation...");
}

void SeedFinderPassPage::showSeedFinderProgress(const bool showProgress)
{
  m_pbSeedFinder->setVisible(showProgress);
  m_lblSeedFinderStatus->setVisible(showProgress);
  m_inputWidget->setEnabled(!showProgress);
}

int SeedFinderPassPage::nextId() const
{
  if (m_seedFinderDone)
    return SeedFinderWizard::pageID::End;
  else
    return SeedFinderWizard::numberPass + SeedFinderWizard::pageID::SeedFinderPass;
}

SeedFinderPassColosseum::SeedFinderPassColosseum(QWidget* parent, const int nbrFoundSeeds,
                                                 const int rtcErrorMarginSeconds, const bool useWii,
                                                 const bool usePrecalc)
    : SeedFinderPassPage(parent, nbrFoundSeeds, rtcErrorMarginSeconds, useWii, usePrecalc)
{
  m_playerNameIndexBtnGroup = new QButtonGroup(this);

  QRadioButton* rbtnWes = new QRadioButton("Wes", this);
  QRadioButton* rbtnSeth = new QRadioButton("Seth", this);
  QRadioButton* rbtnThomas = new QRadioButton("Thomas", this);

  m_playerNameIndexBtnGroup->addButton(rbtnWes, ColosseumRNGSystem::QuickBattlePlayerName::Wes);
  m_playerNameIndexBtnGroup->addButton(rbtnSeth, ColosseumRNGSystem::QuickBattlePlayerName::Seth);
  m_playerNameIndexBtnGroup->addButton(rbtnThomas,
                                       ColosseumRNGSystem::QuickBattlePlayerName::Thomas);

  m_playerNameIndexBtnGroup->button(0)->setChecked(true);

  for (auto button : m_playerNameIndexBtnGroup->buttons())
  {
    button->setMinimumWidth(125);
  }

  QHBoxLayout* rbtnNameLayout = new QHBoxLayout;
  rbtnNameLayout->addWidget(rbtnWes);
  rbtnNameLayout->addWidget(rbtnSeth);
  rbtnNameLayout->addWidget(rbtnThomas);
  rbtnNameLayout->addStretch();

  m_playerTeamIndexBtnGroup = new QButtonGroup(this);

  QRadioButton* rbtnBlaziken = new QRadioButton("Blazkien", this);
  QRadioButton* rbtnEntei = new QRadioButton("Entei", this);
  QRadioButton* rbtnSwampert = new QRadioButton("Swampert", this);
  QRadioButton* rbtnRaikou = new QRadioButton("Raikou", this);
  QRadioButton* rbtnMeganium = new QRadioButton("Meganium", this);
  QRadioButton* rbtnSuicune = new QRadioButton("Suicune", this);
  QRadioButton* rbtnMetagross = new QRadioButton("Metagross", this);
  QRadioButton* rbtnHeracross = new QRadioButton("Heracross", this);

  m_playerTeamIndexBtnGroup->addButton(rbtnBlaziken,
                                       ColosseumRNGSystem::QuickBattleTeamLeader::Blaziken);
  m_playerTeamIndexBtnGroup->addButton(rbtnEntei, ColosseumRNGSystem::QuickBattleTeamLeader::Entei);
  m_playerTeamIndexBtnGroup->addButton(rbtnSwampert,
                                       ColosseumRNGSystem::QuickBattleTeamLeader::Swampert);
  m_playerTeamIndexBtnGroup->addButton(rbtnRaikou,
                                       ColosseumRNGSystem::QuickBattleTeamLeader::Raikou);
  m_playerTeamIndexBtnGroup->addButton(rbtnMeganium,
                                       ColosseumRNGSystem::QuickBattleTeamLeader::Meganium);
  m_playerTeamIndexBtnGroup->addButton(rbtnSuicune,
                                       ColosseumRNGSystem::QuickBattleTeamLeader::Suicune);
  m_playerTeamIndexBtnGroup->addButton(rbtnMetagross,
                                       ColosseumRNGSystem::QuickBattleTeamLeader::Metagross);
  m_playerTeamIndexBtnGroup->addButton(rbtnHeracross,
                                       ColosseumRNGSystem::QuickBattleTeamLeader::Heracross);

  m_playerTeamIndexBtnGroup->button(0)->setChecked(true);

  for (auto button : m_playerTeamIndexBtnGroup->buttons())
  {
    button->setMinimumWidth(125);
  }

  QHBoxLayout* rbtnTeamLayout1 = new QHBoxLayout;
  rbtnTeamLayout1->addWidget(rbtnBlaziken);
  rbtnTeamLayout1->addWidget(rbtnEntei);
  rbtnTeamLayout1->addWidget(rbtnSwampert);
  rbtnTeamLayout1->addWidget(rbtnRaikou);
  rbtnTeamLayout1->addStretch();

  QHBoxLayout* rbtnTeamLayout2 = new QHBoxLayout;
  rbtnTeamLayout2->addWidget(rbtnMeganium);
  rbtnTeamLayout2->addWidget(rbtnSuicune);
  rbtnTeamLayout2->addWidget(rbtnMetagross);
  rbtnTeamLayout2->addWidget(rbtnHeracross);
  rbtnTeamLayout2->addStretch();

  QVBoxLayout* rbtnTeamLayout = new QVBoxLayout;
  rbtnTeamLayout->addLayout(rbtnTeamLayout1);
  rbtnTeamLayout->addLayout(rbtnTeamLayout2);

  QLabel* lblName = new QLabel("Choose your generated trainer name");
  QLabel* lblTeam = new QLabel("Choose the leader of your generated team");

  QVBoxLayout* inputLayout = new QVBoxLayout;
  inputLayout->addWidget(lblName);
  inputLayout->addLayout(rbtnNameLayout);
  inputLayout->addSpacing(10);
  inputLayout->addWidget(lblTeam);
  inputLayout->addLayout(rbtnTeamLayout);
  inputLayout->addSpacing(10);

  m_inputWidget->setLayout(inputLayout);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_inputWidget);
  mainLayout->addWidget(m_lblSeedFinderStatus);
  mainLayout->addWidget(m_pbSeedFinder);
  mainLayout->addStretch();

  QWidget* mainWidget = new QWidget(this);
  mainWidget->setLayout(mainLayout);
  layout()->addWidget(mainWidget);
  adjustSize();
  setFixedHeight(height() + 100);
}

std::vector<int> SeedFinderPassColosseum::obtainCriteria()
{
  std::vector<int> criteria;
  criteria.push_back(m_playerTeamIndexBtnGroup->checkedId());
  criteria.push_back(m_playerNameIndexBtnGroup->checkedId());
  return criteria;
}

SeedFinderPassXD::SeedFinderPassXD(QWidget* parent, const int nbrFoundSeeds,
                                   const int rtcErrorMarginSeconds, const bool useWii,
                                   const bool usePrecalc)
    : SeedFinderPassPage(parent, nbrFoundSeeds, rtcErrorMarginSeconds, useWii, usePrecalc)
{
  m_playerTeamIndexBtnGroup = new QButtonGroup(this);

  QRadioButton* rbtnMewtwo = new QRadioButton("Mewtwo", this);
  QRadioButton* rbtnMew = new QRadioButton("Mew", this);
  QRadioButton* rbtnDeoxys = new QRadioButton("Deoxys", this);
  QRadioButton* rbtnRayquaza = new QRadioButton("Rayquaza", this);
  QRadioButton* rbtnJirachi = new QRadioButton("Jirachi", this);

  m_playerTeamIndexBtnGroup->addButton(rbtnMewtwo,
                                       GaleDarknessRNGSystem::BattleNowTeamLeaderPlayer::Mewtwo);
  m_playerTeamIndexBtnGroup->addButton(rbtnMew,
                                       GaleDarknessRNGSystem::BattleNowTeamLeaderPlayer::Mew);
  m_playerTeamIndexBtnGroup->addButton(rbtnDeoxys,
                                       GaleDarknessRNGSystem::BattleNowTeamLeaderPlayer::Deoxys);
  m_playerTeamIndexBtnGroup->addButton(rbtnRayquaza,
                                       GaleDarknessRNGSystem::BattleNowTeamLeaderPlayer::Rayquaza);
  m_playerTeamIndexBtnGroup->addButton(rbtnJirachi,
                                       GaleDarknessRNGSystem::BattleNowTeamLeaderPlayer::Jirachi);

  m_playerTeamIndexBtnGroup->button(0)->setChecked(true);

  for (auto button : m_playerTeamIndexBtnGroup->buttons())
  {
    button->setMinimumWidth(125);
  }

  QLabel* lblPlayerTeam = new QLabel("Choose the leader of your generated team", this);
  lblPlayerTeam->setWordWrap(true);

  QVBoxLayout* rbtnPlayerTeamLayout = new QVBoxLayout;
  rbtnPlayerTeamLayout->addWidget(lblPlayerTeam);
  rbtnPlayerTeamLayout->addWidget(rbtnMewtwo);
  rbtnPlayerTeamLayout->addWidget(rbtnMew);
  rbtnPlayerTeamLayout->addWidget(rbtnDeoxys);
  rbtnPlayerTeamLayout->addWidget(rbtnRayquaza);
  rbtnPlayerTeamLayout->addWidget(rbtnJirachi);

  m_enemyTeamIndexBtnGroup = new QButtonGroup(this);

  QRadioButton* rbtnArticuno = new QRadioButton("Articuno", this);
  QRadioButton* rbtnZapdos = new QRadioButton("Zapdos", this);
  QRadioButton* rbtnMoltres = new QRadioButton("Moltres", this);
  QRadioButton* rbtnKangaskhan = new QRadioButton("Kangaskhan", this);
  QRadioButton* rbtnLatias = new QRadioButton("Latias", this);

  m_enemyTeamIndexBtnGroup->addButton(rbtnArticuno,
                                      GaleDarknessRNGSystem::BattleNowTeamLeaderEnemy::Articuno);
  m_enemyTeamIndexBtnGroup->addButton(rbtnZapdos,
                                      GaleDarknessRNGSystem::BattleNowTeamLeaderEnemy::Zapidos);
  m_enemyTeamIndexBtnGroup->addButton(rbtnMoltres,
                                      GaleDarknessRNGSystem::BattleNowTeamLeaderEnemy::Moltres);
  m_enemyTeamIndexBtnGroup->addButton(rbtnKangaskhan,
                                      GaleDarknessRNGSystem::BattleNowTeamLeaderEnemy::Kangaskhan);
  m_enemyTeamIndexBtnGroup->addButton(rbtnLatias,
                                      GaleDarknessRNGSystem::BattleNowTeamLeaderEnemy::Latias);

  m_enemyTeamIndexBtnGroup->button(0)->setChecked(true);

  for (auto button : m_enemyTeamIndexBtnGroup->buttons())
  {
    button->setMinimumWidth(150);
  }

  QLabel* lblEnemyTeam = new QLabel("Choose the leader of your opponent's generated team", this);
  lblEnemyTeam->setWordWrap(true);

  QVBoxLayout* rbtnEnemyTeamLayout = new QVBoxLayout;
  rbtnEnemyTeamLayout->addWidget(lblEnemyTeam);
  rbtnEnemyTeamLayout->addWidget(rbtnArticuno);
  rbtnEnemyTeamLayout->addWidget(rbtnZapdos);
  rbtnEnemyTeamLayout->addWidget(rbtnMoltres);
  rbtnEnemyTeamLayout->addWidget(rbtnKangaskhan);
  rbtnEnemyTeamLayout->addWidget(rbtnLatias);

  QHBoxLayout* rbtnTeamLayout = new QHBoxLayout;
  rbtnTeamLayout->addLayout(rbtnPlayerTeamLayout);
  rbtnTeamLayout->addLayout(rbtnEnemyTeamLayout);

  QLabel* lblTopLeftPkmnHP = new QLabel("Top left", this);
  lblTopLeftPkmnHP->setWordWrap(true);
  lblTopLeftPkmnHP->setAlignment(Qt::AlignHCenter);
  m_spnTopLeftPkmnHP = new QSpinBox(this);
  m_spnTopLeftPkmnHP->setMinimumWidth(150);
  m_spnTopLeftPkmnHP->setMinimum(1);
  m_spnTopLeftPkmnHP->setMaximum(999);
  QVBoxLayout* topLeftPkmnHPLayout = new QVBoxLayout;
  topLeftPkmnHPLayout->addWidget(lblTopLeftPkmnHP);
  topLeftPkmnHPLayout->addWidget(m_spnTopLeftPkmnHP);

  QLabel* lblBottomLeftPkmnHP = new QLabel("Bottom left", this);
  lblBottomLeftPkmnHP->setWordWrap(true);
  lblBottomLeftPkmnHP->setAlignment(Qt::AlignHCenter);
  m_spnBottomLeftPkmnHP = new QSpinBox(this);
  m_spnBottomLeftPkmnHP->setMinimumWidth(150);
  m_spnBottomLeftPkmnHP->setMinimum(1);
  m_spnBottomLeftPkmnHP->setMaximum(999);
  QVBoxLayout* bottomLeftPkmnHPLayout = new QVBoxLayout;
  bottomLeftPkmnHPLayout->addWidget(lblBottomLeftPkmnHP);
  bottomLeftPkmnHPLayout->addWidget(m_spnBottomLeftPkmnHP);

  QLabel* lblTopRightPkmnHP = new QLabel("Top right", this);
  lblTopRightPkmnHP->setWordWrap(true);
  lblTopRightPkmnHP->setAlignment(Qt::AlignHCenter);
  m_spnTopRightPkmnHP = new QSpinBox(this);
  m_spnTopRightPkmnHP->setMinimumWidth(150);
  m_spnTopRightPkmnHP->setMinimum(1);
  m_spnTopRightPkmnHP->setMaximum(999);
  QVBoxLayout* topRightPkmnHPLayout = new QVBoxLayout;
  topRightPkmnHPLayout->addWidget(lblTopRightPkmnHP);
  topRightPkmnHPLayout->addWidget(m_spnTopRightPkmnHP);

  QLabel* lblBottomRightPkmnHP = new QLabel("Bottom right", this);
  lblBottomRightPkmnHP->setWordWrap(true);
  lblBottomRightPkmnHP->setAlignment(Qt::AlignHCenter);
  m_spnBottomRightPkmnHP = new QSpinBox(this);
  m_spnBottomRightPkmnHP->setMinimumWidth(150);
  m_spnBottomRightPkmnHP->setMinimum(1);
  m_spnBottomRightPkmnHP->setMaximum(999);
  QVBoxLayout* bottomRightPkmnHPLayout = new QVBoxLayout;
  bottomRightPkmnHPLayout->addWidget(lblBottomRightPkmnHP);
  bottomRightPkmnHPLayout->addWidget(m_spnBottomRightPkmnHP);

  QHBoxLayout* topPkmnHPLayout = new QHBoxLayout;
  topPkmnHPLayout->addStretch();
  topPkmnHPLayout->addLayout(topLeftPkmnHPLayout);
  topPkmnHPLayout->addStretch();
  topPkmnHPLayout->addLayout(topRightPkmnHPLayout);
  topPkmnHPLayout->addStretch();

  QHBoxLayout* bottomPkmnHPLayout = new QHBoxLayout;
  bottomPkmnHPLayout->addStretch();
  bottomPkmnHPLayout->addLayout(bottomLeftPkmnHPLayout);
  bottomPkmnHPLayout->addStretch();
  bottomPkmnHPLayout->addLayout(bottomRightPkmnHPLayout);
  bottomPkmnHPLayout->addStretch();

  QLabel* lblPkmnHP = new QLabel("Enter the HP of the indicated Pokémon:", this);
  lblPkmnHP->setAlignment(Qt::AlignHCenter);

  QVBoxLayout* inputLayout = new QVBoxLayout;
  inputLayout->addLayout(rbtnTeamLayout);
  inputLayout->addSpacing(10);
  inputLayout->addWidget(lblPkmnHP);
  inputLayout->addSpacing(10);
  inputLayout->addLayout(topPkmnHPLayout);
  inputLayout->addLayout(bottomPkmnHPLayout);
  inputLayout->addSpacing(10);

  m_inputWidget->setLayout(inputLayout);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_inputWidget);
  mainLayout->addWidget(m_lblSeedFinderStatus);
  mainLayout->addWidget(m_pbSeedFinder);
  mainLayout->addStretch();

  QWidget* mainWidget = new QWidget(this);
  mainWidget->setLayout(mainLayout);
  layout()->addWidget(mainWidget);
  adjustSize();
  setFixedHeight(height() + 100);
}

std::vector<int> SeedFinderPassXD::obtainCriteria()
{
  std::vector<int> criteria;
  criteria.push_back(m_playerTeamIndexBtnGroup->checkedId());
  criteria.push_back(m_enemyTeamIndexBtnGroup->checkedId());
  criteria.push_back(m_spnTopLeftPkmnHP->value());
  criteria.push_back(m_spnBottomLeftPkmnHP->value());
  criteria.push_back(m_spnTopRightPkmnHP->value());
  criteria.push_back(m_spnBottomRightPkmnHP->value());
  return criteria;
}
