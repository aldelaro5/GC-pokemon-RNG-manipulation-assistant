#pragma once

#include <vector>

#include <QButtonGroup>
#include <QSpinBox>
#include <QWizardPage>

#include "../GUICommon.h"

class SeedFinderPassPage : public QWizardPage
{
public:
  SeedFinderPassPage(QWidget* parent = nullptr);

  int nextId() const override;

  virtual std::vector<int> obtainCriteria() = 0;

  void setSeedFinderPassDone(const bool seedFinderPassDone);

private:
  bool m_seedFinderPassDone = false;
};

class SeedFinderPassColosseum : public SeedFinderPassPage
{
public:
  SeedFinderPassColosseum(QWidget* parent = nullptr);

  std::vector<int> obtainCriteria() override;

private:
  QButtonGroup* m_playerTeamIndexBtnGroup;
  QButtonGroup* m_playerNameIndexBtnGroup;
};

class SeedFinderPassXD : public SeedFinderPassPage
{
public:
  SeedFinderPassXD(QWidget* parent = nullptr);

  std::vector<int> obtainCriteria() override;

private:
  QButtonGroup* m_playerTeamIndexBtnGroup;
  QButtonGroup* m_enemyTeamIndexBtnGroup;
  QSpinBox* m_spnTopLeftPkmnHP;
  QSpinBox* m_spnBottomLeftPkmnHP;
  QSpinBox* m_spnTopRightPkmnHP;
  QSpinBox* m_spnBottomRightPkmnHP;
};
