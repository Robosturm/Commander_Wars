#include "ai/influencefrontmap.h"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"
#include "coreengine/console.h"
#include "ai/coreai.h"

#include "game/gamemap.h"
#include "game/unitpathfindingsystem.h"
#include "resource_management/unitspritemanager.h"
#include "resource_management/fontmanager.h"


InfluenceFrontMap::InfluenceFrontMap(const QVector<spIslandMap> & islands)
    : m_islands(islands)
{
    setObjectName("InfluenceFrontMap");
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

void InfluenceFrontMap::addBuildingInfluence()
{
    Console::print("InfluenceFrontMap::addBuildingInfluence()", Console::eDEBUG);
    QVector<QPoint> buildingPositions;
    QVector<qint32> buildingOwners;
    QVector<QStringList> buildLists;
    QMap<QString, qint32> unitIdToIsland;
    spGameMap pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    qint32 maxScore = width * heigth;
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            Building* pBuilding = pMap->getTerrain(x, y)->getBuilding();
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
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            QPoint curPos(x, y);
            for (qint32 building = 0; building < buildingPositions.size(); building++)
            {
                QPoint pos = buildingPositions[building];
                for (qint32 unitIdx = 0; unitIdx < buildLists[building].size(); unitIdx++)
                {
                    QString unitId = buildLists[building][unitIdx];
                    qint32 island = getIslandFromUnitId(unitId, unitIdToIsland);
                    if (island >= 0 && m_islands[island]->sameIsland(x, y, pos.x(), pos.y()))
                    {
                        qint32 dis = GlobalUtils::getDistance(curPos, pos);
                        if (dis > 0)
                        {
                            m_InfluenceMap[x][y].playerValues[buildingOwners[building]] += maxScore / dis;
                        }
                        else
                        {
                            m_InfluenceMap[x][y].playerValues[buildingOwners[building]] += maxScore;
                        }
                    }
                }
            }
        }
    }
}

qint32 InfluenceFrontMap::getIslandFromUnitId(QString unitId, QMap<QString, qint32> & unitIdToIsland)
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
{
    qint32 playerCount = pMap->getPlayerCount();
    for (qint32 x = 0; x < playerCount; x++)
    {
        playerValues.append(0);
    }
}

void InfluenceFrontMap::InfluenceInfo::updateOwner()
{
    owners.clear();
    qint32 highestValue = 0;
    for (qint32 player = 0; player < playerValues.size(); ++player)
    {
        if (playerValues[player] > highestValue)
        {
            highestValue = playerValues[player];
            owners.clear();
            owners.append(player);
        }
        else if (playerValues[player] > 0 &&
                 playerValues[player] == highestValue)
        {
            owners.append(player);
        }
    }
    highestInfluence = highestValue;
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

void InfluenceFrontMap::reset()
{
    Console::print("InfluenceFrontMap::reset()", Console::eDEBUG);
    hide();
    spGameMap pMap = GameMap::getInstance();
    for (qint32 x = 0; x < m_InfluenceMap.size(); ++x)
    {
        for (qint32 y = 0; y < m_InfluenceMap[x].size(); ++y)
        {
            m_InfluenceMap[x][y].reset();
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

void InfluenceFrontMap::updateOwners()
{
    spGameMap pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
    for (qint32 x = 0; x < width; x++)
    {
        for (qint32 y = 0; y < heigth; y++)
        {
            m_InfluenceMap[x][y].updateOwner();
        }
    }
}

void InfluenceFrontMap::show()
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 x = 0; x < m_InfluenceMap.size(); ++x)
    {
        for (qint32 y = 0; y < m_InfluenceMap[x].size(); ++y)
        {
            if (m_InfluenceMap[x][y].owners.size() == 1)
            {
                qint32 owner = m_InfluenceMap[x][y].owners[0];
                oxygine::spColorRectSprite sprite = oxygine::spColorRectSprite::create();
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

void InfluenceFrontMap::showFrontlines()
{
    oxygine::TextStyle style = FontManager::getMainFont24();
    style.color = FontManager::getFontColor();
    style.vAlign = oxygine::TextStyle::VALIGN_DEFAULT;
    style.hAlign = oxygine::TextStyle::HALIGN_LEFT;
    spGameMap pMap = GameMap::getInstance();
    qint32 count = 0;
    for (auto & frontline : m_frontLines)
    {
        ++count;
        for (auto & field : frontline)
        {
            auto & info = m_InfluenceMap[field.x()][field.y()];
            qint32 size = GameMap::getImageSize() / info.frontOwners.size();
            for (qint32 i = 0; i < info.frontOwners.size(); ++i)
            {
                qint32 owner = info.frontOwners[i];
                oxygine::spColorRectSprite sprite = oxygine::spColorRectSprite::create();
                sprite->setSize(size, GameMap::getImageSize());
                QColor color = pMap->getPlayer(owner)->getColor();
                color.setAlphaF(0.5f);
                sprite->setColor(color);
                sprite->setPosition(field.x() * GameMap::getImageSize() + size * i, field.y() * GameMap::getImageSize());
                sprite->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
                pMap->addChild(sprite);
                m_info.append(sprite);
            }
            oxygine::spTextField pTextfield = oxygine::spTextField::create();
            pTextfield->setStyle(style);
            pTextfield->setHtmlText(QString::number(count));
            pTextfield->setPosition(field.x() * GameMap::getImageSize(), field.y() * GameMap::getImageSize());
            pTextfield->setPriority(static_cast<qint32>(Mainapp::ZOrder::MarkedFields));
            pMap->addChild(pTextfield);
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

void InfluenceFrontMap::findFrontLines()
{
    Console::print("InfluenceFrontMap::findFrontLines()", Console::eDEBUG);
    findFrontLineTiles();
    createFrontLine();
}

void InfluenceFrontMap::findFrontLineTiles()
{
    Console::print("InfluenceFrontMap::findFrontLineTiles()", Console::eDEBUG);
    spQmlVectorPoint circle = GlobalUtils::getCircle(1, 1);
    spGameMap pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
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
                    if (pMap->onMap(neighbourX, neighbourY))
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
    Console::print("InfluenceFrontMap::createFrontLine()", Console::eDEBUG);
    spQmlVectorPoint circle = GlobalUtils::getCircle(1, 1);
    spGameMap pMap = GameMap::getInstance();
    qint32 width = pMap->getMapWidth();
    qint32 heigth = pMap->getMapHeight();
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
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < neighbours->size(); ++i)
    {
        auto neighbour = neighbours->at(i);
        qint32 newX = x + neighbour.x();
        qint32 newY = y + neighbour.y();
        if (pMap->onMap(newX, newY))
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
