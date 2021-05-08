#include <limits>
#include "QRandomGenerator"
#include "QVector"
#include "QPoint"

#include "coreengine/mainapp.h"
#include "coreengine/globalutils.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/building.h"
#include "game/unit.h"
#include "game/terrain.h"
#include "game/co.h"
#include "game/unitpathfindingsystem.h"

#include "resource_management/terrainmanager.h"

#include "objects/loadingscreen.h"

const char* const RANDOMMAPGENERATORNAME = "RANDOMMAPGENERATOR";
qint32 GameMap::randomMap(qint32 width, qint32 heigth, qint32 playerCount,
                          bool roadSupport, qint32 seed,
                          QVector<std::tuple<QString, float>> terrains,
                          QVector<std::tuple<QString, float>> buildings,
                          QVector<float> ownedBaseSize,
                          float startBaseSize,
                          QVector<std::tuple<QString, float>> units,
                          qint32 unitCount,
                          float startBaseUnitSize,
                          QVector<float> unitDistribution,
                          bool unitsDistributed)
{
    clearMap();

    LoadingScreen* pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->show();
    qint32 maxSteps = terrains.size() + buildings.size() + units.size() + 1;
    qint32 progress = 0;

    qint32 startSeed = seed;
    if (seed < 0)
    {
        startSeed = GlobalUtils::randInt(0, std::numeric_limits<qint32>::max());

    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QString baseTerrain = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "getBaseTerrainID").toString();
    if (!TerrainManager::getInstance()->exists(baseTerrain))
    {
        baseTerrain = "PLAINS";
    }
    // seed map generator
    QRandomGenerator randInt(static_cast<quint32>(startSeed));
    newMap(width, heigth, playerCount, baseTerrain);
    float buildingchance = 0.0f;
    for (qint32 i = 0; i < terrains.size(); i++)
    {

        QString terrainID = std::get<0>(terrains[i]);
        pLoadingScreen->setProgress(tr("Generating ") + terrainID, progress * 100 / maxSteps);
        progress++;
        if (terrainID == "Buildings")
        {
            buildingchance = std::get<1>(terrains[i]);
        }
        else
        {
            float terrainChance = std::get<1>(terrains[i]);
            QStringList list = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "get" + terrainID + "TopTerrainIDs").toVariant().toStringList();
            QList<QVariant> chances = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "get" + terrainID + "TopTerrainChances").toVariant().toList();
            QJSValue distribution = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "get" + terrainID + "Distribution");
            QJSValue terrainType = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "get" + terrainID + "CreateType");
            if (list.size() == chances.size())
            {
                randomMapPlaceTerain(terrainID, width, heigth, terrainChance / 100.0f,
                                     distribution.toVariant().toPoint(), list, chances,
                                     static_cast<GameEnums::RandomMapTerrainType>(terrainType.toInt()), randInt);
            }
        }
    }
    
    qint32 fieldChance = static_cast<qint32>(width * heigth * buildingchance / 100.0f);
    qint32 minBuildings = playerCount * 2;
    if (fieldChance < minBuildings)
    {
        fieldChance = minBuildings;
    }
    // place buildings
    QVector<QPoint> basePoints = randomMapCreateBuildings(fieldChance, randInt, buildings, ownedBaseSize,
                                                          startBaseSize, progress, maxSteps);

    if (roadSupport)
    {
        pLoadingScreen->setProgress(tr("Generating ") + "Roads", (progress) * 100 / (maxSteps));
        ++progress;
        randomMapCreateRoad(randInt, basePoints);
    }
    randomMapPlaceUnits(units, unitCount, startBaseUnitSize, unitDistribution, basePoints, unitsDistributed, progress, maxSteps, randInt);

    pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "customStep");

    updateSprites();
    centerMap(width / 2, heigth / 2);

    pLoadingScreen->hide();
    return startSeed;
}

