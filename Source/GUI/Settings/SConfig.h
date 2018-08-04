#pragma once

#include <QSettings>
#include <QVector>

#include "../GUICommon.h"

class SConfig
{
public:
  static SConfig& getInstance();
  SConfig(SConfig const&) = delete;
  void operator=(SConfig const&) = delete;

  int getThreadLimit() const;
  int getPredictionTime() const;
  int getFrameDelay() const;
  int getMaxAutoReroll() const;
  bool getSkipInstructionPage() const;
  int getMinHpIv(const GUICommon::starter starter) const;
  int getMinAtkIv(const GUICommon::starter starter) const;
  int getMinDefIv(const GUICommon::starter starter) const;
  int getMinSpAtkIv(const GUICommon::starter starter) const;
  int getMinSpDefIv(const GUICommon::starter starter) const;
  int getMinSpeedIv(const GUICommon::starter starter) const;
  int getMinPowerHiddenPower(const GUICommon::starter starter) const;
  bool getEnableNatureFilter(const GUICommon::starter starter) const;
  QVector<bool> getNatureFilters(const GUICommon::starter starter) const;
  bool getEnableHiddenPowerTypesFilter(const GUICommon::starter starter) const;
  QVector<bool> getHiddenPowerTypesFilters(const GUICommon::starter starter) const;

  GUICommon::shininess getEeveeShininess() const;
  GUICommon::gender getEeveeGender() const;

  void setThreadLimit(const int threadLimit);
  void setPredictionTime(const int predictionTime);
  void setFrameDelay(const int frameDelay);
  void setMaxAutoReroll(const int maxAutoReroll);
  void setSkipInstructionPage(const bool skipInstructionPage);
  void setMinHpIv(const GUICommon::starter starter, const int minHpIv);
  void setMinAtkIv(const GUICommon::starter starter, const int minAtkIv);
  void setMinDefIv(const GUICommon::starter starter, const int minDefIv);
  void setMinSpAtkIv(const GUICommon::starter starter, const int minSpAtkIv);
  void setMinSpDefIv(const GUICommon::starter starter, const int minSpDefIv);
  void setMinSpeedIv(const GUICommon::starter starter, const int minSpeedIv);
  void setMinPowerHiddenPower(const GUICommon::starter starter, const int minPowerHiddenPower);
  void setEnableNatureFilter(const GUICommon::starter starter, const bool enableNatureFilter);
  void setNatureFilters(const GUICommon::starter starter, const QVector<bool> natureFilters);
  void setEnableHiddenPowerTypesFilter(const GUICommon::starter starter,
                                       const bool enableHiddenPowerTypeFilter);
  void setHiddenPowerTypesFilters(const GUICommon::starter starter,
                                  const QVector<bool> hiddenPowerTypeFilters);

  void setEeveeShininess(const GUICommon::shininess shininess) const;
  void setEeveeGender(const GUICommon::gender gender) const;

private:
  SConfig();
  ~SConfig();

  QString gameGroupStrForStarter(const GUICommon::starter starter) const;

  const QStringList s_starterGroupsStr =
      QStringList({QObject::tr("umbreon"), QObject::tr("espeon"), QObject::tr("eevee")});
  QSettings* m_settings;
};
