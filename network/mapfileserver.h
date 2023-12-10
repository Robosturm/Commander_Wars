#pragma once

#include <QObject>
#include "mapsupport/mapfilter.h"

class MainServer;

class MapFileServer : public QObject
{
    Q_OBJECT
public:
    explicit MapFileServer(MainServer *parent);


    void onMapUpload(quint64 socketID, const QJsonObject & objData);
    void onRequestFilteredMaps(quint64 socketID, const QJsonObject & objData);
    void onRequestDownloadMap(quint64 socketID, const QJsonObject & objData);
    Q_INVOKABLE void removeMapFromServer(const QString & sqlFilter);
private:
    bool sameUploader(const QJsonObject & objData);
    void addFilterOption(QString & filterCommand, qint32 value, qint32 & filterCount, const char* const item, const char* const opCommand);
    void addFilterOption(QString & filterCommand, QString value, qint32 & filterCount, const char* const item);
    void addFlagFilterOption(QString & filterCommand, qint32 & filterCount, const QVector<MapFilter::FlagFilter> & filters);
private:
    MainServer* m_mainServer{nullptr};
};

Q_DECLARE_INTERFACE(MapFileServer, "MapFileServer");
