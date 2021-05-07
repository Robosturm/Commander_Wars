#include "game/gamemap.h"
#include "game/terrain.h"
#include "game/unit.h"
#include "game/building.h"
#include "game/player.h"
#include "game/co.h"

void GameMap::newMap(qint32 width, qint32 heigth, qint32 playerCount, QString baseTerrain)
{
    Mainapp::getInstance()->pauseRendering();
    clearMap();
    for (qint32 y = 0; y < heigth; y++)
    {
        m_fields.append(QVector<spTerrain>());
        for (qint32 x = 0; x < width; x++)
        {
            spTerrain pTerrain = Terrain::createTerrain(baseTerrain, x, y, "");
            this->addChild(pTerrain);
            m_fields[y].append(pTerrain);
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        }
    }
    // add two players to a default map :)
    for (qint32 i = 0; i < playerCount; i++)
    {
        m_players.append(spPlayer::create());
        m_players[m_players.size() - 1]->init();
    }

    updateSprites();
    centerMap(width / 2, heigth / 2);
    Mainapp::getInstance()->continueRendering();
}

void GameMap::changeMap(qint32 width, qint32 heigth, qint32 playerCount)
{
    Mainapp::getInstance()->pauseRendering();
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
                m_fields[y].append(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
                pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            }
        }
    }
    else if (width < currentWidth)
    {
        for (qint32 y = 0; y < currentHeigth; y++)
        {
            while (width < m_fields[y].size())
            {
                m_fields[y][m_fields[y].size() - 1]->detach();
                m_fields[y].removeLast();
            }
        }
    }

    if (heigth > currentHeigth)
    {
        for (qint32 y = currentHeigth; y < heigth; y++)
        {
            m_fields.append(QVector<spTerrain>());
            for (qint32 x = 0; x < width; x++)
            {
                spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y, "");
                this->addChild(pTerrain);
                m_fields[y].append(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
                pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            }
        }
    }
    else if (heigth < currentHeigth)
    {
        while (heigth < m_fields.size())
        {
            for (qint32 x = 0; x < m_fields[m_fields.size() - 1].size(); x++)
            {
                m_fields[m_fields.size() - 1][x]->detach();
            }
            m_fields[m_fields.size() - 1].clear();
            m_fields.removeLast();
        }
    }

    if (playerCount > currentPlayerCount)
    {
        while (playerCount > m_players.size())
        {
            m_players.append(spPlayer::create());
            m_players[m_players.size() - 1]->init();
        }
    }
    else if (playerCount < currentPlayerCount)
    {
        while (playerCount < m_players.size())
        {
            m_players.removeLast();
        }
    }

    updateSprites();
    centerMap(width / 2, heigth / 2);
    Mainapp::getInstance()->continueRendering();
}

void GameMap::resizeMap(qint32 left, qint32 top, qint32 right, qint32 bottom)
{
    Mainapp::getInstance()->pauseRendering();
    qint32 currentHeigth = getMapHeight();
    if (left + getMapWidth() > 0)
    {
        if (left > 0)
        {
            for (qint32 i = 0; i < left; i++)
            {
                for (qint32 y = 0; y < currentHeigth; y++)
                {
                    spTerrain pTerrain = Terrain::createTerrain("PLAINS", -1, -1, "");
                    this->addChild(pTerrain);
                    m_fields[y].push_front(pTerrain);
                }
            }
        }
        else if (left < 0)
        {
            for (qint32 i = 0; i < -left; i++)
            {
                for (qint32 y = 0; y < currentHeigth; y++)
                {
                    if (m_fields[y][0]->getUnit() != nullptr)
                    {
                        m_fields[y][0]->getUnit()->detach();
                    }
                    m_fields[y][0]->detach();                    
                    m_fields[y].pop_front();
                }
            }
        }
    }
    if (right + getMapWidth() > 0)
    {
        if (right > 0)
        {
            for (qint32 i = 0; i < right; i++)
            {
                for (qint32 y = 0; y < currentHeigth; y++)
                {
                    spTerrain pTerrain = Terrain::createTerrain("PLAINS", -1, -1, "");
                    this->addChild(pTerrain);
                    m_fields[y].append(pTerrain);
                }
            }
        }
        else if (right < 0)
        {
            for (qint32 i = 0; i < -right; i++)
            {
                for (qint32 y = 0; y < currentHeigth; y++)
                {
                    if (m_fields[y].back()->getUnit() != nullptr)
                    {
                        m_fields[y].back()->getUnit()->detach();
                    }
                    m_fields[y].back()->detach();
                    m_fields[y].pop_back();
                }
            }
        }
    }
    qint32 currentWidth = getMapWidth();
    if (top + getMapHeight() > 0)
    {
        if (top > 0)
        {
            for (qint32 i = 0; i < top; i++)
            {
                m_fields.push_front(QVector<spTerrain>());
                for (qint32 x = 0; x < currentWidth; x++)
                {
                    spTerrain pTerrain = Terrain::createTerrain("PLAINS", -1, -1, "");
                    this->addChild(pTerrain);
                    m_fields[0].append(pTerrain);
                }
            }
        }
        else if (top < 0)
        {
            for (qint32 i = 0; i < -top; i++)
            {
                for (qint32 x = 0; x < currentWidth; x++)
                {
                    if (m_fields[0][x]->getUnit() != nullptr)
                    {
                        m_fields[0][x]->getUnit()->detach();
                    }
                    m_fields[0][x]->detach();
                }
                m_fields.pop_front();
            }
        }
    }
    if (bottom + getMapHeight() > 0)
    {
        if (bottom > 0)
        {
            for (qint32 i = 0; i < bottom; i++)
            {
                m_fields.append(QVector<spTerrain>());
                for (qint32 x = 0; x < currentWidth; x++)
                {
                    spTerrain pTerrain = Terrain::createTerrain("PLAINS", -1, -1, "");
                    this->addChild(pTerrain);
                    m_fields.back().append(pTerrain);
                }
            }
        }
        else if (bottom < 0)
        {
            for (qint32 i = 0; i < -bottom; i++)
            {
                for (qint32 x = 0; x < currentWidth; x++)
                {
                    if (m_fields.back()[x]->getUnit() != nullptr)
                    {
                        m_fields.back()[x]->getUnit()->detach();
                    }
                    m_fields.back()[x]->detach();
                }
                m_fields.pop_back();
            }
        }
    }
    currentWidth = getMapWidth();
    currentHeigth = getMapHeight();
    for (qint32 y = 0; y < currentHeigth; y++)
    {
        for (qint32 x = 0; x < currentWidth; x++)
        {
            spTerrain pTerrain = m_fields[y][x];
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            pTerrain->setX(x);
            pTerrain->setY(y);
        }
    }
    for (qint32 y = 0; y < currentHeigth; y++)
    {
        for (qint32 x = 0; x < currentWidth; x++)
        {
            spBuilding pBuilding = m_fields[y][x]->getBuilding();
            if (pBuilding.get() != nullptr)
            {
                if (pBuilding->Building::getX() - pBuilding->getBuildingWidth() + 1 < 0 ||
                    pBuilding->Building::getY() - pBuilding->getBuildingHeigth() + 1 < 0)
                {
                    m_fields[y][x]->setBuilding(nullptr);
                }
            }
        }
    }
    updateSprites();
    centerMap(currentWidth / 2, currentHeigth / 2);
    Mainapp::getInstance()->continueRendering();
}

