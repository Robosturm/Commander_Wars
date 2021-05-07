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
            m_players.append(spPlayer::create());
            m_players[i]->init();
        }
        // read byte for terrain style which we ignore
        stream >> sign;
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
                                spBuilding pBuilding = spBuilding::create("ZMONOLITH");
                                pBuilding->setOwner(getPlayer(4));
                                getTerrain(x + 1, y + 1)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("ZFORTRESS");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x + 2, y + 1)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 45:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 46:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 47:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 48:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 49:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 50:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 54:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 55:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 56:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 57:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 58:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 59:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
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
                                spBuilding pBuilding = spBuilding::create("LABOR");
                                pBuilding->setOwner(getPlayer(1));
                                getTerrain(x, y)->setBuilding(pBuilding.get());
                                break;
                            }
                        }
                        break;
                    }
                    case 64:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 65:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 66:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 67:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 68:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 69:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 70:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 74:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 75:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 76:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 77:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 78:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 79:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 80:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 84:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 85:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 86:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 87:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 88:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 89:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
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
                                spBuilding pBuilding = spBuilding::create("LABOR");
                                pBuilding->setOwner(getPlayer(4));
                                getTerrain(x, y)->setBuilding(pBuilding.get());
                                break;
                            }
                        }
                        break;
                    }

                    case 94:
                    {
                        spBuilding pBuilding = spBuilding::create("SILO_ROCKET");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 95:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 96:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 97:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 98:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 99:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 100:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 132:
                    {
                        spBuilding pBuilding = spBuilding::create("ZMINICANNON_N");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 133:
                    {
                        spBuilding pBuilding = spBuilding::create("ZMINICANNON_W");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 134:
                    {
                        spBuilding pBuilding = spBuilding::create("ZLASER");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("ZMINICANNON_E");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 153:
                    {
                        spBuilding pBuilding = spBuilding::create("ZMINICANNON_S");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 155:
                    {
                        spBuilding pBuilding = spBuilding::create("ZCRYSTALL");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("ZVOLCAN");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x + 2, y + 1)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("ZOMEGA_BASE");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x + 2, y + 1)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("ZBLACKHOLE_CANNON_S");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x + 1, y + 1)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("ZBLACKHOLE_CANNON_N");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x + 1, y + 1)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("ZDEATHRAY_S");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x + 1, y + 1)->setBuilding(pBuilding.get());
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
                        spUnit pUnit = spUnit::create("INFANTRY", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 245:
                    {
                        spUnit pUnit = spUnit::create("HEAVY_TANK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 246:
                    {
                        spUnit pUnit = spUnit::create("RECON", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 247:
                    {
                        spUnit pUnit = spUnit::create("ARTILLERY", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 248:
                    {
                        spUnit pUnit = spUnit::create("FLAK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 249:
                    {
                        spUnit pUnit = spUnit::create("FIGHTER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 250:
                    {
                        spUnit pUnit = spUnit::create("K_HELI", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 251:
                    {
                        spUnit pUnit = spUnit::create("BATTLESHIP", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 252:
                    {
                        spUnit pUnit = spUnit::create("LANDER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 253:
                    {
                        spUnit pUnit = spUnit::create("NEOTANK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 254:
                    {
                        spUnit pUnit = spUnit::create("MEGATANK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 255:
                    {
                        spUnit pUnit = spUnit::create("PIPERUNNER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 0:
                    {
                        spUnit pUnit = spUnit::create("HOELLIUM", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 8:
                    {
                        spUnit pUnit = spUnit::create("MECH", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 9:
                    {
                        spUnit pUnit = spUnit::create("LIGHT_TANK", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 10:
                    {
                        spUnit pUnit = spUnit::create("APC", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 11:
                    {
                        spUnit pUnit = spUnit::create("ROCKETTHROWER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 12:
                    {
                        spUnit pUnit = spUnit::create("MISSILE", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 13:
                    {
                        spUnit pUnit = spUnit::create("BOMBER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 14:
                    {
                        spUnit pUnit = spUnit::create("T_HELI", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 15:
                    {
                        spUnit pUnit = spUnit::create("CRUISER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 16:
                    {
                        spUnit pUnit = spUnit::create("SUBMARINE", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 17:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOAT", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 18:
                    {
                        spUnit pUnit = spUnit::create("AIRCRAFTCARRIER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 19:
                    {
                        spUnit pUnit = spUnit::create("STEALTHBOMBER", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 20:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOMB", getPlayer(0), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                        // Blue Moon
                    case 28:
                    {
                        spUnit pUnit = spUnit::create("INFANTRY", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 29:
                    {
                        spUnit pUnit = spUnit::create("HEAVY_TANK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 30:
                    {
                        spUnit pUnit = spUnit::create("RECON", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 31:
                    {
                        spUnit pUnit = spUnit::create("ARTILLERY", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 32:
                    {
                        spUnit pUnit = spUnit::create("FLAK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 33:
                    {
                        spUnit pUnit = spUnit::create("FIGHTER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 34:
                    {
                        spUnit pUnit = spUnit::create("K_HELI", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 35:
                    {
                        spUnit pUnit = spUnit::create("BATTLESHIP", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 36:
                    {
                        spUnit pUnit = spUnit::create("LANDER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 37:
                    {
                        spUnit pUnit = spUnit::create("NEOTANK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 38:
                    {
                        spUnit pUnit = spUnit::create("MEGATANK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 39:
                    {
                        spUnit pUnit = spUnit::create("PIPERUNNER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 40:
                    {
                        spUnit pUnit = spUnit::create("HOELLIUM", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 48:
                    {
                        spUnit pUnit = spUnit::create("MECH", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 49:
                    {
                        spUnit pUnit = spUnit::create("LIGHT_TANK", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 50:
                    {
                        spUnit pUnit = spUnit::create("APC", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 51:
                    {
                        spUnit pUnit = spUnit::create("ROCKETTHROWER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 52:
                    {
                        spUnit pUnit = spUnit::create("MISSILE", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 53:
                    {
                        spUnit pUnit = spUnit::create("BOMBER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 54:
                    {
                        spUnit pUnit = spUnit::create("T_HELI", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 55:
                    {
                        spUnit pUnit = spUnit::create("CRUISER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 56:
                    {
                        spUnit pUnit = spUnit::create("SUBMARINE", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 57:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOAT", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 58:
                    {
                        spUnit pUnit = spUnit::create("AIRCRAFTCARRIER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 59:
                    {
                        spUnit pUnit = spUnit::create("STEALTHBOMBER", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 60:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOMB", getPlayer(1), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                        // Green Earth
                    case 68:
                    {
                        spUnit pUnit = spUnit::create("INFANTRY", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 69:
                    {
                        spUnit pUnit = spUnit::create("HEAVY_TANK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 70:
                    {
                        spUnit pUnit = spUnit::create("RECON", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 71:
                    {
                        spUnit pUnit = spUnit::create("ARTILLERY", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 72:
                    {
                        spUnit pUnit = spUnit::create("FLAK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 73:
                    {
                        spUnit pUnit = spUnit::create("FIGHTER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 74:
                    {
                        spUnit pUnit = spUnit::create("K_HELI", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 75:
                    {
                        spUnit pUnit = spUnit::create("BATTLESHIP", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 76:
                    {
                        spUnit pUnit = spUnit::create("LANDER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 77:
                    {
                        spUnit pUnit = spUnit::create("NEOTANK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 78:
                    {
                        spUnit pUnit = spUnit::create("MEGATANK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 79:
                    {
                        spUnit pUnit = spUnit::create("PIPERUNNER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 80:
                    {
                        spUnit pUnit = spUnit::create("HOELLIUM", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 88:
                    {
                        spUnit pUnit = spUnit::create("MECH", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 89:
                    {
                        spUnit pUnit = spUnit::create("LIGHT_TANK", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 90:
                    {
                        spUnit pUnit = spUnit::create("APC", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 91:
                    {
                        spUnit pUnit = spUnit::create("ROCKETTHROWER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 92:
                    {
                        spUnit pUnit = spUnit::create("MISSILE", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 93:
                    {
                        spUnit pUnit = spUnit::create("BOMBER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 94:
                    {
                        spUnit pUnit = spUnit::create("T_HELI", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 95:
                    {
                        spUnit pUnit = spUnit::create("CRUISER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 96:
                    {
                        spUnit pUnit = spUnit::create("SUBMARINE", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 97:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOAT", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 98:
                    {
                        spUnit pUnit = spUnit::create("AIRCRAFTCARRIER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 99:
                    {
                        spUnit pUnit = spUnit::create("STEALTHBOMBER", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 100:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOMB", getPlayer(2), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                        // Yellow Comet
                    case 108:
                    {
                        spUnit pUnit = spUnit::create("INFANTRY", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 109:
                    {
                        spUnit pUnit = spUnit::create("HEAVY_TANK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 110:
                    {
                        spUnit pUnit = spUnit::create("RECON", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 111:
                    {
                        spUnit pUnit = spUnit::create("ARTILLERY", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 112:
                    {
                        spUnit pUnit = spUnit::create("FLAK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 113:
                    {
                        spUnit pUnit = spUnit::create("FIGHTER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 114:
                    {
                        spUnit pUnit = spUnit::create("K_HELI", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 115:
                    {
                        spUnit pUnit = spUnit::create("BATTLESHIP", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 116:
                    {
                        spUnit pUnit = spUnit::create("LANDER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 117:
                    {
                        spUnit pUnit = spUnit::create("NEOTANK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 118:
                    {
                        spUnit pUnit = spUnit::create("MEGATANK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 119:
                    {
                        spUnit pUnit = spUnit::create("PIPERUNNER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 120:
                    {
                        spUnit pUnit = spUnit::create("HOELLIUM", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 128:
                    {
                        spUnit pUnit = spUnit::create("MECH", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 129:
                    {
                        spUnit pUnit = spUnit::create("LIGHT_TANK", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 130:
                    {
                        spUnit pUnit = spUnit::create("APC", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 131:
                    {
                        spUnit pUnit = spUnit::create("ROCKETTHROWER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 132:
                    {
                        spUnit pUnit = spUnit::create("MISSILE", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 133:
                    {
                        spUnit pUnit = spUnit::create("BOMBER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 134:
                    {
                        spUnit pUnit = spUnit::create("T_HELI", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 135:
                    {
                        spUnit pUnit = spUnit::create("CRUISER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 136:
                    {
                        spUnit pUnit = spUnit::create("SUBMARINE", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 137:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOAT", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 138:
                    {
                        spUnit pUnit = spUnit::create("AIRCRAFTCARRIER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 139:
                    {
                        spUnit pUnit = spUnit::create("STEALTHBOMBER", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 140:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOMB", getPlayer(3), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                        // Black Hole
                    case 148:
                    {
                        spUnit pUnit = spUnit::create("INFANTRY", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 149:
                    {
                        spUnit pUnit = spUnit::create("HEAVY_TANK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 150:
                    {
                        spUnit pUnit = spUnit::create("RECON", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 151:
                    {
                        spUnit pUnit = spUnit::create("ARTILLERY", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 152:
                    {
                        spUnit pUnit = spUnit::create("FLAK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 153:
                    {
                        spUnit pUnit = spUnit::create("FIGHTER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 154:
                    {
                        spUnit pUnit = spUnit::create("K_HELI", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 155:
                    {
                        spUnit pUnit = spUnit::create("BATTLESHIP", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 156:
                    {
                        spUnit pUnit = spUnit::create("LANDER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 157:
                    {
                        spUnit pUnit = spUnit::create("NEOTANK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 158:
                    {
                        spUnit pUnit = spUnit::create("MEGATANK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 159:
                    {
                        spUnit pUnit = spUnit::create("PIPERUNNER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 160:
                    {
                        spUnit pUnit = spUnit::create("HOELLIUM", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 168:
                    {
                        spUnit pUnit = spUnit::create("MECH", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 169:
                    {
                        spUnit pUnit = spUnit::create("LIGHT_TANK", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 170:
                    {
                        spUnit pUnit = spUnit::create("APC", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 171:
                    {
                        spUnit pUnit = spUnit::create("ROCKETTHROWER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 172:
                    {
                        spUnit pUnit = spUnit::create("MISSILE", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 173:
                    {
                        spUnit pUnit = spUnit::create("BOMBER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 174:
                    {
                        spUnit pUnit = spUnit::create("T_HELI", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 175:
                    {
                        spUnit pUnit = spUnit::create("CRUISER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 176:
                    {
                        spUnit pUnit = spUnit::create("SUBMARINE", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 177:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOAT", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 178:
                    {
                        spUnit pUnit = spUnit::create("AIRCRAFTCARRIER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 179:
                    {
                        spUnit pUnit = spUnit::create("STEALTHBOMBER", getPlayer(4), false);
                        getTerrain(x, y)->setUnit(pUnit);
                        break;
                    }
                    case 180:
                    {
                        spUnit pUnit = spUnit::create("BLACK_BOMB", getPlayer(4), false);
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
        m_mapName = "";
        for (qint32 i = 0; i < stringLength; i++)
        {
            stream >> sign;
            m_mapName += static_cast<char>(sign);
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
        m_mapAuthor = "";
        for (qint32 i = 0; i < stringLength; i++)
        {
            stream >> sign;
            m_mapAuthor += static_cast<char>(sign);
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
        m_mapDescription = "";
        for (qint32 i = 0; i < stringLength; i++)
        {
            stream >> sign;
            m_mapDescription += static_cast<char>(sign);
        }
        m_mapDescription = m_mapDescription.replace("\n", " ");
        EditorMenue::getInstance()->optimizePlayers();
        // update the whole fucking map
        updateSprites();
    }
}

void GameMap::exportAWDSMap(QString file)
{
    spGameMap pMap = GameMap::getInstance();
    if (pMap->getPlayerCount() <= 5)
    {
        QFile data(file);
        data.open(QFile::WriteOnly);
        QDataStream stream(&data);
        stream << static_cast<uchar>(4 * 16 + 1);
        stream << static_cast<uchar>(5 * 16 + 7);
        stream << static_cast<uchar>(5 * 16 + 3);
        stream << static_cast<uchar>(4 * 16 + 13);
        stream << static_cast<uchar>(6 * 16 + 1);
        stream << static_cast<uchar>(7 * 16 + 0);
        stream << static_cast<uchar>(3 * 16 + 0);
        stream << static_cast<uchar>(3 * 16 + 0);
        stream << static_cast<uchar>(3 * 16 + 1);
        stream << static_cast<uchar>(0);
        stream << static_cast<uchar>(pMap->getMapWidth());
        stream << static_cast<uchar>(pMap->getMapHeight());
        // Terrain Style
        stream << static_cast<uchar>(0);
        for (qint32 x = 0; x < pMap->getMapWidth(); x++)
        {
            for (qint32 y = 0 ; y < pMap->getMapHeight(); y++)
            {
                spBuilding pBuilding = pMap->getTerrain(x, y)->getBuilding();
                Terrain* pTerrain = pMap->getTerrain(x, y);
                if (pBuilding.get() != nullptr)
                {
                    if (pBuilding->getBuildingID() == "HQ")
                    {
                        switch (pBuilding->getOwnerID() + 1)
                        {
                            case 0:
                                stream << static_cast<uchar>(94);
                                stream << static_cast<uchar>(1);
                                break;
                            case 1:
                                stream << static_cast<uchar>(44);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(54);
                                stream << static_cast<uchar>(1);
                                break;
                            case 3:
                                stream << static_cast<uchar>(64);
                                stream << static_cast<uchar>(1);
                                break;
                            case 4:
                                stream << static_cast<uchar>(74);
                                stream << static_cast<uchar>(1);
                                break;
                            case 5:
                                stream << static_cast<uchar>(84);
                                stream << static_cast<uchar>(1);
                                break;
                        }
                    }
                    else if (pBuilding->getBuildingID() == "TOWN")
                    {
                        switch (pBuilding->getOwnerID() + 1)
                        {
                            case 0:
                                stream << static_cast<uchar>(95);
                                stream << static_cast<uchar>(1);
                                break;
                            case 1:
                                stream << static_cast<uchar>(45);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(55);
                                stream << static_cast<uchar>(1);
                                break;
                            case 3:
                                stream << static_cast<uchar>(65);
                                stream << static_cast<uchar>(1);
                                break;
                            case 4:
                                stream << static_cast<uchar>(75);
                                stream << static_cast<uchar>(1);
                                break;
                            case 5:
                                stream << static_cast<uchar>(85);
                                stream << static_cast<uchar>(1);
                                break;
                        }
                    }
                    else if (pBuilding->getBuildingID() == "FACTORY")
                    {
                        switch (pBuilding->getOwnerID() + 1)
                        {
                            case 0:
                                stream << static_cast<uchar>(96);
                                stream << static_cast<uchar>(1);
                                break;
                            case 1:
                                stream << static_cast<uchar>(46);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(56);
                                stream << static_cast<uchar>(1);
                                break;
                            case 3:
                                stream << static_cast<uchar>(66);
                                stream << static_cast<uchar>(1);
                                break;
                            case 4:
                                stream << static_cast<uchar>(76);
                                stream << static_cast<uchar>(1);
                                break;
                            case 5:
                                stream << static_cast<uchar>(86);
                                stream << static_cast<uchar>(1);
                                break;
                        }
                    }
                    else if (pBuilding->getBuildingID() == "AIRPORT")
                    {
                        switch (pBuilding->getOwnerID() + 1)
                        {
                            case 0:
                                stream << static_cast<uchar>(97);
                                stream << static_cast<uchar>(1);
                                break;
                            case 1:
                                stream << static_cast<uchar>(47);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(57);
                                stream << static_cast<uchar>(1);
                                break;
                            case 3:
                                stream << static_cast<uchar>(67);
                                stream << static_cast<uchar>(1);
                                break;
                            case 4:
                                stream << static_cast<uchar>(77);
                                stream << static_cast<uchar>(1);
                                break;
                            case 5:
                                stream << static_cast<uchar>(87);
                                stream << static_cast<uchar>(1);
                                break;
                        }
                    }
                    else if (pBuilding->getBuildingID() == "HARBOUR")
                    {
                        switch (pBuilding->getOwnerID() + 1)
                        {
                            case 0:
                                stream << static_cast<uchar>(98);
                                stream << static_cast<uchar>(1);
                                break;
                            case 1:
                                stream << static_cast<uchar>(48);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(58);
                                stream << static_cast<uchar>(1);
                                break;
                            case 3:
                                stream << static_cast<uchar>(68);
                                stream << static_cast<uchar>(1);
                                break;
                            case 4:
                                stream << static_cast<uchar>(78);
                                stream << static_cast<uchar>(1);
                                break;
                            case 5:
                                stream << static_cast<uchar>(88);
                                stream << static_cast<uchar>(1);
                                break;
                        }
                    }
                    else if (pBuilding->getBuildingID() == "TOWER")
                    {
                        switch (pBuilding->getOwnerID() + 1)
                        {
                            case 0:
                                stream << static_cast<uchar>(99);
                                stream << static_cast<uchar>(1);
                                break;
                            case 1:
                                stream << static_cast<uchar>(49);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(59);
                                stream << static_cast<uchar>(1);
                                break;
                            case 3:
                                stream << static_cast<uchar>(69);
                                stream << static_cast<uchar>(1);
                                break;
                            case 4:
                                stream << static_cast<uchar>(79);
                                stream << static_cast<uchar>(1);
                                break;
                            case 5:
                                stream << static_cast<uchar>(89);
                                stream << static_cast<uchar>(1);
                                break;
                        }
                    }
                    else if (pBuilding->getBuildingID() == "LABOR")
                    {
                        switch (pBuilding->getOwnerID() + 1)
                        {
                            case 0:
                                stream << static_cast<uchar>(100);
                                stream << static_cast<uchar>(1);
                                break;
                            case 1:
                                stream << static_cast<uchar>(50);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(60);
                                stream << static_cast<uchar>(1);
                                break;
                            case 3:
                                stream << static_cast<uchar>(70);
                                stream << static_cast<uchar>(1);
                                break;
                            case 4:
                                stream << static_cast<uchar>(80);
                                stream << static_cast<uchar>(1);
                            case 5:
                                stream << static_cast<uchar>(90);
                                stream << static_cast<uchar>(1);
                                break;
                        }
                    }
                    else if (pBuilding->getBuildingID() == "SILO_ROCKET" ||
                             pBuilding->getBuildingID() ==  "SILO")
                    {
                        stream << static_cast<uchar>(94);
                        stream << static_cast<uchar>(1);
                    }
                    else if (pBuilding->getBuildingID() == "ZMINICANNON_N")
                    {
                        stream << static_cast<uchar>(8 * 16 + 4);
                        stream << static_cast<uchar>(3);
                    }
                    else if (pBuilding->getBuildingID() == "ZMINICANNON_W")
                    {
                        stream << static_cast<uchar>(8 * 16 + 5);
                        stream << static_cast<uchar>(3);
                    }
                    else if (pBuilding->getBuildingID() == "LASER")
                    {
                        stream << static_cast<uchar>(8 * 16 + 6);
                        stream << static_cast<uchar>(3);
                    }
                    else if (pBuilding->getBuildingID() == "ZMINICANNON_E")
                    {
                        stream << static_cast<uchar>(9 * 16 + 8);
                        stream << static_cast<uchar>(3);
                    }
                    else if (pBuilding->getBuildingID() == "ZMINICANNON_S")
                    {
                        stream << static_cast<uchar>(9 * 16 + 9);
                        stream << static_cast<uchar>(3);
                    }
                    else if (pBuilding->getBuildingID() == "ZCRYSTALL")
                    {
                        stream << static_cast<uchar>(9 * 16 + 11);
                        stream << static_cast<uchar>(3);
                    }
                    else if (pBuilding->getBuildingID() == "ZBLACKHOLE_CANNON_N")
                    {
                        QPoint offset = pBuilding->getOffset(pTerrain);
                        switch (offset.x())
                        {
                            case 0:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(10 * 16 + 15);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 3);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(13 * 16 + 7);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 1:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 0);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 4);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(13 * 16 + 8);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 2:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 1);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 5);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                        stream << static_cast<uchar>(13 * 16 + 9);
                                        stream << static_cast<uchar>(3);
                                        break;
                                }
                            }
                        }
                    }
                    else if (pBuilding->getBuildingID() == "ZBLACKHOLE_CANNON_S")
                    {
                        QPoint offset = pBuilding->getOffset(pTerrain);
                        switch (offset.x())
                        {
                            case 0:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(10 * 16 + 12);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 0);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                        stream << static_cast<uchar>(13 * 16 + 4);
                                        stream << static_cast<uchar>(3);
                                        break;
                                }
                            }
                            case 1:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(10 * 16 + 13);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 1);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(13 * 16 + 5);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 2:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(10 * 16 + 14);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 2);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(13 * 16 + 6);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                                                    else if (pBuilding->getBuildingID() == "ZDEATHRAY_S")
                    {
                        QPoint offset = pBuilding->getOffset(pTerrain);
                        switch (offset.x())
                        {
                            case 0:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(14 * 16 + 8);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(15 * 16 + 12);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 0);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                            case 1:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(14 * 16 + 9);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(15 * 16 + 13);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 1);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                            case 2:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(14 * 16 + 10);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(15 * 16 + 14);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 2);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else if (pBuilding->getBuildingID() == "ZMONOLITH")
                    {
                        QPoint offset = pBuilding->getOffset(pTerrain);
                        switch (offset.x())
                        {
                            case 0:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(14 * 16 + 11);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(15 * 16 + 15);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 3);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                            case 1:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(14 * 16 + 12);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(0 * 16 + 0);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 4);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                            case 2:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(14 * 16 + 13);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(0 * 16 + 1);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 5);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else if (pBuilding->getBuildingID() == "ZVOLCAN")
                    {
                        QPoint offset = pBuilding->getOffset(pTerrain);
                        switch (offset.x())
                        {
                            case 0:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(8 * 16 + 11);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(9 * 16 + 15);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 3);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 7);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 1:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(8 * 16 + 12);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(9 * 16 + 16);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 4);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 8);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 2:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(8 * 16 + 13);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(9 * 16 + 17);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 5);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 9);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 3:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(8 * 16 + 14);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(9 * 16 + 18);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 6);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 10);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else if (pBuilding->getBuildingID() == "ZFORTRESS")
                    {
                        QPoint offset = pBuilding->getOffset(pTerrain);
                        switch (offset.x())
                        {
                            case 0:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(13 * 16 + 11);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(14 * 16 + 15);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(0 * 16 + 3);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 7);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                            case 1:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(13 * 16 + 12);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(15 * 16 + 0);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(0 * 16 + 4);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 8);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                            case 2:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(13 * 16 + 13);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(15 * 16 + 1);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(0 * 16 + 5);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 9);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                            case 3:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(13 * 16 + 14);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(15 * 16 + 2);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(0 * 16 + 6);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(1 * 16 + 10);
                                        stream << static_cast<uchar>(4);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else if (pBuilding->getBuildingID() == "ZOMEGA_BASE")
                    {
                        QPoint offset = pBuilding->getOffset(pTerrain);
                        switch (offset.x())
                        {
                            case 0:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(8 * 16 + 15);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(10 * 16 + 3);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 7);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 11);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 1:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(9 * 16 + 0);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(10 * 16 + 4);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 8);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 12);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 2:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(9 * 16 + 1);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(10 * 16 + 5);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 9);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 13);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                            case 3:
                            {
                                switch (offset.y())
                                {
                                    case 0:
                                    {
                                        stream << static_cast<uchar>(9 * 16 + 2);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 1:
                                    {
                                        stream << static_cast<uchar>(10 * 16 + 6);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 2:
                                    {
                                        stream << static_cast<uchar>(11 * 16 + 10);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                    case 3:
                                    {
                                        stream << static_cast<uchar>(12 * 16 + 14);
                                        stream << static_cast<uchar>(3);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else
                    {
                        stream << static_cast<uchar>(0);
                        stream << static_cast<uchar>(0);
                    }
                }
                else
                {
                    if (pTerrain->getTerrainID() == "DESTROYEDWELD")
                    {
                        stream << static_cast<uchar>(10 * 16 + 7);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "PLAINS")
                    {
                        stream << static_cast<uchar>(0);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "STREET")
                    {
                        stream << static_cast<uchar>(1);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "BRIDGE")
                    {
                        // North South bridge
                        stream << static_cast<uchar>(2);
                        stream << static_cast<uchar>(0);
                        // east west brdige
                        //                        stream << static_cast<uchar>(32);
                        //                        stream << static_cast<uchar>(0);

                    }
                    else if (pTerrain->getTerrainID() == "RIVER")
                    {
                        stream << static_cast<uchar>(3);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "SEA")
                    {
                        stream << static_cast<uchar>(3 * 16 + 12);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "PIPELINE")
                    {
                        stream << static_cast<uchar>(16);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "REAF")
                    {
                        stream << static_cast<uchar>(30);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "BEACH")
                    {
                        stream << static_cast<uchar>(39);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "FOREST")
                    {
                        stream << static_cast<uchar>(90);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "MOUNTAIN")
                    {
                        stream << static_cast<uchar>(150);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "PLAINS+DESTROYED")
                    {
                        stream << static_cast<uchar>(167);
                        stream << static_cast<uchar>(0);
                    }
                    else if (pTerrain->getTerrainID() == "WELD")
                    {
                        stream << static_cast<uchar>(226);
                        stream << static_cast<uchar>(0);
                    }
                    else
                    {
                        stream << static_cast<uchar>(0);
                        stream << static_cast<uchar>(0);
                    }
                }
            }
        }
        for (qint32 x = 0; x < pMap->getMapWidth(); x++)
        {
            for (qint32 y = 0 ; y < pMap->getMapHeight(); y++)
            {
                Unit* pUnit = pMap->getTerrain(x, y)->getUnit();
                if (pUnit == nullptr)
                {
                    stream << static_cast<uchar>(std::numeric_limits<uchar>::max());
                    stream << static_cast<uchar>(std::numeric_limits<uchar>::max());
                }
                else
                {
                    if (pUnit->getUnitID() == "INFANTRY")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(244);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(28);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(68);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(108);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(148);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "MECH")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(8);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(48);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(88);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(128);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(168);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "APC")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(10);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(50);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(90);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(130);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(170);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "HOELLIUM")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(0);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(40);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(80);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(120);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(160);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "RECON")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(246);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(30);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(70);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(110);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(150);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "LIGHT_TANK")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(9);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(49);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(89);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(129);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(169);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "HEAVY_TANK")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(245);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(29);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(69);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(109);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(149);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "NEOTANK")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(253);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(37);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(77);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(117);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(157);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "MEGATANK")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(254);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(38);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(78);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(118);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(158);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "ARTILLERY")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(247);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(31);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(71);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(111);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(151);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "ROCKETTHROWER")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(11);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(51);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(91);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(131);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(171);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "PIPERUNNER")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(255);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(39);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(79);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(119);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(159);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "MISSILE")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(12);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(52);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(92);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(132);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(172);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "FLAK")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(248);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(32);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(72);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(112);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(152);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "T_HELI")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(14);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(54);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(94);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(134);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(174);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "FIGHTER")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(249);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(33);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(73);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(113);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(153);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "K_HELI")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(250);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(34);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(74);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(114);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(154);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "BATTLESHIP")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(251);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(35);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(75);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(115);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(155);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "LANDER")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(252);
                                stream << static_cast<uchar>(1);
                                break;
                            case 2:
                                stream << static_cast<uchar>(36);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(76);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(116);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(156);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "BOMBER")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(13);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(53);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(93);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(133);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(173);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "CRUISER")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(15);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(55);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(95);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(135);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(175);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "SUBMARINE")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(16);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(56);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(96);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(136);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(176);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "BLACK_BOAT")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(17);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(57);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(97);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(137);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(177);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "AIRCRAFTCARRIER")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(18);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(58);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(98);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(138);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(178);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "STEALTHBOMBER")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(19);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(59);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(99);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(139);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(179);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else if (pUnit->getUnitID() == "BLACK_BOMB")
                    {
                        switch (pUnit->getOwner()->getPlayerID() + 1)
                        {
                            case 1:
                                stream << static_cast<uchar>(20);
                                stream << static_cast<uchar>(2);
                                break;
                            case 2:
                                stream << static_cast<uchar>(70);
                                stream << static_cast<uchar>(2);
                                break;
                            case 3:
                                stream << static_cast<uchar>(100);
                                stream << static_cast<uchar>(2);
                                break;
                            case 4:
                                stream << static_cast<uchar>(140);
                                stream << static_cast<uchar>(2);
                                break;
                            case 5:
                                stream << static_cast<uchar>(180);
                                stream << static_cast<uchar>(2);
                                break;
                        }
                    }
                    else
                    {
                        stream << static_cast<uchar>(std::numeric_limits<uchar>::max());
                        stream << static_cast<uchar>(std::numeric_limits<uchar>::max());
                    }
                }
            }
        }
        QString mapName = pMap->getMapName();
        mapName = mapName.remove(254, mapName.size());
        stream << static_cast<uchar>(mapName.length());
        stream << static_cast<uchar>(0);
        stream << static_cast<uchar>(0);
        stream << static_cast<uchar>(0);
        QByteArray array = mapName.toUtf8();
        for (qint32 i = 0; i < array.length(); i++)
        {
            stream << static_cast<uchar>(array[i]);
        }
        QString mapAuthor = pMap->getMapAuthor();
        mapAuthor = mapAuthor.remove(254, mapAuthor.size());
        stream << static_cast<uchar>(mapAuthor.length());
        stream << static_cast<uchar>(0);
        stream << static_cast<uchar>(0);
        stream << static_cast<uchar>(0);
        array = mapAuthor.toUtf8();
        for (qint32 i = 0; i < array.length(); i++)
        {
            stream << static_cast<uchar>(array[i]);
        }
        QString mapDescription = pMap->getMapDescription();
        mapDescription = mapDescription.remove(254, mapDescription.size());
        stream << static_cast<uchar>(mapDescription.length());
        stream << static_cast<uchar>(0);
        stream << static_cast<uchar>(0);
        stream << static_cast<uchar>(0);
        array = mapDescription.toUtf8();
        for (qint32 i = 0; i < array.length(); i++)
        {
            stream << static_cast<uchar>(array[i]);
        }
    }
}
