#pragma once

#include <QComboBox>
#include <QWidget>

#include "../GUICommon.h"
#include "CommonPredictorFiltersWidget.h"

class GaleDarknessTab : public QWidget
{
public:
  GaleDarknessTab(QWidget* parent = nullptr);
  ~GaleDarknessTab();

  CommonPredictorFiltersWidget* getEeveeFiltersWidget() const;
  GUICommon::shininess getShininess() const;
  GUICommon::gender getGender() const;
  bool getPalVersionEnabled() const;

  void setShininess(const GUICommon::shininess shininess);
  void setGender(const GUICommon::gender gender);
  void setPalVersionEnabled(const bool palVersionEnabled);

private:
  CommonPredictorFiltersWidget* m_predictorFiltersEevee;
  QComboBox* m_cmbShininess;
  QComboBox* m_cmbGender;
  QCheckBox* m_chkPalVersion;
};
