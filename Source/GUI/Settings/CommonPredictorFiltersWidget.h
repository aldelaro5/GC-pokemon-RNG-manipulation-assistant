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
  int getMinPowerHiddenPower();
  bool getEnableNatureFilter();
  QVector<bool> getNatureFilters();
  bool getEnableHiddenPowerTypesFilter();
  QVector<bool> getHiddenPowerTypesFilters();

  void setMinHpIv(int minHpIv);
  void setMinAtkIv(int minAtkIv);
  void setMinDefIv(int minDefIv);
  void setMinSpAtkIv(int minSpAtkIv);
  void setMinSpDefIv(int minSpDefIv);
  void setMinSpeedIv(int minSpeedIv);
  void setMinPowerHiddenPower(int minPowerHiddenPower);
  void setEnableNatureFilter(bool enableNatureFilter);
  void setNatureFilters(QVector<bool> natureFilters);
  void setEnableHiddenPowerTypesFilter(bool enableHiddenPowerTypeFilter);
  void setHiddenPowerTypesFilters(QVector<bool> hiddenPowerTypeFilters);

private:
  QSpinBox* m_spnMinHpIv;
  QSpinBox* m_spnMinAtkIv;
  QSpinBox* m_spnMinDefIv;
  QSpinBox* m_spnMinSpAtkIv;
  QSpinBox* m_spnMinSpDefIv;
  QSpinBox* m_spnMinSpeedIv;
  QSpinBox* m_spnMinPowerHiddenPower;
  QVector<QCheckBox*> m_chkHiddenPowerTypes;
  QCheckBox* m_chkEnableHiddenPowerTypeFilter;
  QCheckBox* m_chkEnableNatureFilter;
  QWidget* m_naturesWidget;
  QWidget* m_typesWidget;
  QVector<QCheckBox*> m_chkNatures;
};
