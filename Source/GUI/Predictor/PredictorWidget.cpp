#include "PredictorWidget.h"

#include <QHeaderView>
#include <QSettings>
#include <QStringList>
#include <QVBoxLayout>

#include "../GUICommon.h"
#include "../SPokemonRNG.h"

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
  m_tblStartersPrediction->setEditTriggers(QAbstractItemView::NoEditTriggers);
  m_tblStartersPrediction->setSelectionMode(QAbstractItemView::NoSelection);
  m_tblStartersPrediction->setSelectionBehavior(QAbstractItemView::SelectRows);
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

void PredictorWidget::resetPredictor(GUICommon::gameSelection currentGame)
{
  switchGame(currentGame);
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
  m_tblHeaderLabels.append(tr("Seed"));
  m_tblHeaderLabels.append(tr("Frame (seconds)"));
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

  if (game == GUICommon::gameSelection::Colosseum)
    setMinimumWidth(1150);
  if (game == GUICommon::gameSelection::XD)
    setMinimumWidth(900);
  setMinimumHeight(500);
}

void PredictorWidget::setStartersPrediction(
    std::vector<BaseRNGSystem::StartersPrediction> startersPrediction,
    GUICommon::gameSelection game)
{
  clearLabels();
  std::vector<std::string> names = SPokemonRNG::getInstance()->getSystem()->getStartersName();
  for (auto name : names)
  {
    QLabel* lblName = new QLabel(QString::fromStdString(name));
    lblName->setAlignment(Qt::AlignmentFlag::AlignCenter);

    m_lblStartersNames.append(lblName);
  }
  m_startersNamesLayout->addStretch();
  for (auto label : m_lblStartersNames)
  {
    m_startersNamesLayout->addSpacing(200);
    m_startersNamesLayout->addWidget(label);
    m_startersNamesLayout->addStretch();
  }
  if (m_lblStartersNames.size() > 1)
    m_startersNamesLayout->addStretch();

  m_tblStartersPrediction->setRowCount(startersPrediction.size());
  QSettings settings("settings.ini", QSettings::IniFormat);

  int test = settings.value("colosseumPredictor/espeon/minAtkIv").toInt();
  if (game == GUICommon::gameSelection::Colosseum)
    settings.beginGroup("colosseumPredictor");
  else if (game == GUICommon::gameSelection::XD)
    settings.beginGroup("galeDarknessPredictor");
  for (int i = 0; i < startersPrediction.size(); i++)
  {
    bool passAllFilters = true;
    m_tblStartersPrediction->setItem(
        i, 0,
        new QTableWidgetItem(QString::number(startersPrediction[i].startingSeed, 16).toUpper()));
    if (i == 0)
    {
      m_tblStartersPrediction->setItem(
          i, 1,
          new QTableWidgetItem(QString::number(startersPrediction[i].frameNumber) +
                               " (frame perfect)"));
    }
    else
    {
      m_tblStartersPrediction->setItem(
          i, 1,
          new QTableWidgetItem(QString::number(startersPrediction[i].frameNumber) + " (" +
                               QString::number(startersPrediction[i].frameNumber / 60.0) + ")"));
    }

    int nbrColPerStarter = 7;
    if (game == GUICommon::gameSelection::XD)
      nbrColPerStarter = 9;

    QBrush greenBrush = QBrush(QColor("#32CD32"));
    QBrush redBrush = QBrush(QColor("#B22222"));
    for (int j = 0; j < startersPrediction[i].starters.size(); j++)
    {
      settings.beginGroup(QString::fromStdString(names[j]).toLower());
      BaseRNGSystem::StarterGen starter = startersPrediction[i].starters[j];

      m_tblStartersPrediction->setItem(i, 2 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.hpIV)));
      if (starter.hpIV >= settings.value("minHpIv", 0).toInt())
      {
        m_tblStartersPrediction->item(i, 2 + j * nbrColPerStarter)->setBackground(greenBrush);
      }
      else
      {
        m_tblStartersPrediction->item(i, 2 + j * nbrColPerStarter)->setBackground(redBrush);
        passAllFilters = false;
      }

      m_tblStartersPrediction->setItem(i, 3 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.atkIV)));
      if (starter.atkIV >= settings.value("minAtkIv", 0).toInt())
      {
        m_tblStartersPrediction->item(i, 3 + j * nbrColPerStarter)->setBackground(greenBrush);
      }
      else
      {
        m_tblStartersPrediction->item(i, 3 + j * nbrColPerStarter)->setBackground(redBrush);
        passAllFilters = false;
      }
      m_tblStartersPrediction->setItem(i, 4 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.defIV)));
      if (starter.defIV >= settings.value("minDefIv", 0).toInt())
      {
        m_tblStartersPrediction->item(i, 4 + j * nbrColPerStarter)->setBackground(greenBrush);
      }
      else
      {
        m_tblStartersPrediction->item(i, 4 + j * nbrColPerStarter)->setBackground(redBrush);
        passAllFilters = false;
      }
      m_tblStartersPrediction->setItem(i, 5 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.spAtkIV)));
      if (starter.spAtkIV >= settings.value("minSpAtkIv", 0).toInt())
      {
        m_tblStartersPrediction->item(i, 5 + j * nbrColPerStarter)->setBackground(greenBrush);
      }
      else
      {
        m_tblStartersPrediction->item(i, 5 + j * nbrColPerStarter)->setBackground(redBrush);
        passAllFilters = false;
      }
      m_tblStartersPrediction->setItem(i, 6 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.spDefIV)));
      if (starter.spDefIV >= settings.value("minSpDefIv", 0).toInt())
      {
        m_tblStartersPrediction->item(i, 6 + j * nbrColPerStarter)->setBackground(greenBrush);
      }
      else
      {
        m_tblStartersPrediction->item(i, 6 + j * nbrColPerStarter)->setBackground(redBrush);
        passAllFilters = false;
      }
      m_tblStartersPrediction->setItem(i, 7 + j * nbrColPerStarter,
                                       new QTableWidgetItem(QString::number(starter.speedIV)));
      if (starter.speedIV >= settings.value("minSpeedIv", 0).toInt())
      {
        m_tblStartersPrediction->item(i, 7 + j * nbrColPerStarter)->setBackground(greenBrush);
      }
      else
      {
        m_tblStartersPrediction->item(i, 7 + j * nbrColPerStarter)->setBackground(redBrush);
        passAllFilters = false;
      }
      m_tblStartersPrediction->setItem(
          i, 8 + j * nbrColPerStarter,
          new QTableWidgetItem(GUICommon::naturesStr[starter.natureIndex]));

      bool enableNatureFilters = settings.value("enableNatureFilter", false).toBool();
      settings.beginReadArray("naturesFilter");
      settings.setArrayIndex(starter.natureIndex);
      if (settings.value(GUICommon::naturesStr[starter.natureIndex], true).toBool() ||
          !enableNatureFilters)
      {
        m_tblStartersPrediction->item(i, 8 + j * nbrColPerStarter)->setBackground(greenBrush);
      }
      else
      {
        m_tblStartersPrediction->item(i, 8 + j * nbrColPerStarter)->setBackground(redBrush);
        passAllFilters = false;
      }
      settings.endArray();

      if (game == GUICommon::gameSelection::XD)
      {
        m_tblStartersPrediction->setItem(
            i, 9 + j * nbrColPerStarter,
            new QTableWidgetItem(GUICommon::genderStr[starter.genderIndex]));
        int genderIndex =
            settings.value("gender", static_cast<int>(GUICommon::gender::AnyGender)).toInt();
        if (starter.genderIndex == genderIndex ||
            genderIndex == static_cast<int>(GUICommon::gender::AnyGender))
        {
          m_tblStartersPrediction->item(i, 9 + j * nbrColPerStarter)->setBackground(greenBrush);
        }
        else
        {
          m_tblStartersPrediction->item(i, 9 + j * nbrColPerStarter)->setBackground(redBrush);
          passAllFilters = false;
        }
        m_tblStartersPrediction->setItem(i, 10 + j * nbrColPerStarter,
                                         new QTableWidgetItem(tr(starter.isShiny ? "Yes" : "No")));
        int shinynessIndex =
            settings.value("shininess", static_cast<int>(GUICommon::shininess::AnyShininess))
                .toInt();
        int isShinyInt = starter.isShiny ? static_cast<int>(GUICommon::shininess::Shiny)
                                         : static_cast<int>(GUICommon::shininess::NotShiny);
        if (isShinyInt == shinynessIndex ||
            shinynessIndex == static_cast<int>(GUICommon::shininess::AnyShininess))
        {
          m_tblStartersPrediction->item(i, 10 + j * nbrColPerStarter)->setBackground(greenBrush);
        }
        else
        {
          m_tblStartersPrediction->item(i, 10 + j * nbrColPerStarter)->setBackground(redBrush);
          passAllFilters = false;
        }
      }
      settings.endGroup();
    }
    if (passAllFilters)
      m_tblStartersPrediction->item(i, 1)->setBackground(greenBrush);
    else
      m_tblStartersPrediction->item(i, 1)->setBackground(redBrush);
  }
  settings.endGroup();
  m_tblStartersPrediction->resizeColumnsToContents();
  if (game == GUICommon::gameSelection::Colosseum)
    setMinimumWidth(1150);
  if (game == GUICommon::gameSelection::XD)
    setMinimumWidth(900);
  setMinimumHeight(500);
}
