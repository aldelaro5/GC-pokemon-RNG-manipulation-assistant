#include "SConfig.h"

#include <thread>

SConfig::SConfig()
{
  m_settings = new QSettings("settings.ini", QSettings::IniFormat);
}

SConfig::~SConfig()
{
  delete m_settings;
}

SConfig& SConfig::getInstance()
{
  static SConfig instance;
  return instance;
}

QString SConfig::gameGroupStrForStarter(const GUICommon::starter starter) const
{
  if (starter == GUICommon::starter::Eevee)
    return QString("galeDarknessPredictor");
  return QString("colosseumPredictor");
}

int SConfig::getThreadLimit() const
{
  unsigned int threadLimit = m_settings->value("generalSettings/CPUThreadLimit", 0).toUInt();
  if (threadLimit != 0 && threadLimit >= std::thread::hardware_concurrency())
    return 0;
  return threadLimit;
}

int SConfig::getPredictionTime() const
{
  return m_settings->value("generalSettings/predictor/time", 10).toInt();
}

int SConfig::getFrameDelay() const
{
  return m_settings->value("generalSettings/predictor/FrameDelay", 0).toInt();
}

int SConfig::getMaxAutoReroll() const
{
  return m_settings->value("generalSettings/predictor/MaxAutoReroll", 100).toInt();
}

bool SConfig::getSkipInstructionPage() const
{
  return m_settings->value("generalSettings/skipInstructionPage", false).toBool();
}

int SConfig::getMinHpIv(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings->value(gameGroupStr + "/" + starterGroupStr + "/minHpIv", 0).toInt();
}

int SConfig::getMinAtkIv(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings->value(gameGroupStr + "/" + starterGroupStr + "/minAtkIv", 0).toInt();
}

int SConfig::getMinDefIv(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings->value(gameGroupStr + "/" + starterGroupStr + "/minDefIv", 0).toInt();
}

int SConfig::getMinSpAtkIv(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings->value(gameGroupStr + "/" + starterGroupStr + "/minSpAtkIv", 0).toInt();
}

int SConfig::getMinSpDefIv(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings->value(gameGroupStr + "/" + starterGroupStr + "/minSpDefIv", 0).toInt();
}

int SConfig::getMinSpeedIv(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings->value(gameGroupStr + "/" + starterGroupStr + "/minSpeedIv", 0).toInt();
}

int SConfig::getMinPowerHiddenPower(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings->value(gameGroupStr + "/" + starterGroupStr + "/minPowerHiddenPower", 30)
      .toInt();
}

bool SConfig::getEnableNatureFilter(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings->value(gameGroupStr + "/" + starterGroupStr + "/enableNatureFilter", false)
      .toBool();
}

QVector<bool> SConfig::getNatureFilters(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  QVector<bool> natureFilters;
  int sizeNatureFilters =
      m_settings->beginReadArray(gameGroupStr + "/" + starterGroupStr + "/naturesFilter");
  for (int i = 0; i < sizeNatureFilters; i++)
  {
    m_settings->setArrayIndex(i);
    natureFilters.append(m_settings->value(GUICommon::naturesStr[i], false).toBool());
  }
  m_settings->endArray();
  return natureFilters;
}

bool SConfig::getEnableHiddenPowerTypesFilter(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  return m_settings
      ->value(gameGroupStr + "/" + starterGroupStr + "/enableHiddenPowerTypeFilter", false)
      .toBool();
}

QVector<bool> SConfig::getHiddenPowerTypesFilters(const GUICommon::starter starter) const
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  QVector<bool> hiddenPowerTypeFilters;
  int sizeHiddenPowerTypeFilters =
      m_settings->beginReadArray(gameGroupStr + "/" + starterGroupStr + "/hiddenPowerTypesFilter");
  for (int i = 0; i < sizeHiddenPowerTypeFilters; i++)
  {
    m_settings->setArrayIndex(i);
    hiddenPowerTypeFilters.append(m_settings->value(GUICommon::typesStr[i], false).toBool());
  }
  m_settings->endArray();
  return hiddenPowerTypeFilters;
}

GUICommon::shininess SConfig::getEeveeShininess() const
{
  return static_cast<GUICommon::shininess>(
      m_settings
          ->value("galeDarknessPredictor/eevee/shininess",
                  static_cast<int>(GUICommon::shininess::AnyShininess))
          .toInt());
}

