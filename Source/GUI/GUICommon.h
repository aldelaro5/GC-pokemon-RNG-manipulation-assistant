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

enum platform
{
  GameCube = 0,
  Wii
};

enum shininess
{
  Shiny = 0,
  NotShiny,
  AnyShininess
};

enum gender
{
  Male = 0,
  Female,
  AnyGender
};

extern QStringList gamesStr;
extern QStringList platformsStr;

// Ordered by in-game indexes
extern QStringList naturesStr;
extern QStringList typesStr;
extern QStringList shininessStr;
extern QStringList genderStr;
} // namespace GUICommon