void GameMap::randomMapPlaceTerain(QString terrainID, qint32 width, qint32 heigth, float placeChance, QPoint placeSize, QStringList topTerrainIDs, QList<QVariant> placeChances, GameEnums::RandomMapTerrainType type, QRandomGenerator& randInt)
{
    qint32 maximumTry = 1000;
    if ((width * heigth * placeChance < 1.0f) && (placeChance > 0.0f))
    {
        randomMapPlaceGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 1, terrainID, 1, randInt);
    }
    else
    {
        qint32 baseChance = static_cast<qint32>(width * heigth * placeChance);
        qint32 chance = static_cast<qint32>(width * heigth * placeChance);
        while (chance > 0 && maximumTry > 0)
        {
            qint32 divider = 1;
            if (placeSize.x() < placeSize.y())
            {
                divider = randInt.bounded(placeSize.x(), placeSize.y());
            }
            else if (placeSize.x() == placeSize.y())
            {
                divider = placeSize.x();
            }
            qint32 groupSize = baseChance;
            if (groupSize < 10)
            {
                groupSize = 10;
            }
            qint32 high = GlobalUtils::roundUp(groupSize / divider);
            if (high > 1)
            {
                groupSize = randInt.bounded(1, high);
            }
            else
            {
                groupSize = 1;
            }
            switch (type)
            {
                case GameEnums::RandomMapTerrainType_Line:
                {
                    chance -= randomMapPlaceLine(randInt.bounded(0, width), randInt.bounded(0, heigth), groupSize, terrainID, randInt);
                    break;
                }
                case GameEnums::RandomMapTerrainType_Group:
                {
                    chance -= randomMapPlaceGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), groupSize, terrainID, 1, randInt);
                    break;
                }
            }
            maximumTry--;
        }
    }
    for (qint32 i = 0; i < topTerrainIDs.size(); i++)
    {
        randomMapPlaceOnTop(terrainID, topTerrainIDs[i], placeChances[i].toFloat(), randInt);
    }
}

qint32 GameMap::randomMapPlaceLine(qint32 startX, qint32 startY, qint32 count, QString terrainID, QRandomGenerator& randInt)
{
    QVector<QPoint> points;
    QVector<GameEnums::Directions> directions;
    points.push_back(QPoint(startX, startY));
    directions.append(GameEnums::Directions_None);
    qint32 placed = 0;
    qint32 maximumTries = 1000;
    qint32 maximumTry = maximumTries;
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "get" + terrainID + "StraigthLineChance");
    qint32 lineChance = 4;
    if (erg.isNumber())
    {
        lineChance = erg.toInt();
    }
    while (count > 0 && maximumTry > 0)
    {
        // we need to place terrains
        qint32 pInd = randInt.bounded(0, points.size());
        qint32 x = points[pInd].x();
        qint32 y = points[pInd].y();
        GameEnums::Directions direction = directions[pInd];
        if (terrainID != getTerrain(x, y)->getTerrainID() && randomMapTerrainPlaceable(x, y, terrainID))
        {
            maximumTry = maximumTries;
            placed++;
            points.clear();
            directions.clear();
            replaceTerrain(terrainID, x, y, false, false);
            switch (direction)
            {
                case GameEnums::Directions_East:
                {
                    if (randomMapAddTerrainPoint(points, x, y + 1, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_South);
                    }
                    if (randomMapAddTerrainPoint(points, x, y - 1, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_North);
                    }
                    if (randomMapAddTerrainPoint(points, x + 1, y, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_East);
                        for (qint32 i = 0; i < lineChance - 1; ++i)
                        {
                            points.append(QPoint(x + 1, y));
                            directions.append(GameEnums::Directions_East);
                        }
                    }
                    break;
                }
                case GameEnums::Directions_West:
                {
                    if (randomMapAddTerrainPoint(points, x, y + 1, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_South);
                    }
                    if (randomMapAddTerrainPoint(points, x, y - 1, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_North);
                    }
                    if (randomMapAddTerrainPoint(points, x - 1, y, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_West);
                        for (qint32 i = 0; i < lineChance - 1; ++i)
                        {
                            points.append(QPoint(x - 1, y));
                            directions.append(GameEnums::Directions_West);
                        }
                    }
                    break;
                }
                case GameEnums::Directions_North:
                {
                    if (randomMapAddTerrainPoint(points, x + 1, y, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_East);
                    }
                    if (randomMapAddTerrainPoint(points, x - 1, y, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_West);
                    }
                    if (randomMapAddTerrainPoint(points, x, y - 1, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_North);
                        for (qint32 i = 0; i < lineChance - 1; ++i)
                        {
                            points.append(QPoint(x, y - 1));
                            directions.append(GameEnums::Directions_North);
                        }
                    }
                    break;
                }
                case GameEnums::Directions_South:
                {
                    if (randomMapAddTerrainPoint(points, x + 1, y, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_East);
                    }
                    if (randomMapAddTerrainPoint(points, x - 1, y, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_West);
                    }
                    if (randomMapAddTerrainPoint(points, x, y + 1, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_South);
                        for (qint32 i = 0; i < lineChance - 1; ++i)
                        {
                            points.append(QPoint(x, y + 1));
                            directions.append(GameEnums::Directions_South);
                        }
                    }
                    break;
                }
                default:
                {
                    if (randomMapAddTerrainPoint(points, x + 1, y, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_East);
                    }
                    if (randomMapAddTerrainPoint(points, x - 1, y, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_West);
                    }
                    if (randomMapAddTerrainPoint(points, x, y - 1, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_North);
                    }
                    if (randomMapAddTerrainPoint(points, x, y + 1, terrainID, 0))
                    {
                        directions.append(GameEnums::Directions_South);
                    }
                }
            }
            count--;
        }
        else
        {
            points.removeAt(pInd);
            directions.removeAt(pInd);
        }
        if (points.size() <= 0)
        {
            break; // no free fields avaible
        }
        maximumTry--;
    }
    return placed;
}

