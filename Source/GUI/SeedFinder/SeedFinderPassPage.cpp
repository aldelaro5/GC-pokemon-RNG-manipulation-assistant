#include "SeedFinderPassPage.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>

#include "../../PokemonRNGSystem/BaseRNGSystem.h"
#include "../../PokemonRNGSystem/Colosseum/ColosseumRNGSystem.h"
#include "../../PokemonRNGSystem/XD/GaleDarknessRNGSystem.h"
#include "../SPokemonRNG.h"
#include "SeedFinderWizard.h"

SeedFinderPassPage::SeedFinderPassPage(QWidget* parent) : QWizardPage(parent)
{
  setTitle(tr("Seed Finder Pass"));

  QLabel* label = new QLabel(QString::number(SeedFinderWizard::numberPass));
  m_pbSeedFinder = new QProgressBar(this);
  m_pbSeedFinder->setVisible(false);
  BaseRNGSystem::seedRange range =
      SPokemonRNG::getInstance()->getSystem()->getRangeForSettings(false, 5);
  m_pbSeedFinder->setMinimum(0);
  m_pbSeedFinder->setMaximum(range.max - range.min + 1);
  m_pbSeedFinder->setValue(0);

  QVBoxLayout* layout = new QVBoxLayout;
  setLayout(layout);
}

void SeedFinderPassPage::setSeedFinderDone(const bool seedFinderDone)
{
  m_seedFinderDone = seedFinderDone;
}

void SeedFinderPassPage::setSeedFinderProgress(const int nbrSeedsSimulated)
{
  m_pbSeedFinder->setValue(nbrSeedsSimulated);
}

void SeedFinderPassPage::showSeedFinderProgress(const bool showProgress)
{
  m_pbSeedFinder->setVisible(showProgress);
}

int SeedFinderPassPage::nextId() const
{
  if (m_seedFinderDone)
    return SeedFinderWizard::pageID::End;
  else
    return SeedFinderWizard::numberPass;
}

SeedFinderPassColosseum::SeedFinderPassColosseum(QWidget* parent) : SeedFinderPassPage(parent)
{
  m_playerNameIndexBtnGroup = new QButtonGroup(this);

  QRadioButton* rbtnWes = new QRadioButton("Wes", this);
  QRadioButton* rbtnSeth = new QRadioButton("Seth", this);
  QRadioButton* rbtnThomas = new QRadioButton("Thomas", this);

  m_playerNameIndexBtnGroup->addButton(rbtnWes, ColosseumRNGSystem::QuickBattlePlayerName::Wes);
  m_playerNameIndexBtnGroup->addButton(rbtnSeth, ColosseumRNGSystem::QuickBattlePlayerName::Seth);
  m_playerNameIndexBtnGroup->addButton(rbtnThomas,
                                       ColosseumRNGSystem::QuickBattlePlayerName::Thomas);

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

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(lblName);
  mainLayout->addLayout(rbtnNameLayout);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(lblTeam);
  mainLayout->addLayout(rbtnTeamLayout);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(m_pbSeedFinder);

  QWidget* mainWidget = new QWidget(this);
  mainWidget->setLayout(mainLayout);
  layout()->addWidget(mainWidget);
}

std::vector<int> SeedFinderPassColosseum::obtainCriteria()
{
  std::vector<int> criteria;
  criteria.push_back(m_playerTeamIndexBtnGroup->checkedId());
  criteria.push_back(m_playerNameIndexBtnGroup->checkedId());
  return criteria;
}

SeedFinderPassXD::SeedFinderPassXD(QWidget* parent) : SeedFinderPassPage(parent)
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

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(rbtnTeamLayout);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(lblPkmnHP);
  mainLayout->addSpacing(10);
  mainLayout->addLayout(topPkmnHPLayout);
  mainLayout->addLayout(bottomPkmnHPLayout);
  mainLayout->addSpacing(10);
  mainLayout->addWidget(m_pbSeedFinder);
  mainLayout->addStretch();

  QWidget* mainWidget = new QWidget(this);
  mainWidget->setLayout(mainLayout);
  layout()->addWidget(mainWidget);

  setMinimumWidth(600);
  setMinimumHeight(500);
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
