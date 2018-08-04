#pragma once

#include <QWidget>

#include <QComboBox>
#include <QSpinBox>

#include "../GUICommon.h"

class GeneralTab : public QWidget
{
public:
  GeneralTab(QWidget* parent = nullptr);

  int getThreadLimit() const;
  int getPredictionTime() const;
  int getFrameDelay() const;
  int getMaxAutoReroll() const;

  void setThreadLimit(const int threadLimit);
  void setPredictionTime(const int predictionTime);
  void setFrameDelay(const int frameDelay);
  void setMaxAutoReroll(const int maxAutoReroll);

private:
  QComboBox* m_cmbThreadLimit;
  QSpinBox* m_spbPredictionsTime;
  QSpinBox* m_spbFrameDelay;
  QSpinBox* m_spbMaxAutoReroll;
};