qint32 GameMap::randomMapPlaceGroup(qint32 startX, qint32 startY, qint32 count, QString terrainID, qint32 terrainRadius, QRandomGenerator& randInt)
{
    QVector<QPoint> points;
    points.push_back(QPoint(startX, startY));
    qint32 placed = 0;
    qint32 maximumTries = 1000;
    qint32 maximumTry = maximumTries;
    while (count > 0 && maximumTry > 0)
    {
        // we need to place terrains
        qint32 pInd = randInt.bounded(0, points.size());
        if (terrainID != getTerrain(points[pInd].x(), points[pInd].y())->getTerrainID() &&
            randomMapTerrainPlaceable(points[pInd].x(), points[pInd].y(), terrainID))
        {
            maximumTry = maximumTries;
            placed++;
            replaceTerrain(terrainID, points[pInd].x(), points[pInd].y(), false, false);
            randomMapAddTerrainPoint(points, points[pInd].x() + 1, points[pInd].y(), terrainID, terrainRadius - 1);
            randomMapAddTerrainPoint(points, points[pInd].x() - 1, points[pInd].y(), terrainID, terrainRadius - 1);
            randomMapAddTerrainPoint(points, points[pInd].x(), points[pInd].y() + 1, terrainID, terrainRadius - 1);
            randomMapAddTerrainPoint(points, points[pInd].x(), points[pInd].y() - 1, terrainID, terrainRadius - 1);
            count--;
        }
        points.removeAt(pInd);
        maximumTry--;
        if (points.size() <= 0)
        {
            break; // no free fields avaible
        }
    }
    return placed;
}

bool GameMap::randomMapTerrainPlaceable(qint32 x, qint32 y, QString terrainID)
{
    if (!canBePlaced(terrainID, x, y))
    {
        return false;
    }
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValueList args;
    args << x;
    args << y;
    QJSValue erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "get" + terrainID + "Placeable", args);
    if (erg.isBool())
    {
        return erg.toBool();
    }
    return false;
}

bool GameMap::randomMapAddTerrainPoint(QVector<QPoint>& points, qint32 x, qint32 y, QString terrainID, qint32 terrainRadius)
{
    if (x < 0)
    {
        return false;
    }
    if (y < 0)
    {
        return false;
    }
    if (x >= getMapWidth())
    {
        return false;
    }
    if (y >= getMapHeight())
    {
        return false;
    }
    for (qint32 i = 0; i < points.size(); i++)
    {
        if ((points[i].x() == x) && (points[i].y() == y))
        {
            return false;
        }
    }

    Terrain* pTerrain = getTerrain(x, y);
    if (pTerrain->getTerrainID() == terrainID)
    {
        return false;
    }
    if (!randomMapTerrainPlaceable(x, y, terrainID))
    {
        return false;
    }

    points.push_back(QPoint(x, y));
    if (terrainRadius > 0)
    {
        randomMapAddTerrainPoint(points, x + 1, y, terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, x - 1, y, terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, x, y + 1, terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, x, y - 1, terrainID, terrainRadius - 1);
    }
    return true;
}

void GameMap::randomMapPlaceOnTop(QString terrainID, QString topId, float chance, QRandomGenerator& randInt)
{
    QVector<QPoint> reafOptions;
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            if (getTerrain(x, y)->getTerrainID() == terrainID)
            {
                if (canBePlaced(topId, x, y))
                {
                    reafOptions.push_back(QPoint(x, y));
                }
            }
        }
    }
    for (qint32 i = 0; i < reafOptions.size(); i++)
    {
        if (randInt.bounded(0, 100) / 100.0f <= chance)
        {
            replaceTerrain(topId, reafOptions[i].x(), reafOptions[i].y());
        }
    }
}

