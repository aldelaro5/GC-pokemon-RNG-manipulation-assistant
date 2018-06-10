#pragma once

#include <QComboBox>
#include <QWidget>

#include "CommonPredictorFiltersWidget.h"

class GaleDarknessTab : public QWidget
{
public:
  GaleDarknessTab(QWidget* parent = nullptr);

private:
  CommonPredictorFiltersWidget* m_predictorFiltersEevee;
  QComboBox* m_cmbShininess;
  QComboBox* m_cmbGender;
};
