#pragma once

#include <QDialog>

#include <QDialogButtonBox>
#include <QTabWidget>

#include "ColosseumTab.h"
#include "GaleDarknessTab.h"
#include "SeedFinderTab.h"

class DlgSettings : public QDialog
{
public:
  DlgSettings(QWidget* parent = nullptr);

private:
  QTabWidget* m_tabWidget;
  SeedFinderTab* m_seedFinderTab;
  ColosseumTab* m_coloTab;
  GaleDarknessTab* m_xdTab;
  QDialogButtonBox* m_buttonsDlg;
};
