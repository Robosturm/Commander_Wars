#include <QApplication>

#include "ai/influencefrontmap.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/console.h"
#include "ai/coreai.h"

#include "game/gamemap.h"
#include "game/unitpathfindingsystem.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/fontmanager.h"

#include "objects/base/label.h"

InfluenceFrontMap::InfluenceFrontMap(GameMap* pMap, const QVector<spIslandMap> & islands)
    : m_islands(islands),
      m_pMap(pMap)
{
    setObjectName("InfluenceFrontMap");
    Mainapp* pApp = Mainapp::getInstance();
    moveToThread(pApp->getWorkerthread());
    Interpreter::setCppOwnerShip(this);
    
    qint32 width = m_pMap->getMapWidth();
    qint32 heigth = m_pMap->getMapHeight();
    for (qint32 x = 0; x < width; x++)
    {
        m_InfluenceMap.append(QVector<InfluenceInfo>(heigth, InfluenceInfo(m_pMap)));
    }
}

void InfluenceFrontMap::addBuildingInfluence()
{
    CONSOLE_PRINT("InfluenceFrontMap::addBuildingInfluence()", Console::eDEBUG);
    QVector<QPoint> buildingPositions;
    QVector<qint32> buildingOwners;
    QVector<QStringList> buildLists;
    QMap<QString, qint32> unitIdToIsland;
    
    QVector<qint32> income;
    for (qint32 i = 0; i < m_pMap->getPlayerCount(); ++i)
    {
        income.append(m_pMap->getPlayer(i)->calcIncome());
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
                    buildingPositions.append(QPoint(x, y));
                    buildingOwners.append(pBuilding->getOwner()->getPlayerID());
                    buildLists.append(pBuilding->getConstructionList());
                }
            }
        }
    }
    qint32 fullInfluenceRange = 6;
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            QPoint curPos(x, y);
            for (qint32 building = 0; building < buildingPositions.size(); building++)
            {
                QApplication::processEvents();
                QPoint pos = buildingPositions[building];
                for (qint32 unitIdx = 0; unitIdx < buildLists[building].size(); unitIdx++)
                {
                    QString unitId = buildLists[building][unitIdx];
                    qint32 island = getIslandFromUnitId(unitId, unitIdToIsland);

                    if (island >= 0 && m_islands[island]->sameIsland(x, y, pos.x(), pos.y()))
                    {
                        qint32 dis = GlobalUtils::getDistance(curPos, pos);
                        if (dis > fullInfluenceRange)
                        {
                            qint32 dayDivider = fullInfluenceRange / dis + 1;
                            m_InfluenceMap[x][y].increaseInfluence(buildingOwners[building], income[buildingOwners[building]] / dayDivider / buildLists[building].size());
                        }
                        else
                        {
                            m_InfluenceMap[x][y].increaseInfluence(buildingOwners[building], income[buildingOwners[building]] / buildLists[building].size());
                        }
                    }
                }
            }
        }
    }
}

qint32 InfluenceFrontMap::getIslandFromUnitId(const QString & unitId, QMap<QString, qint32> & unitIdToIsland)
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
        unitIdToIsland.insert(unitId, island);
    }
    return island;
}

InfluenceFrontMap::InfluenceInfo::InfluenceInfo(GameMap* pMap)
    : m_pMap(pMap)
{
    qint32 playerCount = pMap->getPlayerCount();
    for (qint32 x = 0; x < playerCount; x++)
    {
        playerValues.append(0);
    }
}

void InfluenceFrontMap::InfluenceInfo::updateOwner(Player* pOwner)
{
    owners.clear();
    qint32 highestValue = 0;
    enemyInfluence = 0;
    ownInfluence = 0;
    qint32 playerId = pOwner->getPlayerID();
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
            owners.append(player);
        }
        else if (influence > 0 &&
                 influence == highestValue)
        {
            owners.append(player);
        }
    }
    highestInfluence = highestValue;
}

qint32 InfluenceFrontMap::InfluenceInfo::getPlayerInfluence(qint32 playerId)
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