void GameMap::flipX()
{
    Mainapp::getInstance()->pauseRendering();
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    for (qint32 y = 0; y < currentHeigth; y++)
    {
        for (qint32 x = currentWidth / 2; x < currentWidth; x++)
        {
            spTerrain currentTerrain = m_fields[y][x];
            currentTerrain->detach();
            if (currentTerrain->getUnit() != nullptr)
            {
                currentTerrain->getUnit()->detach();
            }
            spTerrain flipTerrain = m_fields.at(y)[currentWidth - x - 1];
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID());
            this->addChild(pTerrain);
            m_fields[y].replace(x, pTerrain);
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);

            spBuilding pCurrentBuilding = flipTerrain->getBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                spBuilding pBuilding = spBuilding::create(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding.get());
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = spUnit::create(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
                pTerrain->setUnit(pUnit);
            }
        }
    }
    updateSprites();
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
    Mainapp::getInstance()->continueRendering();
}

void GameMap::rotateX()
{
    Mainapp::getInstance()->pauseRendering();
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    for (qint32 y = 0; y < currentHeigth; y++)
    {
        for (qint32 x = currentWidth / 2; x < currentWidth; x++)
        {
            spTerrain currentTerrain = m_fields[y][x];
            currentTerrain->detach();
            if (currentTerrain->getUnit() != nullptr)
            {
                currentTerrain->getUnit()->detach();
            }
            spTerrain flipTerrain = m_fields[currentHeigth - y - 1][currentWidth - x - 1];
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID());
            this->addChild(pTerrain);
            m_fields[y].replace(x, pTerrain);
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);

            spBuilding pCurrentBuilding = flipTerrain->getBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                spBuilding pBuilding = spBuilding::create(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding.get());
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = spUnit::create(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
                pTerrain->setUnit(pUnit);
            }
        }
    }
    updateSprites();
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
    Mainapp::getInstance()->continueRendering();
}

void GameMap::flipY()
{
    Mainapp::getInstance()->pauseRendering();
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    for (qint32 y = currentHeigth / 2; y < currentHeigth; y++)
    {
        for (qint32 x = 0; x < currentWidth; x++)
        {
            spTerrain currentTerrain = m_fields[y][x];
            currentTerrain->detach();
            if (currentTerrain->getUnit() != nullptr)
            {
                currentTerrain->getUnit()->detach();
            }
            spTerrain flipTerrain = m_fields[currentHeigth - y - 1][x];
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID());
            this->addChild(pTerrain);
            m_fields[y].replace(x, pTerrain);
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);

            spBuilding pCurrentBuilding = flipTerrain->getBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                spBuilding pBuilding = spBuilding::create(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding.get());
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = spUnit::create(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
                pTerrain->setUnit(pUnit);
            }
        }
    }
    updateSprites();
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
    Mainapp::getInstance()->continueRendering();
}

void GameMap::rotateY()
{
    Mainapp::getInstance()->pauseRendering();
    qint32 currentHeigth = getMapHeight();
    qint32 currentWidth = getMapWidth();
    for (qint32 y = currentHeigth / 2; y < currentHeigth; y++)
    {
        for (qint32 x = 0; x < currentWidth; x++)
        {
            spTerrain currentTerrain = m_fields[y][x];
            currentTerrain->detach();
            if (currentTerrain->getUnit() != nullptr)
            {
                currentTerrain->getUnit()->detach();
            }
            spTerrain flipTerrain = m_fields[currentHeigth - y - 1][currentWidth - x - 1];
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID());
            this->addChild(pTerrain);
            m_fields[y].replace(x, pTerrain);
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);

            spBuilding pCurrentBuilding = flipTerrain->getBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                spBuilding pBuilding = spBuilding::create(pCurrentBuilding->getBuildingID());
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding.get());
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = spUnit::create(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false);
                pTerrain->setUnit(pUnit);
            }
        }
    }
    updateSprites();
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
    Mainapp::getInstance()->continueRendering();
}
