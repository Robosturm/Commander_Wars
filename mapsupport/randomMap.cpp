#include "game/gamemap.h"

#include <limits>

#include "coreengine/mainapp.h"

#include "QRandomGenerator"

#include "QVector"

#include "QPoint"

#include "game/player.h"

#include "game/building.h"

#include "game/unit.h"

#include "game/terrain.h"

#include "game/co.h"

qint32 GameMap::randomMap(qint32 width,qint32 heigth, qint32 playerCount,
                          bool roadSupport, qint32 seed,
                          float forestchance, float mountainChance, float seachance, float buildingchance,
                          float factoryChance, float airPortChance, float harbourChance, float startBaseSize)
{
    clearMap();

    qint32 startSeed = seed;
    if (seed < 0)
    {
        startSeed = Mainapp::randInt(0, std::numeric_limits<qint32>::max());

    }
    // seed map generator
    QRandomGenerator randInt(static_cast<quint32>(startSeed));
    newMap(width, heigth, playerCount);

    // create Forest
    if ((width * heigth * forestchance < 1.0f) && (forestchance > 0.0f))
    {
        placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 1, "FOREST", 2, randInt);
    }
    else
    {
        qint32 chance = static_cast<qint32>(width * heigth * forestchance) / 2;
        while (chance > 0)
        {
            qint32 groupSize = randInt.bounded(1, 10);
            chance -= placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), groupSize, "FOREST", 2, randInt);
        }
    }
    // create mountain
    if ((width * heigth * mountainChance < 1.0f) && (mountainChance > 0.0f))
    {
        placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 1, "MOUNTAIN", 2, randInt);
    }
    else
    {
        qint32 chance = static_cast<qint32>(width * heigth * mountainChance);
        while (chance > 0)
        {
            qint32 groupSize = randInt.bounded(1, 10);
            chance -= placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), groupSize, "MOUNTAIN", 2, randInt);
        }
    }
    // create sea
    if ((width * heigth * seachance < 1.0f) && (seachance > 0.0f))
    {
        placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 1, "SEA", 1, randInt);
    }
    else
    {
        qint32 chance = static_cast<qint32>(width * heigth * seachance) / 2;
        while (chance > 0)
        {
            qint32 groupSize = randInt.bounded(1, 10);
            chance -= placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), groupSize, "SEA", 1, randInt);
        }
    }

    // place roads and reafs
    placeBeach(randInt);
    placeReaf(randInt);

    qint32 fieldChance = static_cast<qint32>(width * heigth * buildingchance);
    qint32 minBuildings = playerCount * 2;
    if (fieldChance < minBuildings)
    {
        fieldChance = minBuildings;
    }
    // place buildings
    QVector<QPoint> basePoints = createBuildings(fieldChance, randInt, factoryChance, airPortChance, harbourChance, startBaseSize);

    if (roadSupport)
    {
        createRoad(randInt, basePoints);
    }

    updateSprites();
    centerMap(width / 2, heigth / 2);

    return startSeed;
}

qint32 GameMap::placeGroup(qint32 startX, qint32 startY, qint32 count, QString terrainID, qint32 terrainRadius, QRandomGenerator& randInt)
{
    QVector<QPoint> points;
    points.push_back(QPoint(startX, startY));
    qint32 placed = 0;
    while (count > 0)
    {
        // we need to place terrains
        qint32 pInd = randInt.bounded(0, points.size());
        if (terrainID != getTerrain(points[pInd].x(), points[pInd].y())->getTerrainID())
        {
            placed++;
        }
        replaceTerrain(terrainID, points[pInd].x(), points[pInd].y(), false, false);

        addTerrainPoint(points, points[pInd].x() + 1, points[pInd].y(), terrainID, terrainRadius - 1);
        addTerrainPoint(points, points[pInd].x() - 1, points[pInd].y(), terrainID, terrainRadius - 1);
        addTerrainPoint(points, points[pInd].x(), points[pInd].y() + 1, terrainID, terrainRadius - 1);
        addTerrainPoint(points, points[pInd].x(), points[pInd].y() - 1, terrainID, terrainRadius - 1);

        count--;
        points.removeAt(pInd);
        if (points.size() <= 0)
        {
            break; // no free fields avaible
        }
    }
    return placed;
}

