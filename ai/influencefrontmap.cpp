#include <QCoreApplication>

#include "ai/influencefrontmap.h"

#include "coreengine/globalutils.h"
#include "coreengine/gameconsole.h"
#include "ai/coreai.h"

#include "game/gamemap.h"
#include "game/unitpathfindingsystem.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/label.h"

#include "3rd_party/oxygine-framework/oxygine/actor/ColorRectSprite.h"

InfluenceInfo::InfluenceInfo()
{
    Interpreter::setCppOwnerShip(this);
}

InfluenceInfo::InfluenceInfo(GameMap* pMap)
    : m_pMap(pMap)
{
    Interpreter::setCppOwnerShip(this);
}

void InfluenceInfo::updateOwner(Player* pOwner)
{
    owners.clear();
    qint32 highestValue = 0;
    enemyInfluence = 0;
    ownInfluence = 0;
    for (qint32 player = 0; player < playerValues.size(); ++player)
    {
        qint32 influence = getPlayerInfluence(player);
        if (pOwner->isPlayerIdEnemy(player) &&
            influence > enemyInfluence)
        {
            enemyInfluence = influence;
        }
        else if (pOwner->isPlayerIdAlly(player) &&
                 influence > ownInfluence)
        {
            ownInfluence = influence;
        }
        if (influence > highestValue)
        {
            highestValue = influence;
            owners.clear();
            owners.push_back(player);
        }
        else if (influence > 0 &&
                 influence == highestValue)
        {
            owners.push_back(player);
        }
    }
    highestInfluence = highestValue;
}

qint32 InfluenceInfo::getPlayerInfluence(qint32 playerId)
{
    qint32 influence = 0;
    Player* pOwner = m_pMap->getPlayer(playerId);
    for (qint32 player = 0; player < playerValues.size(); ++player)
    {
        if (pOwner->isPlayerIdAlly(player))
        {
            influence += playerValues[player];
        }
    }
    return influence;
}

void InfluenceInfo::reset(GameMap* pMap)
{
    m_pMap = pMap;
    qint32 playerCount = m_pMap->getPlayerCount();
    if (playerValues.size() == 0)
    {
        playerValues.reserve(playerCount);
        for (qint32 i = 0; i < playerCount; ++i)
        {
            playerValues.push_back(0);
        }
    }
    else
    {
        for (auto & value : playerValues)
        {
            value = 0;
        }
    }
    highestInfluence = 0;
    owners.clear();
    frontMovetype.clear();
    frontOwners.clear();
    frontLineCreated = false;
}

void InfluenceInfo::increaseInfluence(qint32 player, qint32 value)
{
    playerValues[player] += value;
}

QStringList InfluenceInfo::getFrontMovetype() const
{
    return frontMovetype;
}

qint32 InfluenceInfo::getHighestInfluence() const
{
    return highestInfluence;
}

qint32 InfluenceInfo::getEnemyInfluence() const
{
    return enemyInfluence;
}

const std::vector<qint32> &InfluenceInfo::getFrontOwners() const
{
    return frontOwners;
}

qint32 InfluenceInfo::getOwnInfluence() const
{
    return ownInfluence;
}

InfluenceFrontMap::InfluenceFrontMap(GameMap* pMap, const std::vector<spIslandMap> & islands)
    : m_islands(islands),
      m_pMap(pMap)
{
#ifdef GRAPHICSUPPORT
    setObjectName("InfluenceFrontMap");
#endif
    Interpreter::setCppOwnerShip(this);
    reset();
}

