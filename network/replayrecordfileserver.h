#pragma once

#include <QObject>
#include <QJsonObject>
#include <QTimer>
#include "network/sqlmapfiltercreator.h"
#include "network/filepeer.h"

class MainServer;

class ReplayRecordFileserver : public QObject, public SqlMapFilterCreator
{
    Q_OBJECT
public:
    explicit ReplayRecordFileserver(MainServer* pParent);

    void onSlaveInfoGameResult(quint64 socketID, const QJsonObject &objData);
    Q_INVOKABLE void addRecordToDatabase(const QString & filePath);
public slots:
    void onRequestFilteredRecords(quint64 socketID, const QJsonObject & objData);
    void onRequestDownloadRecord(quint64 socketID, const QJsonObject & objData);
private slots:
    void onDeleteOldReplays();
    void onRequestFilePacket(quint64 socketID, const QJsonObject & objData);
    void addRecordToDatabase(const QJsonObject &objData);
private:
    MainServer* m_mainServer{nullptr};
    QTimer m_timer{this};
    std::map<quint64, spFilePeer> m_filePeers;
};

Q_DECLARE_INTERFACE(ReplayRecordFileserver, "ReplayRecordFileserver");
