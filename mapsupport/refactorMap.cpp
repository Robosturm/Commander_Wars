#include "coreengine/mainapp.h"

#include "game/gamemap.h"
#include "game/terrain.h"
#include "game/unit.h"
#include "game/building.h"
#include "game/player.h"
#include "game/co.h"

#include "ai/coreai.h"

void GameMap::extendMap(const QString mapFile, GameEnums::Directions direction)
{
    QString file = mapFile;
    if (!QFile::exists(file))
    {
        file = oxygine::Resource::RCC_PREFIX_PATH + mapFile;
    }
    if (QFile::exists(file) &&
        (direction == GameEnums::Directions_North ||
         direction == GameEnums::Directions_South ||
         direction == GameEnums::Directions_East ||
         direction == GameEnums::Directions_West))
    {
        Mainapp::getInstance()->pauseRendering();
        {
            spGameMap pMap = MemoryManagement::create<GameMap>(file, true, false, false);
            qint32 endWidth = 0;
            qint32 endHeight = 0;
            qint32 offsetX = 0;
            qint32 offsetY = 0;
            if (direction == GameEnums::Directions_North)
            {
                resizeMap(0, pMap->getMapHeight(), 0, 0);
                endWidth = getMapWidth();
                endHeight = pMap->getMapHeight();
            }
            else if (direction == GameEnums::Directions_South)
            {
                offsetY = getMapHeight();
                resizeMap(0, 0, 0, pMap->getMapHeight());
                endWidth = getMapWidth();
                endHeight = pMap->getMapHeight();
            }
            else if (direction == GameEnums::Directions_East)
            {
                offsetX = getMapWidth();
                resizeMap(0, 0, pMap->getMapWidth(), 0);
                endWidth = pMap->getMapWidth();
                endHeight = getMapHeight();
            }
            else if (direction == GameEnums::Directions_West)
            {
                resizeMap(pMap->getMapWidth(), 0, 0, 0);
                endWidth = pMap->getMapWidth();
                endHeight = getMapHeight();
            }
            for (qint32 y = 0; y < endHeight; ++y)
            {
                for (qint32 x = 0; x < endWidth; ++x)
                {
                    qint32 targetX = x + offsetX;
                    qint32 targetY = y + offsetY;
                    if (pMap->onMap(x, y))
                    {
                        spTerrain pTerrain = pMap->m_fields[y][x];
                        pMap->m_fields[y][x] = spTerrain();
                        m_fields[targetY][targetX]->detach();
                        pTerrain->setX(targetX);
                        pTerrain->setY(targetY);
                        m_fields[targetY][targetX] = pTerrain;
                        m_rowSprites[targetY]->addChild(pTerrain);
                        pTerrain->setPosition(targetX * m_imagesize, targetY * m_imagesize);
                        pTerrain->setMapForExtending(this);
                        Unit* pUnit = pTerrain->getUnit();
                        if (pUnit != nullptr)
                        {
                            pUnit->setAiMode(pUnit->getAiMode());
                        }
                    }
                    else
                    {
                        spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, targetX, targetY, "", this);
                        m_rowSprites[y]->addChild(pTerrain);
                        m_fields[targetY][targetX] = pTerrain;
                        m_rowSprites[targetY]->addChild(pTerrain);
                        pTerrain->setPosition(targetX * m_imagesize, targetY * m_imagesize);
                    }
                }
            }
        }
        updateSprites(-1, -1, true);
        for (qint32 i = 0; i < m_players.size(); ++i)
        {
            m_players[i]->loadVisionFields();
            CoreAI* pAi = dynamic_cast<CoreAI*>(m_players[i]->getBaseGameInput());
            if (pAi != nullptr)
            {
                pAi->resetToTurnStart();
            }
        }
        m_gameRules->resetFogSprites();
        m_gameRules->createWeatherSprites();
        Mainapp::getInstance()->continueRendering();
    }
}

