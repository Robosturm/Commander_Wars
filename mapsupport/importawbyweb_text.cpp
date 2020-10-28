#include <QTextStream>
#include <QFile>

#include "QVector"

#include "coreengine/console.h"

#include "game/gamemap.h"

#include "game/player.h"

#include "game/co.h"

#include "game/building.h"

#include "game/unit.h"

#include "menue/editormenue.h"

#include "objects/loadingscreen.h"

void GameMap::importAWByWebMap(QString file)
{
    spLoadingScreen pLoadingScreen = LoadingScreen::getInstance();
    pLoadingScreen->show();
    if (QFile::exists(file))
    {
        clearMap();        

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
        pLoadingScreen->setProgress(tr("Creating Player"), 10);
        // load 16 players :)
        for (qint32 i = 0; i < 16; i++)
        {
            players.append(new Player());
            players[i]->init();
        }
        // load empty map
        qint32 mapHeigth = mapIDs.size();
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            pLoadingScreen->setProgress(tr("Loading Empty Map Row ") + QString::number(y) + tr(" of ") + QString::number(mapHeigth), 10 + 20 * y / mapHeigth);
            fields.append(QVector<spTerrain>());
            for (qint32 x = 0; x < mapIDs[y].size(); x++)
            {
                spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y, "");
                this->addChild(pTerrain);
                fields[y].append(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
                pTerrain->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            }
        }

        for (qint32 y = 0; y < mapIDs.size(); y++)
        {
            pLoadingScreen->setProgress(tr("Importing Map Row ") + QString::number(y) + tr(" of ") + QString::number(mapHeigth), 30 + 50 * y / mapHeigth);
            for (qint32 x = 0; x < mapIDs[y].size(); x++)
            {
                switch (mapIDs[y][x])
                {
                    case std::numeric_limits<quint32>::max():
                    {
                        replaceTerrain("TELEPORTTILE", x, y);
                        break;
                    }
                    case 0:
                        break;
                        // plains
                    case 1:
                    {
                        replaceTerrain("PLAINS", x, y);
                        break;
                    }
                    case 2:
                    {
                        replaceTerrain("MOUNTAIN", x, y);
                        break;
                    }
                        // forest
                    case 3:
                    {
                        replaceTerrain("FOREST", x, y);
                        break;
                    }
                    case 4:
                    case 5:
                    case 6:
                    case 7:
                    case 8:
                    case 9:
                    case 10:
                    case 11:
                    case 12:
                    case 13:
                    case 14:
                    {
                        replaceTerrain("RIVER", x, y);
                        break;
                    }
                    case 15:
                    case 16:
                    case 17:
                    case 18:
                    case 19:
                    case 20:
                    case 21:
                    case 22:
                    case 23:
                    case 24:
                    case 25:
                    {
                        replaceTerrain("STREET", x, y);
                        break;
                    }
                    case 26:
                    case 27:
                    {
                        replaceTerrain("BRIDGE", x, y);
                        break;
                    }
                    case 28:
                    {
                        replaceTerrain("SEA", x, y);
                        break;
                    }
                        // beach
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    {
                        replaceTerrain("BEACH", x, y);
                        break;
                    }
                        // reaf
                    case 33:
                    {
                        replaceTerrain("REAF", x, y);
                        break;
                    }
                    case 34:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 35:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 36:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 37:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 38:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 39:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 40:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 41:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 42:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 43:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 44:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 45:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 46:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 47:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 48:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 49:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 50:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 51:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 52:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 53:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 54:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 55:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 56:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 57:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 81:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 82:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 83:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 84:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 85:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 86:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 87:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 88:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 89:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 90:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 91:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 92:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 93:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 94:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 95:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 96:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 97:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 98:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 99:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 100:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 101:
                    case 102:
                    case 103:
                    case 104:
                    case 105:
                    case 106:
                    case 107:
                    case 108:
                    case 109:
                    case 110:
                    {
                        replaceTerrain("PIPELINE", x, y);
                        break;
                    }
                    case 111:
                    {
                        Building* pBuilding = new Building("SILO_ROCKET");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 112:
                    {
                        Building* pBuilding = new Building("SILO");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 113:
                    case 114:
                    {
                        replaceTerrain("WELD", x, y);
                        break;
                    }
                    case 115:
                    case 116:
                    {
                        replaceTerrain("DESTROYEDWELD", x, y);
                        break;
                    }
                    case 117:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 118:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 119:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 120:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 121:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 122:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 123:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 124:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 125:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 126:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 127:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 128:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 129:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 130:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 131:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 132:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 133:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 134:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 135:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 136:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 137:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 138:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 139:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 140:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 141:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 142:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 143:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 144:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 145:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 146:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 147:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 148:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }

                    case 149:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 150:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 151:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 152:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 153:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 154:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 155:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 156:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 157:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 158:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 159:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 160:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 161:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 162:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }

                    case 163:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 164:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 165:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 166:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 167:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 168:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 169:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 170:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 171:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 172:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 173:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 174:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 175:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 176:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 177:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 178:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 179:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 180:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 181:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 182:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 183:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 184:
                    {
                        Building* pBuilding = new Building("AIRPORT");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 185:
                    {
                        Building* pBuilding = new Building("FACTORY");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 186:
                    {
                        Building* pBuilding = new Building("TOWN");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 187:
                    {
                        Building* pBuilding = new Building("TOWER");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 188:
                    {
                        Building* pBuilding = new Building("HQ");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 189:
                    {
                        Building* pBuilding = new Building("LABOR");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 190:
                    {
                        Building* pBuilding = new Building("HARBOUR");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        QStringList list = file.split("/");
        m_mapName = list[list.size() - 1].remove(list[list.size() - 1].lastIndexOf("."), list[list.size() - 1].size());
        m_mapDescription = "";
        m_mapAuthor = Settings::getUsername();
    }
    EditorMenue::getInstance()->optimizePlayers();
    // update the whole fucking map
    pLoadingScreen->setProgress(tr("Loading Sprites"), 90);
    updateSprites();
    pLoadingScreen->hide();
}
