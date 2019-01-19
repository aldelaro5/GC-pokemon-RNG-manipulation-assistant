#include "StatsReporterWidget.h"

#include <QFormLayout>
#include <QHeaderView>

#include "../SPokemonRNG.h"

StatsReporterWidget::StatsReporterWidget(QWidget* parent) : QWidget(parent)
{
  QLabel* lblTitle = new QLabel("Stats reporter");
  lblTitle->setAlignment(Qt::AlignCenter);

  m_cmbSecondaryPokemon = new QComboBox;
  QLabel* lblSecondaryPokemon = new QLabel("Secondary Pokémon:");
  lblSecondaryPokemon->setAlignment(Qt::AlignRight);

  m_cmbGender = new QComboBox();
  m_cmbGender->addItems({"", "Male", "Female"});
  m_cmbGender->setCurrentIndex(0);

  QFormLayout* secondaryPokemonLayout = new QFormLayout;
  secondaryPokemonLayout->addRow("Secondary Pokémon", m_cmbSecondaryPokemon);
  secondaryPokemonLayout->addRow("Gender", m_cmbGender);
  secondaryPokemonLayout->setLabelAlignment(Qt::AlignRight);

  QLabel* lblStats = new QLabel("Stats (fill these to get the possibilities)");

  m_cmbHpStat = new QComboBox();
  m_cmbAtkStat = new QComboBox();
  m_cmbDefStat = new QComboBox();
  m_cmbSpAtkStat = new QComboBox();
  m_cmbSpDefStat = new QComboBox();
  m_cmbSpeedStat = new QComboBox();

  PokemonPropertiesFrame* starterDummyFrame1 = new PokemonPropertiesFrame("", QPixmap());
  PokemonPropertiesFrame* starterDummyFrame2 = new PokemonPropertiesFrame("", QPixmap());
  m_secondaryFrame = new PokemonPropertiesFrame("", QPixmap());

  m_starterFrames.append(starterDummyFrame1);
  m_starterFrames.append(starterDummyFrame2);

  m_tblSecondaryPossibilities = new QTableWidget();
  m_tblSecondaryPossibilities->verticalHeader()->hide();
  m_tblSecondaryPossibilities->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_tblSecondaryPossibilities->setSelectionMode(QAbstractItemView::SingleSelection);
  m_tblSecondaryPossibilities->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_tblSecondaryPossibilities->setColumnCount(10);
  m_tblSecondaryPossibilities->setRowCount(0);
  m_possibilitiesHeaderLabels = QStringList(
      {"Seed", "Nature", "HP", "Atk", "Def", "SpA", "SpD", "Spe", "H. Power", "Gender"});
  m_tblSecondaryPossibilities->setHorizontalHeaderLabels(m_possibilitiesHeaderLabels);
  m_tblSecondaryPossibilities->setMinimumHeight(200);
  m_tblSecondaryPossibilities->resizeColumnsToContents();
  connect(m_tblSecondaryPossibilities, &QTableWidget::itemSelectionChanged, this,
          &StatsReporterWidget::selectedPossibilityChanged);

  m_lblResultsCount = new QLabel("0 result(s)");

  QFormLayout* leftStatsLayout = new QFormLayout();
  leftStatsLayout->addRow("HP", m_cmbHpStat);
  leftStatsLayout->addRow("Attack", m_cmbAtkStat);
  leftStatsLayout->addRow("Defense", m_cmbDefStat);
  leftStatsLayout->setLabelAlignment(Qt::AlignRight);

  QFormLayout* RightStatsLayout = new QFormLayout();
  RightStatsLayout->addRow("Sp. Atk", m_cmbSpDefStat);
  RightStatsLayout->addRow("Sp. Def", m_cmbSpAtkStat);
  RightStatsLayout->addRow("Speed", m_cmbSpeedStat);
  RightStatsLayout->setLabelAlignment(Qt::AlignRight);

  QHBoxLayout* statsLayout = new QHBoxLayout;
  statsLayout->addLayout(leftStatsLayout);
  statsLayout->addLayout(RightStatsLayout);

  m_startersLayout = new QVBoxLayout;

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(lblTitle);
  mainLayout->addLayout(secondaryPokemonLayout);
  mainLayout->addSpacing(20);
  mainLayout->addWidget(lblStats);
  mainLayout->addLayout(statsLayout);
  mainLayout->addWidget(m_tblSecondaryPossibilities);
  mainLayout->addWidget(m_lblResultsCount);
  mainLayout->addLayout(m_startersLayout, Qt::AlignCenter);
  mainLayout->addWidget(m_secondaryFrame, Qt::AlignCenter);

  setLayout(mainLayout);
  updateStartersLayout();

  connect(m_cmbGender, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &StatsReporterWidget::onStatsGenderChanged);

  connect(m_cmbHpStat, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &StatsReporterWidget::onStatsGenderChanged);
  connect(m_cmbAtkStat, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &StatsReporterWidget::onStatsGenderChanged);
  connect(m_cmbDefStat, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &StatsReporterWidget::onStatsGenderChanged);
  connect(m_cmbSpAtkStat, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &StatsReporterWidget::onStatsGenderChanged);
  connect(m_cmbSpDefStat, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &StatsReporterWidget::onStatsGenderChanged);
  connect(m_cmbSpeedStat, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &StatsReporterWidget::onStatsGenderChanged);
}

