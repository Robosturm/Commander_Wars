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
                          float forestchance, float mountainChance, float seachance, float buildingchance)
{

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
        placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 7, "FOREST", 2, randInt);
    }
    else
    {
        for (qint32 i = 1; i <= static_cast<qint32>(width * heigth * forestchance); i++)
        {
            placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 7, "FOREST", 2, randInt);
        }
    }
    // create mountain
    if ((width * heigth * mountainChance < 1.0f) && (forestchance > 0.0f))
    {
        placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 10, "MOUNTAIN", 2, randInt);
    }
    else
    {
        for (qint32 i = 1; i <= static_cast<qint32>(width * heigth * mountainChance); i++)
        {
            placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 10, "MOUNTAIN", 2, randInt);
        }
    }
    // create sea
    if ((width * heigth * seachance * 3.0f / 4.0f < 1.0f) && (forestchance > 0.0f))
    {
        placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 30, "SEA", 1, randInt);
    }
    else
    {
        for (qint32 i = 1; i <= static_cast<qint32>(width * heigth * seachance * 3.0f / 4.0f); i++)
        {
            placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 30, "SEA", 1, randInt);
        }
        for (qint32 i = 1; i <= static_cast<qint32>(width * heigth * seachance * 1.0f / 4.0f); i++)
        {
            placeGroup(randInt.bounded(0, width), randInt.bounded(0, heigth), 7, "SEA", 1, randInt);
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
    createBuildings(fieldChance, roadSupport, randInt, true);

    updateSprites();
    centerMap(width / 2, heigth / 2);

    return startSeed;
}

void GameMap::placeGroup(qint32 startX, qint32 startY, qint32 count, QString terrainID, qint32 terrainRadius, QRandomGenerator& randInt)
{
    QVector<QPoint> points;
    points.push_back(QPoint(startX, startY));
    while (count > 0)
    {
        // we need to place terrains
        qint32 pInd = randInt.bounded(0, points.size());

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
        if (randInt.bounded(0, 40) < 25)
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
        if (randInt.bounded(0, 40) < 25)
        {
            replaceTerrain("BEACH", beachOptions[i].x(), beachOptions[i].y());
        }
    }
}

