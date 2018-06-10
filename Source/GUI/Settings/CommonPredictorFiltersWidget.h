#pragma once

#include <QWidget>

#include <QCheckBox>
#include <QSpinBox>

#include <QVector>

class CommonPredictorFiltersWidget : public QWidget
{
public:
  CommonPredictorFiltersWidget(QWidget* parent = nullptr);

  int getMinHpIv();
  int getMinAtkIv();
  int getMinDefIv();
  int getMinSpAtkIv();
  int getMinSpDefIv();
  int getMinSpeedIv();
  bool getEnableNatureFilter();
  QVector<bool> getNatureFilters();

  void setMinHpIv(int minHpIv);
  void setMinAtkIv(int minAtkIv);
  void setMinDefIv(int minDefIv);
  void setMinSpAtkIv(int minSpAtkIv);
  void setMinSpDefIv(int minSpDefIv);
  void setMinSpeedIv(int minSpeedIv);
  void setEnableNatureFilter(bool enableNatureFilter);
  void setNatureFilters(QVector<bool> natureFilters);

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