void StatsReporterWidget::gameChanged(GUICommon::gameSelection game)
{
  if (game == GUICommon::gameSelection::Unselected)
    return;

  reset();
  m_cmbSecondaryPokemon->disconnect();

  for (auto frame : m_starterFrames)
    delete frame;
  m_starterFrames.clear();

  m_cmbSecondaryPokemon->clear();

  if (game == GUICommon::gameSelection::Colosseum)
  {
    PokemonPropertiesFrame* espeonFrame =
        new PokemonPropertiesFrame("Espeon", pixmapForPokemon("Espeon"));
    PokemonPropertiesFrame* umbreonFrame =
        new PokemonPropertiesFrame("Umbreon", pixmapForPokemon("Umbreon"));
    m_starterFrames.append(espeonFrame);
    m_starterFrames.append(umbreonFrame);
    m_cmbSecondaryPokemon->addItems({"Quilava", "Croconaw", "Bayleef"});
  }
  else if (game == GUICommon::gameSelection::XD)
  {
    PokemonPropertiesFrame* espeonFrame =
        new PokemonPropertiesFrame("Espeon", pixmapForPokemon("Espeon"));
    m_starterFrames.append(espeonFrame);
    m_cmbSecondaryPokemon->addItems({"Teddiursa"});
  }

  m_cmbSecondaryPokemon->setCurrentIndex(0);
  connect(m_cmbSecondaryPokemon, QOverload<int>::of(&QComboBox::currentIndexChanged),
          [=](int index) { onSecondaryChanged(index); });

  m_secondaryFrame->setPokemonName(m_cmbSecondaryPokemon->currentText());
  m_secondaryFrame->setPokemonIcon(pixmapForPokemon(m_cmbSecondaryPokemon->currentText()));
  updateStartersLayout();
}

void StatsReporterWidget::startersPredictionChanged(BaseRNGSystem::StartersPrediction starters)
{
  reset();
  updateStatsSelection();

  for (int i = 0; i < m_starterFrames.size(); i++)
    m_starterFrames[i]->setPokemonProperties(starters.starters[i]);

  SPokemonRNG::getCurrentSystem()->generateAllSecondaryPokemonsInSearchRange(
      starters.startingSeed, m_cmbSecondaryPokemon->currentIndex());
}

void StatsReporterWidget::onSecondaryChanged(int newIndex)
{
  m_secondaryFrame->setPokemonName(m_cmbSecondaryPokemon->currentText());
  m_secondaryFrame->setPokemonIcon(pixmapForPokemon(m_cmbSecondaryPokemon->currentText()));

  updateStatsSelection();
}