void GameMap::randomMapCreateRoad(QRandomGenerator& randInt, QVector<QPoint>& playerPositions)
{
    for (qint32 i = 0; i < m_players.size(); i++)
    {
        QPoint currentPoint = playerPositions[i];
        QPoint endPoint;
        if (i == m_players.size() - 1)
        {
            endPoint = playerPositions[0];
        }
        else
        {
            endPoint = playerPositions[i + 1];
        }
        while (currentPoint != endPoint)
        {
            qint32 xDistance = endPoint.x() - currentPoint.x();
            qint32 yDistance = endPoint.y() - currentPoint.y();
            qint32 erg = -1;
            if (xDistance != 0 && yDistance != 0)
            {
                erg = randInt.bounded(0, qAbs(xDistance) + qAbs(yDistance));
            }
            if ((xDistance != 0 && yDistance == 0) || (erg >= 0 && erg < qAbs(xDistance)))
            {
                if (xDistance > 0)
                {
                    currentPoint.setX(currentPoint.x() + 1);
                }
                else
                {
                    currentPoint.setX(currentPoint.x() - 1);
                }
            }
            else
            {
                if (yDistance > 0)
                {
                    currentPoint.setY(currentPoint.y() + 1);
                }
                else
                {
                    currentPoint.setY(currentPoint.y() - 1);
                }
            }
            Terrain* pTerrain = getTerrain(currentPoint.x(), currentPoint.y());
            Interpreter* pInterpreter = Interpreter::getInstance();
            if (pTerrain->getBuilding() == nullptr)
            {
                QJSValueList args;
                QJSValue obj = pInterpreter->newQObject(pTerrain);
                args << obj;
                QStringList ret = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "getRoadCreation", args).toVariant().toStringList();
                if (ret.size() >= 2)
                {
                    if (!ret[0].isEmpty())
                    {
                        replaceTerrain(ret[0], currentPoint.x(), currentPoint.y());
                    }
                    replaceTerrain(ret[1], currentPoint.x(), currentPoint.y(), true);
                }
            }
        }
    }
}

QVector<QPoint> GameMap::randomMapCreateBuildings(qint32 buildings, QRandomGenerator& randInt, QVector<std::tuple<QString, float>> buildingDistributions, QVector<float> ownedBaseSize,
                                                  float startBaseSize, qint32& progress, qint32 maxProgess)
{
    QVector<QPoint> playerPositions;
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    qint32 maximumBuildingTry = 3 * mapWidth * mapHeigth;
    qint32 maxTries = maximumBuildingTry;
    qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (m_players.size()) * 0.7);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "getHQBaseTerrainID");
    spUnit pUnit = spUnit::create("INFANTRY", m_players[0].get(), false);
    pUnit->setIgnoreUnitCollision(true);
    qint32 days = minimalDistance / 2;
    LoadingScreen::getInstance()->setWorktext(tr("Generating ") + "HQ's");
    for (qint32 i = 0; i < m_players.size(); i++)
    {
        QPoint position;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            position = QPoint(randInt.bounded(0, mapWidth), randInt.bounded(0, mapHeigth));
            if (!playerPositions.contains(position) &&
                randomMapIsBuildingPlace("HQ", position.x(), position.y()))
            {
                bool positionOk = true;
                for (qint32 i3 = 0; i3 < i; i3++)
                {
                    if (qAbs(playerPositions[i3].x() - position.x()) + qAbs(playerPositions[i3].y() - position.y()) < minimalDistance)
                    {
                        positionOk = false;
                        break;
                    }
                }
                UnitPathFindingSystem pfs(pUnit.get());
                pfs.setMovepoints(days);
                pfs.setStartPoint(position.x(), position.y());
                pfs.explore();
                if (pfs.getAllNodePoints().size() < minimalDistance)
                {
                    positionOk = false;
                }
                if (positionOk)
                {
                    break;
                }
            }
        }
        replaceTerrain(erg.toString(), position.x(), position.y());
        playerPositions.push_back(position);
        spBuilding pBuilding = spBuilding::create("HQ");
        pBuilding->setOwner(getPlayer(i));
        getTerrain(position.x(), position.y())->setBuilding(pBuilding.get());
    }

    buildings -= playerPositions.size();
    // place start bases
    erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "getFACTORYBaseTerrainID");
    for (qint32 i = 0; i < playerPositions.size(); i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = maximumBuildingTry;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(-2, 3) + playerPositions.at(i).x();
            y = randInt.bounded(-2, 3) + playerPositions.at(i).y();
            if (randomMapIsBuildingPlace("FACTORY", x, y))
            {
                break;
            }
            else if (i2 == maxTries - 1)
            {
                x = -1;
                y = -1;
            }
        }
        if ((x >= 0) && (y >= 0))
        {
            replaceTerrain(erg.toString(), x, y);
            spBuilding pBuilding = spBuilding::create("FACTORY");
            pBuilding->setOwner(getPlayer(i));
            getTerrain(x, y)->setBuilding(pBuilding.get());
        }
    }
    buildings -= playerPositions.size();

    LoadingScreen::getInstance()->setWorktext(tr("Generating ") + "Factory");
    for (qint32 i = 0; i < buildingDistributions.size(); i++)
    {
        QString buildingID = std::get<0>(buildingDistributions[i]);
        LoadingScreen::getInstance()->setProgress(tr("Generating ") + buildingID, progress * 100 / maxProgess);
        progress++;
        float buildingChance = std::get<1>(buildingDistributions[i]) / 100.0f;
        erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "get" + buildingID + "BaseTerrainID");
        randomMapPlaceBuildings(buildingID, erg.toString(), buildings, playerPositions,
                                ownedBaseSize, buildingChance, startBaseSize, randInt);
    }
    return playerPositions;
}

