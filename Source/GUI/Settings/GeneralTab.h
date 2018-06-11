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

  void setPlatform(GUICommon::platform platform);
  void setRtcMarginError(int rtcMarginError);

private:
  QComboBox* m_cmbPlatform;
  QSpinBox* m_spbRtcMarginError;
};
