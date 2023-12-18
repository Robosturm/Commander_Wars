#pragma once

#include <QObject>
#include <QJsonObject>
#include <QTimer>
#include "network/sqlmapfiltercreator.h"

class MainServer;

class ReplayRecordFileserver : public QObject, public SqlMapFilterCreator
{
    Q_OBJECT
public:
    explicit ReplayRecordFileserver(MainServer* pParent);

    void onSlaveInfoGameResult(quint64 socketID, const QJsonObject &objData);
signals:

public slots:
    void onRequestFilteredRecords(quint64 socketID, const QJsonObject & objData);

private slots:
    void onDeleteOldReplays();
private:
    MainServer* m_mainServer{nullptr};
    QTimer m_timer{this};
};

