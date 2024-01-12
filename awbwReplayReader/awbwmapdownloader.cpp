#include "awbwReplayReader/awbwmapdownloader.h"
#include "awbwReplayReader/awbwdatatypes.h"
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

void AwbwMapDownloader::loadMap(GameMap* pMap, bool withOutUnits, bool optimizePlayer)
{
    QJsonArray terrainMap = m_data.value(JSONKEY_TERRAINMAP).toArray();
    if (terrainMap.size() > 0)
    {
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
        if (!withOutUnits)
        {
            QJsonArray unitsArray = m_data.value(JSONKEY_PREDEPLOYEDUNITS).toArray();
            for (const auto & unit : unitsArray)
            {
                QJsonObject obj = unit.toObject();
                GameMap::ImporterUnitInfo info;
                info.x = obj.value(JSONKEY_UNITX).toInt();
                info.y = obj.value(JSONKEY_UNITY).toInt();
                info.unitId = AwbwDataTypes::UNITID_MAP[obj.value(JSONKEY_UNITID).toInt()];
                info.player = AwbwDataTypes::FACTION_MAP[obj.value(JSONKEY_COUNTRYCODE).toString()];
                units.append(info);
            }
        }
        pMap->importAWByWebMap(mapIDs, units, optimizePlayer);
        pMap->setMapName(m_data.value(JSONKEY_NAME).toString());
        pMap->setMapAuthor(m_data.value(JSONKEY_AUTHOR).toString());
    }
    else
    {
        sigDownloadResult(false);
    }
}
