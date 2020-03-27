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

qint32 GameMap::randomMap(qint32 width, qint32 heigth, qint32 playerCount,
                          bool roadSupport, qint32 seed,
						  QVector<std::tuple<QString, float>> terrains,
						  QVector<std::tuple<QString, float>> buildings, 
						  QVector<float> ownedBaseSize,
						  float startBaseSize)
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
	Interpreter* pInterpreter = Interpreter::getInstance();
    float buildingchance = 0.0f;
	for (qint32 i = 0; i < terrains.size(); i++)
	{
		QString terrainID = std::get<0>(terrains[i]);
        if (terrainID == "PLAINS")
        {
        }
        else if (terrainID == "Buildings")
        {
            buildingchance = std::get<1>(terrains[i]);
        }
        else
        {
            float terrainChance = std::get<1>(terrains[i]);
            QStringList list = pInterpreter->doFunction("RANDOMMAPGENERATOR", "get" + terrainID + "TopTerrainIDs").toVariant().toStringList();
            QList<QVariant> chances = pInterpreter->doFunction("RANDOMMAPGENERATOR", "get" + terrainID + "TopTerrainChances").toVariant().toList();
            QJSValue distribution = pInterpreter->doFunction("RANDOMMAPGENERATOR", "get" + terrainID + "Distribution");
            if (list.size() == chances.size())
            {
                randomMapPlaceTerain(terrainID, width, heigth, terrainChance / 100.0f, distribution.toVariant().toPoint(), list, chances, randInt);
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
    QVector<QPoint> basePoints = randomMapCreateBuildings(fieldChance, randInt, buildings, ownedBaseSize, startBaseSize);

    if (roadSupport)
    {
        randomMapCreateRoad(randInt, basePoints);
    }

    updateSprites();
    centerMap(width / 2, heigth / 2);

    return startSeed;
}

void GameMap::randomMapPlaceTerain(QString terrainID, qint32 width, qint32 heigth, float placeChance, QPoint placeSize, QStringList topTerrainIDs, QList<QVariant> placeChances, QRandomGenerator& randInt)
{
	qint32 maximumTry = 1000;
	if ((width * heigth * placeChance < 1.0f) && (placeChance > 0.0f))
    {
        randomMapPlaceGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 1, terrainID, 1, randInt);
    }
    else
    {
        qint32 chance = static_cast<qint32>(width * heigth * placeChance);
        while (chance > 0 && maximumTry > 0)
        {
            qint32 divider = 1;
            if (placeSize.x() < placeSize.y())
            {
                divider = randInt.bounded(placeSize.x(), placeSize.y());
            }
            qint32 groupSize = chance;
            if (groupSize < 10)
			{
                groupSize = 10;
			}
            qint32 high = Mainapp::roundUp(groupSize / divider);
            if (high > 1)
            {
                groupSize = randInt.bounded(1, high);
            }
            else
            {
                groupSize = 1;
            }
            chance -= randomMapPlaceGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), groupSize, terrainID, 1, randInt);
            maximumTry--;
        }
    }
	for (qint32 i = 0; i < topTerrainIDs.size(); i++)
	{
        randomMapPlaceOnTop(terrainID, topTerrainIDs[i], placeChances[i].toFloat(), randInt);
	}
}

qint32 GameMap::randomMapPlaceGroup(qint32 startX, qint32 startY, qint32 count, QString terrainID, qint32 terrainRadius, QRandomGenerator& randInt)
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

        randomMapAddTerrainPoint(points, points[pInd].x() + 1, points[pInd].y(), terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, points[pInd].x() - 1, points[pInd].y(), terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, points[pInd].x(), points[pInd].y() + 1, terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, points[pInd].x(), points[pInd].y() - 1, terrainID, terrainRadius - 1);

        count--;
        points.removeAt(pInd);
        if (points.size() <= 0)
        {
            break; // no free fields avaible
        }
    }
    return placed;
}

void GameMap::randomMapAddTerrainPoint(QVector<QPoint>& points, qint32 x, qint32 y, QString terrainID, qint32 terrainRadius)
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
        randomMapAddTerrainPoint(points, x + 1, y, terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, x - 1, y, terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, x, y + 1, terrainID, terrainRadius - 1);
        randomMapAddTerrainPoint(points, x, y - 1, terrainID, terrainRadius - 1);
    }
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

QVector<QPoint> GameMap::randomMapCreateBuildings(qint32 buildings, QRandomGenerator& randInt, QVector<std::tuple<QString, float>> buildingDistributions, QVector<float> ownedBaseSize, float startBaseSize)
{
    qint32 maximumBuildingTry = 1000;
    qint32 maxTries = maximumBuildingTry;
    QVector<QPoint> playerPositions;
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (players.size()) * 0.7);
    Interpreter* pInterpreter = Interpreter::getInstance();
    QJSValue erg = pInterpreter->doFunction("RANDOMMAPGENERATOR", "getHQBaseTerrainID");
    for (qint32 i = 0; i < players.size(); i++)
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
                if (positionOk)
                {
                    break;
                }
            }
        }
        replaceTerrain(erg.toString(), position.x(), position.y());
        playerPositions.push_back(position);
        Building* pBuilding = new Building("HQ");
        pBuilding->setOwner(getPlayer(i));
        getTerrain(position.x(), position.y())->setBuilding(pBuilding);
    }

    buildings -= playerPositions.size();
    // place start bases
    erg = pInterpreter->doFunction("RANDOMMAPGENERATOR", "getFACTORYBaseTerrainID");
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
            Building* pBuilding = new Building("FACTORY");
            pBuilding->setOwner(getPlayer(i));
            getTerrain(x, y)->setBuilding(pBuilding);
        }
    }
    buildings -= playerPositions.size();
	
    for (qint32 i = 0; i < buildingDistributions.size(); i++)
	{
        QString buildingID = std::get<0>(buildingDistributions[i]);
        float buildingChance = std::get<1>(buildingDistributions[i]) / 100.0f;
        erg = pInterpreter->doFunction("RANDOMMAPGENERATOR", "get" + buildingID + "BaseTerrainID");
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
	qint32 minimalDistance = static_cast<qint32>((mapWidth * 2 + mapHeigth * 2) / (players.size()) * 0.7);
	qint32 maximumBuildingTry = 1000;
	// number of factorys at start
    qint32 count = static_cast<qint32>(Mainapp::roundUp(static_cast<float>(buildings) * chance / static_cast<float>(players.size())) * players.size());
    qint32 innerBase = players.size() * 2;
    qint32 playerBuldings = static_cast<qint32>(Mainapp::roundUp(count * startBaseSize / static_cast<float>(players.size())) * players.size());
	
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
		qint32 player = i % players.size();
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
        if ((x >= 0) && (y >= 0))
        {
            replaceTerrain(baseTerrainID, x, y);
            Building* pBuilding = new Building(buildingId);
			for (qint32 i2 = 0; i2 < ownedBaseSize.size(); i2++)
			{
                if ((i < playerBuldings && i2 == 0) ||
                    (i >= playerBuldings))
				{
					qint32 player = (i + i2) % players.size();
					if (ownerBuildings[player] > 0)
					{
						ownerBuildings[player]--;				
                        pBuilding->setOwner(players[player].get());
						break;
					}
				}
				else
				{
					break;
				}
			}			
            getTerrain(x, y)->setBuilding(pBuilding);
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
		QJSValue erg = pInterpreter->doFunction("RANDOMMAPGENERATOR", "get" + buildingId + "Placeable", args);
		if (erg.isBool())
		{
			return erg.toBool();
		}
    }
    return false;
}