GUICommon::gender SConfig::getEeveeGender() const
{
  return static_cast<GUICommon::gender>(m_settings
                                            ->value("galeDarknessPredictor/eevee/gender",
                                                    static_cast<int>(GUICommon::gender::AnyGender))
                                            .toInt());
}

void SConfig::setThreadLimit(const int threadLimit)
{
  m_settings->setValue("generalSettings/CPUThreadLimit", threadLimit);
}

void SConfig::setPredictionTime(const int predictionTime)
{
  m_settings->setValue("generalSettings/predictor/time", predictionTime);
}

void SConfig::setFrameDelay(const int frameDelay)
{
  m_settings->setValue("generalSettings/predictor/FrameDelay", frameDelay);
}

void SConfig::setMaxAutoReroll(const int maxAutoReroll)
{
  m_settings->setValue("generalSettings/predictor/MaxAutoReroll", maxAutoReroll);
}

void SConfig::setSkipInstructionPage(const bool skipInstructionPage)
{
  m_settings->setValue("generalSettings/skipInstructionPage", skipInstructionPage);
}

void SConfig::setMinHpIv(const GUICommon::starter starter, const int minHpIv)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/minHpIv", minHpIv);
}

void SConfig::setMinAtkIv(const GUICommon::starter starter, const int minAtkIv)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/minAtkIv", minAtkIv);
}

void SConfig::setMinDefIv(const GUICommon::starter starter, const int minDefIv)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/minDefIv", minDefIv);
}

void SConfig::setMinSpAtkIv(const GUICommon::starter starter, const int minSpAtkIv)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/minSpAtkIv", minSpAtkIv);
}

void SConfig::setMinSpDefIv(const GUICommon::starter starter, const int minSpDefIv)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/minSpDefIv", minSpDefIv);
}

void SConfig::setMinSpeedIv(const GUICommon::starter starter, const int minSpeedIv)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/minSpeedIv", minSpeedIv);
}

void SConfig::setMinPowerHiddenPower(const GUICommon::starter starter,
                                     const int minPowerHiddenPower)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/minPowerHiddenPower",
                       minPowerHiddenPower);
}

void SConfig::setEnableNatureFilter(const GUICommon::starter starter, const bool enableNatureFilter)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/enableNatureFilter",
                       enableNatureFilter);
}

void SConfig::setNatureFilters(const GUICommon::starter starter, const QVector<bool> natureFilters)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->beginWriteArray(gameGroupStr + "/" + starterGroupStr + "/naturesFilter");
  for (int i = 0; i < natureFilters.size(); i++)
  {
    m_settings->setArrayIndex(i);
    m_settings->setValue(GUICommon::naturesStr[i], natureFilters[i]);
  }
  m_settings->endArray();
}

void SConfig::setEnableHiddenPowerTypesFilter(const GUICommon::starter starter,
                                              const bool enableHiddenPowerTypeFilter)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->setValue(gameGroupStr + "/" + starterGroupStr + "/enableHiddenPowerTypeFilter",
                       enableHiddenPowerTypeFilter);
}

void SConfig::setHiddenPowerTypesFilters(const GUICommon::starter starter,
                                         const QVector<bool> hiddenPowerTypeFilters)
{
  QString gameGroupStr = gameGroupStrForStarter(starter);
  QString starterGroupStr = s_starterGroupsStr.at(static_cast<int>(starter));
  m_settings->beginWriteArray(gameGroupStr + "/" + starterGroupStr + "/hiddenPowerTypesFilter");
  for (int i = 0; i < hiddenPowerTypeFilters.size(); i++)
  {
    m_settings->setArrayIndex(i);
    m_settings->setValue(GUICommon::typesStr[i], hiddenPowerTypeFilters[i]);
  }
  m_settings->endArray();
}

void SConfig::setEeveeShininess(const GUICommon::shininess shininess) const
{
  m_settings->setValue("galeDarknessPredictor/eevee/shininess", static_cast<int>(shininess));
}

void SConfig::setEeveeGender(const GUICommon::gender gender) const
{
  m_settings->setValue("galeDarknessPredictor/eevee/gender", static_cast<int>(gender));
}
