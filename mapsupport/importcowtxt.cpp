#include <QTextStream>
#include <QFile>

#include "game/gamemap.h"

#include "game/player.h"

#include "game/building.h"

#include "game/unit.h"

#include "game/terrain.h"

#include "game/co.h"

// some local constants to map old cow stuff to this brand new cool version
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

static const qint32 buildingCount = 12;
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
                                                            {"STADT", "TOWN"}};


static const qint32 unitCount = 36;
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
                                                    {"WASSERFLUGZEUG", "WATERPLANE"}};

void GameMap::importTxtMap(QString file)
{
    if (QFile::exists(file))
    {
        // delete all data
        for (qint32 y = 0; y < heigth; y++)
        {
            for (qint32 x = 0; x < width; x++)
            {
                this->removeChild(fields.at(y)->at(x));
            }
            fields.at(y)->clear();
        }
        fields.clear();
        players.clear();

        QFile data(file);
        data.open(QFile::ReadOnly);
        QTextStream out(&data);
        while (!out.atEnd())
        {

            // we need to remap the german map stuff
            QString line = out.readLine().toUpper();
            if (line.startsWith("KARTENNAME="))
            {
                mapName = line.split("=")[1];
            }
            else if (line.startsWith("SPIELER|"))
            {
                QStringList data = line.split("|");
                players.append(new Player(players.size()));
                qint32 idx = players.size() - 1;
                players[idx]->setFondsModifier(static_cast<float>(data[1].toInt()) / 1000.0f);
                players[idx]->setFonds(data[2].toInt());
            }
            else if (line.startsWith(("GROESSE")))
            {
                QStringList data = line.split("|");
                width = data[1].toInt();
                heigth = data[2].toInt();
                for (qint32 y = 0; y < heigth; y++)
                {
                    fields.append(new QVector<spTerrain>());
                    for (qint32 x = 0; x < width; x++)
                    {
                        spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y);
                        this->addChild(pTerrain);
                        fields[y]->append(pTerrain);
                        pTerrain->setPosition(x * Imagesize, y * Imagesize);
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
                            Building* pBuilding = new Building(buildingIdMapping[i][1]);
                            qint32 player = data[5].toInt();
                            if (player > 0)
                            {
                                pBuilding->setOwner(getspPlayer(player - 1));
                            }
                            pBuilding->setFireCount(data[7].toInt());
                            pBuilding->setHp(data[8].toInt());
                            pTerrain->setBuilding(pBuilding);
                            break;
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
                            spUnit pUnit = new Unit(unitIdMapping[i][1], getspPlayer(player - 1));
                            pUnit->setFuel(data[6].toInt());
                            pUnit->setHp(data[7].toInt());
                            pTerrain->setUnit(pUnit);
                        }
                    }
                }
            }
        }
    }
    // update the whole fucking map
    updateSprites();
}
