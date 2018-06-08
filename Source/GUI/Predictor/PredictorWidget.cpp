#include "PredictorWidget.h"

#include "../GUICommon.h"
#include "../SPokemonRNG.h"

#include <QHeaderView>
#include <QStringList>
#include <QVBoxLayout>

PredictorWidget::PredictorWidget(QWidget* parent) : QWidget(parent)
{
  initialiseWidgets();
  makeLayouts();

  connect(SPokemonRNG::getInstance(), &SPokemonRNG::onSwitchGame, this,
          &PredictorWidget::switchGame);
}

void PredictorWidget::initialiseWidgets()
{
  QLabel* lblUninitialised = new QLabel(tr("Select a game"));
  lblUninitialised->setAlignment(Qt::AlignmentFlag::AlignCenter);
  m_lblStartersNames.append(lblUninitialised);

  m_tblStartersPrediction = new QTableWidget();
  m_tblStartersPrediction->verticalHeader()->hide();
}

void PredictorWidget::makeLayouts()
{
  m_startersNamesLayout = new QHBoxLayout;
  for (auto label : m_lblStartersNames)
    m_startersNamesLayout->addWidget(label);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(m_startersNamesLayout);
  mainLayout->addWidget(m_tblStartersPrediction);

  setLayout(mainLayout);
}

void PredictorWidget::clearLabels()
{
  m_lblStartersNames.clear();
  QLayoutItem* item;
  while ((item = m_startersNamesLayout->takeAt(0)) != 0)
  {
    delete item->widget();
    delete item;
  }
}

void PredictorWidget::switchGame(GUICommon::gameSelection game)
{
  clearLabels();
  QLabel* lblSeedFinder = new QLabel(tr("Find your seed for predictions"));
  lblSeedFinder->setAlignment(Qt::AlignmentFlag::AlignCenter);

  m_lblStartersNames.append(lblSeedFinder);
  m_startersNamesLayout->addWidget(m_lblStartersNames[0]);
  m_tblStartersPrediction->clear();
  m_tblHeaderLabels.clear();
  m_tblHeaderLabels.append(tr("Frame"));
  m_tblHeaderLabels.append(tr("Seed"));
  for (int i = 0; i < SPokemonRNG::getInstance()->getSystem()->getNbrStartersPrediction(); i++)
  {
    m_tblHeaderLabels.append(tr("HP"));
    m_tblHeaderLabels.append(tr("Atk"));
    m_tblHeaderLabels.append(tr("Def"));
    m_tblHeaderLabels.append(tr("SpAtk"));
    m_tblHeaderLabels.append(tr("SpDef"));
    m_tblHeaderLabels.append(tr("Speed"));
    m_tblHeaderLabels.append(tr("Nature"));

    if (game == GUICommon::gameSelection::XD)
    {
      m_tblHeaderLabels.append(tr("Gender"));
      m_tblHeaderLabels.append(tr("Shiny?"));
    }
  }
  m_tblStartersPrediction->setColumnCount(m_tblHeaderLabels.count());
  m_tblStartersPrediction->setHorizontalHeaderLabels(m_tblHeaderLabels);
  m_tblStartersPrediction->resizeColumnsToContents();
}

void PredictorWidget::setStartersPrediction(
    std::vector<BaseRNGSystem::StartersPrediction> startersPrediction,
    GUICommon::gameSelection game)
{
  clearLabels();
  for (auto name : SPokemonRNG::getInstance()->getSystem()->getStartersName())
  {
    QLabel* lblName = new QLabel(QString::fromStdString(name));
    lblName->setAlignment(Qt::AlignmentFlag::AlignCenter);

    m_lblStartersNames.append(lblName);
  }
  m_startersNamesLayout->addStretch();
  for (auto label : m_lblStartersNames)
  {
    m_startersNamesLayout->addWidget(label);
    m_startersNamesLayout->addStretch();
  }
  if (m_lblStartersNames.size() > 1)
    m_startersNamesLayout->addStretch();

  m_tblStartersPrediction->setRowCount(startersPrediction.size());
  for (int i = 0; i < startersPrediction.size(); i++)
  {
    m_tblStartersPrediction->setItem(
        i, 0, new QTableWidgetItem(QString::number(startersPrediction[i].frameNumber)));
    m_tblStartersPrediction->setItem(
        i, 1, new QTableWidgetItem(QString::number(startersPrediction[i].startingSeed, 16)));

    int nbrColPerStarter = 7;
    if (game == GUICommon::gameSelection::XD)
      nbrColPerStarter = 9;

    for (int j = 0; j < startersPrediction[i].starters.size(); j++)
    {
      BaseRNGSystem::StarterGen starter = startersPrediction[i].starters[j];
      m_tblStartersPrediction->setItem(i, 2 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.hpIV)));
      m_tblStartersPrediction->setItem(i, 3 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.atkIV)));
      m_tblStartersPrediction->setItem(i, 4 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.defIV)));
      m_tblStartersPrediction->setItem(i, 5 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.spAtkIV)));
      m_tblStartersPrediction->setItem(i, 6 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.spDefIV)));
      m_tblStartersPrediction->setItem(i, 7 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.speedIV)));
      m_tblStartersPrediction->setItem(
          i, 8 + j * nbrColPerStarter,
          new QTableWidgetItem(GUICommon::naturesStr[starter.natureIndex]));

      if (game == GUICommon::gameSelection::XD)
      {
        m_tblStartersPrediction->setItem(
            i, 9 + j * nbrColPerStarter,
            new QTableWidgetItem(GUICommon::genderStr[starter.genderIndex]));
        m_tblStartersPrediction->setItem(i, 10 + j * nbrColPerStarter,
                                         new QTableWidgetItem(tr(starter.isShiny ? "Yes" : "No")));
      }
    }
  }
  m_tblStartersPrediction->resizeColumnsToContents();
}