void StatsReporterWidget::updateStatsSelection()
{
  std::array<BaseRNGSystem::StatsRange, 6> statsRange =
      SPokemonRNG::getCurrentSystem()->getPokemonStatsRange(m_cmbSecondaryPokemon->currentIndex());

  resetStatsSelection();

  for (int i = statsRange[0].min; i <= statsRange[0].max; i++)
    m_cmbHpStat->addItem(QString::number(i));

  for (int i = statsRange[1].min; i <= statsRange[1].max; i++)
    m_cmbAtkStat->addItem(QString::number(i));

  for (int i = statsRange[2].min; i <= statsRange[2].max; i++)
    m_cmbDefStat->addItem(QString::number(i));

  for (int i = statsRange[3].min; i <= statsRange[3].max; i++)
    m_cmbSpAtkStat->addItem(QString::number(i));

  for (int i = statsRange[4].min; i <= statsRange[4].max; i++)
    m_cmbSpDefStat->addItem(QString::number(i));

  for (int i = statsRange[5].min; i <= statsRange[5].max; i++)
    m_cmbSpeedStat->addItem(QString::number(i));
}

void StatsReporterWidget::updateStartersLayout()
{
  QLayoutItem* item;
  while ((item = m_startersLayout->layout()->takeAt(0)) != nullptr)
  {
    delete item->widget();
    delete item;
  }

  for (auto frame : m_starterFrames)
    m_startersLayout->addWidget(frame, Qt::AlignCenter);
}

QPixmap StatsReporterWidget::pixmapForPokemon(QString name)
{
  if (name == "Umbreon")
    return QPixmap("Resources/MDP197.png");
  else if (name == "Espeon")
    return QPixmap("Resources/MDP196.png");
  else if (name == "Quilava")
    return QPixmap("Resources/MDP156.png");
  else if (name == "Croconaw")
    return QPixmap("Resources/MDP159.png");
  else if (name == "Bayleef")
    return QPixmap("Resources/MDP153.png");
  else if (name == "Teddiursa")
    return QPixmap("Resources/MDP217.png");
  else
    return QPixmap();
}

void StatsReporterWidget::onStatsGenderChanged()
{
  int hp = (m_cmbHpStat->currentIndex() != 0) ? m_cmbHpStat->currentText().toInt() : -1;
  int atk = (m_cmbAtkStat->currentIndex() != 0) ? m_cmbAtkStat->currentText().toInt() : -1;
  int def = (m_cmbDefStat->currentIndex() != 0) ? m_cmbDefStat->currentText().toInt() : -1;
  int spAtk = (m_cmbSpAtkStat->currentIndex() != 0) ? m_cmbSpAtkStat->currentText().toInt() : -1;
  int spDef = (m_cmbSpDefStat->currentIndex() != 0) ? m_cmbSpDefStat->currentText().toInt() : -1;
  int speed = (m_cmbSpeedStat->currentIndex() != 0) ? m_cmbSpeedStat->currentText().toInt() : -1;
  int gender = (m_cmbGender->currentIndex() != 0) ? m_cmbGender->currentIndex() - 1 : -1;

  m_filteredCandidates = SPokemonRNG::getCurrentSystem()->getFilteredSecondaryPokemon(
      hp, atk, def, spAtk, spDef, speed, gender);

  if (m_filteredCandidates.size() <= c_maxResultsShown)
  {
    m_tblSecondaryPossibilities->setRowCount(static_cast<int>(m_filteredCandidates.size()));
    for (int i = 0; i < m_filteredCandidates.size(); i++)
    {
      QTableWidgetItem* seedItem = new QTableWidgetItem(
          QString("%1").arg(m_filteredCandidates[i].startingSeed, 8, 16, QChar('0')).toUpper());
      QTableWidgetItem* natureItem = new QTableWidgetItem(
          GUICommon::naturesStr[m_filteredCandidates[i].properties.natureIndex]);
      QTableWidgetItem* hpItem =
          new QTableWidgetItem(QString::number(m_filteredCandidates[i].properties.hpIV));
      QTableWidgetItem* atkItem =
          new QTableWidgetItem(QString::number(m_filteredCandidates[i].properties.atkIV));
      QTableWidgetItem* defItem =
          new QTableWidgetItem(QString::number(m_filteredCandidates[i].properties.defIV));
      QTableWidgetItem* spAtkItem =
          new QTableWidgetItem(QString::number(m_filteredCandidates[i].properties.spAtkIV));
      QTableWidgetItem* spDefItem =
          new QTableWidgetItem(QString::number(m_filteredCandidates[i].properties.spDefIV));
      QTableWidgetItem* speedItem =
          new QTableWidgetItem(QString::number(m_filteredCandidates[i].properties.speedIV));
      QTableWidgetItem* hiddenPowerItem = new QTableWidgetItem(
          GUICommon::typesStr[m_filteredCandidates[i].properties.hiddenPowerTypeIndex] +
          QString(" ") + QString::number(m_filteredCandidates[i].properties.hiddenPowerPower));
      QTableWidgetItem* genderItem =
          new QTableWidgetItem(m_filteredCandidates[i].properties.genderIndex ? "Female" : "Male");

      m_tblSecondaryPossibilities->setItem(i, 0, seedItem);
      m_tblSecondaryPossibilities->setItem(i, 1, natureItem);
      m_tblSecondaryPossibilities->setItem(i, 2, hpItem);
      m_tblSecondaryPossibilities->setItem(i, 3, atkItem);
      m_tblSecondaryPossibilities->setItem(i, 4, defItem);
      m_tblSecondaryPossibilities->setItem(i, 5, spAtkItem);
      m_tblSecondaryPossibilities->setItem(i, 6, spDefItem);
      m_tblSecondaryPossibilities->setItem(i, 7, speedItem);
      m_tblSecondaryPossibilities->setItem(i, 8, hiddenPowerItem);
      m_tblSecondaryPossibilities->setItem(i, 9, genderItem);
    }

    m_tblSecondaryPossibilities->resizeColumnsToContents();
    m_lblResultsCount->setText(QString::number(m_filteredCandidates.size()) +
                               QString(" result(s)"));
  }
  else
  {
    m_lblResultsCount->setText(QString::number(m_filteredCandidates.size()) +
                               QString(" result(s) (not shown)"));
  }
}

