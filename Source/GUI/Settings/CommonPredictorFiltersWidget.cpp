#include "CommonPredictorFiltersWidget.h"

#include <QFormLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

#include "../GUICommon.h"

CommonPredictorFiltersWidget::CommonPredictorFiltersWidget(QWidget* parent) : QWidget(parent)
{
  QLabel* lblIvs = new QLabel(tr("Filter wanted predictions by\nthe following minimal IVs"));

  QLabel* lblHpIv = new QLabel(tr("HP: "));
  m_spnMinHpIv = new QSpinBox();
  m_spnMinHpIv->setMinimum(0);
  m_spnMinHpIv->setMaximum(31);
  m_spnMinHpIv->setValue(0);
  m_spnMinHpIv->setMaximumWidth(75);

  QLabel* lblAtkIv = new QLabel(tr("Attack: "));
  m_spnMinAtkIv = new QSpinBox();
  m_spnMinAtkIv->setMinimum(0);
  m_spnMinAtkIv->setMaximum(31);
  m_spnMinAtkIv->setValue(0);
  m_spnMinAtkIv->setMaximumWidth(75);

  QLabel* lblDefIv = new QLabel(tr("Defense: "));
  m_spnMinDefIv = new QSpinBox();
  m_spnMinDefIv->setMinimum(0);
  m_spnMinDefIv->setMaximum(31);
  m_spnMinDefIv->setValue(0);
  m_spnMinDefIv->setMaximumWidth(75);

  QLabel* lblSpAtkIv = new QLabel(tr("Special Attack: "));
  m_spnMinSpAtkIv = new QSpinBox();
  m_spnMinSpAtkIv->setMinimum(0);
  m_spnMinSpAtkIv->setMaximum(31);
  m_spnMinSpAtkIv->setValue(0);
  m_spnMinSpAtkIv->setMaximumWidth(75);

  QLabel* lblSpDefIv = new QLabel(tr("Special Defense: "));
  m_spnMinSpDefIv = new QSpinBox();
  m_spnMinSpDefIv->setMinimum(0);
  m_spnMinSpDefIv->setMaximum(31);
  m_spnMinSpDefIv->setValue(0);
  m_spnMinSpDefIv->setMaximumWidth(75);

  QLabel* lblSpeedIv = new QLabel(tr("Speed: "));
  m_spnMinSpeedIv = new QSpinBox();
  m_spnMinSpeedIv->setMinimum(0);
  m_spnMinSpeedIv->setMaximum(31);
  m_spnMinSpeedIv->setValue(0);
  m_spnMinSpeedIv->setMaximumWidth(75);

  QLabel* lblHiddenPowerPower = new QLabel(tr("Minimum power of Hidden power: "));
  m_spnMinPowerHiddenPower = new QSpinBox();
  m_spnMinPowerHiddenPower->setMinimum(30);
  m_spnMinPowerHiddenPower->setMaximum(70);
  m_spnMinPowerHiddenPower->setValue(30);
  m_spnMinPowerHiddenPower->setMaximumWidth(75);

  QHBoxLayout* hiddenPowerPowerLayout = new QHBoxLayout;
  hiddenPowerPowerLayout->addWidget(lblHiddenPowerPower);
  hiddenPowerPowerLayout->addWidget(m_spnMinPowerHiddenPower);

  QFormLayout* IvInputLayout = new QFormLayout();
  IvInputLayout->setLabelAlignment(Qt::AlignRight);
  IvInputLayout->addRow(lblHpIv, m_spnMinHpIv);
  IvInputLayout->addRow(lblAtkIv, m_spnMinAtkIv);
  IvInputLayout->addRow(lblDefIv, m_spnMinDefIv);
  IvInputLayout->addRow(lblSpAtkIv, m_spnMinSpAtkIv);
  IvInputLayout->addRow(lblSpDefIv, m_spnMinSpDefIv);
  IvInputLayout->addRow(lblSpeedIv, m_spnMinSpeedIv);

  QGridLayout* naturesChkLayout = new QGridLayout;
  for (int i = 0; i < GUICommon::naturesStr.size(); i++)
  {
    QCheckBox* chk = new QCheckBox(GUICommon::naturesStr[i]);
    chk->setChecked(false);
    m_chkNatures.append(chk);
    naturesChkLayout->addWidget(chk, i / 4, i % 4);
  }

  m_naturesWidget = new QWidget;
  m_naturesWidget->setLayout(naturesChkLayout);
  m_naturesWidget->setEnabled(false);

  m_chkEnableNatureFilter = new QCheckBox(tr("Filter wanted predictions by nature"), this);
  connect(m_chkEnableNatureFilter, &QCheckBox::stateChanged, this,
          [=](int state) { m_naturesWidget->setEnabled(state == Qt::CheckState::Checked); });

  QGridLayout* typesChkLayout = new QGridLayout;
  for (int i = 0; i < GUICommon::typesStr.size(); i++)
  {
    QCheckBox* chk = new QCheckBox(GUICommon::typesStr[i]);
    chk->setChecked(false);
    m_chkHiddenPowerTypes.append(chk);
    typesChkLayout->addWidget(chk, i / 4, i % 4);
  }

  m_typesWidget = new QWidget;
  m_typesWidget->setLayout(typesChkLayout);
  m_typesWidget->setEnabled(false);

  m_chkEnableHiddenPowerTypeFilter =
      new QCheckBox(tr("Filter wanted predictions by hidden power type"), this);
  connect(m_chkEnableHiddenPowerTypeFilter, &QCheckBox::stateChanged, this,
          [=](int state) { m_typesWidget->setEnabled(state == Qt::CheckState::Checked); });

  QVBoxLayout* leftLayout = new QVBoxLayout;
  leftLayout->addWidget(lblIvs);
  leftLayout->addLayout(IvInputLayout);
  leftLayout->addSpacing(30);
  leftLayout->addLayout(hiddenPowerPowerLayout);
  leftLayout->addStretch();

  QVBoxLayout* rigthLayout = new QVBoxLayout;
  rigthLayout->addWidget(m_chkEnableNatureFilter);
  rigthLayout->addWidget(m_naturesWidget);
  rigthLayout->addWidget(m_chkEnableHiddenPowerTypeFilter);
  rigthLayout->addWidget(m_typesWidget);
  rigthLayout->addStretch();

  QHBoxLayout* mainLayout = new QHBoxLayout;
  mainLayout->addLayout(leftLayout);
  mainLayout->addSpacing(30);
  mainLayout->addLayout(rigthLayout);

  setLayout(mainLayout);
}

