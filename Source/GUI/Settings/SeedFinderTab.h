#pragma once

#include <QWidget>

#include <QComboBox>
#include <QSpinBox>

class SeedFinderTab : public QWidget
{
public:
  SeedFinderTab(QWidget* parent = nullptr);

private:
  QComboBox* m_cmbPlatform;
  QSpinBox* m_spbRtcMarginError;
};
