#pragma once

#include <QWidget>

#include <QComboBox>
#include <QSpinBox>

#include "../GUICommon.h"

class GeneralTab : public QWidget
{
public:
  GeneralTab(QWidget* parent = nullptr);

  GUICommon::platform getPlatform() const;
  int getRtcMarginError() const;
  int getPredictionTime() const;

  void setPlatform(const GUICommon::platform platform);
  void setRtcMarginError(const int rtcMarginError);
  void setPredictionTime(const int predictionTime);

private:
  QComboBox* m_cmbPlatform;
  QSpinBox* m_spbRtcMarginError;
  QSpinBox* m_spbPredictionsTime;
};