void GameMap::addTerrainPoint(QVector<QPoint>& points, qint32 x, qint32 y, QString terrainID, qint32 terrainRadius)
{
    if (x < 0)
    {
        return;
    }
    if (y < 0)
    {
        return;
    }
    if (x >= getMapWidth())
    {
        return;
    }
    if (y >= getMapHeight())
    {
        return;
    }
    for (qint32 i = 0; i < points.size(); i++)
    {
        if ((points[i].x() == x) && (points[i].y() == y))
        {
            return;
        }
    }

    Terrain* pTerrain = getTerrain(x, y);
    if (pTerrain->getTerrainID() == terrainID)
    {
        return;
    }
    if (!canBePlaced(terrainID, x, y))
    {
        return;
    }
    points.push_back(QPoint(x, y));
    if (terrainRadius > 0)
    {
        addTerrainPoint(points, x + 1, y, terrainID, terrainRadius - 1);
        addTerrainPoint(points, x - 1, y, terrainID, terrainRadius - 1);
        addTerrainPoint(points, x, y + 1, terrainID, terrainRadius - 1);
        addTerrainPoint(points, x, y - 1, terrainID, terrainRadius - 1);
    }
}

void GameMap::placeReaf(QRandomGenerator& randInt)
{
    QVector<QPoint> reafOptions;
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            if (getTerrain(x, y)->getTerrainID() == "SEA")
            {
                if (canBePlaced("REAF", x, y))
                {
                    reafOptions.push_back(QPoint(x, y));
                }
            }
        }
    }
    for (qint32 i = 0; i < reafOptions.size(); i++)
    {
        if (randInt.bounded(0, 40) < 10)
        {
            replaceTerrain("REAF", reafOptions[i].x(), reafOptions[i].y());
        }
    }
}

void GameMap::placeBeach(QRandomGenerator& randInt)
{
    QVector<QPoint> beachOptions;
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    for (qint32 x = 0; x < mapWidth; x++)
    {
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            if (getTerrain(x, y)->getTerrainID() == "SEA")
            {
                if (canBePlaced("BEACH", x, y))
                {
                    beachOptions.push_back(QPoint(x, y));
                }
            }
        }
    }
    for (qint32 i = 0; i < beachOptions.size(); i++)
    {
        if (randInt.bounded(0, 40) < 15)
        {
            replaceTerrain("BEACH", beachOptions[i].x(), beachOptions[i].y());
        }
    }
}

void GameMap::createRoad(QRandomGenerator& randInt, QVector<QPoint>& playerPositions)
{
    for (qint32 i = 0; i < players.size(); i++)
    {
        QPoint currentPoint = playerPositions[i];
        QPoint endPoint;
        if (i == players.size() - 1)
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
            if (pTerrain->getBaseTerrainID() == "PLAINS" && pTerrain->getBuilding() == nullptr)
            {
                replaceTerrain("STREET", currentPoint.x(), currentPoint.y());
            }
            else if (pTerrain->getBaseTerrainID() == "SEA" && pTerrain->getBuilding() == nullptr)
            {
                replaceTerrain("SEA", currentPoint.x(), currentPoint.y());
                replaceTerrain("BRIDGE", currentPoint.x(), currentPoint.y(), true);
            }
        }
    }
}

