#include "islandmap.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "game/unitpathfindingsystem.h"

IslandMap::IslandMap(QString unitID, Player* pOwner)
    : QObject()
{
    Mainapp* pApp = Mainapp::getInstance();
    this->moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    GameMap* pMap = GameMap::getInstance();

    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();

    for (qint32 x = 0; x < width; x++)
    {
        m_Islands.append(QVector<qint32>(heigth, -1));
    }
    spUnit pUnit = new Unit(unitID, pOwner);
    pUnit->setIgnoreUnitCollision(true);
    m_MovementType = pUnit->getMovementType();
    qint32 currentIsland = 0;
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            if (m_Islands[x][y] < 0)
            {
                if (pUnit->canMoveOver(x, y))
                {
                    UnitPathFindingSystem pfs(pUnit.get());
                    pfs.setMoveCosts(-1);
                    pfs.setStartPoint(x, y);
                    pfs.explore();
                    QVector<QPoint> nodes = pfs.getAllNodePoints();
                    for (qint32 i = 0; i < nodes.size(); i++)
                    {
                        m_Islands[nodes[i].x()][nodes[i].y()] = currentIsland;
                    }
                    currentIsland++;
                }
            }
        }
    }
}

QString IslandMap::getMovementType() const
{
    return m_MovementType;
}
