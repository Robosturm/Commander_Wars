#include <QTextStream>
#include <QFile>
#include "QVector"

#include "coreengine/mainapp.h"
#include "coreengine/gameconsole.h"
#include "coreengine/settings.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/building.h"

#include "menue/editormenue.h"

#include "objects/loadingscreen.h"

#include "awbwReplayReader/awbwdatatypes.h"

void GameMap::importAWByWebMap(QString file)
{
    if (QFile::exists(file))
    {
        spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
        pLoadingScreen->show();
        QFile data(file);
        data.open(QFile::ReadOnly);
        QTextStream stream(&data);
        QVector<QVector<quint32>> mapIDs;
        pLoadingScreen->setProgress(tr("Reading Map File"), 5);
        while (!stream.atEnd())
        {
            QStringList ids = stream.readLine().split(",");
            mapIDs.append(QVector<quint32>());
            for (qint32 i = 0; i < ids.size(); i++)
            {
                if (ids[i].isEmpty())
                {
                    mapIDs[mapIDs.size() - 1].append(std::numeric_limits<quint32>::max());
                }
                else
                {
                    mapIDs[mapIDs.size() - 1].append(ids[i].toUInt());
                }
            }
        }
        QVector<ImporterUnitInfo> units;
        importAWByWebMap(mapIDs, units, true);
        QStringList list = file.split("/");
        m_headerInfo.m_mapName = list[list.size() - 1].remove(list[list.size() - 1].lastIndexOf("."), list[list.size() - 1].size());
    }
}

void GameMap::importAWByWebMap(const QVector<QVector<quint32>> & mapIDs, QVector<ImporterUnitInfo> units, bool optimizePlayer)
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->show();
    clearMap();

    pLoadingScreen->setProgress(tr("Creating Player"), 10);
    for (qint32 i = 0; i < 24; i++)
    {
        m_players.append(MemoryManagement::create<Player>(this));
        m_players[i]->init();
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
    // load empty map
    qint32 mapHeigth = mapIDs.size();

    m_rowSprites.reserve(mapHeigth);
    m_fields.reserve(mapHeigth);
    for (qint32 y = 0; y < mapHeigth; y++)
    {
        pLoadingScreen->setProgress(tr("Loading Empty Map Row ") + QString::number(y) + tr(" of ") + QString::number(mapHeigth), 10 + 20 * y / mapHeigth);
        m_fields.push_back(std::vector<spTerrain>(mapIDs[y].size(), spTerrain()));
        auto pActor = MemoryManagement::create<oxygine::Actor>();
        pActor->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
        m_rowSprites.push_back(pActor);
        addChild(pActor);
        for (qint32 x = 0; x < mapIDs[y].size(); x++)
        {
            spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, x, y, "", this);
            m_rowSprites[y]->addChild(pTerrain);
            m_fields[y][x] = pTerrain;
            pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
        }
    }

    for (qint32 y = 0; y < mapIDs.size(); y++)
    {
        pLoadingScreen->setProgress(tr("Importing Map Row ") + QString::number(y) + tr(" of ") + QString::number(mapHeigth), 30 + 50 * y / mapHeigth);
        for (qint32 x = 0; x < mapIDs[y].size(); x++)
        {
            const auto & info = AwbwDataTypes::TERRAIN_BUILDING_DATA[mapIDs[y][x]];
            if (!info.id.isEmpty())
            {
                if (info.owner == AwbwDataTypes::TERRAIN_OWNER)
                {
                    if (info.id == "BRIDGE1")
                    {
                        replaceTerrain("SEA", x, y, false, false, false);
                        replaceTerrain(info.id, x, y, true, false, false);
                    }
                    else
                    {
                        replaceTerrain(info.id, x, y, false, false, false);

                    }
                }
                else
                {
                    spBuilding pBuilding = MemoryManagement::create<Building>(info.id, this);
                    if (info.owner == AwbwDataTypes::NEUTRAL_OWNER)
                    {
                        pBuilding->setOwner(nullptr);
                    }
                    else
                    {
                        pBuilding->setOwner(getPlayer(info.owner));
                    }
                    getTerrain(x, y)->setBuilding(pBuilding);
                }
            }
            else
            {
                CONSOLE_PRINT(QString("Unable to load terrain id: ") + QString::number(mapIDs[y][x]) + " during importing of an awbw map", GameConsole::eERROR);
            }
        }
    }
    for (const auto & unit : units)
    {
        if (onMap(unit.x, unit.y))
        {
            spUnit pUnit = MemoryManagement::create<Unit>(unit.unitId, getPlayer(unit.player), true, this);
            getTerrain(unit.x, unit.y)->setUnit(pUnit);
        }
    }

    m_headerInfo.m_mapDescription = "";
    m_headerInfo.m_mapAuthor = Settings::getInstance()->getUsername();
    if (optimizePlayer)
    {
        optimizePlayers();
        m_players[0]->setBaseGameInput(BaseGameInputIF::createAi(this, GameEnums::AiTypes::AiTypes_Human));
        m_players[0]->setControlType(GameEnums::AiTypes::AiTypes_Human);
        // update the whole fucking map
        pLoadingScreen->setProgress(tr("Loading Sprites"), 90);
        updateSprites();
    }
    pLoadingScreen->hide();
}