void InfluenceFrontMap::InfluenceInfo::reset()
{
    for (auto & value : playerValues)
    {
        value = 0;
    }
    highestInfluence = 0;
    owners.clear();
    frontMovetype.clear();
    frontOwners.clear();
    frontLineCreated = false;
}

void InfluenceFrontMap::InfluenceInfo::increaseInfluence(qint32 player, qint32 value)
{
    playerValues[player] += value;
}

void InfluenceFrontMap::reset()
{
    CONSOLE_PRINT("InfluenceFrontMap::reset()", Console::eDEBUG);
    hide();
    
    for (qint32 x = 0; x < m_InfluenceMap.size(); ++x)
    {
        for (qint32 y = 0; y < m_InfluenceMap[x].size(); ++y)
        {
            m_InfluenceMap[x][y].reset();
        }
    }
    m_totalHighestInfluence = 0;
}

void InfluenceFrontMap::addUnitInfluence(Unit* pUnit, UnitPathFindingSystem* pPfs, qint32 movePoints)
{
    if (pUnit->hasWeapons() || pUnit->getLoadedUnitCount() > 0)
    {
        QApplication::processEvents();
        qint32 value = pUnit->getCoUnitValue();
        qint32 owner = pUnit->getOwner()->getPlayerID();
        auto points = pPfs->getAllNodePoints();
        for (const auto & point : points)
        {
            float multiplier = 1.0f;
            qint32 fieldCost = pPfs->getTargetCosts(point.x(), point.y());
            if (movePoints > 0 && fieldCost > 0 && fieldCost > movePoints)
            {                
                multiplier = movePoints / fieldCost + 1;
            }
            m_InfluenceMap[point.x()][point.y()].increaseInfluence(owner, value * multiplier);
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
                m_info.append(sprite);
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
                m_info.append(sprite);
            }
        }
    }
}

void InfluenceFrontMap::showFrontlines()
{
    oxygine::TextStyle style = oxygine::TextStyle(FontManager::getMainFont24());
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
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
                m_info.append(sprite);
            }
            spLabel pTextfield = spLabel::create(GameMap::getImageSize());
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(QString::number(count));
            pTextfield->setPosition(field.x() * GameMap::getImageSize(), field.y() * GameMap::getImageSize());
            pTextfield->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
            m_pMap->addChild(pTextfield);
            m_info.append(pTextfield);
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
    CONSOLE_PRINT("InfluenceFrontMap::calculateGlobalData()", Console::eDEBUG);
    findFrontLineTiles();
    createFrontLine();
    updateHighestInfluence();
}

void InfluenceFrontMap::findFrontLineTiles()
{
    CONSOLE_PRINT("InfluenceFrontMap::findFrontLineTiles()", Console::eDEBUG);
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
                                    if (!info.frontOwners.contains(owner))
                                    {
                                        info.frontOwners.append(owner);
                                    }
                                }
                                addFrontLineMoveTypes(info, x, y, neighbourX, neighbourY);
                            }
                            else if (owners[0] != info2.owners[0] && owner2Size == 1)
                            {
                                if (!info.frontOwners.contains(owners[0]))
                                {
                                    info.frontOwners.append(owners[0]);
                                }
                                if (!info.frontOwners.contains(info2.owners[0]))
                                {
                                    info.frontOwners.append(info2.owners[0]);
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
    CONSOLE_PRINT("InfluenceFrontMap::createFrontLine()", Console::eDEBUG);
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
                QVector<QPoint> frontline;
                frontline.append(QPoint(x, y));
                info.frontLineCreated = true;
                searchFrontLine(circle.get(), info, x, y, frontline);
                m_frontLines.append(frontline);
            }
        }
    }
}

void InfluenceFrontMap::searchFrontLine(QmlVectorPoint* neighbours, InfluenceInfo & info, qint32 x, qint32 y, QVector<QPoint> & frontline)
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
                frontline.append(QPoint(newX, newY));
                info2.frontLineCreated = true;
                searchFrontLine(neighbours, info, newX, newY, frontline);
            }
        }
    }
}

void InfluenceFrontMap::updateHighestInfluence()
{
    CONSOLE_PRINT("InfluenceFrontMap::updateHighestInfluence()", Console::eDEBUG);
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
