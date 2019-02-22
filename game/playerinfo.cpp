#include "playerinfo.h"

#include "game/gamemap.h"

PlayerInfo::PlayerInfo()
    : QObject()
{

}

void PlayerInfo::updateData()
{
    GameMap* pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {

    }
}
