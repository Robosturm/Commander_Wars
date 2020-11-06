#include "ai/influencefrontmap.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "game/gamemap.h"
#include "game/unitpathfindingsystem.h"

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

qint32 InfluenceFrontMap::InfluenceInfo::getOwner()
{
    qint32 owner = -1;
    qint32 highestValue = 0;
    for (qint32 player = 0; player < playerValues.size(); ++player)
    {
        if (playerValues[player] > highestValue)
        {
            highestValue = playerValues[player];
            owner = player;
        }
    }
    return owner;
}

void InfluenceFrontMap::reset()
{
    hide();
    for (qint32 x = 0; x < m_InfluenceMap.size(); ++x)
    {
        for (qint32 y = 0; y < m_InfluenceMap[x].size(); ++y)
        {
            auto& field = m_InfluenceMap[x][y];
            field.frontField = -1;
            for (auto & value : field.playerValues)
            {
                value = 0;
            }
        }
    }
}

void InfluenceFrontMap::addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs, qint32 movePoints)
{
    qint32 value = pUnit->getUnitValue();
    qint32 owner = pUnit->getOwner()->getPlayerID();
    auto points = pPfs->getAllNodePoints();
    for (const auto & point : points)
    {
        float divider = 1.0f;
        qint32 fieldCost = pPfs->getTargetCosts(point.x(), point.y());
        if (movePoints > 0 && fieldCost > 0)
        {
            divider = GlobalUtils::roundUp(static_cast<float>(fieldCost) / static_cast<float>(movePoints));
        }
        m_InfluenceMap[point.x()][point.y()].playerValues[owner] += value / divider;
    }
}

void InfluenceFrontMap::show()
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 x = 0; x < m_InfluenceMap.size(); ++x)
    {
        for (qint32 y = 0; y < m_InfluenceMap[x].size(); ++y)
        {
            qint32 owner = m_InfluenceMap[x][y].getOwner();
            if (owner >= 0)
            {
                oxygine::spColorRectSprite sprite = new oxygine::ColorRectSprite();
                sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
                QColor color = pMap->getPlayer(owner)->getColor();
                color.setAlphaF(0.5f);
                sprite->setColor(color);
                sprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
                sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
                pMap->addChild(sprite);
                m_info.append(sprite);
            }
        }
    }
}

void InfluenceFrontMap::hide()
{
    for (auto & field : m_info)
    {
        field->detach();
    }
    m_info.clear();
}
