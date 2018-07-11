#include "GeneralTab.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>

#include <thread>

GeneralTab::GeneralTab(QWidget* parent) : QWidget(parent)
{
  QLabel* lblDetectedThreadDescription = new QLabel(tr("Amount of threads detected on this CPU: "));
  QLabel* lblDetectedThread = new QLabel(QString::number(std::thread::hardware_concurrency()));

  QLabel* lblThreadLimit = new QLabel(tr("Amount of threads to use in this program: "));
  m_cmbThreadLimit = new QComboBox(this);
  m_cmbThreadLimit->addItem(tr("All threads"));
  for (unsigned int i = 1; i < std::thread::hardware_concurrency(); i++)
    m_cmbThreadLimit->addItem(QString::number(i));
  m_cmbThreadLimit->setCurrentIndex(0);

  QLabel* lblThreadLimitDescription =
      new QLabel(tr("This setting allows you to limit the number of threads the program can use "
                    "for the algorithms of the seed finder and the precalculation file generation. "
                    "This will make them slower, but will allow critical CPU intensive "
                    "applications such as a live streaming software to function properly."));
  lblThreadLimitDescription->setWordWrap(true);

  QFormLayout* threadLimitLayout = new QFormLayout;
  threadLimitLayout->setLabelAlignment(Qt::AlignRight);
  threadLimitLayout->addRow(lblDetectedThreadDescription, lblDetectedThread);
  threadLimitLayout->addRow(lblThreadLimit, m_cmbThreadLimit);

  QVBoxLayout* cpuSettingsLayout = new QVBoxLayout;
  cpuSettingsLayout->addLayout(threadLimitLayout);
  cpuSettingsLayout->addSpacing(10);
  cpuSettingsLayout->addWidget(lblThreadLimitDescription);

  QGroupBox* gbCPUSettings = new QGroupBox(tr("CPU settings"));
  gbCPUSettings->setLayout(cpuSettingsLayout);

  QLabel* lblPredictionsTime =
      new QLabel(tr("Amount of time to generate predictions (in seconds): "));
  m_spbPredictionsTime = new QSpinBox();
  m_spbPredictionsTime->setMinimum(0);
  m_spbPredictionsTime->setValue(10);
  // Let's put a crazy time like 10 hours cause Qt puts 99 at default for no reaosns
  m_spbPredictionsTime->setMaximum(36000);
  m_spbPredictionsTime->setMaximumWidth(150);

  QLabel* lblMaxAutoRerolls =
      new QLabel(tr("Amount of max auto rerolls that can occur: "));
  m_spbMaxAutoRerolls = new QSpinBox();
  m_spbMaxAutoRerolls->setMinimum(1);
  m_spbMaxAutoRerolls->setValue(20);
  m_spbMaxAutoRerolls->setMaximum(200);
  m_spbMaxAutoRerolls->setMaximumWidth(150);

  QLabel* lblFrameDelay = new QLabel(tr("Amount of frames to add as delay to predictions: "));
  m_spbFrameDelay = new QSpinBox();
  m_spbFrameDelay->setMinimum(0);
  m_spbFrameDelay->setValue(0);
  m_spbFrameDelay->setMaximumWidth(150);

  QFormLayout* predictionTimeLayout = new QFormLayout;
  predictionTimeLayout->setLabelAlignment(Qt::AlignRight);
  predictionTimeLayout->addRow(lblPredictionsTime, m_spbPredictionsTime);
  predictionTimeLayout->addRow(lblFrameDelay, m_spbFrameDelay);

  QGroupBox* gbPredictor = new QGroupBox(tr("Starters predictor"));
  gbPredictor->setLayout(predictionTimeLayout);

  QFormLayout* AutoRerollsLayout = new QFormLayout;
  AutoRerollsLayout->setLabelAlignment(Qt::AlignRight);
  AutoRerollsLayout->addRow(lblMaxAutoRerolls, m_spbMaxAutoRerolls);

   QGroupBox* gbRerolls = new QGroupBox(tr("Auto Reroll Settings"));
  gbRerolls->setLayout(AutoRerollsLayout);


  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(gbCPUSettings);
  mainLayout->addWidget(gbPredictor);
  mainLayout->addWidget(gbRerolls);
  mainLayout->addStretch();

  setLayout(mainLayout);
}

int GeneralTab::getPredictionTime() const
{
  return m_spbPredictionsTime->value();
}

int GeneralTab::getMaxAutoRerolls() const
{
  return m_spbMaxAutoRerolls->value();
}

int GeneralTab::getFrameDelay() const
{
  return m_spbFrameDelay->value();
}

int GeneralTab::getThreadLimit() const
{
  return m_cmbThreadLimit->currentIndex();
}

void GeneralTab::setPredictionTime(const int predictionTime)
{
  m_spbPredictionsTime->setValue(predictionTime);
}
void GeneralTab::setMaxAutoRerolls(const int maxAutoRerolls)
{
  m_spbMaxAutoRerolls->setValue(maxAutoRerolls);
}

void GeneralTab::setThreadLimit(const int threadLimit)
{
  m_cmbThreadLimit->setCurrentIndex(threadLimit);
}

void GeneralTab::setFrameDelay(const int frameDelay)
{
  m_spbFrameDelay->setValue(frameDelay);
}
