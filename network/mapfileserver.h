#pragma once

#include <QObject>

class MainServer;

class MapFileServer : public QObject
{
    Q_OBJECT
public:
    explicit MapFileServer(MainServer *parent);


    void onMapUpload(quint64 socketID, const QJsonObject & objData);
    void onRequestFilteredMaps(quint64 socketID, const QJsonObject & objData);
    void onRequestDownloadMap(quint64 socketID, const QJsonObject & objData);
private:
    bool sameUploader(const QJsonObject & objData);
    void addFilterOption(QString & filterCommand, qint32 value, qint32 & filterCount, const char* const item, const char* const opCommand);
    void addFilterOption(QString & filterCommand, QString value, qint32 & filterCount, const char* const item);
private:
    MainServer* m_mainServer{nullptr};
};

