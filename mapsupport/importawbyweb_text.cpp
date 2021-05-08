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
            m_players.append(spPlayer::create());
            m_players[i]->init();
        }
        // load empty map
        qint32 mapHeigth = mapIDs.size();
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            pLoadingScreen->setProgress(tr("Loading Empty Map Row ") + QString::number(y) + tr(" of ") + QString::number(mapHeigth), 10 + 20 * y / mapHeigth);
            m_fields.append(QVector<spTerrain>());
            for (qint32 x = 0; x < mapIDs[y].size(); x++)
            {
                spTerrain pTerrain = Terrain::createTerrain("PLAINS", x, y, "");
                this->addChild(pTerrain);
                m_fields[y].append(pTerrain);
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
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 35:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 36:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 37:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 38:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 39:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 40:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 41:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 42:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 43:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 44:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 45:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 46:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 47:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 48:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 49:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 50:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 51:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 52:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 53:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 54:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 55:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 56:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 57:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 81:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 82:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 83:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 84:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 85:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 86:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 87:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 88:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 89:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 90:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 91:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 92:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 93:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 94:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 95:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 96:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 97:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 98:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 99:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 100:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("SILO_ROCKET");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 112:
                    {
                        spBuilding pBuilding = spBuilding::create("SILO");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
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
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 118:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 119:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 120:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 121:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 122:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 123:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 124:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 125:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 126:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 127:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 128:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 129:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 130:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 131:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 132:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 133:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 134:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 135:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 136:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 137:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 138:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 139:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 140:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 141:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 142:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 143:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 144:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 145:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 146:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 147:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 148:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }

                    case 149:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 150:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 151:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 152:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 153:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 154:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 155:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 156:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 157:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 158:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 159:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 160:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 161:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 162:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }

                    case 163:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 164:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 165:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 166:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 167:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 168:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 169:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 170:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 171:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 172:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 173:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 174:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 175:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 176:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 177:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 178:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 179:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 180:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 181:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 182:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 183:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 184:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 185:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 186:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 187:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 188:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 189:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
                        break;
                    }
                    case 190:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR");
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding.get());
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
