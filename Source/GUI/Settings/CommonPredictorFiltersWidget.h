#pragma once

#include <QWidget>

#include <QCheckBox>
#include <QSpinBox>

#include <QVector>

class CommonPredictorFiltersWidget : public QWidget
{
public:
  CommonPredictorFiltersWidget(QWidget* parent = nullptr);

private:
  QSpinBox* m_spnMinHpIv;
  QSpinBox* m_spnMinAtkIv;
  QSpinBox* m_spnMinDefIv;
  QSpinBox* m_spnMinSpAtkIv;
  QSpinBox* m_spnMinSpDefIv;
  QSpinBox* m_spnMinSpeedIv;
  QCheckBox* m_chkEnableNatureFilter;
  QWidget* m_naturesWidget;
  QVector<QCheckBox*> m_chkNatures;
};