void GameMap::randomMapPlaceBuildings(QString buildingId, QString baseTerrainID, qint32 buildings, QVector<QPoint> playerPositions,
                                      QVector<float> ownedBaseSize, float chance, float startBaseSize, QRandomGenerator& randInt)
{
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (m_players.size()) * 0.7);
    qint32 maximumBuildingTry = 1000;
    // number of factorys at start
    qint32 count = static_cast<qint32>(GlobalUtils::roundUp(static_cast<float>(buildings) * chance / static_cast<float>(m_players.size())) * m_players.size());
    qint32 innerBase = m_players.size() * 2;
    qint32 playerBuldings = static_cast<qint32>(GlobalUtils::roundUp(count * startBaseSize / static_cast<float>(m_players.size())) * m_players.size());

    QVector<qint32> ownerBuildings;
    for (qint32 i = 0; i < ownedBaseSize.size(); i++)
    {
        ownerBuildings.append(ownedBaseSize[i] / 100.0f * count);
    }

    for (qint32 i = 0; i < count; i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        qint32 maxTries = maximumBuildingTry;
        qint32 player = i % m_players.size();
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(0, mapWidth);
            y = randInt.bounded(0, mapHeigth);

            if (i < playerBuldings)
            {
                
                if (innerBase > 0)
                {
                    x = randInt.bounded(-2, 3) + playerPositions.at(player).x();
                    y = randInt.bounded(-2, 3) + playerPositions.at(player).y();
                }
                else
                {
                    x = randInt.bounded(-minimalDistance / 4, minimalDistance / 4 + 1) + playerPositions.at(player).x();
                    y = randInt.bounded(-minimalDistance / 4, minimalDistance / 4 + 1) + playerPositions.at(player).y();
                }
            }

            if (randomMapIsBuildingPlace(buildingId, x, y))
            {
                break;
            }
            else if (i2 == maxTries - 1)
            {
                x = -1;
                y = -1;
            }
        }
        if (onMap(x, y))
        {
            replaceTerrain(baseTerrainID, x, y);
            spBuilding pBuilding = spBuilding::create(buildingId);
            for (qint32 i2 = 0; i2 < ownedBaseSize.size(); i2++)
            {
                if ((i < playerBuldings && i2 == 0) ||
                    (i >= playerBuldings))
                {
                    qint32 player = (i + i2) % m_players.size();
                    if (ownerBuildings[player] > 0)
                    {
                        ownerBuildings[player]--;
                        pBuilding->setOwner(m_players[player].get());
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            getTerrain(x, y)->setBuilding(pBuilding.get());
            if (innerBase > 0)
            {
                innerBase--;
            }
        }
    }

}

bool GameMap::randomMapIsBuildingPlace(QString buildingId, qint32 x, qint32 y)
{
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (onMap(x ,y) &&
        (getTerrain(x, y)->getBuilding() == nullptr))
    {
        QJSValueList args;
        args << x;
        args << y;
        QJSValue erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "get" + buildingId + "Placeable", args);
        if (erg.isBool())
        {
            return erg.toBool();
        }
    }
    return false;
}

