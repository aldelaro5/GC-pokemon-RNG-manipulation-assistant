#pragma once

#include <QComboBox>
#include <QMainWindow>
#include <QPushButton>
#include <QTableWidget>

#include "../PokemonRNGSystem/BaseRNGSystem.h"

class MainWindow : public QMainWindow
{
public:
  enum gameSelection
  {
    Colosseum = 0,
    XD,
    Unselected
  };

  MainWindow();
  ~MainWindow();

  void gameChanged();
  void startSeedFinder();

private:
  void initialiseWidgets();
  void makeLayouts();

  BaseRNGSystem* m_system = nullptr;

  QComboBox* m_cmbGame;
  QPushButton* m_btnStartSeedFinder;
};