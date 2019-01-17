#include "StatsReporterWidget.h"

#include <QFormLayout>
#include <QHeaderView>

StatsReporterWidget::StatsReporterWidget(QWidget* parent) : QWidget(parent)
{
  QLabel* lblTitle = new QLabel("Stats reporter");
  lblTitle->setAlignment(Qt::AlignCenter);

  m_cmbSecondaryPokemon = new QComboBox;
  QLabel* lblSecondaryPokemon = new QLabel("Secondary Pokémon:");
  lblSecondaryPokemon->setAlignment(Qt::AlignRight);

  m_cmbGender = new QComboBox();
  m_cmbGender->addItems({"", "Male", "Female"});

  QFormLayout* secondaryPokemonLayout = new QFormLayout;
  secondaryPokemonLayout->addRow("Secondary Pokémon", m_cmbSecondaryPokemon);
  secondaryPokemonLayout->addRow("Gender", m_cmbGender);
  secondaryPokemonLayout->setLabelAlignment(Qt::AlignRight);

  QLabel* lblStats = new QLabel("Stats");

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
  m_tblSecondaryPossibilities->setColumnCount(9);
  m_tblSecondaryPossibilities->setRowCount(0);
  m_tblSecondaryPossibilities->setHorizontalHeaderLabels(
      {"RNG calls", "Nature", "HP", "Atk", "Def", "SpA", "SpD", "Spe", "H. Power"});
  m_tblSecondaryPossibilities->resizeColumnsToContents();

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
  mainLayout->addLayout(m_startersLayout, Qt::AlignCenter);
  mainLayout->addWidget(m_secondaryFrame, Qt::AlignCenter);

  setLayout(mainLayout);
  updateStartersLayout();
}

void StatsReporterWidget::gameChanged(GUICommon::gameSelection game)
{
  if (game == GUICommon::gameSelection::Unselected)
    return;

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
  m_secondaryFrame->setPokemonName(m_cmbSecondaryPokemon->currentText());
  m_secondaryFrame->setPokemonIcon(pixmapForPokemon(m_cmbSecondaryPokemon->currentText()));
  updateStartersLayout();
}

void StatsReporterWidget::startersPredictionChanged(BaseRNGSystem::StartersPrediction starters)
{
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
  if (name == "Espeon")
    return QPixmap("Resources/MDP196.png");
  if (name == "Quilava")
    return QPixmap("Resources/MDP156.png");
  if (name == "Croconaw")
    return QPixmap("Resources/MDP159.png");
  if (name == "Bayleef")
    return QPixmap("Resources/MDP153.png");
  if (name == "Teddiursa")
    return QPixmap("Resources/MDP217.png");
  else
    return QPixmap();
}
