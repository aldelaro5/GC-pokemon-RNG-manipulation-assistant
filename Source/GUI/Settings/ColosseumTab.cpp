#include "ColosseumTab.h"

#include <QGroupBox>
#include <QVBoxLayout>

ColosseumTab::ColosseumTab(QWidget* parent) : QWidget(parent)
{
  m_starterTabs = new QTabWidget();

  m_predictorFiltersUmbreon = new CommonPredictorFiltersWidget(this);
  m_starterTabs->addTab(m_predictorFiltersUmbreon, tr("Umbreon"));

  m_predictorFiltersEspeon = new CommonPredictorFiltersWidget(this);
  m_starterTabs->addTab(m_predictorFiltersEspeon, tr("Espeon"));

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_starterTabs);

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
