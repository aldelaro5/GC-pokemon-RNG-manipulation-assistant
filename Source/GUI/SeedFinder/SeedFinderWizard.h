#pragma once

#include <QWizard>

#include <QCheckBox>
#include <QLabel>
#include <vector>

#include "../../Common/CommonTypes.h"
#include "../GUICommon.h"
#include "SeedFinderPassPage.h"

class SeedFinderWizard : public QWizard
{
public:
  enum pageID
  {
    Start = 0,
    SeedFinderPass,
    // This number is set to arbirtrarilly high to allow to create new finder pass pages on the fly,
    // but kept high enough so that it will never be hit normally before getting the seed finder
    // done
    End = 1000
  };

  SeedFinderWizard(QWidget* parent, GUICommon::gameSelection game);

  void accept() override;

  void nextSeedFinderPass();
  void pageChanged();

  static int numberPass;

private:
  SeedFinderPassPage* getSeedFinderPassPageForGame();

  bool m_seedFinderPassDone = false;
  std::vector<u32> seeds;
  GUICommon::gameSelection m_game;
};

class StartPage : public QWizardPage
{
public:
  StartPage(QWidget* parent, GUICommon::gameSelection game);

  int nextId() const override;
};

class EndPage : public QWizardPage
{
public:
  EndPage(QWidget* parent = nullptr);

  int nextId() const override;
};
