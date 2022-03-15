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
            m_players.append(spPlayer::create(this));
            m_players[i]->init();
        }
        // load empty map
        qint32 mapHeigth = mapIDs.size();
        for (qint32 y = 0; y < mapHeigth; y++)
        {
            pLoadingScreen->setProgress(tr("Loading Empty Map Row ") + QString::number(y) + tr(" of ") + QString::number(mapHeigth), 10 + 20 * y / mapHeigth);
            m_fields.push_back(std::vector<spTerrain>());
            auto pActor = oxygine::spActor::create();
            pActor->setPriority(static_cast<qint32>(Mainapp::ZOrder::Terrain) + y);
            m_rowSprites.push_back(pActor);
            addChild(pActor);
            for (qint32 x = 0; x < mapIDs[y].size(); x++)
            {
                spTerrain pTerrain = Terrain::createTerrain(GameMap::PLAINS, x, y, "", this);
                m_rowSprites[y]->addChild(pTerrain);
                m_fields[y].push_back(pTerrain);
                pTerrain->setPosition(x * m_imagesize, y * m_imagesize);
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
                        replaceTerrain("TELEPORTTILE", x, y, false, false, false);
                        break;
                    }
                    case 0:
                        break;
                        // plains
                    case 1:
                    {
                        replaceTerrain(GameMap::PLAINS, x, y, false, false, false);
                        break;
                    }
                    case 2:
                    {
                        replaceTerrain("MOUNTAIN", x, y, false, false, false);
                        break;
                    }
                        // forest
                    case 3:
                    {
                        replaceTerrain("FOREST", x, y, false, false, false);
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
                        replaceTerrain("RIVER", x, y, false, false, false);
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
                        replaceTerrain("STREET", x, y, false, false, false);
                        break;
                    }
                    case 26:
                    case 27:
                    {
                        replaceTerrain("BRIDGE", x, y, false, false, false);
                        break;
                    }
                    case 28:
                    {
                        replaceTerrain("SEA", x, y, false, false, false);
                        break;
                    }
                        // beach
                    case 29:
                    case 30:
                    case 31:
                    case 32:
                    {
                        replaceTerrain("BEACH", x, y, false, false, false);
                        break;
                    }
                        // reaf
                    case 33:
                    {
                        replaceTerrain("REAF", x, y, false, false, false);
                        break;
                    }
                    case 34:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 35:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 36:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 37:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 38:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 39:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 40:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 41:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 42:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 43:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 44:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 45:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 46:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 47:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 48:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 49:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 50:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 51:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 52:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 53:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 54:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 55:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 56:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 57:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 81:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 82:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 83:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 84:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 85:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 86:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 87:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 88:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 89:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 90:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 91:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 92:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 93:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 94:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 95:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 96:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 97:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 98:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 99:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 100:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
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
                        replaceTerrain("PIPELINE", x, y, false, false, false);
                        break;
                    }
                    case 111:
                    {
                        spBuilding pBuilding = spBuilding::create("SILO_ROCKET", this);
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 112:
                    {
                        spBuilding pBuilding = spBuilding::create("SILO", this);
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 113:
                    case 114:
                    {
                        replaceTerrain("WELD", x, y, false, false, false);
                        break;
                    }
                    case 115:
                    case 116:
                    {
                        replaceTerrain("DESTROYEDWELD", x, y, false, false, false);
                        break;
                    }
                    case 117:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 118:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 119:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 120:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 121:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 122:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 123:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 124:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 125:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 126:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 127:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 128:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 129:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 130:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 131:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 132:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 133:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 134:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 135:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 136:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 137:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 138:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(8));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 139:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(4));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 140:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(1));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 141:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(7));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 142:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(2));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 143:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(6));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 144:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(9));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 145:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(nullptr);
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 146:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(0));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 147:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(5));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 148:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(3));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 149:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 150:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 151:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 152:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 153:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 154:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 155:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(10));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 156:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 157:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 158:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 159:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 160:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 161:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 162:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(11));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }

                    case 163:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 164:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 165:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 166:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 167:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 168:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 169:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(12));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 170:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 171:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 172:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 173:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 174:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 175:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 176:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(13));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 185:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 181:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 182:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 183:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 184:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 186:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 187:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(14));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }

                    case 192:
                    {
                        spBuilding pBuilding = spBuilding::create("HQ", this);
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 188:
                    {
                        spBuilding pBuilding = spBuilding::create("AIRPORT", this);
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 189:
                    {
                        spBuilding pBuilding = spBuilding::create("FACTORY", this);
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 190:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWN", this);
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 191:
                    {
                        spBuilding pBuilding = spBuilding::create("TOWER", this);
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 193:
                    {
                        spBuilding pBuilding = spBuilding::create("LABOR", this);
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    case 194:
                    {
                        spBuilding pBuilding = spBuilding::create("HARBOUR", this);
                        pBuilding->setOwner(getPlayer(15));
                        getTerrain(x, y)->setBuilding(pBuilding);
                        break;
                    }
                    default:
                        CONSOLE_PRINT("Unable terrain id: " + QString::number(mapIDs[y][x]), Console::eERROR);
                        break;
                }
            }
        }

        QStringList list = file.split("/");
        m_headerInfo.m_mapName = list[list.size() - 1].remove(list[list.size() - 1].lastIndexOf("."), list[list.size() - 1].size());
        m_headerInfo.m_mapDescription = "";
        m_headerInfo.m_mapAuthor = Settings::getUsername();
    }
    EditorMenue::getInstance()->optimizePlayers();
    // update the whole fucking map
    pLoadingScreen->setProgress(tr("Loading Sprites"), 90);
    updateSprites();
    pLoadingScreen->hide();
}
