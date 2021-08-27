#include <limits>
#include "QRandomGenerator"
#include "QVector"
#include "QPoint"

#include "mapsupport/randomMapGenerator.h"

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
qint32 RandomMapGenerator::randomMap(qint32 width, qint32 heigth, qint32 playerCount,
                                     bool roadSupport, qint32 seed,
                                     QVector<std::tuple<QString, float>> terrains,
                                     QVector<std::tuple<QString, float>> buildings,
                                     QVector<float> ownedBaseSize,
                                     float startBaseSize,
                                     QVector<std::tuple<QString, float>> units,
                                     qint32 unitCount,
                                     float startBaseUnitSize,
                                     QVector<float> unitDistribution,
                                     bool unitsDistributed,
                                     bool mirrored)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap.get() == nullptr)
    {
        return 0;
    }
    pMap->clearMap();

    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
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

    MirrorMode mirrorX = MirrorMode::none;
    MirrorMode mirrorY = MirrorMode::none;
    qint32 startWidth = width;
    qint32 startHeigth = heigth;
    if (mirrored)
    {
        if (playerCount % 4 == 0)
        {
            mirrorX = MirrorMode::flip;
            startWidth = GlobalUtils::roundUp(static_cast<float>(width) / 2.0f);
            mirrorY = MirrorMode::flip;
            startHeigth = GlobalUtils::roundUp(static_cast<float>(heigth) / 2.0f);
        }
        else
        {
            if (randInt.bounded(0, 2) == 1)
            {
                if (randInt.bounded(0, 2) == 1)
                {
                    mirrorX = MirrorMode::flip;
                }
                else
                {
                    mirrorX = MirrorMode::rotate;
                }
                startWidth = GlobalUtils::roundUp(static_cast<float>(width) / 2.0f);
            }
            else
            {
                if (randInt.bounded(0, 2) == 1)
                {
                    mirrorY = MirrorMode::flip;
                }
                else
                {
                    mirrorY = MirrorMode::rotate;
                }
                startHeigth = GlobalUtils::roundUp(static_cast<float>(heigth) / 2.0f);
            }
        }
    }
    pMap->newMap(startWidth, startHeigth, playerCount, baseTerrain);

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
                randomMapPlaceTerain(terrainID, startWidth, startHeigth, terrainChance / 100.0f,
                                     distribution.toVariant().toPoint(), list, chances,
                                     static_cast<GameEnums::RandomMapTerrainType>(terrainType.toInt()), randInt);
            }
        }
    }
    pMap->changeMap(width, heigth, playerCount);
    if (mirrorX == MirrorMode::flip)
    {
        pMap->flipX();
    }
    else if (mirrorX == MirrorMode::rotate)
    {
        pMap->rotateX();
    }
    if (mirrorY == MirrorMode::flip)
    {
        pMap->flipY();
    }
    else if (mirrorY == MirrorMode::rotate)
    {
        pMap->rotateY();
    }
    
    qint32 fieldChance = static_cast<qint32>(width * heigth * buildingchance / 100.0f);
    qint32 minBuildings = playerCount * 2;
    if (fieldChance < minBuildings)
    {
        fieldChance = minBuildings;
    }
    // place buildings
    QVector<QPoint> basePoints;
    if (mirrored)
    {
        basePoints = randomMapCreateBuildings(startWidth, startHeigth, fieldChance, randInt, buildings, ownedBaseSize,
                                              startBaseSize, progress, maxSteps, mirrorX, mirrorY);
    }
    else
    {
        basePoints = randomMapCreateBuildings(fieldChance, randInt, buildings, ownedBaseSize,
                                              startBaseSize, progress, maxSteps);
    }

    if (roadSupport)
    {
        pLoadingScreen->setProgress(tr("Generating ") + "Roads", (progress) * 100 / (maxSteps));
        ++progress;
        randomMapCreateRoad(randInt, basePoints);
    }
    randomMapPlaceUnits(units, unitCount, startBaseUnitSize, unitDistribution, basePoints, unitsDistributed, progress, maxSteps, randInt);

    pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "customStep");

    pMap->updateSprites();
    pMap->centerMap(width / 2, heigth / 2);

    pLoadingScreen->hide();
    return startSeed;
}

void RandomMapGenerator::randomMapPlaceTerain(QString terrainID, qint32 width, qint32 heigth, float placeChance, QPoint placeSize, QStringList topTerrainIDs, QList<QVariant> placeChances, GameEnums::RandomMapTerrainType type, QRandomGenerator& randInt)
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

