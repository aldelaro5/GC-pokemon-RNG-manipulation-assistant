#include "DlgSettings.h"

#include <QSettings>
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

  m_buttonsDlg = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel |
                                      QDialogButtonBox::Apply);

  connect(m_buttonsDlg, &QDialogButtonBox::rejected, this, &QDialog::reject);
  connect(m_buttonsDlg, &QDialogButtonBox::clicked, this, [=](QAbstractButton* button) {
    if (m_buttonsDlg->buttonRole(button) == QDialogButtonBox::ApplyRole)
    {
      saveSettings();
    }
    else if (m_buttonsDlg->buttonRole(button) == QDialogButtonBox::AcceptRole)
    {
      saveSettings();
      QDialog::accept();
    }
  });

  QVBoxLayout* mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_tabWidget);
  mainLayout->addWidget(m_buttonsDlg);
  setLayout(mainLayout);

  loadSettings();
}

void DlgSettings::loadSettings()
{
}

void DlgSettings::saveSettings()
{
  QSettings settings("settings.ini", QSettings::IniFormat);

  settings.beginGroup("finder");
  settings.setValue("platform", static_cast<int>(m_seedFinderTab->getPlatform()));
  settings.setValue("rtcMarginError", m_seedFinderTab->getRtcMarginError());
  settings.endGroup();

  settings.beginGroup("colosseumPredictor");
  settings.beginGroup("umbreon");
  settings.setValue("minHpIv", m_coloTab->getUmbreonFiltersWidget()->getMinHpIv());
  settings.setValue("minAtkIv", m_coloTab->getUmbreonFiltersWidget()->getMinAtkIv());
  settings.setValue("minDefIv", m_coloTab->getUmbreonFiltersWidget()->getMinDefIv());
  settings.setValue("minSpAtkIv", m_coloTab->getUmbreonFiltersWidget()->getMinSpAtkIv());
  settings.setValue("minSpDefIv", m_coloTab->getUmbreonFiltersWidget()->getMinSpDefIv());
  settings.setValue("minSpeedIv", m_coloTab->getUmbreonFiltersWidget()->getMinSpeedIv());
  settings.setValue("enableNatureFilter",
                    m_coloTab->getUmbreonFiltersWidget()->getEnableNatureFilter());
  QVector<bool> naturesFilterUmbreon = m_coloTab->getUmbreonFiltersWidget()->getNatureFilters();
  settings.beginWriteArray("naturesFilter");
  for (int i = 0; i < naturesFilterUmbreon.size(); i++)
  {
    settings.setArrayIndex(i);
    settings.setValue(GUICommon::naturesStr[i], naturesFilterUmbreon[i]);
  }
  settings.endArray();
  settings.endGroup();

  settings.beginGroup("espeon");
  settings.setValue("minHpIv", m_coloTab->getEspeonFiltersWidget()->getMinHpIv());
  settings.setValue("minAtkIv", m_coloTab->getEspeonFiltersWidget()->getMinAtkIv());
  settings.setValue("minDefIv", m_coloTab->getEspeonFiltersWidget()->getMinDefIv());
  settings.setValue("minSpAtkIv", m_coloTab->getEspeonFiltersWidget()->getMinSpAtkIv());
  settings.setValue("minSpDefIv", m_coloTab->getEspeonFiltersWidget()->getMinSpDefIv());
  settings.setValue("minSpeedIv", m_coloTab->getEspeonFiltersWidget()->getMinSpeedIv());
  settings.setValue("enableNatureFilter",
                    m_coloTab->getEspeonFiltersWidget()->getEnableNatureFilter());
  QVector<bool> naturesFilterEspeon = m_coloTab->getEspeonFiltersWidget()->getNatureFilters();
  settings.beginWriteArray("naturesFilter");
  for (int i = 0; i < naturesFilterEspeon.size(); i++)
  {
    settings.setArrayIndex(i);
    settings.setValue(GUICommon::naturesStr[i], naturesFilterEspeon[i]);
  }
  settings.endArray();
  settings.endGroup();
  settings.endGroup();

  settings.beginGroup("galeDarknessPredictor");
  settings.beginGroup("eevee");
  settings.setValue("minHpIv", m_xdTab->getEeveeFiltersWidget()->getMinHpIv());
  settings.setValue("minAtkIv", m_xdTab->getEeveeFiltersWidget()->getMinAtkIv());
  settings.setValue("minDefIv", m_xdTab->getEeveeFiltersWidget()->getMinDefIv());
  settings.setValue("minSpAtkIv", m_xdTab->getEeveeFiltersWidget()->getMinSpAtkIv());
  settings.setValue("minSpDefIv", m_xdTab->getEeveeFiltersWidget()->getMinSpDefIv());
  settings.setValue("minSpeedIv", m_xdTab->getEeveeFiltersWidget()->getMinSpeedIv());
  settings.setValue("enableNatureFilter",
                    m_xdTab->getEeveeFiltersWidget()->getEnableNatureFilter());
  QVector<bool> naturesFilterEevee = m_xdTab->getEeveeFiltersWidget()->getNatureFilters();
  settings.beginWriteArray("naturesFilter");
  for (int i = 0; i < naturesFilterEevee.size(); i++)
  {
    settings.setArrayIndex(i);
    settings.setValue(GUICommon::naturesStr[i], naturesFilterEevee[i]);
  }
  settings.endArray();
  settings.setValue("shininess", static_cast<int>(m_xdTab->getShininess()));
  settings.setValue("gender", static_cast<int>(m_xdTab->getGender()));
  settings.endGroup();
  settings.endGroup();
}