QVector<QPoint> GameMap::createBuildings(qint32 buildings, QRandomGenerator& randInt, float factoryChance, float airPortChance, float harbourChance, float startBaseSize)
{
    qint32 maximumBuildingTry = 1000;
    qint32 maxTries = maximumBuildingTry;
    QVector<QPoint> playerPositions;
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (players.size()) * 0.7);
    for (qint32 i = 0; i < players.size(); i++)
    {
        QPoint position;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            position = QPoint(randInt.bounded(0, mapWidth), randInt.bounded(0, mapHeigth));
            if (!playerPositions.contains(position) && isBuildingPlace(position.x(), position.y()))
            {
                bool positionOk = true;
                for (qint32 i3 = 0; i3 < i; i3++)
                {
                    if ( qAbs(playerPositions[i3].x() - position.x()) + qAbs(playerPositions[i3].y() - position.y()) < minimalDistance)
                    {
                        positionOk = false;
                        break;
                    }
                }
                if (positionOk)
                {
                    break;
                }
            }
        }
        replaceTerrain("PLAINS", position.x(), position.y());
        playerPositions.push_back(position);
        Building* pBuilding = new Building("HQ");
        pBuilding->setOwner(getPlayer(i));
        getTerrain(position.x(), position.y())->setBuilding(pBuilding);
    }

    buildings -= playerPositions.size();
    // place start bases
    for (qint32 i = 0; i < playerPositions.size(); i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = maximumBuildingTry;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(-2, 3) + playerPositions.at(i).x();
            y = randInt.bounded(-2, 3) + playerPositions.at(i).y();
            if (isBuildingPlace(x, y))
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
            replaceTerrain("PLAINS", x, y);
            Building* pBuilding = new Building("FACTORY");
            pBuilding->setOwner(getPlayer(i));
            getTerrain(x, y)->setBuilding(pBuilding);
        }
    }
    buildings -= playerPositions.size();

    // number of factorys at start
    qint32 factoryCount = static_cast<qint32>(Mainapp::roundUp(static_cast<float>(buildings) * factoryChance / static_cast<float>(players.size())) * players.size());
    qint32 factoryBase = players.size() * 2;
    qint32 playerBuldings = static_cast<qint32>(Mainapp::roundUp(factoryCount * startBaseSize / static_cast<float>(players.size())) * players.size());
    for (qint32 i = 0; i < factoryCount; i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = maximumBuildingTry;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(0, mapWidth);
            y = randInt.bounded(0, mapHeigth);

            if (i < playerBuldings)
            {
                qint32 player = i % players.size();
                if (factoryBase > 0)
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

            if (isBuildingPlace(x, y))
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
            replaceTerrain("PLAINS", x, y);
            Building* pBuilding = new Building("FACTORY");
            getTerrain(x, y)->setBuilding(pBuilding);
            if (factoryBase > 0)
            {
                factoryBase--;
            }
        }
    }

    qint32 airportCount = static_cast<qint32>(Mainapp::roundUp(static_cast<float>(buildings) * airPortChance / static_cast<float>(players.size())) * players.size());
    qint32 airportBase = players.size() * 2;
    playerBuldings = static_cast<qint32>(Mainapp::roundUp(airportCount * startBaseSize / static_cast<float>(players.size())) * players.size());
    for (qint32 i = 0; i < airportCount; i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = maximumBuildingTry;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(0, mapWidth);
            y = randInt.bounded(0, mapHeigth);
            if (i < playerBuldings)
            {
                qint32 player = i % players.size();
                if (airportBase > 0)
                {
                    x = randInt.bounded(-3, 4) + playerPositions.at(player).x();
                    y = randInt.bounded(-3, 4) + playerPositions.at(player).y();
                }
                else
                {
                    x = randInt.bounded(-minimalDistance / 4, minimalDistance / 4 + 1) + playerPositions.at(player).x();
                    y = randInt.bounded(-minimalDistance / 4, minimalDistance / 4 + 1) + playerPositions.at(player).y();
                }
            }
            if (isBuildingPlace(x, y))
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
            replaceTerrain("PLAINS", x, y);
            Building* pBuilding = new Building("AIRPORT");
            getTerrain(x, y)->setBuilding(pBuilding);
            if (airportBase > 0)
            {
                airportBase--;
            }
        }
    }

    qint32 harbourCount = static_cast<qint32>(Mainapp::roundUp(static_cast<float>(buildings) * harbourChance / static_cast<float>(players.size())) * players.size());
    playerBuldings = static_cast<qint32>(Mainapp::roundUp(harbourCount * startBaseSize / static_cast<float>(players.size())) * players.size());
    QVector<QPoint> harbourPoints;
    qint32 heigth = getMapHeight();
    qint32 width = getMapWidth();
    for (qint32 y = 0; y < heigth; y++)
    {
        for (qint32 x = 0; x < width; x++)
        {
            QmlVectorPoint* points = Mainapp::getCircle(1, 1);
            for (qint32 i = 0; i < points->size(); i++)
            {
                qint32 posX = x + points->at(i).x();
                qint32 posY = y + points->at(i).y();
                if (onMap(posX, posY))
                {
                    if (getTerrain(posX, posY)->getTerrainID() == "SEA")
                    {
                        harbourPoints.append(QPoint(x, y));
                    }
                }
            }
            delete points;
        }
    }
    for (qint32 i = 0; i < harbourCount; i++)
    {
        qint32 index = -1;
        maxTries = maximumBuildingTry;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            if (harbourPoints.size() > 0)
            {
                index = randInt.bounded(0, harbourPoints.size());
                if (i < playerBuldings)
                {
                    qint32 player = i % players.size();
                    qint32 distance = qAbs(playerPositions.at(player).x() - harbourPoints[index].x()) + qAbs(playerPositions.at(player).y() - harbourPoints[index].y());
                    if (distance > minimalDistance / 2)
                    {
                        index = -1;
                    }
                }
                if (index >= 0)
                {
                    if (isBuildingPlace(harbourPoints[index].x(), harbourPoints[index].y()))
                    {
                        break;
                    }
                }
                if (i2 == maxTries - 1)
                {
                    index = -1;
                }
            }
        }
        if (index >= 0)
        {
            qint32 x = harbourPoints[index].x();
            qint32 y = harbourPoints[index].y();
            if ((x >= 0) && (y >= 0))
            {
                replaceTerrain("PLAINS", x, y);
                Building* pBuilding = new Building("HARBOUR");
                getTerrain(x, y)->setBuilding(pBuilding);
            }
        }
    }


    qint32 townCount = static_cast<qint32>(Mainapp::roundUp(static_cast<float>(buildings) * (1.0f - factoryChance - airPortChance - harbourChance)  / static_cast<float>(players.size())) * players.size());
    qint32 townBase = players.size() * 3;
    playerBuldings = static_cast<qint32>(Mainapp::roundUp(townCount * startBaseSize / static_cast<float>(players.size())) * players.size());
    for (qint32 i = 0; i < townCount; i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = maximumBuildingTry;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(0, mapWidth);
            y = randInt.bounded(0, mapHeigth);
            if (i < playerBuldings)
            {
                qint32 player = i % players.size();
                if (townBase > 0)
                {
                    x = randInt.bounded(-3, 4) + playerPositions.at(player).x();
                    y = randInt.bounded(-3, 4) + playerPositions.at(player).y();
                }
                else
                {
                    x = randInt.bounded(-minimalDistance / 4, minimalDistance / 4 + 1) + playerPositions.at(player).x();
                    y = randInt.bounded(-minimalDistance / 4, minimalDistance / 4 + 1) + playerPositions.at(player).y();
                }
            }
            if (isBuildingPlace(x, y))
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
            replaceTerrain("PLAINS", x, y);
            Building* pBuilding = new Building("TOWN");
            getTerrain(x, y)->setBuilding(pBuilding);
            if (townBase > 0)
            {
                townBase--;
            }

        }
    }
    return playerPositions;
}

bool GameMap::isBuildingPlace(qint32 x, qint32 y)
{
    if (onMap(x ,y) &&
        (getTerrain(x, y)->getBuilding() == nullptr) &&
        ((getTerrain(x, y)->getTerrainID() == "PLAINS") ||
         (getTerrain(x, y)->getTerrainID() == "FOREST") ||
         (getTerrain(x, y)->getTerrainID() == "MOUNTAIN")))
    {
        return true;
    }
    return false;
}
