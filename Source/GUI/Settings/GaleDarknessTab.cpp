#include "GaleDarknessTab.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

#include "../GUICommon.h"

GaleDarknessTab::GaleDarknessTab(QWidget* parent) : QWidget(parent)
{
  m_predictorFiltersEevee = new CommonPredictorFiltersWidget(this);
  QLabel* lblShininess = new QLabel(tr("Filter wanted predictions by shininess: "));
  m_cmbShininess = new QComboBox();
  m_cmbShininess->addItem(tr("Do not filter"));
  m_cmbShininess->addItems(GUICommon::shininessStr);
  m_cmbShininess->setCurrentIndex(0);

  QLabel* lblGender = new QLabel(tr("Filter wanted predictions by gender: "));
  m_cmbGender = new QComboBox();
  m_cmbGender->addItem(tr("Do not filter"));
  m_cmbGender->addItems(GUICommon::genderStr);
  m_cmbGender->setCurrentIndex(0);

  QFormLayout* additionalInputLayout = new QFormLayout;
  additionalInputLayout->setLabelAlignment(Qt::AlignRight);
  additionalInputLayout->addRow(lblShininess, m_cmbShininess);
  additionalInputLayout->addRow(lblGender, m_cmbGender);

  QVBoxLayout* eeveeLayout = new QVBoxLayout;
  eeveeLayout->addWidget(m_predictorFiltersEevee);
  eeveeLayout->addLayout(additionalInputLayout);
  QGroupBox* m_gbEevee = new QGroupBox(tr("Eevee"));
  m_gbEevee->setLayout(eeveeLayout);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_gbEevee);
  mainLayout->addStretch();

  setLayout(mainLayout);
}