void GameMap::createBuildings(qint32 buildings, bool roadSupport, QRandomGenerator& randInt, bool noHarbour)
{
    qint32  maxTries = 1000;
    QVector<QPoint> playerPositions;
    qint32 mapWidth = getMapWidth();
    qint32 mapHeigth = getMapHeight();
    qint32 minimalDistance = (mapWidth + mapHeigth) / (players.size());
    for (qint32 i = 0; i < players.size(); i++)
    {
        QPoint position;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            position = QPoint(randInt.bounded(0, mapWidth), randInt.bounded(0, mapHeigth));
            if (!playerPositions.contains(position))
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
        pBuilding->setOwner(getspPlayer(i));
        getTerrain(position.x(), position.y())->setBuilding(pBuilding);
    }

    buildings -= playerPositions.size();
    // place start bases
    for (qint32 i = 0; i < playerPositions.size(); i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = 100;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(-2, 3) + playerPositions.at(i).x();
            y = randInt.bounded(-2, 3) + playerPositions.at(i).y();
            if (onMap(x ,y) && (getTerrain(x, y)->getBuilding() == nullptr))
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
            pBuilding->setOwner(getspPlayer(i));
            getTerrain(x, y)->setBuilding(pBuilding);
        }
    }
    buildings -= playerPositions.size();

    // number of factorys at start
    qint32 factoryCount = static_cast<qint32>(static_cast<float>(buildings) * 0.2f);

    //        For I = 0 To 2
    //            If BasenZahl - Basen.Length >= 0 Then
    //                For I2 = 0 To Basen.Length - 1
    //                    PlaceBuildingRandomUmOrt(Basen(I2), "BASIS", 3, I2 + 1, R)
    //                Next
    //                BasenZahl -= Basen.Length
    //            Else
    //                Exit For
    //            End If
    //        Next
    // place factories
    for (qint32 i = 0; i < factoryCount; i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = 100;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(0, mapWidth);
            y = randInt.bounded(0, mapHeigth);
            if (onMap(x ,y) &&
                (getTerrain(x, y)->getBuilding() == nullptr))
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
        }
    }

    qint32 airportCount = static_cast<qint32>(static_cast<float>(buildings) * 0.1f);
    //        For I = 0 To 1
    //            If FlughafenZahl - Basen.Length >= 0 Then
    //                For I2 = 0 To Basen.Length - 1
    //                    PlaceBuildingRandomUmOrt(Basen(I2), "FLUGHAFEN", 3, I2 + 1, R)
    //                Next
    //                FlughafenZahl -= Basen.Length
    //            Else
    //                Exit For
    //            End If
    //        Next



    for (qint32 i = 0; i < airportCount; i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = 1000;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(0, mapWidth);
            y = randInt.bounded(0, mapHeigth);
            if (onMap(x ,y) &&
                (getTerrain(x, y)->getBuilding() == nullptr))
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
        }
    }

    qint32 townCount = static_cast<qint32>(static_cast<float>(buildings) * 0.6f);

    for (qint32 i = 0; i < townCount; i++)
    {
        qint32 x = -1;
        qint32 y = -1;
        maxTries = 1000;
        for (qint32 i2 = 0; i2 < maxTries; i2++)
        {
            x = randInt.bounded(0, mapWidth);
            y = randInt.bounded(0, mapHeigth);
            if (onMap(x ,y) &&
                (getTerrain(x, y)->getBuilding() == nullptr) &&
                ((getTerrain(x, y)->getTerrainID() == "PLAINS") ||
                 (getTerrain(x, y)->getTerrainID() == "FOREST") ||
                 (getTerrain(x, y)->getTerrainID() == "MOUNTAIN")))
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
        }
    }

    //        'So wir haben alle Gebaeude plaziert
    //        'So jetzt erstellen wir noch einen Strassenkreis
    //        If Strassenkreis = True Then
    //            For I = 0 To Basen.Length - 1
    //                Dim Startpunkt As Point = Basen(I)
    //                Dim EndPunkt As New Point
    //                If I = Basen.Length - 1 Then
    //                    EndPunkt = Basen(0)
    //                Else
    //                    EndPunkt = Basen(I + 1)
    //                End If
    //                CreateStrasse(Startpunkt, EndPunkt, R)
    //                AktuallisiereStrassen()
    //            Next
    //        End If

    //        'So viele Haefen werden plaziert
    //        Dim HafenZahl As Integer = AnzahlderGebaeude * 0.1#
    //        'Und dort koennen sie plaziert werden
    //        Dim HafenMoeglichkeiten As New Stack(Of Point)
    //        For X = 0 To Objektverwalter.Spielfeld.Breite - 1
    //            For Y = 0 To Objektverwalter.Spielfeld.Hoehe - 1
    //                If IsEinAngrenzendesFeld(X, Y, "SEE", ObjektSpielfeld.Bodenschlacht) = True Then
    //                    If OhneGebaeude(X, Y, ObjektSpielfeld.Bodenschlacht) = True Then
    //                        If GetLandschaftsID(X, Y, ObjektSpielfeld.Bodenschlacht) <> "SEE" Then
    //                            HafenMoeglichkeiten.Push(New Point(X, Y))
    //                        End If
    //                    End If
    //                End If
    //            Next
    //        Next
    //        If Not KeineHaefen Then
    //            'Jeder Spieler erhaelt maximal einen Hafen.
    //            If HafenZahl - Basen.Length >= 0 Then
    //                For I2 = 0 To Basen.Length - 1
    //                    Dim Ort As Point
    //                    Dim Stelle As Integer = -1
    //                    Dim Durchlaeufe As Integer = 0
    //                    If HafenMoeglichkeiten.Count <= 0 Then
    //                        Do
    //                            Ort = New Point(R.Next(Objektverwalter.Spielfeld.Breite - 1), R.Next(Objektverwalter.Spielfeld.Hoehe - 1))
    //                            Durchlaeufe += 1
    //                        Loop While OhneGebaeude(Ort.X, Ort.Y, ObjektSpielfeld.Bodenschlacht) = False And GetDistance(Basen(I2), Ort) > 10 Or Durchlaeufe < MaximaleVersuche
    //                    Else
    //                        Do
    //                            Stelle = R.Next(HafenMoeglichkeiten.Count - 1)
    //                            Ort = HafenMoeglichkeiten(Stelle)
    //                            Durchlaeufe += 1
    //                        Loop While GetDistance(Basen(I2), Ort) > 10 Or Durchlaeufe < MaximaleVersuche

    //                    End If
    //                    PlaceGebaeude(Ort.X, Ort.Y, "WERFT", I2 + 1)
    //                    If HafenMoeglichkeiten.Count > 0 Then
    //                        HafenMoeglichkeiten = PopFeld(HafenMoeglichkeiten, Stelle)
    //                    End If
    //                Next
    //                HafenZahl -= Basen.Length
    //            End If

    //            For I = 1 To HafenZahl
    //                Dim Ort As Point
    //                Dim Stelle As Integer = -1
    //                If HafenMoeglichkeiten.Count <= 0 Then
    //                    Do
    //                        Ort = New Point(R.Next(Objektverwalter.Spielfeld.Breite - 1), R.Next(Objektverwalter.Spielfeld.Hoehe - 1))
    //                    Loop While OhneGebaeude(Ort.X, Ort.Y, ObjektSpielfeld.Bodenschlacht) = False
    //                Else
    //                    Stelle = R.Next(HafenMoeglichkeiten.Count - 1)
    //                    Ort = HafenMoeglichkeiten(Stelle)
    //                End If
    //                PlaceGebaeude(Ort.X, Ort.Y, "WERFT", 0)
    //                If HafenMoeglichkeiten.Count > 0 Then
    //                    HafenMoeglichkeiten = PopFeld(HafenMoeglichkeiten, Stelle)
    //                End If
    //            Next
    //        End If
}