void InfluenceFrontMap::addBuildingInfluence()
{
    AI_CONSOLE_PRINT("InfluenceFrontMap::addBuildingInfluence()", GameConsole::eDEBUG);
    std::vector<QPoint> buildingPositions;
    std::vector<qint32> buildingOwners;
    std::vector<QStringList> buildLists;
    std::map<QString, qint32> unitIdToIsland;
    
    std::vector<float> income;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
    {
        income.push_back(m_pMap->getPlayer(i)->calcIncome());
    }

    qint32 width = m_pMap->getMapWidth();
    qint32 heigth = m_pMap->getMapHeight();
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            Building* pBuilding = m_pMap->getTerrain(x, y)->getBuilding();
            if (pBuilding != nullptr && pBuilding->getOwner() != nullptr)
            {
                if (pBuilding->getActionList().contains(CoreAI::ACTION_BUILD_UNITS))
                {
                    buildingPositions.push_back(QPoint(x, y));
                    buildingOwners.push_back(pBuilding->getOwner()->getPlayerID());
                    buildLists.push_back(pBuilding->getConstructionList());
                }
            }
        }
    }
    float fullInfluenceRange = 6;
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            QPoint curPos(x, y);
            for (qint32 building = 0; building < buildingPositions.size(); building++)
            {
                QCoreApplication::processEvents();
                QPoint pos = buildingPositions[building];
                float buildSize = static_cast<float>(buildLists[building].size());
                qint32 owner = buildingOwners[building];
                float singleInfluence = income[owner] / buildSize;
                for (auto & unitId : buildLists[building])
                {
                    qint32 island = getIslandFromUnitId(unitId, unitIdToIsland);

                    if (island >= 0 && m_islands[island]->sameIsland(x, y, pos.x(), pos.y()))
                    {
                        float dis = GlobalUtils::getDistance(curPos, pos);
                        if (dis > fullInfluenceRange)
                        {
                            float dayDivider = dis / fullInfluenceRange + 1.0f;
                            m_InfluenceMap[x][y].increaseInfluence(owner, singleInfluence / (dayDivider));
                        }
                        else
                        {
                            m_InfluenceMap[x][y].increaseInfluence(owner, singleInfluence);
                        }
                    }
                }
            }
        }
    }
}

qint32 InfluenceFrontMap::getIslandFromUnitId(const QString & unitId, std::map<QString, qint32> & unitIdToIsland)
{
    UnitSpriteManager* pUnitSpriteManager = UnitSpriteManager::getInstance();
    qint32 island = -1;

    if (unitIdToIsland.contains(unitId))
    {
        island = unitIdToIsland[unitId];
    }
    else
    {
        QString moveType = pUnitSpriteManager->getMovementType(unitId);
        for (qint32 i = 0; i < m_islands.size(); i++)
        {
            if (m_islands[i]->getMovementType() == moveType)
            {
                island = i;
                break;
            }
        }
        unitIdToIsland.insert_or_assign(unitId, island);
    }
    return island;
}

void InfluenceFrontMap::reset()
{
    AI_CONSOLE_PRINT("InfluenceFrontMap::reset()", GameConsole::eDEBUG);
    hide();
    if (m_InfluenceMap.size() == 0)
    {
        qint32 width = m_pMap->getMapWidth();
        qint32 heigth = m_pMap->getMapHeight();
        for (qint32 x = 0; x < width; x++)
        {
            m_InfluenceMap.push_back(std::vector<InfluenceInfo>(heigth));
        }
    }
    
    for (qint32 x = 0; x < m_InfluenceMap.size(); ++x)
    {
        for (qint32 y = 0; y < m_InfluenceMap[x].size(); ++y)
        {
            m_InfluenceMap[x][y].reset(m_pMap);
        }
    }
    m_totalHighestInfluence = 0;
}

void InfluenceFrontMap::addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs, qint32 movePoints)
{
    if (pUnit->hasWeapons() || pUnit->getLoadedUnitCount() > 0)
    {
        QCoreApplication::processEvents();
        qint32 value = pUnit->getCoUnitValue();
        qint32 owner = pUnit->getOwner()->getPlayerID();
        auto points = pPfs->getAllNodePointsFast();
        for (const auto & point : points)
        {
            float divider = 1.0f;
            float fieldCost = pPfs->getTargetCosts(point.x(), point.y());
            if (movePoints > 0.0f && fieldCost > 0.0f && fieldCost > movePoints)
            {                
                divider = fieldCost / movePoints + 1.0f;
            }
            m_InfluenceMap[point.x()][point.y()].increaseInfluence(owner, value / divider);
        }
    }
}

void InfluenceFrontMap::updateOwners()
{    
    qint32 width = m_pMap->getMapWidth();
    qint32 heigth = m_pMap->getMapHeight();
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            m_InfluenceMap[x][y].updateOwner(m_pOwner);
        }
    }
}

