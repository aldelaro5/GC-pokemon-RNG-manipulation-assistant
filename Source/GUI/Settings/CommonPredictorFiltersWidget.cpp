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

  QVBoxLayout* IvLayout = new QVBoxLayout;
  IvLayout->addWidget(lblIvs);
  IvLayout->addLayout(IvInputLayout);

  QVBoxLayout* naturesLayout = new QVBoxLayout;
  naturesLayout->addWidget(m_chkEnableNatureFilter);
  naturesLayout->addWidget(m_naturesWidget);

  QHBoxLayout* mainLayout = new QHBoxLayout;
  mainLayout->addLayout(IvLayout);
  mainLayout->addSpacing(50);
  mainLayout->addLayout(naturesLayout);

  setLayout(mainLayout);
}

int CommonPredictorFiltersWidget::getMinHpIv()
{
  return m_spnMinHpIv->value();
}

int CommonPredictorFiltersWidget::getMinAtkIv()
{
  return m_spnMinAtkIv->value();
}

int CommonPredictorFiltersWidget::getMinDefIv()
{
  return m_spnMinDefIv->value();
}

int CommonPredictorFiltersWidget::getMinSpAtkIv()
{
  return m_spnMinSpAtkIv->value();
}

int CommonPredictorFiltersWidget::getMinSpDefIv()
{
  return m_spnMinSpDefIv->value();
}

int CommonPredictorFiltersWidget::getMinSpeedIv()
{
  return m_spnMinSpeedIv->value();
}

bool CommonPredictorFiltersWidget::getEnableNatureFilter()
{
  return m_chkEnableNatureFilter->isChecked();
}

QVector<bool> CommonPredictorFiltersWidget::getNatureFilters()
{
  QVector<bool> naturesFilter;
  for (auto i : m_chkNatures)
    naturesFilter.append(i->isChecked());
  return naturesFilter;
}