qint32 RandomMapGenerator::randomMapPlaceLine(qint32 startX, qint32 startY, qint32 count, QString terrainID, QRandomGenerator& randInt)
{
    spGameMap pMap = GameMap::getInstance();
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
        if (terrainID != pMap->getTerrain(x, y)->getTerrainID() && randomMapTerrainPlaceable(x, y, terrainID))
        {
            maximumTry = maximumTries;
            placed++;
            points.clear();
            directions.clear();
            pMap->replaceTerrain(terrainID, x, y, false, false);
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

qint32 RandomMapGenerator::randomMapPlaceGroup(qint32 startX, qint32 startY, qint32 count, QString terrainID, qint32 terrainRadius, QRandomGenerator& randInt)
{
    spGameMap pMap = GameMap::getInstance();
    QVector<QPoint> points;
    points.push_back(QPoint(startX, startY));
    qint32 placed = 0;
    qint32 maximumTries = 1000;
    qint32 maximumTry = maximumTries;
    while (count > 0 && maximumTry > 0)
    {
        // we need to place terrains
        qint32 pInd = randInt.bounded(0, points.size());
        if (terrainID != pMap->getTerrain(points[pInd].x(), points[pInd].y())->getTerrainID() &&
            randomMapTerrainPlaceable(points[pInd].x(), points[pInd].y(), terrainID))
        {
            maximumTry = maximumTries;
            placed++;
            pMap->replaceTerrain(terrainID, points[pInd].x(), points[pInd].y(), false, false);
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

bool RandomMapGenerator::randomMapTerrainPlaceable(qint32 x, qint32 y, QString terrainID)
{
    spGameMap pMap = GameMap::getInstance();
    if (!pMap->canBePlaced(terrainID, x, y))
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

bool RandomMapGenerator::randomMapAddTerrainPoint(QVector<QPoint>& points, qint32 x, qint32 y, QString terrainID, qint32 terrainRadius)
{
    spGameMap pMap = GameMap::getInstance();
    if (x < 0)
    {
        return false;
    }
    if (y < 0)
    {
        return false;
    }
    if (x >= pMap->getMapWidth())
    {
        return false;
    }
    if (y >= pMap->getMapHeight())
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

    Terrain* pTerrain = pMap->getTerrain(x, y);
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

void RandomMapGenerator::randomMapPlaceOnTop(QString terrainID, QString topId, float chance, QRandomGenerator& randInt)
{
    spGameMap pMap = GameMap::getInstance();
    QVector<QPoint> reafOptions;
    qint32 mapWidth = pMap->getMapWidth();
    qint32 mapHeigth = pMap->getMapHeight();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            if (pMap->getTerrain(x, y)->getTerrainID() == terrainID)
            {
                if (pMap->canBePlaced(topId, x, y))
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
            pMap->replaceTerrain(topId, reafOptions[i].x(), reafOptions[i].y());
        }
    }
}

void RandomMapGenerator::randomMapCreateRoad(QRandomGenerator& randInt, QVector<QPoint>& playerPositions)
{
    spGameMap pMap = GameMap::getInstance();
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
    {
        QPoint currentPoint = playerPositions[i];
        QPoint endPoint;
        if (i == pMap->getPlayerCount() - 1)
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
            Terrain* pTerrain = pMap->getTerrain(currentPoint.x(), currentPoint.y());
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
                        pMap->replaceTerrain(ret[0], currentPoint.x(), currentPoint.y());
                    }
                    pMap->replaceTerrain(ret[1], currentPoint.x(), currentPoint.y(), true);
                }
            }
        }
    }
}

QVector<QPoint> RandomMapGenerator::randomMapCreateBuildings(qint32 buildings, QRandomGenerator& randInt, QVector<std::tuple<QString, float>> buildingDistributions, QVector<float> ownedBaseSize,
                                                             float startBaseSize, qint32& progress, qint32 maxProgess)
{
    spGameMap pMap = GameMap::getInstance();
    QVector<QPoint> playerPositions;
    qint32 mapWidth = pMap->getMapWidth();
    qint32 mapHeigth = pMap->getMapHeight();
    qint32 maximumBuildingTry = 3 * mapWidth * mapHeigth;
    qint32 maxTries = maximumBuildingTry;
    qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (pMap->getPlayerCount()) * 0.7);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "getHQBaseTerrainID");
    spUnit pUnit = spUnit::create("INFANTRY", pMap->getPlayer(0), false);
    pUnit->setIgnoreUnitCollision(true);
    qint32 days = minimalDistance / 2;
    LoadingScreen::getInstance()->setWorktext(tr("Generating ") + "HQ's");
    for (qint32 i = 0; i < pMap->getPlayerCount(); i++)
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

        pMap->replaceTerrain(erg.toString(), position.x(), position.y());
        playerPositions.push_back(position);
        spBuilding pBuilding = spBuilding::create("HQ");
        pBuilding->setOwner(pMap->getPlayer(i));
        pMap->getTerrain(position.x(), position.y())->setBuilding(pBuilding);
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
            pMap->replaceTerrain(erg.toString(), x, y);
            spBuilding pBuilding = spBuilding::create("FACTORY");
            pBuilding->setOwner(pMap->getPlayer(i));
            pMap->getTerrain(x, y)->setBuilding(pBuilding);
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

void RandomMapGenerator::randomMapPlaceBuildings(QString buildingId, QString baseTerrainID, qint32 buildings, QVector<QPoint> playerPositions,
                                                 QVector<float> ownedBaseSize, float chance, float startBaseSize, QRandomGenerator& randInt)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 mapWidth = pMap->getMapWidth();
    qint32 mapHeigth = pMap->getMapHeight();
    qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (pMap->getPlayerCount()) * 0.7);
    qint32 maximumBuildingTry = 1000;
    // number of factorys at start
    qint32 count = static_cast<qint32>(GlobalUtils::roundUp(static_cast<float>(buildings) * chance / static_cast<float>(pMap->getPlayerCount())) * pMap->getPlayerCount());
    qint32 innerBase = pMap->getPlayerCount() * 2;
    qint32 playerBuldings = static_cast<qint32>(GlobalUtils::roundUp(count * startBaseSize / static_cast<float>(pMap->getPlayerCount())) * pMap->getPlayerCount());

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
        qint32 player = i % pMap->getPlayerCount();
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
        if (pMap->onMap(x, y))
        {
            pMap->replaceTerrain(baseTerrainID, x, y);
            spBuilding pBuilding = spBuilding::create(buildingId);
            for (qint32 i2 = 0; i2 < ownedBaseSize.size(); i2++)
            {
                if ((i < playerBuldings && i2 == 0) ||
                    (i >= playerBuldings))
                {
                    qint32 player = (i + i2) % pMap->getPlayerCount();
                    if (ownerBuildings[player] > 0)
                    {
                        ownerBuildings[player]--;
                        pBuilding->setOwner(pMap->getPlayer(player));
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            pMap->getTerrain(x, y)->setBuilding(pBuilding);
            if (innerBase > 0)
            {
                innerBase--;
            }
        }
    }

}

bool RandomMapGenerator::randomMapIsBuildingPlace(QString buildingId, qint32 x, qint32 y)
{
    spGameMap pMap = GameMap::getInstance();
    Interpreter* pInterpreter = Interpreter::getInstance();
    if (pMap->onMap(x ,y) &&
        (pMap->getTerrain(x, y)->getBuilding() == nullptr))
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

QVector<QPoint> RandomMapGenerator::randomMapCreateBuildings(qint32 width, qint32 heigth, qint32 buildings, QRandomGenerator& randInt, QVector<std::tuple<QString, float>> buildingDistributions, QVector<float> ownedBaseSize,
                                                             float startBaseSize, qint32& progress, qint32 maxProgess, MirrorMode mirrorX, MirrorMode mirrorY)
{
    spGameMap pMap = GameMap::getInstance();
    QVector<QPoint> playerPositions;
    qint32 maximumBuildingTry = 3 * width * heigth;
    qint32 maxTries = maximumBuildingTry;
    qint32 minimalDistance = static_cast<qint32>((width * 2 + heigth * 2) / (pMap->getPlayerCount()) * 0.7);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "getHQBaseTerrainID");
    spUnit pUnit = spUnit::create("INFANTRY", pMap->getPlayer(0), false);
    pUnit->setIgnoreUnitCollision(true);
    qint32 days = minimalDistance / 2;
    LoadingScreen::getInstance()->setWorktext(tr("Generating ") + "HQ's");
    qint32 playerCount = pMap->getPlayerCount();
    qint32 mirrorPlacing = 0;
    if (mirrorX != MirrorMode::none)
    {
        mirrorPlacing += 2;
        playerCount /= 2;
    }
    if (mirrorY != MirrorMode::none)
    {
        mirrorPlacing += 2;
        playerCount /= 2;
    }
    for (qint32 i = 0; i < playerCount; i++)
    {
        QPoint position;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            position = QPoint(randInt.bounded(0, width), randInt.bounded(0, heigth));
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

        for (qint32 i3 = 0; i3 < mirrorPlacing; ++i3)
        {
            QPoint mirrorPosition = getMirroredPosition(i3, position, mirrorX, mirrorY);
            pMap->replaceTerrain(erg.toString(), mirrorPosition.x(), mirrorPosition.y());
            spBuilding pBuilding = spBuilding::create("HQ");
            pBuilding->setOwner(pMap->getPlayer(playerPositions.size()));
            playerPositions.push_back(mirrorPosition);
            pMap->getTerrain(mirrorPosition.x(), mirrorPosition.y())->setBuilding(pBuilding);
        }
    }
    buildings -= playerPositions.size();
    // place start bases
    erg = pInterpreter->doFunction(RANDOMMAPGENERATORNAME, "getFACTORYBaseTerrainID");
    for (qint32 i = 0; i < playerCount; i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = maximumBuildingTry;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(-2, 3) + playerPositions.at(i).x();
            y = randInt.bounded(-2, 3) + playerPositions.at(i).y();
            if (x < width && y < heigth &&
                randomMapIsBuildingPlace("FACTORY", x, y))
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
            for (qint32 i3 = 0; i3 < mirrorPlacing; ++i3)
            {
                QPoint mirrorPosition = getMirroredPosition(i3, QPoint(x, y), mirrorX, mirrorY);
                pMap->replaceTerrain(erg.toString(), mirrorPosition.x(), mirrorPosition.y());
                spBuilding pBuilding = spBuilding::create("FACTORY");
                pBuilding->setOwner(pMap->getPlayer(i * mirrorPlacing + i3));
                pMap->getTerrain(mirrorPosition.x(), mirrorPosition.y())->setBuilding(pBuilding);
            }
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
        // randomMapPlaceBuildings(buildingID, erg.toString(), buildings, playerPositions,
        //                         ownedBaseSize, buildingChance, startBaseSize, randInt);
    }
    return playerPositions;
}

QPoint RandomMapGenerator::getMirroredPosition(qint32 index, QPoint position, MirrorMode mirrorX, MirrorMode mirrorY)
{
    QPoint ret = position;
    if (index > 0)
    {
        spGameMap pMap = GameMap::getInstance();
        qint32 mapWidth = pMap->getMapWidth();
        qint32 mapHeigth = pMap->getMapHeight();
        if (index == 3)
        {
            ret = QPoint(mapWidth - position.x() - 1,
                         mapHeigth - position.y() - 1);
        }
        else if (index == 2)
        {
            ret = QPoint(position.x(),
                         mapHeigth - position.y() - 1);
        }
        else if (index == 1)
        {
            if (mirrorX == MirrorMode::flip)
            {
                ret = QPoint(mapWidth - position.x() - 1,
                             position.y());
            }
            else if (mirrorX == MirrorMode::rotate ||
                     mirrorY == MirrorMode::rotate)
            {
                ret = QPoint(mapWidth - position.x() - 1,
                             mapHeigth - position.y() - 1);
            }
            else if (mirrorY == MirrorMode::flip)
            {
                ret = QPoint(position.x(),
                             mapHeigth - position.y() - 1);
            }
        }
    }
    return ret;
}

void RandomMapGenerator::randomMapPlaceUnits(QVector<std::tuple<QString, float>> units,
                                             qint32 unitCount,
                                             float startBaseUnitSize,
                                             QVector<float> unitDistribution,
                                             QVector<QPoint> playerPositions,
                                             bool distributed,
                                             qint32& progress, qint32 maxProgess, QRandomGenerator& randInt)
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
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

void RandomMapGenerator::randomMapSpawnUnit(QString unitId, qint32 owner, bool nearHq, QVector<QPoint> playerPositions, QRandomGenerator& randInt)
{
    spGameMap pMap = GameMap::getInstance();
    qint32 mapWidth = pMap->getMapWidth();
    qint32 mapHeigth = pMap->getMapHeight();
    qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (pMap->getPlayerCount()) * 0.7);
    qint32 maximumUnitTry = 2000;
    spUnit pUnit = spUnit::create(unitId, pMap->getPlayer(owner), false);
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
        if (pMap->onMap(x, y) &&
            pUnit->getBaseMovementCosts(x, y) > 0 &&
            pMap->getTerrain(x, y)->getUnit() == nullptr)
        {
            pMap->spawnUnit(x, y, unitId, pMap->getPlayer(owner), 0);
            break;
        }
    }
}
