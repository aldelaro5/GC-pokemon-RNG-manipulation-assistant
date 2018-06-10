#include "ColosseumTab.h"

#include <QGroupBox>
#include <QVBoxLayout>

ColosseumTab::ColosseumTab(QWidget* parent) : QWidget(parent)
{
  m_predictorFiltersUmbreon = new CommonPredictorFiltersWidget(this);
  QVBoxLayout* umbreonLayout = new QVBoxLayout;
  umbreonLayout->addWidget(m_predictorFiltersUmbreon);
  QGroupBox* m_gbUmbreon = new QGroupBox(tr("Umbreon"));
  m_gbUmbreon->setLayout(umbreonLayout);

  m_predictorFiltersEspeon = new CommonPredictorFiltersWidget(this);
  QVBoxLayout* espeonLayout = new QVBoxLayout;
  espeonLayout->addWidget(m_predictorFiltersEspeon);
  QGroupBox* m_gbEspeon = new QGroupBox(tr("Espeon"));
  m_gbEspeon->setLayout(espeonLayout);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_gbUmbreon);
  mainLayout->addWidget(m_gbEspeon);

  setLayout(mainLayout);
}

CommonPredictorFiltersWidget* ColosseumTab::getUmbreonFiltersWidget()
{
  return m_predictorFiltersUmbreon;
}

CommonPredictorFiltersWidget* ColosseumTab::getEspeonFiltersWidget()
{
  return m_predictorFiltersEspeon;
}
