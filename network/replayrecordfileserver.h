#pragma once

#include <QObject>
#include <QTimer>

class MainServer;

class ReplayRecordFileserver : public QObject
{
    Q_OBJECT
public:
    explicit ReplayRecordFileserver(MainServer* pParent);

    void onSlaveInfoGameResult(quint64 socketID, const QJsonObject &objData);
signals:

private:
    MainServer* m_mainServer{nullptr};
    QTimer m_timer{this};
};

