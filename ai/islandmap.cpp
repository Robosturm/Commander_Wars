#include "ai/islandmap.h"

#include "coreengine/mainapp.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/co.h"
#include "game/unitpathfindingsystem.h"

IslandMap::IslandMap(GameMap* pMap, const QString & unitID, Player* pOwner)
      : m_pOwner(pOwner),
        m_pMap(pMap)
{
    setObjectName("IslandMap");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    
    if (m_pMap != nullptr)
    {
        qint32 width = m_pMap->getMapWidth();
        qint32 heigth = m_pMap->getMapHeight();

        for (qint32 x = 0; x < width; x++)
        {
            m_Islands.append(QVector<qint32>(heigth, UNKNOWN));
        }
        spUnit pUnit = spUnit::create(unitID, pOwner, false, pMap);
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
                        UnitPathFindingSystem pfs(m_pMap, pUnit.get());
                        pfs.setMovepoints(-2);
                        pfs.setFast(true);
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
}

qint32 IslandMap::getIslandSize(qint32 island) const
{
    qint32 count = 0;
    for (const auto & data : qAsConst(m_Islands))
    {
        for (const auto & field : qAsConst(data))
        {
            if (field == island)
            {
                ++count;
            }
        }
    }
    return count;
}

QString IslandMap::getMovementType() const
{
    return m_MovementType;
}

void IslandMap::getValueOnIsland(qint32 island, qint32 &ownValue, qint32 & enemyValue)
{
    ownValue = 0;
    enemyValue = 0;
    
    if (m_pMap != nullptr)
    {
        qint32 width = m_pMap->getMapWidth();
        qint32 heigth = m_pMap->getMapHeight();
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                if (m_Islands[x][y] == island)
                {
                    Unit* pUnit = m_pMap->getTerrain(x, y)->getUnit();
                    if (pUnit != nullptr)
                    {
                        if (pUnit->getOwner() == m_pOwner)
                        {
                            ownValue += pUnit->getCoUnitValue();
                        }
                        else if (m_pOwner->isEnemyUnit(pUnit))
                        {
                            enemyValue += pUnit->getCoUnitValue();
                        }
                    }
                }
            }
        }
    }
}

void IslandMap::show()
{
    for (qint32 x = 0; x < m_Islands.size(); ++x)
    {
        for (qint32 y = 0; y < m_Islands[x].size(); ++y)
        {
            if (m_Islands[x][y] > UNKNOWN)
            {
                oxygine::spColorRectSprite sprite = oxygine::spColorRectSprite::create();
                sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
                QColor color = Qt::red;
                color.setAlphaF(0.5f);
                sprite->setColor(color);
                sprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
                sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
                m_pMap->addChild(sprite);
                m_info.append(sprite);
            }
        }
    }
}

void IslandMap::hide()
{
    for (auto & field : m_info)
    {
        field->detach();
    }
    m_info.clear();
}

