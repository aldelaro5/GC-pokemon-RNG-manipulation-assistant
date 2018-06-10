#pragma once

#include <QComboBox>
#include <QWidget>

#include "../GUICommon.h"
#include "CommonPredictorFiltersWidget.h"

class GaleDarknessTab : public QWidget
{
public:
  GaleDarknessTab(QWidget* parent = nullptr);

  CommonPredictorFiltersWidget* getEeveeFiltersWidget();
  GUICommon::shininess getShininess();
  GUICommon::gender getGender();

  void setShininess(GUICommon::shininess shininess);
  void setGender(GUICommon::gender gender);

private:
  CommonPredictorFiltersWidget* m_predictorFiltersEevee;
  QComboBox* m_cmbShininess;
  QComboBox* m_cmbGender;
};
