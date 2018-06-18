#include "GUICommon.h"

#include <QObject>

namespace GUICommon
{
QStringList gamesStr =
    QStringList({QObject::tr("Pokemon Colosseum"), QObject::tr("Pokemon XD: Gale of Darkness")});
QStringList platformsStr = QStringList({"Nintendo GameCube", "Nintendo Wii"});

QStringList naturesStr = QStringList(
    {QObject::tr("Hardy"),   QObject::tr("Lonely"), QObject::tr("Brave"),   QObject::tr("Adamant"),
     QObject::tr("Naughty"), QObject::tr("Bold"),   QObject::tr("Docile"),  QObject::tr("Relaxed"),
     QObject::tr("Impish"),  QObject::tr("Lax"),    QObject::tr("Timid"),   QObject::tr("Hasty"),
     QObject::tr("Serious"), QObject::tr("Jolly"),  QObject::tr("Naive"),   QObject::tr("Modest"),
     QObject::tr("Mild"),    QObject::tr("Quiet"),  QObject::tr("Bashful"), QObject::tr("Rash"),
     QObject::tr("Calm"),    QObject::tr("Gentle"), QObject::tr("Sassy"),   QObject::tr("Careful"),
     QObject::tr("Quirky")});

QStringList typesStr = QStringList(
    {QObject::tr("Fighting"), QObject::tr("Flying"), QObject::tr("Poison"), QObject::tr("Ground"),
     QObject::tr("Rock"), QObject::tr("Bug"), QObject::tr("Ghost"), QObject::tr("Steel"),
     QObject::tr("Fire"), QObject::tr("Water"), QObject::tr("Grass"), QObject::tr("Electric"),
     QObject::tr("Psychic"), QObject::tr("Ice"), QObject::tr("Dragon"), QObject::tr("Dark")});

QStringList shininessStr =
    QStringList({QObject::tr("Shiny"), QObject::tr("Not shiny"), QObject::tr("Any")});
QStringList genderStr =
    QStringList({QObject::tr("Male"), QObject::tr("Female"), QObject::tr("Any")});
}; // namespace GUICommon
