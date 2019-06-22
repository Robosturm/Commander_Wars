#include "scripteventdefeatplayer.h"

ScriptEventDefeatPlayer::ScriptEventDefeatPlayer()
    : ScriptEvent (ScriptEvent::EventType::defeatPlayer)
{

}

void ScriptEventDefeatPlayer::readEvent(QTextStream& rStream)
{
    QString line = rStream.readLine().simplified();
    QString item = line.replace("map.getPlayer(", "")
                    .replace(").setIsDefeated(true); // " + EventDefeatPlayer, "");
    player = item.toInt();
}

void ScriptEventDefeatPlayer::writeEvent(QTextStream& rStream)
{
    rStream <<  "            map.getPlayer(" << QString::number(player) << ").setIsDefeated(true); // "
            << EventDefeatPlayer << "\n";
}

qint32 ScriptEventDefeatPlayer::getPlayer() const
{
    return player;
}

void ScriptEventDefeatPlayer::setPlayer(const qint32 &value)
{
    player = value;
}
