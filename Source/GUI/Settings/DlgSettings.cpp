#include "DlgSettings.h"

#include <QSettings>
#include <QVBoxLayout>

#include "../GUICommon.h"

DlgSettings::DlgSettings(QWidget* parent) : QDialog(parent)
{
  m_tabWidget = new QTabWidget(this);
  m_generalTab = new GeneralTab(this);
  m_tabWidget->addTab(m_generalTab, tr("General"));
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
  QSettings settings("settings.ini", QSettings::IniFormat);

  settings.beginGroup("general");
  settings.beginGroup("finder");
  m_generalTab->setPlatform(static_cast<GUICommon::platform>(
      settings.value("platform", static_cast<int>(GUICommon::platform::GameCube)).toInt()));
  m_generalTab->setRtcMarginError(settings.value("rtcMarginError", 5).toInt());
  settings.endGroup();
  settings.beginGroup("predictor");
  m_generalTab->setPredictionTime(settings.value("time", 10).toInt());
  settings.endGroup();
  settings.endGroup();

  settings.beginGroup("colosseumPredictor");
  settings.beginGroup("umbreon");
  m_coloTab->getUmbreonFiltersWidget()->setMinHpIv(settings.value("minHpIv", 0).toInt());
  m_coloTab->getUmbreonFiltersWidget()->setMinAtkIv(settings.value("minAtkIv", 0).toInt());
  m_coloTab->getUmbreonFiltersWidget()->setMinDefIv(settings.value("minDefIv", 0).toInt());
  m_coloTab->getUmbreonFiltersWidget()->setMinSpAtkIv(settings.value("minSpAtkIv", 0).toInt());
  m_coloTab->getUmbreonFiltersWidget()->setMinSpDefIv(settings.value("minSpDefIv", 0).toInt());
  m_coloTab->getUmbreonFiltersWidget()->setMinSpeedIv(settings.value("minSpeedIv", 0).toInt());
  m_coloTab->getUmbreonFiltersWidget()->setEnableNatureFilter(
      settings.value("enableNatureFilter", false).toBool());
  QVector<bool> naturesFilterUmbreon;
  int sizeNaturesUmbreon = settings.beginReadArray("naturesFilter");
  for (int i = 0; i < sizeNaturesUmbreon; i++)
  {
    settings.setArrayIndex(i);
    naturesFilterUmbreon.append(settings.value(GUICommon::naturesStr[i], false).toBool());
  }
  settings.endArray();
  m_coloTab->getUmbreonFiltersWidget()->setNatureFilters(naturesFilterUmbreon);
  settings.endGroup();

  settings.beginGroup("espeon");
  m_coloTab->getEspeonFiltersWidget()->setMinHpIv(settings.value("minHpIv", 0).toInt());
  m_coloTab->getEspeonFiltersWidget()->setMinAtkIv(settings.value("minAtkIv", 0).toInt());
  m_coloTab->getEspeonFiltersWidget()->setMinDefIv(settings.value("minDefIv", 0).toInt());
  m_coloTab->getEspeonFiltersWidget()->setMinSpAtkIv(settings.value("minSpAtkIv", 0).toInt());
  m_coloTab->getEspeonFiltersWidget()->setMinSpDefIv(settings.value("minSpDefIv", 0).toInt());
  m_coloTab->getEspeonFiltersWidget()->setMinSpeedIv(settings.value("minSpeedIv", 0).toInt());
  m_coloTab->getEspeonFiltersWidget()->setEnableNatureFilter(
      settings.value("enableNatureFilter", false).toBool());
  QVector<bool> naturesFilterEspeon;
  int sizeNaturesEspeon = settings.beginReadArray("naturesFilter");
  for (int i = 0; i < sizeNaturesEspeon; i++)
  {
    settings.setArrayIndex(i);
    naturesFilterEspeon.append(settings.value(GUICommon::naturesStr[i], false).toBool());
  }
  settings.endArray();
  m_coloTab->getEspeonFiltersWidget()->setNatureFilters(naturesFilterEspeon);
  settings.endGroup();
  settings.endGroup();

  settings.beginGroup("galeDarknessPredictor");
  settings.beginGroup("eevee");
  m_xdTab->getEeveeFiltersWidget()->setMinHpIv(settings.value("minHpIv", 0).toInt());
  m_xdTab->getEeveeFiltersWidget()->setMinAtkIv(settings.value("minAtkIv", 0).toInt());
  m_xdTab->getEeveeFiltersWidget()->setMinDefIv(settings.value("minDefIv", 0).toInt());
  m_xdTab->getEeveeFiltersWidget()->setMinSpAtkIv(settings.value("minSpAtkIv", 0).toInt());
  m_xdTab->getEeveeFiltersWidget()->setMinSpDefIv(settings.value("minSpDefIv", 0).toInt());
  m_xdTab->getEeveeFiltersWidget()->setMinSpeedIv(settings.value("minSpeedIv", 0).toInt());
  m_xdTab->getEeveeFiltersWidget()->setEnableNatureFilter(
      settings.value("enableNatureFilter", false).toBool());
  QVector<bool> naturesFilterEevee;
  int sizeNaturesEevee = settings.beginReadArray("naturesFilter");
  for (int i = 0; i < sizeNaturesEevee; i++)
  {
    settings.setArrayIndex(i);
    naturesFilterEevee.append(settings.value(GUICommon::naturesStr[i], false).toBool());
  }
  settings.endArray();
  m_xdTab->getEeveeFiltersWidget()->setNatureFilters(naturesFilterEevee);
  m_xdTab->setShininess(static_cast<GUICommon::shininess>(
      settings.value("shininess", static_cast<int>(GUICommon::shininess::AnyShininess)).toInt()));
  m_xdTab->setGender(static_cast<GUICommon::gender>(
      settings.value("gender", static_cast<int>(GUICommon::gender::AnyGender)).toInt()));
  settings.endGroup();
  settings.endGroup();
}

void DlgSettings::saveSettings()
{
  QSettings settings("settings.ini", QSettings::IniFormat);

  settings.beginGroup("general");
  settings.beginGroup("finder");
  settings.setValue("platform", static_cast<int>(m_generalTab->getPlatform()));
  settings.setValue("rtcMarginError", m_generalTab->getRtcMarginError());
  settings.setValue("rtcMarginError", m_generalTab->getRtcMarginError());
  settings.endGroup();
  settings.beginGroup("predictor");
  settings.setValue("time", m_generalTab->getPredictionTime());
  settings.endGroup();
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