void InfluenceFrontMap::show()
{
    for (qint32 x = 0; x < m_InfluenceMap.size(); ++x)
    {
        for (qint32 y = 0; y < m_InfluenceMap[x].size(); ++y)
        {
            if (m_InfluenceMap[x][y].owners.size() == 1)
            {
                qint32 owner = m_InfluenceMap[x][y].owners[0];
                oxygine::spColorRectSprite sprite = oxygine::spColorRectSprite::create();
                sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
                QColor color = m_pMap->getPlayer(owner)->getColor();
                color.setAlphaF(0.5f);
                sprite->setColor(color);
                sprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
                sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
                m_pMap->addChild(sprite);
                m_info.push_back(sprite);
            }
        }
    }
}

void InfluenceFrontMap::showPfs(UnitPathFindingSystem* pPfs)
{
    
    for (qint32 x = 0; x < m_InfluenceMap.size(); ++x)
    {
        for (qint32 y = 0; y < m_InfluenceMap[x].size(); ++y)
        {
            qint32 cost = pPfs->getTargetCosts(x, y);
            if (cost >= 0)
            {
                oxygine::spColorRectSprite sprite = oxygine::spColorRectSprite::create();
                sprite->setSize(GameMap::getImageSize(), GameMap::getImageSize());
                QColor color = m_pMap->getCurrentPlayer()->getColor();
                if (cost > 0)
                {
                    color.setAlphaF(0.5f);
                }
                else
                {
                    color.setAlphaF(0.2f);
                }
                sprite->setColor(color);
                sprite->setPosition(x * GameMap::getImageSize(), y * GameMap::getImageSize());
                sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
                m_pMap->addChild(sprite);
                m_info.push_back(sprite);
            }
        }
    }
}

void InfluenceFrontMap::showFrontlines()
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    
    qint32 count = 0;
    for (auto & frontline : m_frontLines)
    {
        ++count;
        for (auto & field : frontline)
        {
            auto & info = m_InfluenceMap[field.x()][field.y()];
            qint32 size = GameMap::getImageSize();
            if (info.frontOwners.size() > 0)
            {
                size /= info.frontOwners.size();
            }
            for (qint32 i = 0; i < info.frontOwners.size(); ++i)
            {
                qint32 owner = info.frontOwners[i];
                oxygine::spColorRectSprite sprite = oxygine::spColorRectSprite::create();
                sprite->setSize(size, GameMap::getImageSize());
                QColor color = m_pMap->getPlayer(owner)->getColor();
                color.setAlphaF(0.5f);
                sprite->setColor(color);
                sprite->setPosition(field.x() * GameMap::getImageSize() + size * i, field.y() * GameMap::getImageSize());
                sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
                m_pMap->addChild(sprite);
                m_info.push_back(sprite);
            }
            spLabel pTextfield = spLabel::create(GameMap::getImageSize());
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(QString::number(count));
            pTextfield->setPosition(field.x() * GameMap::getImageSize(), field.y() * GameMap::getImageSize());
            pTextfield->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
            m_pMap->addChild(pTextfield);
            m_info.push_back(pTextfield);
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

void InfluenceFrontMap::calculateGlobalData()
{
    AI_CONSOLE_PRINT("InfluenceFrontMap::calculateGlobalData()", GameConsole::eDEBUG);
    findFrontLineTiles();
    createFrontLine();
    updateHighestInfluence();
}

void InfluenceFrontMap::findFrontLineTiles()
{
    AI_CONSOLE_PRINT("InfluenceFrontMap::findFrontLineTiles()", GameConsole::eDEBUG);
    spQmlVectorPoint circle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
    
    qint32 width = m_pMap->getMapWidth();
    qint32 heigth = m_pMap->getMapHeight();
    for (qint32 x = 0; x < width; ++x)
    {
        for (qint32 y = 0; y < heigth; ++y)
        {
            auto & info = m_InfluenceMap[x][y];
            auto & owners = info.owners;
            qint32 ownerSize = owners.size();
            if (ownerSize > 0)
            {
                for (qint8 neighbours = 0 ; neighbours < circle->size(); ++neighbours)
                {
                    qint32 neighbourX = x + circle->at(neighbours).x();
                    qint32 neighbourY = y + circle->at(neighbours).y();
                    if (m_pMap->onMap(neighbourX, neighbourY))
                    {
                        auto & info2 = m_InfluenceMap[neighbourX][neighbourY];
                        qint32 owner2Size = info2.owners.size();
                        if (owner2Size > 0)
                        {
                            if (ownerSize > 1)
                            {
                                for (auto & owner : owners)
                                {
                                    if (!GlobalUtils::contains(info.frontOwners, owner))
                                    {
                                        info.frontOwners.push_back(owner);
                                    }
                                }
                                addFrontLineMoveTypes(info, x, y, neighbourX, neighbourY);
                            }
                            else if (owners[0] != info2.owners[0] && owner2Size == 1)
                            {
                                if (!GlobalUtils::contains(info.frontOwners, owners[0]))
                                {
                                    info.frontOwners.push_back(owners[0]);
                                }
                                if (!GlobalUtils::contains(info.frontOwners, info2.owners[0]))
                                {
                                    info.frontOwners.push_back(info2.owners[0]);
                                }
                                addFrontLineMoveTypes(info, x, y, neighbourX, neighbourY);
                            }
                        }
                    }
                }
            }
        }
    }
}

void InfluenceFrontMap::addFrontLineMoveTypes(InfluenceInfo & info, qint32 x1, qint32 y1, qint32 x2, qint32 y2)
{
    for (auto & island : m_islands)
    {
        if (island->sameIsland(x1, y1, x2, y2))
        {
            if (!info.frontMovetype.contains((island->getMovementType())))
            {
                info.frontMovetype.append(island->getMovementType());
            }
        }
    }
    info.frontMovetype.sort();
}

void InfluenceFrontMap::createFrontLine()
{
    AI_CONSOLE_PRINT("InfluenceFrontMap::createFrontLine()", GameConsole::eDEBUG);
    spQmlVectorPoint circle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));
    
    qint32 width = m_pMap->getMapWidth();
    qint32 heigth = m_pMap->getMapHeight();
    for (qint32 x = 0; x < width; ++x)
    {
        for (qint32 y = 0; y < heigth; ++y)
        {
            auto & info = m_InfluenceMap[x][y];
            if (info.frontMovetype.size() > 0 &&
                info.frontLineCreated == false)
            {
                std::vector<QPoint> frontline;
                frontline.push_back(QPoint(x, y));
                info.frontLineCreated = true;
                searchFrontLine(circle.get(), info, x, y, frontline);
                m_frontLines.push_back(frontline);
            }
        }
    }
}

