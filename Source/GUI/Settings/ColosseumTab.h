#pragma once

#include <QWidget>

#include "CommonPredictorFiltersWidget.h"

class ColosseumTab : public QWidget
{
public:
  ColosseumTab(QWidget* parent = nullptr);

private:
  CommonPredictorFiltersWidget* m_predictorFiltersUmbreon;
  CommonPredictorFiltersWidget* m_predictorFiltersEspeon;
};
