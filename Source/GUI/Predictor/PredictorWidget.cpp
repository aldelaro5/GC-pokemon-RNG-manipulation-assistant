#include "PredictorWidget.h"

#include <QVBoxLayout>

PredictorWidget::PredictorWidget(QWidget* parent) : QWidget(parent)
{
  initialiseWidgets();
  makeLayouts();
}

void PredictorWidget::initialiseWidgets()
{
  m_lblStartersNames.append(new QLabel(tr("Select a game")));

  m_tblStartersPrediction = new QTableWidget();
}

void PredictorWidget::makeLayouts()
{
  for (auto label : m_lblStartersNames)
    m_startersNamesLayout->addWidget(label);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addLayout(m_startersNamesLayout);
  mainLayout->addWidget(m_tblStartersPrediction);

  setLayout(mainLayout);
}
