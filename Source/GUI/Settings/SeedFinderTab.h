#pragma once

#include <QWidget>

#include <QComboBox>
#include <QSpinBox>

#include "../GUICommon.h"

class SeedFinderTab : public QWidget
{
public:
  SeedFinderTab(QWidget* parent = nullptr);

  GUICommon::platform getPlatform();
  int getRtcMarginError();

private:
  QComboBox* m_cmbPlatform;
  QSpinBox* m_spbRtcMarginError;
};
