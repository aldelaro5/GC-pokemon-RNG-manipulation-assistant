#include "GUICommon.h"

#include <QObject>

namespace GUICommon
{
QStringList gamesStr =
    QStringList({QObject::tr("Pokemon Colosseum"), QObject::tr("Pokemon XD: Gale of Darkness")});
QStringList naturesStr = QStringList(
    {QObject::tr("Hardy"),   QObject::tr("Lonely"), QObject::tr("Brave"),   QObject::tr("Adamant"),
     QObject::tr("Naughty"), QObject::tr("Bold"),   QObject::tr("Docile"),  QObject::tr("Relaxed"),
     QObject::tr("Impish"),  QObject::tr("Lax"),    QObject::tr("Timid"),   QObject::tr("Hasty"),
     QObject::tr("Serious"), QObject::tr("Jolly"),  QObject::tr("Naive"),   QObject::tr("Modest"),
     QObject::tr("Mild"),    QObject::tr("Quiet"),  QObject::tr("Bashful"), QObject::tr("Rash"),
     QObject::tr("Calm"),    QObject::tr("Gentle"), QObject::tr("Sassy"),   QObject::tr("Careful"),
     QObject::tr("Quirky")});

QStringList typesStr = QStringList({""});

QStringList genderStr = QStringList({QObject::tr("Male"), QObject::tr("Female")});
}; // namespace GUICommon