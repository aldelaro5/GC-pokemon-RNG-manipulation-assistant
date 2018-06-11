#pragma once

#include <QDialog>

#include <QDialogButtonBox>
#include <QTabWidget>

#include "ColosseumTab.h"
#include "GaleDarknessTab.h"
#include "GeneralTab.h"

class DlgSettings : public QDialog
{
public:
  DlgSettings(QWidget* parent = nullptr);

  void loadSettings();
  void saveSettings();

private:
  QTabWidget* m_tabWidget;
  GeneralTab* m_generalTab;
  ColosseumTab* m_coloTab;
  GaleDarknessTab* m_xdTab;
  QDialogButtonBox* m_buttonsDlg;
};
