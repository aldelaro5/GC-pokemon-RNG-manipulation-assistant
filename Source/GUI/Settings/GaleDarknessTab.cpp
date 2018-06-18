#include "GaleDarknessTab.h"

#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>

GaleDarknessTab::GaleDarknessTab(QWidget* parent) : QWidget(parent)
{
  m_predictorFiltersEevee = new CommonPredictorFiltersWidget(this);
  QLabel* lblShininess = new QLabel(tr("Filter wanted predictions by shininess: "));
  m_cmbShininess = new QComboBox();
  m_cmbShininess->addItems(GUICommon::shininessStr);
  m_cmbShininess->setCurrentIndex(static_cast<int>(GUICommon::shininess::AnyShininess));

  QLabel* lblGender = new QLabel(tr("Filter wanted predictions by gender: "));
  m_cmbGender = new QComboBox();
  m_cmbGender->addItems(GUICommon::genderStr);
  m_cmbGender->setCurrentIndex(static_cast<int>(GUICommon::gender::AnyGender));

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

CommonPredictorFiltersWidget* GaleDarknessTab::getEeveeFiltersWidget() const
{
  return m_predictorFiltersEevee;
}

GUICommon::shininess GaleDarknessTab::getShininess() const
{
  return static_cast<GUICommon::shininess>(m_cmbShininess->currentIndex());
}

GUICommon::gender GaleDarknessTab::getGender() const
{
  return static_cast<GUICommon::gender>(m_cmbGender->currentIndex());
}

void GaleDarknessTab::setShininess(const GUICommon::shininess shininess)
{
  m_cmbShininess->setCurrentIndex(static_cast<int>(shininess));
}

void GaleDarknessTab::setGender(const GUICommon::gender gender)
{
  m_cmbGender->setCurrentIndex(static_cast<int>(gender));
}
