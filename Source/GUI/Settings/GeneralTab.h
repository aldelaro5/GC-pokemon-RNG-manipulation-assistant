#pragma once

#include <QWidget>

#include <QComboBox>
#include <QSpinBox>

#include "../GUICommon.h"

class GeneralTab : public QWidget
{
public:
  GeneralTab(QWidget* parent = nullptr);

  int getPredictionTime() const;

  void setPredictionTime(const int predictionTime);

private:
  QSpinBox* m_spbPredictionsTime;
};
