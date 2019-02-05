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

  m_chkPalVersion =
      new QCheckBox("Use the PAL version of the game - LEAVE UNCHECKED IF YOU DO NOT USE IT");
  m_chkPalVersion->setChecked(false);
  QLabel* lblPalDetails = new QLabel(
      "The PAL version of the game has only a single possible prediction per reroll and it is "
      "valid no matter how long it takes to confirm the player name. You MUST check this box if "
      "you are using the PAL version.");
  lblPalDetails->setWordWrap(true);

  QVBoxLayout* palVersionLayout = new QVBoxLayout;
  palVersionLayout->addWidget(m_chkPalVersion);
  palVersionLayout->addWidget(lblPalDetails);

  QFormLayout* additionalInputLayout = new QFormLayout;
  additionalInputLayout->setLabelAlignment(Qt::AlignRight);
  additionalInputLayout->addRow(lblShininess, m_cmbShininess);
  additionalInputLayout->addRow(lblGender, m_cmbGender);

  QVBoxLayout* eeveeLayout = new QVBoxLayout;
  eeveeLayout->addWidget(m_predictorFiltersEevee);
  eeveeLayout->addLayout(additionalInputLayout);
  eeveeLayout->addSpacing(10);
  eeveeLayout->addLayout(palVersionLayout);
  QGroupBox* m_gbEevee = new QGroupBox(tr("Eevee"));
  m_gbEevee->setLayout(eeveeLayout);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_gbEevee);
  mainLayout->addStretch();

  setLayout(mainLayout);
}

GaleDarknessTab::~GaleDarknessTab()
{
  delete m_predictorFiltersEevee;
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

bool GaleDarknessTab::getPalVersionEnabled() const
{
  return m_chkPalVersion->isChecked();
}

void GaleDarknessTab::setShininess(const GUICommon::shininess shininess)
{
  m_cmbShininess->setCurrentIndex(static_cast<int>(shininess));
}

void GaleDarknessTab::setGender(const GUICommon::gender gender)
{
  m_cmbGender->setCurrentIndex(static_cast<int>(gender));
}

void GaleDarknessTab::setPalVersionEnabled(const bool palVersionEnabled)
{
  m_chkPalVersion->setChecked(palVersionEnabled);
}
