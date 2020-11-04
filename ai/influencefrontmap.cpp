#include "ai/influencefrontmap.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

InfluenceFrontMap::InfluenceFrontMap()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    spGameMap pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    for (qint32 x = 0; x < width; x++)
    {
        m_InfluenceMap.append(QVector<InfluenceInfo>(heigth, InfluenceInfo(pMap.get())));
    }
}

InfluenceFrontMap::InfluenceInfo::InfluenceInfo(GameMap* pMap)
{
    qint32 playerCount = pMap->getPlayerCount();
    for (qint32 x = 0; x < playerCount; x++)
    {
        playerValues.append(0);
    }
}

void InfluenceFrontMap::addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs)
{

}
