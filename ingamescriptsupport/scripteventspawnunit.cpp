#include "scripteventspawnunit.h"

ScriptEventSpawnUnit::ScriptEventSpawnUnit()
    : ScriptEvent (ScriptEvent::EventType::spawnUnit)
{

}

void ScriptEventSpawnUnit::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QStringList items = line.replace("map.spawnUnit(", "")
                            .replace(", ", ",")
                            .replace(", \"", ",")
                            .replace("\", map.getPlayer(", ",")
                            .replace("), ", ",")
                            .replace("); // " + EventSpawnUnit, "").split(",");
    if (items.size() == 5)
    {
        x = items[0].toInt();
        y = items[1].toInt();
        unitID = items[2];
        player = items[3].toInt();
        radius = items[4].toInt();
    }
}

void ScriptEventSpawnUnit::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.spawnUnit(" << QString::number(x) << ", "
            << QString::number(y) << ", \"" << unitID << "\", map.getPlayer("
            << QString::number(player) << "), " << QString::number(radius) << "); // "
            << EventSpawnUnit << "\n";
}

qint32 ScriptEventSpawnUnit::getX() const
{
    return x;
}

void ScriptEventSpawnUnit::setX(const qint32 &value)
{
    x = value;
}

qint32 ScriptEventSpawnUnit::getY() const
{
    return y;
}

void ScriptEventSpawnUnit::setY(const qint32 &value)
{
    y = value;
}

QString ScriptEventSpawnUnit::getUnitID() const
{
    return unitID;
}

void ScriptEventSpawnUnit::setUnitID(const QString &value)
{
    unitID = value;
}

qint32 ScriptEventSpawnUnit::getPlayer() const
{
    return player;
}

void ScriptEventSpawnUnit::setPlayer(const qint32 &value)
{
    player = value;
}

qint32 ScriptEventSpawnUnit::getRadius() const
{
    return radius;
}

void ScriptEventSpawnUnit::setRadius(const qint32 &value)
{
    radius = value;
}
