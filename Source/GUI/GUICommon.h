#pragma once

#include <QString>
#include <QStringList>

namespace GUICommon
{
enum gameSelection
{
  Colosseum = 0,
  XD,
  Unselected
};

extern QStringList gamesStr;

// Ordered by in-game indexes
extern QStringList naturesStr;
extern QStringList typesStr;
extern QStringList genderStr;
} // namespace GUICommon