void StatsReporterWidget::selectedPossibilityChanged()
{
  m_secondaryFrame->reset();
  m_secondaryFrame->setPokemonProperties(
      m_filteredCandidates[m_tblSecondaryPossibilities->currentRow()].properties);
}

void StatsReporterWidget::resetStatsSelection()
{
  m_cmbHpStat->blockSignals(true);
  m_cmbAtkStat->blockSignals(true);
  m_cmbDefStat->blockSignals(true);
  m_cmbSpAtkStat->blockSignals(true);
  m_cmbSpDefStat->blockSignals(true);
  m_cmbSpeedStat->blockSignals(true);

  m_cmbHpStat->clear();
  m_cmbHpStat->addItem("");
  m_cmbAtkStat->clear();
  m_cmbAtkStat->addItem("");
  m_cmbDefStat->clear();
  m_cmbDefStat->addItem("");
  m_cmbSpAtkStat->clear();
  m_cmbSpAtkStat->addItem("");
  m_cmbSpDefStat->clear();
  m_cmbSpDefStat->addItem("");
  m_cmbSpeedStat->clear();
  m_cmbSpeedStat->addItem("");

  m_cmbHpStat->blockSignals(false);
  m_cmbAtkStat->blockSignals(false);
  m_cmbDefStat->blockSignals(false);
  m_cmbSpAtkStat->blockSignals(false);
  m_cmbSpDefStat->blockSignals(false);
  m_cmbSpeedStat->blockSignals(false);

  m_cmbGender->setCurrentIndex(0);

  m_tblSecondaryPossibilities->clear();
  m_tblSecondaryPossibilities->setRowCount(0);
  m_tblSecondaryPossibilities->setHorizontalHeaderLabels(m_possibilitiesHeaderLabels);
  m_lblResultsCount->setText("0 result(s)");
}

void StatsReporterWidget::reset()
{
  resetStatsSelection();

  for (auto frame : m_starterFrames)
    frame->reset();

  m_secondaryFrame->reset();
}
