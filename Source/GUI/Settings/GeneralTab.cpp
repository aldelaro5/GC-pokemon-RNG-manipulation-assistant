#include "GeneralTab.h"

#include <QFormLayout>
#include <QLabel>

GeneralTab::GeneralTab(QWidget* parent) : QWidget(parent)
{
  QLabel* lblPlatform = new QLabel(tr("Platform: "));
  m_cmbPlatform = new QComboBox();
  m_cmbPlatform->addItems(GUICommon::platformsStr);
  m_cmbPlatform->setCurrentIndex(GUICommon::platform::GameCube);

  QLabel* lblRtcMarginError = new QLabel(tr("Margin of error of the clock (in seconds): "));
  m_spbRtcMarginError = new QSpinBox();
  m_spbRtcMarginError->setMinimum(1);
  // It takes about 1 minute and 45 seconds for the RTC on a GameCube to go over the upper 32 bits
  m_spbRtcMarginError->setMaximum(105);
  m_spbRtcMarginError->setValue(5);
  m_spbRtcMarginError->setMaximumWidth(100);

  QFormLayout* inputLayout = new QFormLayout();
  inputLayout->setLabelAlignment(Qt::AlignRight);
  inputLayout->addRow(lblPlatform, m_cmbPlatform);
  inputLayout->addRow(lblRtcMarginError, m_spbRtcMarginError);

  QLabel* lblWarning = new QLabel(
      tr("Changing these settings will invalidate the precalculation file if it was generated with "
         "these settings. This means that to benefit from the precalculation optimisation after "
         "changing these settings, you will have to generate a new one"));
  lblWarning->setWordWrap(true);

  QLabel* lblNoteMarginError = new QLabel(tr(
      "Note about the the margin of error of the clock: this coresponds to the time "
      "you can waste maximum from frame perfect input to boot the console after "
      "setting the time. This means that the longer this margin is, the more lenient "
      "you can be when booting the console fast enough. However, keep in mind that increasing this "
      "margin will make the seed finding procedure slower. Conversely, decreasing it "
      "will make the procedure faster, but less forgiving when booting the console."));
  lblNoteMarginError->setWordWrap(true);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addSpacing(25);
  mainLayout->addWidget(lblWarning);
  mainLayout->addSpacing(25);
  mainLayout->addLayout(inputLayout);
  mainLayout->addSpacing(25);
  mainLayout->addWidget(lblNoteMarginError);
  mainLayout->addStretch();
  setLayout(mainLayout);
}

GUICommon::platform GeneralTab::getPlatform()
{
  return static_cast<GUICommon::platform>(m_cmbPlatform->currentIndex());
}

int GeneralTab::getRtcMarginError()
{
  return m_spbRtcMarginError->value();
}

void GeneralTab::setPlatform(GUICommon::platform platform)
{
  m_cmbPlatform->setCurrentIndex(static_cast<int>(platform));
}

void GeneralTab::setRtcMarginError(int rtcMarginError)
{
  m_spbRtcMarginError->setValue(rtcMarginError);
}
