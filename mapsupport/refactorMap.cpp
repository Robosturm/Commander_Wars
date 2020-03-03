#include "game/gamemap.h"

#include "game/terrain.h"

#include "game/unit.h"

#include "game/building.h"

#include "game/player.h"
#include "game/co.h"

void GameMap::newMap(qint32 width, qint32 heigth, qint32 playerCount)
{
    clearMap();
    for (qint32 y = 0; y < heigth; y++)
    {
        fields.append(new QVector<spTerrain>());
        for (qint32 x = 0; x < width; x++)
        {
            spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y, "");
            this->addChild(pTerrain);
            fields[y]->append(pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        }
    }
    // add two players to a default map :)
    for (qint32 i = 0; i < playerCount; i++)
    {
        players.append(new Player());
        players[players.size() - 1]->init();
    }

    updateSprites();
    centerMap(width / 2, heigth / 2);
}

void GameMap::changeMap(qint32 width, qint32 heigth, qint32 playerCount)
{
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    qint32 currentPlayerCount = getPlayerCount();

    if (width > currentWidth)
    {
        for (qint32 y = 0; y < currentHeigth; y++)
        {
            for (qint32 x = currentWidth; x < width; x++)
            {
                spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y, "");
                this->addChild(pTerrain);
                fields[y]->append(pTerrain);
                pTerrain->setPosition(x * Imagesize, y * Imagesize);
                pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            }
        }
    }
    else if (width < currentWidth)
    {
        for (qint32 y = 0; y < currentHeigth; y++)
        {
            while (width < fields[y]->size())
            {
                fields.at(y)->at(fields.at(y)->size() - 1)->detach();
                fields.at(y)->removeLast();
            }
        }
    }

    if (heigth > currentHeigth)
    {
        for (qint32 y = currentHeigth; y < heigth; y++)
        {
            fields.append(new QVector<spTerrain>());
            for (qint32 x = 0; x < width; x++)
            {
                spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y, "");
                this->addChild(pTerrain);
                fields[y]->append(pTerrain);
                pTerrain->setPosition(x * Imagesize, y * Imagesize);
                pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            }
        }
    }
    else if (heigth < currentHeigth)
    {
        while (heigth < fields.size())
        {
            for (qint32 x = 0; x < fields.at(fields.size() - 1)->size(); x++)
            {
                fields.at(fields.size() - 1)->at(x)->detach();
            }
            fields.at(fields.size() - 1)->clear();
            fields.removeLast();
        }
    }

    if (playerCount > currentPlayerCount)
    {
        while (playerCount > players.size())
        {
            players.append(new Player());
            players[players.size() - 1]->init();
        }
    }
    else if (playerCount < currentPlayerCount)
    {
        while (playerCount < players.size())
        {
            players.removeLast();
        }
    }

    updateSprites();
    centerMap(width / 2, heigth / 2);
}

void GameMap::flipX()
{
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    for (qint32 y = 0; y < currentHeigth; y++)
    {
        for (qint32 x = currentWidth / 2; x < currentWidth; x++)
        {
            spTerrain currentTerrain = fields[y]->at(x);
            currentTerrain->detach();
            spTerrain flipTerrain = fields.at(y)->at(currentWidth - x - 1);
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID());
            this->addChild(pTerrain);
            fields[y]->replace(x, pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);

            Building* pCurrentBuilding = flipTerrain->getBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                Building* pBuilding = new Building(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding);
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = new Unit(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
                pTerrain->setUnit(pUnit);
            }
        }
    }
    updateSprites();
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
}

void GameMap::rotateX()
{
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    for (qint32 y = 0; y < currentHeigth; y++)
    {
        for (qint32 x = currentWidth / 2; x < currentWidth; x++)
        {
            spTerrain currentTerrain = fields[y]->at(x);
            currentTerrain->detach();
            spTerrain flipTerrain = fields.at(currentHeigth - y - 1)->at(currentWidth - x - 1);
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID());
            this->addChild(pTerrain);
            fields[y]->replace(x, pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);

            Building* pCurrentBuilding = flipTerrain->getBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                Building* pBuilding = new Building(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding);
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = new Unit(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
                pTerrain->setUnit(pUnit);
            }
        }
    }
    updateSprites();
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
}

void GameMap::flipY()
{
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    for (qint32 y = currentHeigth / 2; y < currentHeigth; y++)
    {
        for (qint32 x = 0; x < currentWidth; x++)
        {
            spTerrain currentTerrain = fields[y]->at(x);
            currentTerrain->detach();
            spTerrain flipTerrain = fields.at(currentHeigth - y - 1)->at(x);
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID());
            this->addChild(pTerrain);
            fields[y]->replace(x, pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);

            Building* pCurrentBuilding = flipTerrain->getBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                Building* pBuilding = new Building(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding);
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = new Unit(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
                pTerrain->setUnit(pUnit);
            }
        }
    }
    updateSprites();
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
}

void GameMap::rotateY()
{
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    for (qint32 y = currentHeigth / 2; y < currentHeigth; y++)
    {
        for (qint32 x = 0; x < currentWidth; x++)
        {
            spTerrain currentTerrain = fields[y]->at(x);
            currentTerrain->detach();
            spTerrain flipTerrain = fields.at(currentHeigth - y - 1)->at(currentWidth - x - 1);
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID());
            this->addChild(pTerrain);
            fields[y]->replace(x, pTerrain);
            pTerrain->setPosition(x * Imagesize, y * Imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);

            Building* pCurrentBuilding = flipTerrain->getBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                Building* pBuilding = new Building(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding);
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = new Unit(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
                pTerrain->setUnit(pUnit);
            }
        }
    }
    updateSprites();
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
}
