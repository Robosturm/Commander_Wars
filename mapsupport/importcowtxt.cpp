#include <QTextStream>
#include <QFile>

#include "coreengine/console.h"

#include "game/gamemap.h"
#include "game/player.h"
#include "game/building.h"
#include "game/unit.h"
#include "game/terrain.h"
#include "game/co.h"

// some local constants to map old cow stuff to this cool version
static const qint32 terrainCount = 19;
static const QString terrainIdMapping[terrainCount][2] = {{"SEE", "SEA"},
                                              {"EBENE", "PLAINS"},
                                              {"WALD", "FOREST"},
                                              {"STRAND", "BEACH"},
                                              {"RIFF", "REAF"},
                                              {"BERG", "MOUNTAIN"},
                                              {"STRASSE", "STREET"},
                                              {"BRUECKE", "BRIDGE"},
                                              {"KAPUTTESCHWEISSNAHT", "DESTROYEDWELD"},
                                              {"FEUER", "FIRE"},
                                              {"METEOR", "METEOR"},
                                              {"PIPELINE", "PIPELINE"},
                                              {"PLASMA", "PLASMA"},
                                              {"FLUSS", "RIVER"},
                                              {"RAUESEE", "ROUGH_SEA"},
                                              {"RUINE", "RUIN"},
                                              {"MAUER", "WALL"},
                                              {"OEDLAND", "WASTELAND"},
                                              {"SCHWEISSNAHT", "WELD"}};

static const qint32 buildingCount = 31;
static const qint32 building3x3Start = 19;
static const qint32 building4x4Start = 27;
static const qint32 building3x4Start = 30;
static const QString buildingIdMapping[buildingCount][2] = {{"FLUGHAFEN", "AIRPORT"},
                                                            {"BASIS", "FACTORY"},
                                                            {"WERFT", "HARBOUR"},
                                                            {"HQ", "HQ"},
                                                            {"LABOR", "LABOR"},
                                                            {"MINE", "MINE"},
                                                            {"PIPESTATION", "PIPESTATION"},
                                                            {"RADAR", "RADAR"},
                                                            {"RAKETENSILO", "SILO"},
                                                            {"RAKETENSILO+RAKETE", "SILO_ROCKET"},
                                                            {"TURM", "TOWER"},
                                                            {"STADT", "TOWN"},
                                                            {"TEMPFLUGHAFEN", "TEMPORARY_AIRPORT"},
                                                            {"TEMPWERFT", "TEMPORARY_HARBOUR"},
                                                            {"MINIGESCHUETZ+N", "ZMINICANNON_N"},
                                                            {"MINIGESCHUETZ+S", "ZMINICANNON_S"},
                                                            {"MINIGESCHUETZ+W", "ZMINICANNON_W"},
                                                            {"MINIGESCHUETZ+O", "ZMINICANNON_E"},
                                                            {"LASER", "ZLASER"},
                                                            {"SCHOCKLASER+O", "ZDEATHRAY_E"},
                                                            {"SCHOCKLASER+S", "ZDEATHRAY_S"},
                                                            {"SCHOCKLASER+N", "ZDEATHRAY_N"},
                                                            {"SCHOCKLASER+W", "ZDEATHRAY_W"},
                                                            {"SCHWARZE GESCHUETZ+O", "ZBLACKHOLE_CANNON_E"},
                                                            {"SCHWARZE GESCHUETZ+S", "ZBLACKHOLE_CANNON_S"},
                                                            {"SCHWARZE GESCHUETZ+N", "ZBLACKHOLE_CANNON_N"},
                                                            {"SCHWARZE GESCHUETZ+W", "ZBLACKHOLE_CANNON_W"},
                                                            {"FESTUNG", "ZFORTRESS"},
                                                            {"VULKAN", "ZVOLCAN"},
                                                            {"OMEGARAKETE", "ZOMEGA_BASE"},
                                                            {"FABRIK", "ZBLACKHOLE_FACTORY"},};



static const qint32 unitCount = 38;
static const QString unitIdMapping[unitCount][2] = {{"FLTR", "AIRCRAFTCARRIER"},
                                                    {"PANZERABWEHRKANONE", "ANTITANKCANNON"},
                                                    {"TTP", "APC"},
                                                    {"ART", "ARTILLERY"},
                                                    {"SCHLFF", "BATTLESHIP"},
                                                    {"BOMBER", "BOMBER"},
                                                    {"KANONENBOOT", "CANNONBOAT"},
                                                    {"KREUZER", "CRUISER"},
                                                    {"ZERSTOERER", "DESTROYER"},
                                                    {"AUFKLAERER", "DUSTER"},
                                                    {"JAEGER", "FIGHTER"},
                                                    {"FLAK", "FLAK"},
                                                    {"FLARE", "FLARE"},
                                                    {"SCHWERESHOVERCRAFT", "HEAVY_HOVERCRAFT"},
                                                    {"KPZ", "HEAVY_TANK"},
                                                    {"HOELLIUM", "HOELLIUM"},
                                                    {"HOVERCRAFT", "HOVERCRAFT"},
                                                    {"HOVERFLAK", "HOVERFLAK"},
                                                    {"INF", "INFANTRY"},
                                                    {"K-HELI", "K_HELI"},
                                                    {"L-BOOT", "LANDER"},
                                                    {"JGPZ", "LIGHT_TANK"},
                                                    {"MECH", "MECH"},
                                                    {"MEGAPZ", "MEGATANK"},
                                                    {"MISSILE", "MISSILE"},
                                                    {"MOTORRAD", "MOTORBIKE"},
                                                    {"TITANPZ", "NEOTANK"},
                                                    {"LAEUFER", "PIPERUNNER"},
                                                    {"SPAEHER", "RECON"},
                                                    {"RKW", "ROCKETTHROWER"},
                                                    {"SSIEINHEIT", "SNIPER"},
                                                    {"TARNK", "STEALTHBOMBER"},
                                                    {"U-BOOT", "SUBMARINE"},
                                                    {"T-HELI", "T_HELI"},
                                                    {"T-FLUGZEUG", "TRANSPORTPLANE"},
                                                    {"WASSERFLUGZEUG", "WATERPLANE"},
                                                    {"S-BOOT", "BLACK_BOAT"},
                                                    {"S-BOMBE", "BLACK_BOMB"}};

