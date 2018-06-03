#pragma once

#include <vector>

#include <QButtonGroup>
#include <QProgressBar>
#include <QSpinBox>
#include <QWizardPage>

#include "../GUICommon.h"

class SeedFinderPassPage : public QWizardPage
{
public:
  SeedFinderPassPage(QWidget* parent = nullptr);

  int nextId() const override;

  virtual std::vector<int> obtainCriteria() = 0;

  void setSeedFinderDone(const bool seedFinderDone);
  void setSeedFinderProgress(const int nbrSeedsSimulated);
  void showSeedFinderProgress(const bool showProgress);

protected:
  QProgressBar* m_pbSeedFinder;

private:
  bool m_seedFinderDone = false;
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
