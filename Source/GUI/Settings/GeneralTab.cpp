#include "GeneralTab.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>

GeneralTab::GeneralTab(QWidget* parent) : QWidget(parent)
{
  QLabel* lblPredictionsTime =
      new QLabel(tr("Amount of time to geenrate predictions (in seconds): "));
  m_spbPredictionsTime = new QSpinBox();
  m_spbPredictionsTime->setMinimum(0);
  m_spbPredictionsTime->setValue(10);
  // Let's put a crazy time like 10 hours cause Qt puts 99 at default for no reaosns
  m_spbPredictionsTime->setMaximum(36000);
  m_spbPredictionsTime->setMaximumWidth(150);

  QFormLayout* predictionTimeLayout = new QFormLayout;
  predictionTimeLayout->setLabelAlignment(Qt::AlignRight);
  predictionTimeLayout->addRow(lblPredictionsTime, m_spbPredictionsTime);

  QGroupBox* gbPredictor = new QGroupBox(tr("Starters predictor"));
  gbPredictor->setLayout(predictionTimeLayout);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(gbPredictor);
  mainLayout->addStretch();

  setLayout(mainLayout);
}

int GeneralTab::getPredictionTime() const
{
  return m_spbPredictionsTime->value();
}

void GeneralTab::setPredictionTime(const int predictionTime)
{
  m_spbPredictionsTime->setValue(predictionTime);
}