void GameMap::newMap(qint32 width, qint32 heigth, qint32 playerCount, const QString baseTerrain)
{
    Mainapp::getInstance()->pauseRendering();
    clearMap();

    m_fields.reserve(heigth);
    m_rowSprites.reserve(heigth);
    for (qint32 y = 0; y < heigth; y++)
    {
        m_fields.push_back(std::vector<spTerrain>(width, spTerrain()));
        auto pActor = MemoryManagement::create<oxygine::Actor>();
        pActor->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        m_rowSprites.push_back(pActor);
        addChild(pActor);
        for (qint32 x = 0; x < width; x++)
        {
            spTerrain pTerrain = Terrain::createTerrain(baseTerrain, x, y, "", this);
            m_rowSprites[y]->addChild(pTerrain);
            m_fields[y][x] = pTerrain;
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
        }
    }
    // add two players to a default map :)
    for (qint32 i = 0; i < playerCount; i++)
    {
        m_players.append(MemoryManagement::create<Player>(this));
        m_players[m_players.size() - 1]->init();
        if (i == 0)
        {
            m_players[i]->setBaseGameInput(BaseGameInputIF::createAi(this, GameEnums::AiTypes::AiTypes_Human));
            m_players[i]->setControlType(GameEnums::AiTypes::AiTypes_Human);
        }
        else
        {
            m_players[i]->setBaseGameInput(BaseGameInputIF::createAi(this, GameEnums::AiTypes::AiTypes_Normal));
            m_players[i]->setControlType(GameEnums::AiTypes::AiTypes_Normal);
        }
    }
    updateSprites(-1, -1, true);
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
                spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, x, y, "", this);
                m_rowSprites[y]->addChild(pTerrain);
                m_fields[y].push_back(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
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
                m_fields[y].pop_back();
            }
        }
    }

    if (heigth > currentHeigth)
    {
        for (qint32 y = currentHeigth; y < heigth; y++)
        {
            m_fields.push_back(std::vector<spTerrain>());
            auto pActor = MemoryManagement::create<oxygine::Actor>();
            pActor->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            m_rowSprites.push_back(pActor);
            addChild(pActor);
            for (qint32 x = 0; x < width; x++)
            {
                spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, x, y, "", this);
                m_rowSprites[y]->addChild(pTerrain);
                m_fields[y].push_back(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
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
            m_fields.pop_back();
        }
    }

    if (playerCount > currentPlayerCount)
    {
        while (playerCount > m_players.size())
        {
            m_players.append(MemoryManagement::create<Player>(this));
            m_players[m_players.size() - 1]->init();
        }
    }
    else if (playerCount < currentPlayerCount)
    {
        while (playerCount < m_players.size())
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                for (qint32 x = 0; x < width; x++)
                {
                    Terrain* pTerrain = getTerrain(x, y);
                    Building* pBuilding = pTerrain->getBuilding();
                    if (pBuilding != nullptr &&
                        pBuilding->getOwner() == m_players.last().get())
                    {
                        pBuilding->setOwner(nullptr);
                    }
                    Unit* pUnit = pTerrain->getUnit();
                    if (pUnit != nullptr &&
                        pUnit->getOwner() == m_players.last().get())
                    {
                        pTerrain->setUnit(spUnit());
                    }
                }
            }

            m_players.removeLast();
        }
    }
    updateSprites(-1, -1, true);
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
                    spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", this);
                    m_rowSprites[y]->addChild(pTerrain);
                    m_fields[y].insert(m_fields[y].begin(), pTerrain);
                }
            }
        }
        else if (left < 0)
        {
            for (qint32 i = 0; i < -left; i++)
            {
                for (qint32 y = 0; y < currentHeigth; y++)
                {
                    m_fields[y][0]->detach();                    
                    m_fields[y].erase(m_fields[y].begin());
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
                    spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", this);
                    m_rowSprites[y]->addChild(pTerrain);
                    m_fields[y].push_back(pTerrain);
                }
            }
        }
        else if (right < 0)
        {
            for (qint32 i = 0; i < -right; i++)
            {
                for (qint32 y = 0; y < currentHeigth; y++)
                {
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
                m_fields.insert(m_fields.begin(), std::vector<spTerrain>());
                auto pActor = MemoryManagement::create<oxygine::Actor>();
                m_rowSprites.push_back(pActor);
                addChild(pActor);
                for (qint32 x = 0; x < currentWidth; x++)
                {
                    spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", this);
                    m_rowSprites[i]->addChild(pTerrain);
                    m_fields[0].push_back(pTerrain);
                }
            }
        }
        else if (top < 0)
        {
            for (qint32 i = 0; i < -top; i++)
            {
                for (qint32 x = 0; x < currentWidth; x++)
                {
                    m_fields[0][x]->detach();
                }
                m_fields.erase(m_fields.begin());
                m_rowSprites[0]->detach();
                m_rowSprites.erase(m_rowSprites.cbegin());
            }
        }
    }
    if (bottom + getMapHeight() > 0)
    {
        if (bottom > 0)
        {
            for (qint32 i = 0; i < bottom; i++)
            {
                m_fields.push_back(std::vector<spTerrain>());
                auto pActor = MemoryManagement::create<oxygine::Actor>();
                m_rowSprites.push_back(pActor);
                addChild(pActor);
                for (qint32 x = 0; x < currentWidth; x++)
                {
                    spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, -1, -1, "", this);
                    m_rowSprites[i]->addChild(pTerrain);
                    m_fields.back().push_back(pTerrain);
                }
            }
        }
        else if (bottom < 0)
        {
            for (qint32 i = 0; i < -bottom; i++)
            {
                for (qint32 x = 0; x < currentWidth; x++)
                {
                    m_fields.back()[x]->detach();
                }
                m_fields.pop_back();
                m_rowSprites[m_rowSprites.size() - 1]->detach();
                m_rowSprites.pop_back();
            }
        }
    }
    currentWidth = getMapWidth();
    currentHeigth = getMapHeight();
    for (qint32 y = 0; y < currentHeigth; y++)
    {
        m_rowSprites[y]->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        for (qint32 x = 0; x < currentWidth; x++)
        {
            spTerrain pTerrain = m_fields[y][x];
            spUnit pUnit = pTerrain->getSpUnit();
            spBuilding pBuilding = pTerrain->getSpBuilding();
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
            pTerrain->setX(x);
            pTerrain->setY(y);
            pTerrain->setPriority(pTerrain->getMapTerrainDrawPriority());
            pTerrain->setUnit(pUnit);
        }
    }
    for (qint32 y = 0; y < currentHeigth; y++)
    {
        for (qint32 x = 0; x < currentWidth; x++)
        {
            spBuilding pBuilding = m_fields[y][x]->getSpBuilding();
            if (pBuilding.get() != nullptr)
            {
                qint32 buildX = pBuilding->Building::getX();
                qint32 buildY = pBuilding->Building::getY();
                if (buildX - pBuilding->getBuildingWidth() + 1 < 0 ||
                    buildY - pBuilding->getBuildingHeigth() + 1 < 0)
                {
                    m_fields[y][x]->setBuilding(spBuilding());
                }
                else
                {
                    m_fields[buildY][buildX]->setBuilding(pBuilding);
                }
            }
        }
    }
    updateSprites(-1, -1, true);
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
            spTerrain flipTerrain = m_fields.at(y)[currentWidth - x - 1];
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID(), this);
            m_rowSprites[y]->addChild(pTerrain);
            m_fields[y][x] = pTerrain;
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);

            spBuilding pCurrentBuilding = flipTerrain->getSpBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                spBuilding pBuilding = MemoryManagement::create<Building>(pCurrentBuilding->getBuildingID(), this);
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding);
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = MemoryManagement::create<Unit>(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false, this);
                pTerrain->setUnit(pUnit);
                pUnit->setAiMode(pCurrentUnit->getAiMode());
            }
        }
    }
    updateSprites(-1, -1, true);
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
            spTerrain flipTerrain = m_fields[currentHeigth - y - 1][currentWidth - x - 1];
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID(), this);
            m_rowSprites[y]->addChild(pTerrain);
            m_fields[y][x] = pTerrain;
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);

            spBuilding pCurrentBuilding = flipTerrain->getSpBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                spBuilding pBuilding = MemoryManagement::create<Building>(pCurrentBuilding->getBuildingID(), this);
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding);
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = MemoryManagement::create<Unit>(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false, this);
                pTerrain->setUnit(pUnit);
                pUnit->setAiMode(pCurrentUnit->getAiMode());
            }
        }
    }
    updateSprites(-1, -1, true);
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
            spTerrain flipTerrain = m_fields[currentHeigth - y - 1][x];
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID(), this);
            m_rowSprites[y]->addChild(pTerrain);
            m_fields[y][x] = pTerrain;
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);

            spBuilding pCurrentBuilding = flipTerrain->getSpBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                spBuilding pBuilding = MemoryManagement::create<Building>(pCurrentBuilding->getBuildingID(), this);
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding);
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = MemoryManagement::create<Unit>(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false, this);
                pTerrain->setUnit(pUnit);
                pUnit->setAiMode(pCurrentUnit->getAiMode());
            }
        }
    }
    updateSprites(-1, -1, true);
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
            spTerrain flipTerrain = m_fields[currentHeigth - y - 1][currentWidth - x - 1];
            spTerrain pTerrain = Terrain::createTerrain(flipTerrain->getTerrainID(), x, y, flipTerrain->getBaseTerrainID(), this);
            m_rowSprites[y]->addChild(pTerrain);
            m_fields[y][x] = pTerrain;
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);

            spBuilding pCurrentBuilding = flipTerrain->getSpBuilding();
            if (flipTerrain->getBuilding() != nullptr)
            {
                spBuilding pBuilding = MemoryManagement::create<Building>(pCurrentBuilding->getBuildingID(), this);
                pBuilding->setOwner(pCurrentBuilding->getOwner());
                pTerrain->setBuilding(pBuilding);
            }

            spUnit pCurrentUnit = flipTerrain->getSpUnit();
            if (pCurrentUnit.get() != nullptr)
            {
                spUnit pUnit = MemoryManagement::create<Unit>(pCurrentUnit->getUnitID(), pCurrentUnit->getOwner(), false, this);
                pTerrain->setUnit(pUnit);
                pUnit->setAiMode(pCurrentUnit->getAiMode());
            }
        }
    }
    updateSprites(-1, -1, true);
    centerMap(getMapWidth() / 2, getMapHeight() / 2);
    Mainapp::getInstance()->continueRendering();
}