void GameMap::importTxtMap(QString file)
{
    if (QFile::exists(file))
    {
        clearMap();

        QFile data(file);
        data.open(QFile::ReadOnly);
        QTextStream out(&data);
        while (!out.atEnd())
        {

            // we need to remap the german map stuff
            QString line = out.readLine().toUpper();
            if (line.startsWith("KARTENNAME="))
            {
                m_mapName = line.split("=")[1];
            }
            else if (line.startsWith("SPIELER|"))
            {
                QStringList data = line.split("|");
                m_players.append(spPlayer::create());
                qint32 idx = m_players.size() - 1;
                m_players[idx]->setFundsModifier(static_cast<float>(data[1].toInt()) / 1000.0f);
                m_players[idx]->setFunds(data[2].toInt());
                m_players[idx]->init();
            }
            else if (line.startsWith(("GROESSE")))
            {
                QStringList data = line.split("|");
                qint32 width = data[1].toInt();
                qint32 heigth = data[2].toInt();
                for (qint32 y = 0; y < heigth; y++)
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
            else if (line.startsWith("LANDSCHAFT|"))
            {
                QStringList data = line.split("|");
                if (data[3].toInt() == 0)
                {
                    QString terraindID = data[4];
                    for (qint32 i = 0; i < terrainCount; i++)
                    {
                        if (terrainIdMapping[i][0] == terraindID)
                        {
                            replaceTerrain(terrainIdMapping[i][1], data[1].toInt(), data[2].toInt());
                            getTerrain(data[1].toInt(), data[2].toInt())->setHp(data[5].toInt());
                            break;
                        }
                        else if (i == terrainCount - 1)
                        {
                            Console::print("Error unable to import terrain: " + terraindID, Console::eLogLevels::eERROR);
                        }
                    }
                }
            }
            else if (line.startsWith("GEBAEUDE|"))
            {
                QStringList data = line.split("|");
                if (data[3].toInt() == 0)
                {
                    Terrain* pTerrain = getTerrain(data[1].toInt(), data[2].toInt());
                    QString buildingID = data[4];
                    for (qint32 i = 0; i < buildingCount; i++)
                    {
                        if (buildingIdMapping[i][0] == buildingID)
                        {
                            spBuilding pBuilding = spBuilding::create(buildingIdMapping[i][1]);
                            qint32 player = data[5].toInt();
                            if (player > 0)
                            {
                                pBuilding->setOwner(getPlayer(player - 1));
                            }
                            pBuilding->setFireCount(data[7].toInt());
                            pBuilding->setHp(data[8].toInt());
                            if (i < building3x3Start)
                            {
                                pTerrain->setBuilding(pBuilding.get());
                            }
                            else if (i < building4x4Start)
                            {
                                Terrain* pTargetTerrain = getTerrain(pTerrain->Terrain::getX() + 1, pTerrain->Terrain::getY() + 1);
                                pTargetTerrain->setBuilding(pBuilding.get());
                            }
                            else if (i < building3x4Start)
                            {
                                Terrain* pTargetTerrain = getTerrain(pTerrain->Terrain::getX() + 2, pTerrain->Terrain::getY() + 1);
                                pTargetTerrain->setBuilding(pBuilding.get());
                            }
                            else
                            {
                                Terrain* pTargetTerrain = getTerrain(pTerrain->Terrain::getX() + 1, pTerrain->Terrain::getY() + 1);
                                pTargetTerrain->setBuilding(pBuilding.get());
                            }
                            break;
                        }
                        else if (i == buildingCount - 1)
                        {
                            Console::print("Error unable to import building: " + buildingID, Console::eLogLevels::eERROR);
                        }
                    }
                }
            }
            else if (line.startsWith("EINHEIT"))
            {
                QStringList data = line.split("|");
                if (data[3].toInt() == 0)
                {
                    Terrain* pTerrain = getTerrain(data[1].toInt(), data[2].toInt());
                    QString unitID = data[4];
                    for (qint32 i = 0; i < unitCount; i++)
                    {
                        if (unitIdMapping[i][0] == unitID)
                        {
                            qint32 player = data[8].toInt();
                            spUnit pUnit = spUnit::create(unitIdMapping[i][1], getPlayer(player - 1), false);
                            pUnit->setFuel(data[6].toInt());
                            pUnit->setHp(data[7].toInt());
                            pTerrain->setUnit(pUnit);
                            break;
                        }
                        else if (i == unitCount - 1)
                        {
                            Console::print("Error unable to import unit: " + unitID, Console::eLogLevels::eERROR);
                        }
                    }
                }
            }
        }
    }
    // update the whole fucking map
    updateSprites();
}