void InfluenceFrontMap::searchFrontLine(QmlVectorPoint* neighbours, InfluenceInfo & info, qint32 x, qint32 y, std::vector<QPoint> & frontline)
{    
    for (qint32 i = 0; i < neighbours->size(); ++i)
    {
        auto neighbour = neighbours->at(i);
        qint32 newX = x + neighbour.x();
        qint32 newY = y + neighbour.y();
        if (m_pMap->onMap(newX, newY))
        {
            auto & info2 = m_InfluenceMap[newX][newY];
            if (info2.frontLineCreated == false &&
                info2.frontMovetype == info.frontMovetype)
            {
                frontline.push_back(QPoint(newX, newY));
                info2.frontLineCreated = true;
                searchFrontLine(neighbours, info, newX, newY, frontline);
            }
        }
    }
}

void InfluenceFrontMap::updateHighestInfluence()
{
    AI_CONSOLE_PRINT("InfluenceFrontMap::updateHighestInfluence()", GameConsole::eDEBUG);
    spQmlVectorPoint circle = spQmlVectorPoint(GlobalUtils::getCircle(1, 1));    
    qint32 width = m_pMap->getMapWidth();
    qint32 heigth = m_pMap->getMapHeight();
    for (qint32 x = 0; x < width; ++x)
    {
        for (qint32 y = 0; y < heigth; ++y)
        {
            auto & info = m_InfluenceMap[x][y];
            if (info.highestInfluence > m_totalHighestInfluence)
            {
                m_totalHighestInfluence = info.highestInfluence;
            }
        }
    }
}

Player *InfluenceFrontMap::getOwner() const
{
    return m_pOwner;
}

void InfluenceFrontMap::setOwner(Player *newPOwner)
{
    m_pOwner = newPOwner;
}

qint32 InfluenceFrontMap::getTotalHighestInfluence() const
{
    return m_totalHighestInfluence;
}
