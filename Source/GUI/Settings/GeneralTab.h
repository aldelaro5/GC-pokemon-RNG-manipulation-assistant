#pragma once

#include <QWidget>

#include <QComboBox>
#include <QSpinBox>

#include "../GUICommon.h"

class GeneralTab : public QWidget
{
public:
  GeneralTab(QWidget* parent = nullptr);

  GUICommon::platform getPlatform();
  int getRtcMarginError();
  int getPredictionTime();

  void setPlatform(GUICommon::platform platform);
  void setRtcMarginError(int rtcMarginError);
  void setPredictionTime(int predictionTime);

private:
  QComboBox* m_cmbPlatform;
  QSpinBox* m_spbRtcMarginError;
  QSpinBox* m_spbPredictionsTime;
};