CommonPredictorFiltersWidget::~CommonPredictorFiltersWidget()
{
}

int CommonPredictorFiltersWidget::getMinHpIv() const
{
  return m_spnMinHpIv->value();
}

int CommonPredictorFiltersWidget::getMinAtkIv() const
{
  return m_spnMinAtkIv->value();
}

int CommonPredictorFiltersWidget::getMinDefIv() const
{
  return m_spnMinDefIv->value();
}

int CommonPredictorFiltersWidget::getMinSpAtkIv() const
{
  return m_spnMinSpAtkIv->value();
}

int CommonPredictorFiltersWidget::getMinSpDefIv() const
{
  return m_spnMinSpDefIv->value();
}

int CommonPredictorFiltersWidget::getMinSpeedIv() const
{
  return m_spnMinSpeedIv->value();
}

int CommonPredictorFiltersWidget::getMinPowerHiddenPower() const
{
  return m_spnMinPowerHiddenPower->value();
}

bool CommonPredictorFiltersWidget::getEnableNatureFilter() const
{
  return m_chkEnableNatureFilter->isChecked();
}

QVector<bool> CommonPredictorFiltersWidget::getNatureFilters() const
{
  QVector<bool> naturesFilter;
  for (auto i : m_chkNatures)
    naturesFilter.append(i->isChecked());
  return naturesFilter;
}

bool CommonPredictorFiltersWidget::getEnableHiddenPowerTypesFilter() const
{
  return m_chkEnableHiddenPowerTypeFilter->isChecked();
}

QVector<bool> CommonPredictorFiltersWidget::getHiddenPowerTypesFilters() const
{
  QVector<bool> typesFilter;
  for (auto i : m_chkHiddenPowerTypes)
    typesFilter.append(i->isChecked());
  return typesFilter;
}

void CommonPredictorFiltersWidget::setMinHpIv(const int minHpIv)
{
  m_spnMinHpIv->setValue(minHpIv);
}

void CommonPredictorFiltersWidget::setMinAtkIv(const int minAtkIv)
{
  m_spnMinAtkIv->setValue(minAtkIv);
}

void CommonPredictorFiltersWidget::setMinDefIv(const int minDefIv)
{
  m_spnMinDefIv->setValue(minDefIv);
}

void CommonPredictorFiltersWidget::setMinSpAtkIv(const int minSpAtkIv)
{
  m_spnMinSpAtkIv->setValue(minSpAtkIv);
}

void CommonPredictorFiltersWidget::setMinSpDefIv(const int minSpDefIv)
{
  m_spnMinSpDefIv->setValue(minSpDefIv);
}

void CommonPredictorFiltersWidget::setMinSpeedIv(const int minSpeedIv)
{
  m_spnMinSpeedIv->setValue(minSpeedIv);
}

void CommonPredictorFiltersWidget::setMinPowerHiddenPower(const int minPowerHiddenPower)
{
  m_spnMinPowerHiddenPower->setValue(minPowerHiddenPower);
}

void CommonPredictorFiltersWidget::setEnableNatureFilter(const bool enableNatureFilter)
{
  m_chkEnableNatureFilter->setChecked(enableNatureFilter);
}

void CommonPredictorFiltersWidget::setNatureFilters(const QVector<bool> natureFilters)
{
  for (int i = 0; i < natureFilters.size(); i++)
    m_chkNatures[i]->setChecked(natureFilters[i]);
}

void CommonPredictorFiltersWidget::setEnableHiddenPowerTypesFilter(
    const bool enableHiddenPowerTypeFilter)
{
  m_chkEnableHiddenPowerTypeFilter->setChecked(enableHiddenPowerTypeFilter);
}

void CommonPredictorFiltersWidget::setHiddenPowerTypesFilters(
    const QVector<bool> hiddenPowerTypeFilters)
{
  for (int i = 0; i < hiddenPowerTypeFilters.size(); i++)
    m_chkHiddenPowerTypes[i]->setChecked(hiddenPowerTypeFilters[i]);
}
