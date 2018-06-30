#pragma once

#include <QWidget>

#include <QCheckBox>
#include <QSpinBox>

#include <QVector>

class CommonPredictorFiltersWidget : public QWidget
{
public:
  CommonPredictorFiltersWidget(QWidget* parent = nullptr);
  virtual ~CommonPredictorFiltersWidget();

  int getMinHpIv() const;
  int getMinAtkIv() const;
  int getMinDefIv() const;
  int getMinSpAtkIv() const;
  int getMinSpDefIv() const;
  int getMinSpeedIv() const;
  int getMinPowerHiddenPower() const;
  bool getEnableNatureFilter() const;
  QVector<bool> getNatureFilters() const;
  bool getEnableHiddenPowerTypesFilter() const;
  QVector<bool> getHiddenPowerTypesFilters() const;

  void setMinHpIv(const int minHpIv);
  void setMinAtkIv(const int minAtkIv);
  void setMinDefIv(const int minDefIv);
  void setMinSpAtkIv(const int minSpAtkIv);
  void setMinSpDefIv(const int minSpDefIv);
  void setMinSpeedIv(const int minSpeedIv);
  void setMinPowerHiddenPower(const int minPowerHiddenPower);
  void setEnableNatureFilter(const bool enableNatureFilter);
  void setNatureFilters(const QVector<bool> natureFilters);
  void setEnableHiddenPowerTypesFilter(const bool enableHiddenPowerTypeFilter);
  void setHiddenPowerTypesFilters(const QVector<bool> hiddenPowerTypeFilters);

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
