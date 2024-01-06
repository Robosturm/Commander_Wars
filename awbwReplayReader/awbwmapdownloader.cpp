#include "awbwReplayReader/awbwmapdownloader.h"
#include "game/gamemap.h"
#include <QJsonDocument>

const char* const AwbwMapDownloader::JSONKEY_NAME = "Name";
const char* const AwbwMapDownloader::JSONKEY_AUTHOR = "Author";
const char* const AwbwMapDownloader::JSONKEY_PLAYERCOUNT = "Player Count";
const char* const AwbwMapDownloader::JSONKEY_TERRAINMAP = "Terrain Map";
const char* const AwbwMapDownloader::JSONKEY_PREDEPLOYEDUNITS = "Predeployed Units";
const char* const AwbwMapDownloader::JSONKEY_UNITID = "Unit ID";
const char* const AwbwMapDownloader::JSONKEY_UNITX = "Unit X";
const char* const AwbwMapDownloader::JSONKEY_UNITY = "Unit Y";
const char* const AwbwMapDownloader::JSONKEY_COUNTRYCODE = "Country Code";
AwbwMapDownloader::AwbwMapDownloader()
{
    connect(&m_webCtrl, &QNetworkAccessManager::finished, this, &AwbwMapDownloader::onResponseFinished, Qt::QueuedConnection);
}

void AwbwMapDownloader::startMapDownload(quint32 mapId)
{
    QUrl requestUrl("https://awbw.amarriner.com/api/map/map_info.php?maps_id=" + QString::number(mapId));
    QNetworkRequest request(requestUrl);
    m_reply = m_webCtrl.get(request);
    connect(m_reply, &QNetworkReply::errorOccurred, this, &AwbwMapDownloader::errorOccurred, Qt::QueuedConnection);
}

void AwbwMapDownloader::onResponseFinished(QNetworkReply* pReply)
{
    auto result = pReply->readAll();
    m_data = QJsonDocument::fromJson(result).object();
    pReply->deleteLater();
    emit sigDownloadResult(true);
}

void AwbwMapDownloader::errorOccurred(QNetworkReply::NetworkError code)
{
    emit sigDownloadResult(false);
}

void AwbwMapDownloader::loadMap(GameMap* pMap)
{
    QJsonArray terrainMap = m_data.value(JSONKEY_TERRAINMAP).toArray();
    QVector<QVector<quint32>> mapIDs;
    auto firstRow = terrainMap[0].toArray();
    for (const auto & row : firstRow)
    {
        mapIDs.append(QVector<quint32>());
    }
    for (qint32 x = 0; x < terrainMap.size(); ++x)
    {
        auto row = terrainMap[x].toArray();
        for (qint32 y = 0; y < row.size(); ++y)
        {
            mapIDs[y].append(row[y].toInt());
        }
    }
    QVector<GameMap::ImporterUnitInfo> units;
    std::map<qint32, QString> UNITID_MAP = {{1, "INFANTRY"},
                                                  {2, "MECH"},
                                                  {3, "HEAVY_TANK"},
                                                  {4, "LIGHT_TANK"},
                                                  {5, "RECON"},
                                                  {6, "APC"},
                                                  {7, "ARTILLERY"},
                                                  {8, "ROCKETTHROWER"},
                                                  {9, "FLAK"},
                                                  {10, "MISSILE"},
                                                  {11, "FIGHTER"},
                                                  {12, "BOMBER"},
                                                  {13, "K_HELI"},
                                                  {14, "T_HELI"},
                                                  {15, "BATTLESHIP"},
                                                  {16, "CRUISER"},
                                                  {17, "LANDER"},
                                                  {18, "SUBMARINE"},
                                                  {28, "BLACK_BOAT"},
                                                  {29, "AIRCRAFTCARRIER"},
                                                  {30, "STEALTHBOMBER"},
                                                  {46, "NEOTANK"},
                                                  {960900, "PIPERUNNER"},
                                                  {968731, "BLACK_BOMB"},
                                                  {1141438, "MEGATANK"},
                                                  };
    std::map<QString, qint32> FACTION_MAP = {{"os", 0},
                                                   {"bm", 1},
                                                   {"ge", 2},
                                                   {"yc", 3},
                                                   {"bh", 4},
                                                   {"rf", 5},
                                                   {"gs", 6},
                                                   {"bd", 7},
                                                   {"ab", 8},
                                                   {"js", 9},
                                                   {"ci", 10},
                                                   {"pc", 11},
                                                   {"tg", 12},
                                                   {"pl", 13},
                                                   {"ar", 14},
                                                   {"wn", 15},};
    QJsonArray unitsArray = m_data.value(JSONKEY_PREDEPLOYEDUNITS).toArray();
    for (const auto & unit : unitsArray)
    {
        QJsonObject obj = unit.toObject();
        GameMap::ImporterUnitInfo info;
        info.x = obj.value(JSONKEY_UNITX).toInt();
        info.y = obj.value(JSONKEY_UNITY).toInt();
        info.unitId = UNITID_MAP[obj.value(JSONKEY_UNITID).toInt()];
        info.player = FACTION_MAP[obj.value(JSONKEY_COUNTRYCODE).toString()];
        units.append(info);
    }

    pMap->importAWByWebMap(mapIDs, units);
    pMap->setMapName(m_data.value(JSONKEY_NAME).toString());
    pMap->setMapAuthor(m_data.value(JSONKEY_AUTHOR).toString());
}
