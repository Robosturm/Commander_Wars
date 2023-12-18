#pragma once

#include <QObject>
#include "network/sqlmapfiltercreator.h"

class MainServer;

class MapFileServer : public QObject, public SqlMapFilterCreator
{
    Q_OBJECT
public:
    explicit MapFileServer(MainServer *parent);


    void onMapUpload(quint64 socketID, const QJsonObject & objData);
    void onRequestFilteredMaps(quint64 socketID, const QJsonObject & objData);
    void onRequestDownloadMap(quint64 socketID, const QJsonObject & objData);
    void onRequestDeleteMap(quint64 socketID, const QJsonObject & objData);
    Q_INVOKABLE bool removeMapFromServer(const QString & sqlFilter);
private:
    bool sameUploader(const QJsonObject & objData);
private:
    MainServer* m_mainServer{nullptr};
};

Q_DECLARE_INTERFACE(MapFileServer, "MapFileServer");
