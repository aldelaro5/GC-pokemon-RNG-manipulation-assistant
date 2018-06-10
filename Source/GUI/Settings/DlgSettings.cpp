#include "DlgSettings.h"

#include <QVBoxLayout>

#include "../GUICommon.h"

DlgSettings::DlgSettings(QWidget* parent) : QDialog(parent)
{
  m_tabWidget = new QTabWidget(this);
  m_seedFinderTab = new SeedFinderTab(this);
  m_tabWidget->addTab(m_seedFinderTab, tr("Seed Finder"));
  m_coloTab = new ColosseumTab(this);
  m_tabWidget->addTab(
      m_coloTab, "Predictor (" + GUICommon::gamesStr[GUICommon::gameSelection::Colosseum] + ")");
  m_xdTab = new GaleDarknessTab(this);
  m_tabWidget->addTab(m_xdTab,
                      "Predictor (" + GUICommon::gamesStr[GUICommon::gameSelection::XD] + ")");

  m_buttonsDlg = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_tabWidget);
  mainLayout->addWidget(m_buttonsDlg);
  setLayout(mainLayout);
}
