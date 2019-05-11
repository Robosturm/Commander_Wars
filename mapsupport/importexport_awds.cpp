#include <QDataStream>
#include <QFile>

#include "coreengine/console.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "game/building.h"

#include "game/unit.h"

#include "menue/editormenue.h"

void GameMap::importAWDSMap(QString file)
{
    quint8 sign = 0;
    if (QFile::exists(file))
    {
        clearMap();

        QFile data(file);
        data.open(QFile::ReadOnly);
        QDataStream stream(&data);
        for (qint32 i = 0; i < 10; i++)
        {
            stream >> sign;
        }
        stream >> sign;
        qint32 width = sign;
        stream >> sign;
        qint32 heigth = sign;
        // load 5 players :)
        for (qint32 i = 0; i < 5; i++)
        {
            players.append(new Player());
            players[i]->init();
        }
        // read byte for terrain style which we ignore
        stream >> sign;
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
        quint8 terrainChar1 = 0;
        quint8 terrainChar2 = 0;
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                stream >> terrainChar1;
                stream >> terrainChar2;
                switch (terrainChar1)
                {
                    case 0: // plains
                    {
                        switch (terrainChar2)
                        {
                            case 4: // monolith
                            {
                                Building* pBuilding = new Building("ZMONOLITH");
                                pBuilding->setOwner(getPlayer(4));
                                getTerrain(x + 1, y + 1)->setBuilding(pBuilding);
                                break;
                            }
                            default:
                            {
                                // do nothing
                            }
                        }
                        break;
                    }
                    case 1: // Street
                    {
                        switch (terrainChar2)
                        {
                            case 4: // monolith
                            {
                                break;
                            }
                            default:
                            {
                                replaceTerrain("STREET", x, y);
                            }
                        }
                        break;
                    }
                    case 2: // Bridge
                    {
                        replaceTerrain("BRIDGE", x, y);
                        break;
                    }
                    case 3: // River
                    {
                        if (terrainChar2 == 0)
                        {
                            replaceTerrain("RIVER", x, y);
                        }
                        break;
                    }
                    case 4: // Fortress
                    {
                        Building* pBuilding = new Building("ZFORTRESS");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x + 2, y + 1)->setBuilding(pBuilding);
                        break;
                    }
                    case 5:
                    case 6:
                    case 7:
                    case 8: // Flying Fortress
                    case 9:
                    case 10:
                    {
                        // do nothing
                        break;
                    }
                    case 11: // Sea
                    {
                        replaceTerrain("SEA", x, y);
                        break;
                    }
                    case 12: // Flying Fortress
                    {
                        break;
                    }
                    case 13: // Sea
                    case 14: // Sea
                    {
                        replaceTerrain("SEA", x, y);
                        break;
                    }
                    case 16: // Pipeline
                    {
                        if (terrainChar2 == 0)
                        {
                            replaceTerrain("PIPELINE", x, y);
                        }
                        break;
                    }
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 23:
                    case 24:
                    case 25:
                    case 26:
                    case 27:
                    case 28:
                    case 29:
                    {
                        // do nothing
                        break;
                    }
                    case 30: //Reaf
                    {
                        if (terrainChar2 != 4)
                        {
                            replaceTerrain("REAF", x, y);
                        }
                        break;
                    }
                    case 31: // Sea
                    {
                        replaceTerrain("SEA", x, y);
                        break;
                    }
                    case 32: // Sea
                    {
                        if (terrainChar2 == 0)
                        {
                            replaceTerrain("BRIDGE", x, y);
                        }
                        else
                        {
                            replaceTerrain("SEA", x, y);
                        }
                        break;
                    }
                    case 33: // Sea
                    case 34: // Sea
                    {
                        replaceTerrain("SEA", x, y);
                        break;
                    }
                    case 39:
                    {
                        replaceTerrain("BEACH", x, y);
                        break;
                    }
                    case 44:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 45:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 46:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 47:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 48:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 49:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 50:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 54:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 55:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 56:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 57:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 58:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 59:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 60:
                    {
                        switch (terrainChar2)
                        {
                            case 0:
                            {
                                replaceTerrain("SEA", x, y);
                                break;
                            }
                            case 1:
                            {
                                Building* pBuilding = new Building("LABOR");
                                pBuilding->setOwner(getPlayer(1));
                                getTerrain(x, y)->setBuilding(pBuilding);
                                break;
                            }
                        }
                        break;
                    }
                    case 64:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 65:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 66:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 67:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 68:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 69:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 70:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 74:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 75:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 76:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 77:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 78:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 79:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 80:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 84:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 85:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 86:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 87:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 88:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 89:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 90:
                    {
                        switch (terrainChar2)
                        {
                            case 0:
                            {
                                replaceTerrain("FOREST", x, y);
                                break;
                            }
                            case 1:
                            {
                                Building* pBuilding = new Building("LABOR");
                                pBuilding->setOwner(getPlayer(4));
                                getTerrain(x, y)->setBuilding(pBuilding);
                                break;
                            }
                        }
                        break;
                    }

                    case 94:
                    {
                        Building* pBuilding = new Building("SILO_ROCKET");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 95:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 96:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 97:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 98:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 99:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 100:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 132:
                    {
                        Building* pBuilding = new Building("ZMINICANNON_N");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 133:
                    {
                        Building* pBuilding = new Building("ZMINICANNON_W");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 134:
                    {
                        Building* pBuilding = new Building("ZLASER");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 139:
                    case 140:
                    case 141:
                    case 142:
                    case 143:
                    case 144:
                    case 145:
                    case 146:
                    {
                        // do nothing
                        break;
                    }
                    case 150: // Mountain
                    {
                        replaceTerrain("MOUNTAIN", x, y);
                        break;
                    }
                    case 152:
                    {
                        Building* pBuilding = new Building("ZMINICANNON_E");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 153:
                    {
                        Building* pBuilding = new Building("ZMINICANNON_S");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 155:
                    {
                        Building* pBuilding = new Building("ZCRYSTALL");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 159:
                    case 160:
                    case 161:
                    case 162:
                    case 163:
                    case 164:
                    case 165:
                    case 166:
                    {
                        // do nothing
                        break;
                    }
                    case 167: // PLAINS_DESTROYED
                    {
                        if (terrainChar2 == 0)
                        {
                            replaceTerrain("PLAINS_DESTROYED", x, y);
                        }
                        break;
                    }
                    case 172:
                    case 173:
                    case 174:
                    case 175: // Black Hole Cannon
                    case 176: // Black Hole Cannon
                    case 177: // Black Hole Cannon
                    case 179:
                    {
                        // do nothing
                        break;
                    }
                    case 180:
                    {
                        Building* pBuilding = new Building("ZVOLCAN");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x + 2, y + 1)->setBuilding(pBuilding);
                        break;
                    }
                    case 181:
                    case 182:
                    case 183:
                    {
                        // do nothing
                        break;
                    }
                    case 184:
                    {
                        Building* pBuilding = new Building("ZOMEGA_BASE");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x + 2, y + 1)->setBuilding(pBuilding);
                        break;
                    }
                    case 185:
                    case 186:
                    case 192:
                    {
                        // do nothing
                        break;
                    }
                    case 193:
                    {
                        Building* pBuilding = new Building("ZBLACKHOLE_CANNON_S");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x + 1, y + 1)->setBuilding(pBuilding);
                        break;
                    }
                    case 194:
                    case 195:
                    {
                        // do nothing
                        break;
                    }
                    case 196:
                    {
                        Building* pBuilding = new Building("ZBLACKHOLE_CANNON_N");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x + 1, y + 1)->setBuilding(pBuilding);
                        break;
                    }
                    case 197: // Black Hole Cannon
                    case 199:
                    case 200:
                    case 201:
                    case 202:
                    case 203:
                    case 204:
                    case 205:
                    case 206:
                    case 212:
                    case 213:
                    case 214:
                    case 215: // Black Hole Cannon
                    case 216: // Black Hole Cannon
                    case 217: // Black Hole Cannon
                    case 219:
                    case 220:
                    case 221:
                    case 222:
                    case 223:
                    case 224:
                    case 225:
                    {
                        // do nothing
                        break;
                    }
                    case 226: // Weld
                    {
                        if (terrainChar2 != 3)
                        {
                            replaceTerrain("WELD", x, y);
                        }
                        break;
                    }
                    case 227: // Sea
                    case 228: // Sea
                    case 229: // Sea
                    case 230: // Sea
                    {
                        replaceTerrain("SEA", x, y);
                        break;
                    }
                    case 232:
                    case 233:
                    case 234:
                    case 235:
                    case 236:
                    case 237:
                    case 239:
                    case 240:
                    case 241:
                    case 242:
                    case 243:
                    case 244:
                    case 245:
                    case 246:
                    {
                        // do nothing
                        break;
                    }
                    case 247: // Sea
                    case 248: // Sea
                    case 249: // Sea
                    case 250: // Sea
                    {
                        replaceTerrain("SEA", x, y);
                        break;
                    }
                    case 252:
                    {
                        // do nothing
                        break;
                    }
                    case 253:
                    {
                        Building* pBuilding = new Building("ZDEATHRAY_S");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x + 1, y + 1)->setBuilding(pBuilding);
                        break;
                    }
                    case 254:
                    case 255:
                    {
                        // do nothing
                        break;
                    }
                }
            }
        }

        // create units
        for (qint32 x = 0; x < width; x++)
        {
            for (qint32 y = 0; y < heigth; y++)
            {
                stream >> terrainChar1;
                stream >> terrainChar2;
                if (terrainChar1 == 255 && terrainChar2 == 255)
                {
                    continue;
                }
                switch (terrainChar1)
                {
                    // Orange Star
                    case 244:
                    {
                        spUnit pUnit = new Unit("INFANTRY", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 245:
                    {
                        spUnit pUnit = new Unit("HEAVY_TANK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 246:
                    {
                        spUnit pUnit = new Unit("RECON", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 247:
                    {
                        spUnit pUnit = new Unit("ARTILLERY", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 248:
                    {
                        spUnit pUnit = new Unit("FLAK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 249:
                    {
                        spUnit pUnit = new Unit("FIGHTER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 250:
                    {
                        spUnit pUnit = new Unit("K_HELI", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 251:
                    {
                        spUnit pUnit = new Unit("BATTLESHIP", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 252:
                    {
                        spUnit pUnit = new Unit("LANDER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 253:
                    {
                        spUnit pUnit = new Unit("NEOTANK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 254:
                    {
                        spUnit pUnit = new Unit("MEGATANK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 255:
                    {
                        spUnit pUnit = new Unit("PIPERUNNER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 0:
                    {
                        spUnit pUnit = new Unit("HOELLIUM", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 8:
                    {
                        spUnit pUnit = new Unit("MECH", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 9:
                    {
                        spUnit pUnit = new Unit("LIGHT_TANK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 10:
                    {
                        spUnit pUnit = new Unit("APC", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 11:
                    {
                        spUnit pUnit = new Unit("ROCKETTHROWER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 12:
                    {
                        spUnit pUnit = new Unit("MISSILE", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 13:
                    {
                        spUnit pUnit = new Unit("BOMBER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 14:
                    {
                        spUnit pUnit = new Unit("T_HELI", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 15:
                    {
                        spUnit pUnit = new Unit("CRUISER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 16:
                    {
                        spUnit pUnit = new Unit("SUBMARINE", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 17:
                    {
                        spUnit pUnit = new Unit("BLACK_BOAT", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 18:
                    {
                        spUnit pUnit = new Unit("AIRCRAFTCARRIER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 19:
                    {
                        spUnit pUnit = new Unit("STEALTHBOMBER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 20:
                    {
                        spUnit pUnit = new Unit("BLACK_BOMB", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    // Blue Moon
                    case 28:
                    {
                        spUnit pUnit = new Unit("INFANTRY", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 29:
                    {
                        spUnit pUnit = new Unit("HEAVY_TANK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 30:
                    {
                        spUnit pUnit = new Unit("RECON", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 31:
                    {
                        spUnit pUnit = new Unit("ARTILLERY", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 32:
                    {
                        spUnit pUnit = new Unit("FLAK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 33:
                    {
                        spUnit pUnit = new Unit("FIGHTER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 34:
                    {
                        spUnit pUnit = new Unit("K_HELI", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 35:
                    {
                        spUnit pUnit = new Unit("BATTLESHIP", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 36:
                    {
                        spUnit pUnit = new Unit("LANDER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 37:
                    {
                        spUnit pUnit = new Unit("NEOTANK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 38:
                    {
                        spUnit pUnit = new Unit("MEGATANK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 39:
                    {
                        spUnit pUnit = new Unit("PIPERUNNER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 40:
                    {
                        spUnit pUnit = new Unit("HOELLIUM", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 48:
                    {
                        spUnit pUnit = new Unit("MECH", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 49:
                    {
                        spUnit pUnit = new Unit("LIGHT_TANK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 50:
                    {
                        spUnit pUnit = new Unit("APC", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 51:
                    {
                        spUnit pUnit = new Unit("ROCKETTHROWER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 52:
                    {
                        spUnit pUnit = new Unit("MISSILE", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 53:
                    {
                        spUnit pUnit = new Unit("BOMBER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 54:
                    {
                        spUnit pUnit = new Unit("T_HELI", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 55:
                    {
                        spUnit pUnit = new Unit("CRUISER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 56:
                    {
                        spUnit pUnit = new Unit("SUBMARINE", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 57:
                    {
                        spUnit pUnit = new Unit("BLACK_BOAT", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 58:
                    {
                        spUnit pUnit = new Unit("AIRCRAFTCARRIER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 59:
                    {
                        spUnit pUnit = new Unit("STEALTHBOMBER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 60:
                    {
                        spUnit pUnit = new Unit("BLACK_BOMB", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    // Green Earth
                    case 68:
                    {
                        spUnit pUnit = new Unit("INFANTRY", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 69:
                    {
                        spUnit pUnit = new Unit("HEAVY_TANK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 70:
                    {
                        spUnit pUnit = new Unit("RECON", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 71:
                    {
                        spUnit pUnit = new Unit("ARTILLERY", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 72:
                    {
                        spUnit pUnit = new Unit("FLAK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 73:
                    {
                        spUnit pUnit = new Unit("FIGHTER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 74:
                    {
                        spUnit pUnit = new Unit("K_HELI", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 75:
                    {
                        spUnit pUnit = new Unit("BATTLESHIP", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 76:
                    {
                        spUnit pUnit = new Unit("LANDER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 77:
                    {
                        spUnit pUnit = new Unit("NEOTANK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 78:
                    {
                        spUnit pUnit = new Unit("MEGATANK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 79:
                    {
                        spUnit pUnit = new Unit("PIPERUNNER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 80:
                    {
                        spUnit pUnit = new Unit("HOELLIUM", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 88:
                    {
                        spUnit pUnit = new Unit("MECH", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 89:
                    {
                        spUnit pUnit = new Unit("LIGHT_TANK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 90:
                    {
                        spUnit pUnit = new Unit("APC", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 91:
                    {
                        spUnit pUnit = new Unit("ROCKETTHROWER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 92:
                    {
                        spUnit pUnit = new Unit("MISSILE", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 93:
                    {
                        spUnit pUnit = new Unit("BOMBER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 94:
                    {
                        spUnit pUnit = new Unit("T_HELI", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 95:
                    {
                        spUnit pUnit = new Unit("CRUISER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 96:
                    {
                        spUnit pUnit = new Unit("SUBMARINE", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 97:
                    {
                        spUnit pUnit = new Unit("BLACK_BOAT", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 98:
                    {
                        spUnit pUnit = new Unit("AIRCRAFTCARRIER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 99:
                    {
                        spUnit pUnit = new Unit("STEALTHBOMBER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 100:
                    {
                        spUnit pUnit = new Unit("BLACK_BOMB", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    // Yellow Comet
                    case 108:
                    {
                        spUnit pUnit = new Unit("INFANTRY", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 109:
                    {
                        spUnit pUnit = new Unit("HEAVY_TANK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 110:
                    {
                        spUnit pUnit = new Unit("RECON", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 111:
                    {
                        spUnit pUnit = new Unit("ARTILLERY", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 112:
                    {
                        spUnit pUnit = new Unit("FLAK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 113:
                    {
                        spUnit pUnit = new Unit("FIGHTER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 114:
                    {
                        spUnit pUnit = new Unit("K_HELI", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 115:
                    {
                        spUnit pUnit = new Unit("BATTLESHIP", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 116:
                    {
                        spUnit pUnit = new Unit("LANDER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 117:
                    {
                        spUnit pUnit = new Unit("NEOTANK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 118:
                    {
                        spUnit pUnit = new Unit("MEGATANK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 119:
                    {
                        spUnit pUnit = new Unit("PIPERUNNER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 120:
                    {
                        spUnit pUnit = new Unit("HOELLIUM", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 128:
                    {
                        spUnit pUnit = new Unit("MECH", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 129:
                    {
                        spUnit pUnit = new Unit("LIGHT_TANK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 130:
                    {
                        spUnit pUnit = new Unit("APC", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 131:
                    {
                        spUnit pUnit = new Unit("ROCKETTHROWER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 132:
                    {
                        spUnit pUnit = new Unit("MISSILE", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 133:
                    {
                        spUnit pUnit = new Unit("BOMBER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 134:
                    {
                        spUnit pUnit = new Unit("T_HELI", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 135:
                    {
                        spUnit pUnit = new Unit("CRUISER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 136:
                    {
                        spUnit pUnit = new Unit("SUBMARINE", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 137:
                    {
                        spUnit pUnit = new Unit("BLACK_BOAT", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 138:
                    {
                        spUnit pUnit = new Unit("AIRCRAFTCARRIER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 139:
                    {
                        spUnit pUnit = new Unit("STEALTHBOMBER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 140:
                    {
                        spUnit pUnit = new Unit("BLACK_BOMB", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    // Black Hole
                    case 148:
                    {
                        spUnit pUnit = new Unit("INFANTRY", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 149:
                    {
                        spUnit pUnit = new Unit("HEAVY_TANK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 150:
                    {
                        spUnit pUnit = new Unit("RECON", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 151:
                    {
                        spUnit pUnit = new Unit("ARTILLERY", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 152:
                    {
                        spUnit pUnit = new Unit("FLAK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 153:
                    {
                        spUnit pUnit = new Unit("FIGHTER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 154:
                    {
                        spUnit pUnit = new Unit("K_HELI", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 155:
                    {
                        spUnit pUnit = new Unit("BATTLESHIP", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 156:
                    {
                        spUnit pUnit = new Unit("LANDER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 157:
                    {
                        spUnit pUnit = new Unit("NEOTANK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 158:
                    {
                        spUnit pUnit = new Unit("MEGATANK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 159:
                    {
                        spUnit pUnit = new Unit("PIPERUNNER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 160:
                    {
                        spUnit pUnit = new Unit("HOELLIUM", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 168:
                    {
                        spUnit pUnit = new Unit("MECH", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 169:
                    {
                        spUnit pUnit = new Unit("LIGHT_TANK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 170:
                    {
                        spUnit pUnit = new Unit("APC", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 171:
                    {
                        spUnit pUnit = new Unit("ROCKETTHROWER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 172:
                    {
                        spUnit pUnit = new Unit("MISSILE", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 173:
                    {
                        spUnit pUnit = new Unit("BOMBER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 174:
                    {
                        spUnit pUnit = new Unit("T_HELI", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 175:
                    {
                        spUnit pUnit = new Unit("CRUISER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 176:
                    {
                        spUnit pUnit = new Unit("SUBMARINE", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 177:
                    {
                        spUnit pUnit = new Unit("BLACK_BOAT", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 178:
                    {
                        spUnit pUnit = new Unit("AIRCRAFTCARRIER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 179:
                    {
                        spUnit pUnit = new Unit("STEALTHBOMBER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 180:
                    {
                        spUnit pUnit = new Unit("BLACK_BOMB", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                }
            }
        }

        qint32 stringLength = 0;
        for (qint32 i = 0; i < 4; i++)
        {
            stream >> sign;
            if (i > 0)
            {
                stringLength += 255 * i * sign;
            }
            else
            {
                stringLength += sign;
            }
        }
        mapName = "";
        for (qint32 i = 0; i < stringLength; i++)
        {
            stream >> sign;
            mapName += static_cast<char>(sign);
        }
        stringLength = 0;
        for (qint32 i = 0; i < 4; i++)
        {
            stream >> sign;
            if (i > 0)
            {
                stringLength += 255 * i * sign;
            }
            else
            {
                stringLength += sign;
            }
        }
        mapAuthor = "";
        for (qint32 i = 0; i < stringLength; i++)
        {
            stream >> sign;
            mapAuthor += static_cast<char>(sign);
        }
        stringLength = 0;
        for (qint32 i = 0; i < 4; i++)
        {
            stream >> sign;
            if (i > 0)
            {
                stringLength += 255 * i * sign;
            }
            else
            {
                stringLength += sign;
            }
        }
        mapDescription = "";
        for (qint32 i = 0; i < stringLength; i++)
        {
            stream >> sign;
            mapDescription += static_cast<char>(sign);
        }
        mapDescription = mapDescription.replace("\n", " ");
        EditorMenue::getInstance()->optimizePlayers();
        // update the whole fucking map
        updateSprites();
    }
}
void GameMap::exportAWDSMap(QString)
{
//    If Objektverwalter.Spielfeld.Spieler.Length <= 6 Then
//                Using Writer As New IO.FileStream(File, IO.FileMode.OpenOrCreate, IO.FileAccess.Write, IO.FileShare.Write)
//                    'Zuerst mal den Start weglesen brauchen wir nicht!
//                    Writer.WriteByte(4 * 16 + 1)
//                    Writer.WriteByte(5 * 16 + 7)
//                    Writer.WriteByte(5 * 16 + 3)
//                    Writer.WriteByte(4 * 16 + 13)
//                    Writer.WriteByte(6 * 16 + 1)
//                    Writer.WriteByte(7 * 16 + 0)
//                    Writer.WriteByte(3 * 16 + 0)
//                    Writer.WriteByte(3 * 16 + 0)
//                    Writer.WriteByte(3 * 16 + 1)
//                    Writer.WriteByte(0)
//                    Writer.WriteByte(Objektverwalter.Spielfeld.Breite)
//                    Writer.WriteByte(Objektverwalter.Spielfeld.Hoehe)
//                    'Terrain Style
//                    Writer.WriteByte(0)
//                    For X = 0 To Objektverwalter.Spielfeld.Breite - 1
//                        For Y = 0 To Objektverwalter.Spielfeld.Hoehe - 1
//                            If Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).isGebaeude Then
//                                Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude.GebaeudeID
//                                    Case "HQ"
//                                        Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude.Spieler
//                                            Case 0
//                                            Case 0
//                                                Writer.WriteByte(94)
//                                                Writer.WriteByte(1)
//                                            Case 1
//                                                Writer.WriteByte(44)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(54)
//                                                Writer.WriteByte(1)
//                                            Case 3
//                                                Writer.WriteByte(64)
//                                                Writer.WriteByte(1)
//                                            Case 4
//                                                Writer.WriteByte(74)
//                                                Writer.WriteByte(1)
//                                            Case 5
//                                                Writer.WriteByte(84)
//                                                Writer.WriteByte(1)
//                                        End Select
//                                    Case "STADT"
//                                        Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude.Spieler
//                                            Case 0
//                                                Writer.WriteByte(95)
//                                                Writer.WriteByte(1)
//                                            Case 1
//                                                Writer.WriteByte(45)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(55)
//                                                Writer.WriteByte(1)
//                                            Case 3
//                                                Writer.WriteByte(65)
//                                                Writer.WriteByte(1)
//                                            Case 4
//                                                Writer.WriteByte(75)
//                                                Writer.WriteByte(1)
//                                            Case 5
//                                                Writer.WriteByte(85)
//                                                Writer.WriteByte(1)
//                                        End Select
//                                    Case "BASIS"
//                                        Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude.Spieler
//                                            Case 0
//                                                Writer.WriteByte(96)
//                                                Writer.WriteByte(1)
//                                            Case 1
//                                                Writer.WriteByte(46)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(56)
//                                                Writer.WriteByte(1)
//                                            Case 3
//                                                Writer.WriteByte(66)
//                                                Writer.WriteByte(1)
//                                            Case 4
//                                                Writer.WriteByte(76)
//                                                Writer.WriteByte(1)
//                                            Case 5
//                                                Writer.WriteByte(86)
//                                                Writer.WriteByte(1)
//                                        End Select
//                                    Case "FLUGHAFEN"
//                                        Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude.Spieler
//                                            Case 0
//                                                Writer.WriteByte(97)
//                                                Writer.WriteByte(1)
//                                            Case 1
//                                                Writer.WriteByte(47)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(57)
//                                                Writer.WriteByte(1)
//                                            Case 3
//                                                Writer.WriteByte(67)
//                                                Writer.WriteByte(1)
//                                            Case 4
//                                                Writer.WriteByte(77)
//                                                Writer.WriteByte(1)
//                                            Case 5
//                                                Writer.WriteByte(87)
//                                                Writer.WriteByte(1)
//                                        End Select
//                                    Case "WERFT"
//                                        Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude.Spieler
//                                            Case 0
//                                                Writer.WriteByte(98)
//                                                Writer.WriteByte(1)
//                                            Case 1
//                                                Writer.WriteByte(48)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(58)
//                                                Writer.WriteByte(1)
//                                            Case 3
//                                                Writer.WriteByte(68)
//                                                Writer.WriteByte(1)
//                                            Case 4
//                                                Writer.WriteByte(78)
//                                                Writer.WriteByte(1)
//                                            Case 5
//                                                Writer.WriteByte(88)
//                                                Writer.WriteByte(1)
//                                        End Select
//                                    Case "TURM"
//                                        Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude.Spieler
//                                            Case 0
//                                                Writer.WriteByte(99)
//                                                Writer.WriteByte(1)
//                                            Case 1
//                                                Writer.WriteByte(49)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(59)
//                                                Writer.WriteByte(1)
//                                            Case 3
//                                                Writer.WriteByte(69)
//                                                Writer.WriteByte(1)
//                                            Case 4
//                                                Writer.WriteByte(79)
//                                                Writer.WriteByte(1)
//                                            Case 5
//                                                Writer.WriteByte(89)
//                                                Writer.WriteByte(1)
//                                        End Select
//                                    Case "LABOR"
//                                        Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude.Spieler
//                                            Case 0
//                                                Writer.WriteByte(100)
//                                                Writer.WriteByte(1)
//                                            Case 1
//                                                Writer.WriteByte(50)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(60)
//                                                Writer.WriteByte(1)
//                                            Case 3
//                                                Writer.WriteByte(70)
//                                                Writer.WriteByte(1)
//                                            Case 4
//                                                Writer.WriteByte(80)
//                                                Writer.WriteByte(1)
//                                            Case 5
//                                                Writer.WriteByte(90)
//                                                Writer.WriteByte(1)
//                                        End Select
//                                    Case "RAKETENSILO+RAKETE", "RAKETENSILO"
//                                        Writer.WriteByte(94)
//                                        Writer.WriteByte(1)
//                                        'Spezialgebaeude
//                                    Case "MINIGESCHUETZ+N"
//                                        Writer.WriteByte(8 * 16 + 4)
//                                        Writer.WriteByte(3)
//                                    Case "MINIGESCHUETZ+W"
//                                        Writer.WriteByte(8 * 16 + 5)
//                                        Writer.WriteByte(3)
//                                    Case "LASER"
//                                        Writer.WriteByte(8 * 16 + 6)
//                                        Writer.WriteByte(3)
//                                    Case "MINIGESCHUETZ+O"
//                                        Writer.WriteByte(9 * 16 + 8)
//                                        Writer.WriteByte(3)
//                                    Case "MINIGESCHUETZ+S"
//                                        Writer.WriteByte(9 * 16 + 9)
//                                        Writer.WriteByte(3)
//                                    Case "KRISTALL"
//                                        Writer.WriteByte(9 * 16 + 11)
//                                        Writer.WriteByte(3)
//                                    Case "SCHWARZE GESCHUETZ+N"
//                                        Dim G As ObjektGebaeude = Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude
//                                        Select Case G.Rechteck.X - X
//                                            Case 1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(10 * 16 + 15)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(12 * 16 + 3)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(13 * 16 + 7)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case 0
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(11 * 16 + 0)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(12 * 16 + 4)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(13 * 16 + 8)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case -1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(11 * 16 + 1)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(12 * 16 + 5)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(13 * 16 + 9)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                        End Select
//                                    Case "SCHWARZE GESCHUETZ+S"
//                                        Dim G As ObjektGebaeude = Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude
//                                        Select Case G.Rechteck.X - X
//                                            Case 1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(10 * 16 + 12)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(12 * 16 + 0)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(13 * 16 + 4)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case 0
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(10 * 16 + 13)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(12 * 16 + 1)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(13 * 16 + 5)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case -1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(10 * 16 + 14)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(12 * 16 + 2)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(13 * 16 + 6)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                        End Select
//                                    Case "SCHOCKLASER+S"
//                                        Dim G As ObjektGebaeude = Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude
//                                        Select Case G.Rechteck.X - X
//                                            Case 1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(14 * 16 + 8)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(15 * 16 + 12)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 0)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                            Case 0
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(14 * 16 + 9)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(15 * 16 + 13)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 1)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                            Case -1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(14 * 16 + 10)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(15 * 16 + 14)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 2)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                        End Select
//                                    Case "MONOLITH"
//                                        Dim G As ObjektGebaeude = Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude
//                                        Select Case G.Rechteck.X - X
//                                            Case 1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(14 * 16 + 11)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(15 * 16 + 15)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 3)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                            Case 0
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(14 * 16 + 12)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(0 * 16 + 0)
//                                                        Writer.WriteByte(4)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 4)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                            Case -1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 1
//                                                        Writer.WriteByte(14 * 16 + 13)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(0 * 16 + 1)
//                                                        Writer.WriteByte(4)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 5)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                        End Select
//                                    Case "VULKAN"
//                                        Dim G As ObjektGebaeude = Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude
//                                        Select Case G.Rechteck.X - X
//                                            Case 1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(8 * 16 + 11)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(9 * 16 + 15)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(11 * 16 + 3)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(12 * 16 + 7)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case 0
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(8 * 16 + 12)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(9 * 16 + 16)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(11 * 16 + 4)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(12 * 16 + 8)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case -1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(8 * 16 + 13)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(9 * 16 + 17)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(11 * 16 + 5)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(12 * 16 + 9)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case -2
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(8 * 16 + 14)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(9 * 16 + 18)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(11 * 16 + 6)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(12 * 16 + 10)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                        End Select
//                                    Case "FESTUNG"
//                                        Dim G As ObjektGebaeude = Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude
//                                        Select Case G.Rechteck.X - X
//                                            Case 1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(13 * 16 + 11)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(14 * 16 + 15)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(0 * 16 + 3)
//                                                        Writer.WriteByte(4)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 7)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                            Case 0
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(13 * 16 + 12)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(15 * 16 + 0)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(0 * 16 + 4)
//                                                        Writer.WriteByte(4)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 8)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                            Case -1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(13 * 16 + 13)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(15 * 16 + 1)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(0 * 16 + 5)
//                                                        Writer.WriteByte(4)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 9)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                            Case -2
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(13 * 16 + 14)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(15 * 16 + 2)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(0 * 16 + 6)
//                                                        Writer.WriteByte(4)
//                                                    Case -1
//                                                        Writer.WriteByte(1 * 16 + 10)
//                                                        Writer.WriteByte(4)
//                                                End Select
//                                        End Select
//                                    Case "OMEGARAKETE"
//                                        Dim G As ObjektGebaeude = Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).Gebaeude
//                                        Select Case G.Rechteck.X - X
//                                            Case 1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(8 * 16 + 15)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(10 * 16 + 3)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(11 * 16 + 7)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(12 * 16 + 11)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case 0
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(9 * 16 + 0)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(10 * 16 + 4)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(11 * 16 + 8)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(12 * 16 + 12)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case -1
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(9 * 16 + 1)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(10 * 16 + 5)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(11 * 16 + 9)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(12 * 16 + 13)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                            Case -2
//                                                Select Case G.Rechteck.Y - Y
//                                                    Case 2
//                                                        Writer.WriteByte(9 * 16 + 2)
//                                                        Writer.WriteByte(3)
//                                                    Case 1
//                                                        Writer.WriteByte(10 * 16 + 6)
//                                                        Writer.WriteByte(3)
//                                                    Case 0
//                                                        Writer.WriteByte(11 * 16 + 10)
//                                                        Writer.WriteByte(3)
//                                                    Case -1
//                                                        Writer.WriteByte(12 * 16 + 14)
//                                                        Writer.WriteByte(3)
//                                                End Select
//                                        End Select
//                                    Case Else
//                                        Writer.WriteByte(0)
//                                        Writer.WriteByte(0)
//                                End Select
//                            Else
//                                Select Case Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).LandschaftsID
//                                    Case "KAPUTTESCHWEISSNAHT"
//                                        Writer.WriteByte(10 * 16 + 7)
//                                        Writer.WriteByte(0)
//                                    Case "EBENE"
//                                        Writer.WriteByte(0)
//                                        Writer.WriteByte(0)
//                                    Case "STRASSE"
//                                        Writer.WriteByte(1)
//                                        Writer.WriteByte(0)
//                                    Case "BRUECKE"
//                                        Dim Ausr As String = Objektverwalter.LandschaftenBilder(Objektverwalter.Spielfeld.Landschaften(X, Y, ObjektSpielfeld.Bodenschlacht).ID).getAusrichtung(X, Y, Objektverwalter.Spielfeld)
//                                        If Ausr = "+N+S" Then
//                                            Writer.WriteByte(2)
//                                            Writer.WriteByte(0)
//                                        Else
//                                            Writer.WriteByte(32)
//                                            Writer.WriteByte(0)
//                                        End If
//                                    Case "FLUSS"
//                                        Writer.WriteByte(3)
//                                        Writer.WriteByte(0)
//                                    Case "SEE"
//                                        Writer.WriteByte(3 * 16 + 12)
//                                        Writer.WriteByte(0)
//                                    Case "PIPELINE"
//                                        Writer.WriteByte(16)
//                                        Writer.WriteByte(0)
//                                    Case "RIFF"
//                                        Writer.WriteByte(30)
//                                        Writer.WriteByte(0)
//                                    Case "STRAND"
//                                        Writer.WriteByte(39)
//                                        Writer.WriteByte(0)
//                                    Case "WALD"
//                                        Writer.WriteByte(90)
//                                        Writer.WriteByte(0)
//                                    Case "BERG"
//                                        Writer.WriteByte(150)
//                                        Writer.WriteByte(0)
//                                    Case "EBENE+SCHROTT"
//                                        Writer.WriteByte(167)
//                                        Writer.WriteByte(0)
//                                    Case "SCHWEISSNAHT"
//                                        Writer.WriteByte(226)
//                                        Writer.WriteByte(0)
//                                    Case Else
//                                        Writer.WriteByte(0)
//                                        Writer.WriteByte(0)
//                                End Select
//                            End If
//                        Next
//                    Next
//                    ''Die Einheiten erschaffen
//                    For X = 0 To Objektverwalter.Spielfeld.Breite - 1
//                        For Y = 0 To Objektverwalter.Spielfeld.Hoehe - 1
//                            If Objektverwalter.Spielfeld.Einheiten(X, Y, 0) Is Nothing Then
//                                Writer.WriteByte(Byte.MaxValue)
//                                Writer.WriteByte(Byte.MaxValue)
//                            Else
//                                Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).EinheitenID
//                                    Case "INF"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(244)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(28)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(68)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(108)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(148)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "MECH"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(8)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(48)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(88)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(128)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(168)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "TTP"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(10)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(50)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(90)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(130)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(170)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "HOELLIUM"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(0)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(40)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(80)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(120)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(160)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "SPAEHER"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(246)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(30)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(70)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(110)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(150)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "JGPZ"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(9)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(49)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(89)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(129)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(169)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "KPZ"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(245)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(29)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(69)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(109)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(149)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "TITANPZ"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(253)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(37)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(77)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(117)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(157)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "MEGAPZ"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(254)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(38)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(78)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(118)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(158)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "ART"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(247)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(31)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(71)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(111)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(151)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "RKW"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(11)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(51)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(91)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(131)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(171)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "LAEUFER"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(255)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(39)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(79)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(119)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(159)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "MISSILE"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(12)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(52)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(92)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(132)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(172)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "FLAK"                            'Flak OS
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(248)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(32)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(72)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(112)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(152)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "T-HELI"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(14)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(54)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(94)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(134)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(174)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "JAEGER" '249 'Jaeger OS
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(249)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(33)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(73)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(113)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(153)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "K-HELI" ' 250 'K-Heli OS
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(250)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(34)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(74)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(114)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(154)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "SCHLFF"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(251)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(35)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(75)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(115)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(155)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "L-BOOT"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(252)
//                                                Writer.WriteByte(1)
//                                            Case 2
//                                                Writer.WriteByte(36)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(76)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(116)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(156)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "BOMBER"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(13)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(53)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(93)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(133)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(173)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "KREUZER"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(15)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(55)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(95)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(135)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(175)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "U-BOOT"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(16)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(56)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(96)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(136)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(176)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "S-BOOT"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(17)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(57)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(97)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(137)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(177)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "FLTR"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(18)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(58)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(98)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(138)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(178)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "TARNK"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(19)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(59)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(99)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(139)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(179)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case "S-BOMBE"
//                                        Select Case Objektverwalter.Spielfeld.Einheiten(X, Y, ObjektSpielfeld.Bodenschlacht).Spieler
//                                            Case 1
//                                                Writer.WriteByte(20)
//                                                Writer.WriteByte(2)
//                                            Case 2
//                                                Writer.WriteByte(70)
//                                                Writer.WriteByte(2)
//                                            Case 3
//                                                Writer.WriteByte(100)
//                                                Writer.WriteByte(2)
//                                            Case 4
//                                                Writer.WriteByte(140)
//                                                Writer.WriteByte(2)
//                                            Case 5
//                                                Writer.WriteByte(180)
//                                                Writer.WriteByte(2)
//                                        End Select
//                                    Case Else
//                                        Writer.WriteByte(Byte.MaxValue)
//                                        Writer.WriteByte(Byte.MaxValue)
//                                End Select
//                            End If
//                        Next
//                    Next
//                    Writer.WriteByte(Objektverwalter.Spielfeld.Kartenname.Length)
//                    Writer.WriteByte(0)
//                    Writer.WriteByte(0)
//                    Writer.WriteByte(0)
//                    'Jetzt kommt der Karten Name
//                    Dim S() As Char = Objektverwalter.Spielfeld.Kartenname.ToCharArray
//                    For I = 0 To S.Length - 1
//                        Writer.WriteByte(CByte(AscW(S(I))))
//                    Next
//                    Writer.WriteByte(Objektverwalter.Spielfeld.Autor.Length)
//                    Writer.WriteByte(0)
//                    Writer.WriteByte(0)
//                    Writer.WriteByte(0)
//                    S = Objektverwalter.Spielfeld.Autor.ToCharArray
//                    For I = 0 To S.Length - 1
//                        Writer.WriteByte(CByte(AscW(S(I))))
//                    Next
//                    ''Jetzt kommt vier Zeichen nichts
//                    Writer.WriteByte(Objektverwalter.Spielfeld.Beschreibung.Length)
//                    Writer.WriteByte(0)
//                    Writer.WriteByte(0)
//                    Writer.WriteByte(0)
//                    S = Objektverwalter.Spielfeld.Beschreibung.ToCharArray
//                    For I = 0 To S.Length - 1
//                        Writer.WriteByte(CByte(AscW(S(I))))
//                    Next
//                End Using
//            End If
}
