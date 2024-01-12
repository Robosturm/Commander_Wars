#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>

class GameMap;

class AwbwMapDownloader : public QObject
{
    Q_OBJECT
public:
    explicit AwbwMapDownloader();

    static const char* const JSONKEY_NAME;
    static const char* const JSONKEY_AUTHOR;
    static const char* const JSONKEY_PLAYERCOUNT;
    static const char* const JSONKEY_TERRAINMAP;
    static const char* const JSONKEY_PREDEPLOYEDUNITS;
    static const char* const JSONKEY_UNITID;
    static const char* const JSONKEY_UNITX;
    static const char* const JSONKEY_UNITY;
    static const char* const JSONKEY_COUNTRYCODE;

    void startMapDownload(quint32 mapId);
    void loadMap(GameMap* pMap, bool withOutUnits = false, bool optimizePlayer = true);

signals:
    void sigDownloadResult(bool success);
private slots:
    void onResponseFinished(QNetworkReply* pReply);
    void errorOccurred(QNetworkReply::NetworkError code);
private:
    QNetworkAccessManager m_webCtrl;
    QNetworkReply* m_reply{nullptr};
    QJsonObject m_data;
};