void GameMap::randomMapPlaceUnits(QVector<std::tuple<QString, float>> units,
                                  qint32 unitCount,
                                  float startBaseUnitSize,
                                  QVector<float> unitDistribution,
                                  QVector<QPoint> playerPositions,
                                  bool distributed,
                                  qint32& progress, qint32 maxProgess, QRandomGenerator& randInt)
{
    LoadingScreen* pLoadingScreen = LoadingScreen::getInstance();
    if (distributed)
    {
        for (qint32 owner = 0; owner < playerPositions.size(); ++owner)
        {
            pLoadingScreen->setProgress(tr("Generating ") + "Units", (progress + owner / playerPositions.size() * unitCount) * 100 / (maxProgess));
            qint32 hqUnits = static_cast<qint32>(unitCount) * unitDistribution[owner] / 100.0f;
            // round up for units near the hq
            qint32 randomSpawnCount = static_cast<qint32>(hqUnits * (1.0f - startBaseUnitSize));
            for (qint32 i = 0; i < hqUnits; ++i)
            {
                qint32 unitIndex = 0;
                float percentPlaced = static_cast<float>(i) / static_cast<float>(hqUnits);
                float sum = std::get<1>(units[unitIndex]) / 100.0f;
                while (percentPlaced >= sum)
                {
                    ++unitIndex;
                    sum += std::get<1>(units[unitIndex]) / 100.0f;
                }
                randomMapSpawnUnit(std::get<0>(units[unitIndex]), owner, true, playerPositions, randInt);
            }
            for (qint32 i = 0; i < randomSpawnCount; ++i)
            {
                qint32 unitIndex = 0;
                float percentPlaced = static_cast<float>(i) / static_cast<float>(hqUnits);
                float sum = std::get<1>(units[unitIndex]) / 100.0f;
                while (percentPlaced >= sum)
                {
                    ++unitIndex;
                    sum += std::get<1>(units[unitIndex]) / 100.0f;
                }
                randomMapSpawnUnit(std::get<0>(units[unitIndex]), owner, false, playerPositions, randInt);
            }
        }
    }
    else
    {
        // random spawning
        // distribution spawning
        QVector<qint32> placedPerPlayer;
        for (qint32 i = 0; i < playerPositions.length(); ++i)
        {
            placedPerPlayer.append(0);
        }
        for (qint32 i = 0; i < unitCount; ++i)
        {
            pLoadingScreen->setProgress(tr("Generating ") + "Units", (progress + i * units.size() / unitCount) * 100 / (maxProgess));
            qint32 owner = 0;
            float percentPlaced = static_cast<float>(i) / static_cast<float>(unitCount);
            float sum = unitDistribution[owner] / 100.0f;
            while (percentPlaced >= sum)
            {
                ++owner;
                sum += unitDistribution[owner] / 100.0f;
            }
            qint32 index = randInt.bounded(0, units.length());
            if (placedPerPlayer[owner] < startBaseUnitSize * unitCount * unitDistribution[owner] / 100.0f)
            {
                randomMapSpawnUnit(std::get<0>(units[index]), owner, true, playerPositions, randInt);
            }
            else
            {
                randomMapSpawnUnit(std::get<0>(units[index]), owner, false, playerPositions, randInt);
            }

            placedPerPlayer[owner] += 1;
        }
    }
}

void GameMap::randomMapSpawnUnit(QString unitId, qint32 owner, bool nearHq, QVector<QPoint> playerPositions, QRandomGenerator& randInt)
{
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (m_players.size()) * 0.7);
    qint32 maximumUnitTry = 2000;
    spUnit pUnit = spUnit::create(unitId, m_players[owner].get(), false);
    qint32 x = 0;
    qint32 y = 0;
    for (qint32 i2 = 0; i2 < maximumUnitTry; ++i2)
    {
        if (nearHq)
        {
            x = randInt.bounded(-minimalDistance / 4, minimalDistance / 4 + 1) + playerPositions.at(owner).x();
            y = randInt.bounded(-minimalDistance / 4, minimalDistance / 4 + 1) + playerPositions.at(owner).y();
        }
        else
        {
            x = randInt.bounded(0, mapWidth);
            y = randInt.bounded(0, mapHeigth);
        }
        if (onMap(x, y) &&
            pUnit->getBaseMovementCosts(x, y) > 0 &&
            getTerrain(x, y)->getUnit() == nullptr)
        {
            spawnUnit(x, y, unitId, m_players[owner].get(), 0);
            break;
        }
    }
}
