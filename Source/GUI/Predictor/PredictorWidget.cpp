#include "PredictorWidget.h"

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

void PredictorWidget::switchGame()
{
  m_lblStartersNames[0]->setText(tr("Find your seed for predictions"));
  m_tblStartersPrediction->clear();
  m_tblHeaderLabels.clear();
  m_tblHeaderLabels.append(tr("Frame number"));
  m_tblHeaderLabels.append(tr("Starting seed"));
  for (int i = 0; i < SPokemonRNG::getInstance()->getSystem()->getNbrStartersPrediction(); i++)
  {
    m_tblHeaderLabels.append(tr("HP"));
    m_tblHeaderLabels.append(tr("Atk"));
    m_tblHeaderLabels.append(tr("Def"));
    m_tblHeaderLabels.append(tr("SpAtk"));
    m_tblHeaderLabels.append(tr("SpDef"));
    m_tblHeaderLabels.append(tr("Speed"));

    m_tblHeaderLabels.append(tr("Gender"));
    m_tblHeaderLabels.append(tr("Nature"));
    m_tblHeaderLabels.append(tr("Shiny?"));
  }
  m_tblStartersPrediction->setColumnCount(m_tblHeaderLabels.count());
  m_tblStartersPrediction->setHorizontalHeaderLabels(m_tblHeaderLabels);
  m_tblStartersPrediction->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}
